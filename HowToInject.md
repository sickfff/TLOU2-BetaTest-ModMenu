# 🎮 How to Inject Your DLL Mod Menu into The Last of Us Part II

Welcome to this simple step-by-step tutorial on injecting your custom DLL mod menu into the game!  
This guide covers three popular methods to get your mod menu running. 🚀

---

## 1️⃣ Using Cheat Engine

Cheat Engine is a powerful tool often used for memory editing and DLL injection.

### Steps:

1. **Open The Last of Us Part II** and let it run.
2. Launch **Cheat Engine** as Administrator.
3. Click on the **computer icon** in Cheat Engine and select the game's process (usually something like `tlou2.exe`).
4. Go to **"Memory View"** → **Tools** → **"Inject DLL"**.
5. Browse and select your compiled DLL file (e.g., `Tlou2ModMenu.dll`).
6. Click **Inject**.
7. If successful, your mod menu will appear in-game.

> ⚠️ *Make sure Cheat Engine matches the game architecture (x64 vs x86).*

---

## 2️⃣ Injecting via Game Directory (Manual)

Sometimes, games allow DLLs to be loaded if placed in their folder — useful for mods or overlays.

### Steps:

1. Locate your game installation folder. For example:  
   `C:\Program Files\The Last of Us Part II\`
2. Copy your compiled DLL (`Tlou2ModMenu.dll`) directly into this folder.
3. Launch the game normally.
4. If your DLL is set to auto-inject on game launch (via your DLL code), it will initialize and load your mod menu automatically.

> 🔍 *This method works only if the game or launcher loads DLLs from the folder.*

---

## 3️⃣ Using Other DLL Injectors

There are many standalone DLL injectors designed for simple and fast injection.

### Popular Injectors:

- **Extreme Injector**  
- **Xenos Injector**  
- **Process Hacker** (with DLL injection features)  

### Steps:

1. Run your chosen DLL injector **as Administrator**.
2. Select the target process (`tlou2.exe`) from the running processes list.
3. Browse and select your DLL file.
4. Click **Inject**.
5. Return to the game and enjoy your mod menu!

---

## ⚠️ Important Notes

- Always run your injector as Administrator for best results.  
- Make sure your DLL matches the game’s architecture (x64 or x86).  
- Antivirus or Windows Defender might flag your DLL — consider adding exceptions.  
- Using mods can cause crashes or instability; use responsibly.

---

## 🙌 Credits

This mod menu project is a **Beta test** created as a learning experience in C++ programming.  
Big thanks to **Gabriel Roriz Silva** — [GitHub](https://github.com/groriz11) — for inspiration and guidance!

---

Enjoy modding and stay safe! 🎉
