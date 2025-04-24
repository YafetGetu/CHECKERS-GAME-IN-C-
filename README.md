🧩 Checkers Game in C++

A simple yet functional Checkers (Dama) game built using C++ and the Raylib graphics library. This project features a classic two-player board game with the ability to save and load game states, making it easy to resume gameplay at any time.
🎮 Features

    🟦 Graphical Interface: Interactive visuals powered by Raylib

    👫 Two-Player Local Gameplay: Enjoy playing with a friend on the same machine

    💾 Save and Load Game State: Save your game progress in saved_game.txt and load it to continue later

    🗂️ Organized Codebase: Separate src, include, and lib folders for better structure

    ✅ Easy Build Setup for Windows: Simple methods to get the game up and running on Windows

🧩 What is "Qorki" (ቆርኪ)?

Qorkis (Amharic: ቆርኪ) is the traditional Amharic word for Checkers pieces.
🛠️ How to Build (Windows)

Before you start, make sure you have Raylib installed or included in your lib/ folder.
🔹 Option 1: Using build.bat

Run the batch script to automatically compile the game:

./build.bat

🔹 Option 2: Manual Compilation

Alternatively, you can compile the game manually using the following g++ command:

g++ src\main.cpp -o build\Game.exe -O2 -Wall -Wno-missing-braces -I include -L lib -lraylib -lopengl32

After building, you can run the game with:

./build/Game.exe
