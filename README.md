# 🧩 Checkers Game in C++

A simple yet functional Checkers (Dama) game built using **C++** and the **Raylib** graphics library. This project features a classic two-player board game with the ability to save and load game states, making it easy to resume gameplay.

## 🎮 Features

- 🟦 Graphical interface using Raylib  
- 👫 Two-player local gameplay  
- 💾 Save and load game state from `saved_game.txt`  
- 🗂️ Clean file organization with separate `src`, `include`, and `lib` folders  
- ✅ Easy build setup for Windows  

## 🧩 What is "Qorki" (ቆርኪ)?

**Qorkis** (Amharic: ቆርኪ) — the traditional Amharic word for **checkers pieces**.  

## 🛠️ How to Build (Windows)

> Make sure Raylib is installed or included in your `lib/` folder.

### 🔹 Option 1: Using `build.bat`
```bash
./build.bat

Manual compile command
g++ src\main.cpp -o build\Game.exe -O2 -Wall -Wno-missing-braces -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm

After building, run:

./build/Game.exe
