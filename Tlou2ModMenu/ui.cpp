// main.cpp
#include <windows.h>
#include <tchar.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// Link necessary d3d11 libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// === Mod Menu code from your dll ===

bool showMenu = true;

void SwapPlayerModel(const char* name) {
    printf("[TestApp] Swapping player model to: %s\n", name);
}

void SpawnEntity(const char* entityName) {
    printf("[TestApp] Spawning entity: %s\n", entityName);
}

void GiveWeapon(const char* weaponName) {
    printf("[TestApp] Giving weapon: %s\n", weaponName);
}

void PlayCutscene(const char* cutsceneId) {
    printf("[TestApp] Playing cutscene: %s\n", cutsceneId);
}

void RenderMenu() {
    if (!showMenu) return;

    static int playerModelIdx = 0;
    static int entityIdx = 0;
    static int weaponIdx = 0;
    static int cutsceneIdx = 0;

    const char* playerModels[] = { "Joel", "Tommy", "Dina", "Ellie", "Abby" };
    const char* entities[] = { "Clicker", "Runner", "Seraphite", "WLF Soldier", "Scars" };
    const char* weapons[] = { "Flamethrower", "MP5", "Crossbow", "Bow", "Revolver" };
    const char* cutscenes[] = { "Intro", "Ambush", "Boss Fight", "Escape", "Endgame" };

    ImGui::Begin("TLOU2 Mod Menu Test App", &showMenu, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Player Model")) {
        ImGui::Combo("Select Player Model", &playerModelIdx, playerModels, IM_ARRAYSIZE(playerModels));
        if (ImGui::Button("Swap Player Model")) {
            SwapPlayerModel(playerModels[playerModelIdx]);
        }
    }

    if (ImGui::CollapsingHeader("Entity Spawner")) {
        ImGui::Combo("Select Entity", &entityIdx, entities, IM_ARRAYSIZE(entities));
        if (ImGui::Button("Spawn Selected Entity")) {
            SpawnEntity(entities[entityIdx]);
        }
    }

    if (ImGui::CollapsingHeader("Weapons")) {
        ImGui::Combo("Select Weapon", &weaponIdx, weapons, IM_ARRAYSIZE(weapons));
        if (ImGui::Button("Give Selected Weapon")) {
            GiveWeapon(weapons[weaponIdx]);
        }
    }

    if (ImGui::CollapsingHeader("Cutscenes / Tasks")) {
        ImGui::Combo("Select Cutscene", &cutsceneIdx, cutscenes, IM_ARRAYSIZE(cutscenes));
        if (ImGui::Button("Play Selected Cutscene")) {
            PlayCutscene(cutscenes[cutsceneIdx]);
        }
    }

    ImGui::Text("Press ESC to close this app");
    ImGui::End();
}

// === Win32 and DirectX boilerplate ===

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      _T("TLOU2ModMenuTestApp"), NULL };
    RegisterClassEx(&wc);

    // Create window
    HWND hwnd = CreateWindow(wc.lpszClassName, _T("TLOU2 Mod Menu Test App"),
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
        NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Render your menu here
        RenderMenu();

        ImGui::Render();
        const float clear_color[4] = { 0.1f, 0.12f, 0.15f, 1.0f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#if defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
        &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL;
    }
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = NULL;
    }
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = NULL;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
}

// Win32 message handler

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) // ESC to close app
            PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
