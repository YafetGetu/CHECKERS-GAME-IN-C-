# ♟️ Checker Game

A sleek and simple terminal-based **Checker Game** built with C/C++. This project demonstrates core game logic, basic file handling for saving/loading, and uses a custom build system via `Makefile`.

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

git clone https://github.com/your-username/checker_game.git
cd checker_game
make

Run the game:

./build/Game.exe   # Windows
# OR
./build/Game       # Linux/macOS

