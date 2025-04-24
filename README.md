# ♟️ Checker Game

A sleek and simple terminal-based **Checker Game** built with C/C++. This project demonstrates core game logic, file handling for saving/loading, and uses a custom build system via `Makefile`.

---

## ✨ Features

- ✅ **Turn-Based Gameplay** – Two-player local mode, taking turns.
- 🧠 **Valid Move Detection** – Prevents illegal moves and enforces game rules.
- 💾 **Auto Save & Resume** – Game progress is saved to `saved_game.txt`.
- 🔄 **Replay Support** – Resume your game anytime from the last saved state.
- ❌ **Piece Capture Logic** – Supports standard checker jump captures.
- 🎯 **Win Condition Handling** – Game ends when one player has no moves left.
- ⌨️ **Keyboard Controls** – Navigate and play entirely from your terminal.
- ⚙️ **Makefile Powered Build** – Clean and simple builds with `make`.
- 🧱 **Modular Codebase** – Clean project structure with `src`, `include`, and `lib`.

---

## 📁 Project Structure

```bash
checker_game/
│
├── .qodo/               # IDE (Qodo) settings
├── .vscode/             # VS Code project settings
├── build/               # Compiled binaries & Makefile
│   ├── Game.exe         # Game executable (Windows)
│   └── Makefile         # Build rules
├── include/             # Header files
├── lib/                 # External libraries (if any)
├── src/                 # Source code (.c / .cpp files)
├── saved_game.txt       # Saved game data
├── .gitignore           # Files/directories ignored by Git

```
🚀 Getting Started
✅ Requirements
C/C++ Compiler (g++, clang, or MSVC)

make (Unix/Windows with MinGW)

Terminal or Command Prompt

🛠️ Build Instructions
Clone and build the project:

git clone  https://github.com/YafetGetu/CHECKERS-GAME-IN-C-.git
cd checker_game
make

Run the game:

./build/Game.exe   # Windows
OR
./build/Game       # Linux/macOS

