#include <raylib.h>
#include <iostream>
#include <fstream>
using namespace std;

// Constants for the screen dimensions, board size, and grid layout
const int screenWidth = 1080;
const int screenHeight = 850;
const int gridSize = 8;          // 8x8 grid for the game board
const int boardSize = 800;       // Board size in pixels
const int cellSize = boardSize / gridSize; // Size of each individual cell
const int maxqorkis = 12;        // Maximum number of qorkis per player

// qorki is in amharic for the pices ...   
struct qorki {
    int x, y;           // Position of the piece on the grid
    bool isKing;        // If the piece has been promoted to a king
    bool isWhite;       // True for white piece, false for black
    bool isCaptured;    // Flag to check if the piece has been captured
};

// Global variables for managing game state
qorki qorkis[maxqorkis * 2]; // Array to store all pieces (24 for each player)
int qorkiCount = 0 ;          // Keeps track of the current number of qorkis

// Game state variables
int selectedqorkiIndex = -1; // Index of the currently selected piece, -1 if none
bool isWhiteTurn = true;     // Keeps track of whose turn it is
bool gameOver = false;       // Game over flag
bool gameStarted = false;    // Flag to determine if the game has started
bool gameSavedMessage = false; // Flag for displaying the "Game Saved" message
bool gameLoadedMessage = false; // Flag for displaying the "Game Loaded" message
double gameSavedMessageTimer = 0.0; // Timer for the save game message
double gameLoadedMessageTimer = 0.0; // Timer for the load game message
const double messageDuration = 2.0;  // How long the save/load messages are displayed

// Player-related variables
string winnerMessage;      // Message showing the winner or if it's a draw
string player1Name = "";    // Name of player 1
string player2Name = "";    // Name of player 2
bool player1IsWhite = true; // Flag to indicate if player 1 is playing as white
bool enteringPlayer1 = true;// True if we're entering player 1's name, false for player 2
bool choosingColor = false; // True if choosing the color (white/black) after entering names

// Counters for the number of captured pieces
int whiteCapturedCount = 0;
int blackCapturedCount = 0;

// Game menu flag
bool gameOptionMenu = true; // True if the game is at the option menu (New Game/Load Game)

// Button positions for in-game buttons (new, load, save, etc.)
Rectangle newGameButton = {20, 100, 150, 50};
Rectangle loadGameButton = {20, 160, 150, 50};
Rectangle saveGameButton = {20, 220, 150, 50};
Rectangle newGameAfterEndButton = {screenWidth / 2 - 100, screenHeight / 2 + 100, 200, 50};
// 1. Game Initialization Functions
void Initqorkis();  // Initializes the qorki pieces at the start of a new game

// 2. Game State Functions (Save/Load)
void SaveGame();  // Saves the current game state to a file
void LoadGame();  // Loads the saved game state from a file

// 3. Input Handling Functions
void CapturePlayerName();  // Captures player name from keyboard input

// 4. Drawing Functions
void DrawBoard();  // Draws the game board
void Drawqorkis();  // Draws the qorki pieces
void DrawButtons();  // Draws in-game buttons
void DrawGameOptionMenu();  // Draws the main game option menu
void DrawColorSelectionMenu();  // Draws the color selection menu
void DrawMenu();  // Draws the name entry menu
void DrawMessages();  // Displays saved/loaded game messages

// 5. Game Logic Functions
bool IsMoveValid(qorki qorki, int x, int y);  // Validates a regular move
bool IsCaptureValid(qorki qorki, int x, int y);  // Validates a capture move
bool CanqorkiCaptureAgain(int index);  // Checks if a qorki can capture again after a move
void Captureqorki(qorki qorki, int x, int y);  // Captures a qorki during a valid move
bool HasValidMoves(bool isWhite);  // Checks if the player has any valid moves
void CheckForWin();  // Checks if the game has been won
void Moveqorki(int index, int x, int y);  // Moves a qorki to a new position
void UpdateGame();  // Updates game state and handles mouse clicks
int GetqorkiIndexAt(int x, int y); //recieves current position of the mouse and returns the index of the qorki

void DrawGameOptionMenu()
{
    ClearBackground(GOLD);
    DrawText("Choose an option:", screenWidth / 2 - MeasureText("Choose an option:", 30) / 2, screenHeight / 2 - 100, 30, DARKGRAY);

    Rectangle newGameButton = {screenWidth / 2 - 100, screenHeight / 2 - 50, 200, 50};
    Rectangle loadGameButton = {screenWidth / 2 - 100, screenHeight / 2 + 10, 200, 50};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, newGameButton))
        {
            gameOptionMenu = false;
            enteringPlayer1 = true; // Go to player name entry
        }
        else if (CheckCollisionPointRec(mousePos, loadGameButton))
        {
            LoadGame(); // Call the LoadGame function to load from a saved file
            gameStarted = true;
            gameOptionMenu = false; // Load game state and start game
        }
    }

    DrawRectangleRec(newGameButton, BLUE);
    DrawText("New Game", screenWidth / 2 - MeasureText("New Game", 20) / 2, screenHeight / 2 - 30, 20, BLACK);

    DrawRectangleRec(loadGameButton, BLUE);
    DrawText("Load Game", screenWidth / 2 - MeasureText("Load Game", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
}

void DrawMenu()
{
    ClearBackground(GOLD);
    string prompt = enteringPlayer1 ? "Enter Player 1 Name:   ( Will Start First )" : "Enter Player 2 name:";
    string currentName = enteringPlayer1 ? player1Name : player2Name;

    DrawText(prompt.c_str(), screenWidth / 2 - MeasureText(prompt.c_str(), 30) / 2, screenHeight / 2 - 100, 30, BLACK);
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 50, 200, 50, BLUE);
    DrawText(currentName.c_str(), screenWidth / 2 - 90, screenHeight / 2 - 30, 20, BLACK);

    CapturePlayerName();

    if (IsKeyPressed(KEY_ENTER))
    {
        if (enteringPlayer1 && !player1Name.empty())
        {
            choosingColor = true;
        }
        else if (!enteringPlayer1 && !player2Name.empty())
        {
            gameStarted = true;
            Initqorkis();
        }
    }
}

void CapturePlayerName() {
    int key = GetCharPressed();

    if (enteringPlayer1)
    {
        // Allow only uppercase letters (A-Z), lowercase letters (a-z), and spaces
        if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122) || key == 32)
        {
            // Check the length before adding the character
            if (player1Name.length() < 10)
            {
                player1Name += static_cast<char>(key);
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !player1Name.empty())
        {
            player1Name.pop_back();
        }
    }
    else
    {
        // Allow only uppercase letters (A-Z), lowercase letters (a-z), and spaces
        if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122) || key == 32)
        {
            // Check the length before adding the character
            if (player2Name.length() < 10)
            {
                player2Name += static_cast<char>(key);
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !player2Name.empty())
        {
            player2Name.pop_back();
        }
    }

}

void DrawColorSelectionMenu()
{
    ClearBackground(GOLD);
    DrawText("Choose your color:", screenWidth / 2 - MeasureText("Choose your color:", 30) / 2, screenHeight / 2 - 100, 30, DARKGRAY);

    Rectangle whiteButton = {screenWidth / 2 - 100, screenHeight / 2 - 50, 200, 50};
    Rectangle blackButton = {screenWidth / 2 - 100, screenHeight / 2 + 10, 200, 50};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, whiteButton))
        {
            player1IsWhite = true;
            isWhiteTurn = true;
            enteringPlayer1 = false;
            choosingColor = false;
        }
        else if (CheckCollisionPointRec(mousePos, blackButton))
        {
            player1IsWhite = false;
            isWhiteTurn = false;
            enteringPlayer1 = false;
            choosingColor = false;
        }
    }

    DrawRectangleRec(whiteButton, BLUE);
    DrawText("Play as White", screenWidth / 2 - MeasureText("Play as White", 20) / 2, screenHeight / 2 - 30, 20, BLACK);

    DrawRectangleRec(blackButton, BLUE);
    DrawText("Play as Black", screenWidth / 2 - MeasureText("Play as Black", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
}


// Function to save the game state into a text file

void SaveGame()
{
    ofstream saveFile("saved_game.txt");

    if (saveFile.is_open())
    {
        saveFile << qorkiCount << endl;
        for (int i = 0; i < qorkiCount; i++)
        {
            saveFile << qorkis[i].x << " " << qorkis[i].y << " " << qorkis[i].isKing << " " << qorkis[i].isWhite << " " << qorkis[i].isCaptured << std::endl;
        }
        saveFile << isWhiteTurn << endl;
        saveFile << whiteCapturedCount << " " << blackCapturedCount << endl;
        saveFile << player1Name << endl;
        saveFile << player2Name << endl;
        saveFile << player1IsWhite << endl;
    }
    saveFile.close();
    gameSavedMessage = true;
    gameSavedMessageTimer = messageDuration;
}

void LoadGame()
{
    ifstream loadFile("saved_game.txt");

    if (loadFile.is_open())
    {
        loadFile >> qorkiCount;
        for (int i = 0; i < qorkiCount; i++)
        {
            loadFile >> qorkis[i].x >> qorkis[i].y >> qorkis[i].isKing >> qorkis[i].isWhite >> qorkis[i].isCaptured;
        }
        loadFile >> isWhiteTurn;
        loadFile >> whiteCapturedCount >> blackCapturedCount;
        loadFile.ignore();
        getline(loadFile, player1Name);
        getline(loadFile, player2Name);
        loadFile >> player1IsWhite;
    }
    loadFile.close();
    gameLoadedMessage = true;
    gameLoadedMessageTimer = messageDuration;
}




void Initqorkis()
{
    qorkiCount = 0;
    whiteCapturedCount = 0;
    blackCapturedCount = 0;

    int whiteStartY = 0;
    int blackStartY = 5;

    for (int y = whiteStartY; y < whiteStartY + 3; y++)
    {
        for (int x = (y % 2 == 0) ? 0 : 1; x < gridSize; x += 2)
        {
            qorkis[qorkiCount++] = {x, y, false, true, false};
        }
    }

    for (int y = blackStartY; y < blackStartY + 3; y++)
    {
        for (int x = (y % 2 == 0) ? 0 : 1; x < gridSize; x += 2)
        {
            qorkis[qorkiCount++] = {x, y, false, false, false};
        }
    }
}

void DrawBoard()
{
    Color darkWood = {101, 67, 33, 255};
    Color lightWood = {222, 184, 135, 255};

    for (int y = 0; y < gridSize; y++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            Color color = (x + y) % 2 == 0 ?  darkWood : lightWood;
            DrawRectangle(x * cellSize + 200, y * cellSize + 50, cellSize, cellSize, color); // Offset board for buttons
        }
    }

    string player1Color = player1IsWhite ? " (White)" : " (Black)";
    string player2Color = player1IsWhite ? " (Black)" : " (White)";

    string whiteCapturedText = (player1IsWhite ? player1Name + player1Color : player2Name + player2Color) + " captured: " + to_string(whiteCapturedCount);
    string blackCapturedText = (player1IsWhite ? player2Name + player2Color : player1Name + player1Color) + " captured: " + to_string(blackCapturedCount);

    DrawText(whiteCapturedText.c_str(), 20, 10, 20, BLACK);
    DrawText(blackCapturedText.c_str(), screenWidth - 300, 10, 20, BLACK);
}

void Drawqorkis()
{
    for (int i = 0; i < qorkiCount; i++)
    {
        if (!qorkis[i].isCaptured)
        {
            Color color = qorkis[i].isWhite ? WHITE : BLACK;
            int qorkiCenterX = qorkis[i].x * cellSize + cellSize / 2 + 200; // Offset for buttons
            int qorkiCenterY = qorkis[i].y * cellSize + cellSize / 2 + 50;
            int radius = cellSize / 2-10 ;

            DrawCircle(qorkiCenterX + 5, qorkiCenterY + 5, radius, DARKGRAY);
            DrawCircle(qorkiCenterX, qorkiCenterY, radius, color);
            DrawCircle(qorkiCenterX, qorkiCenterY - 3, radius - 5, Fade(WHITE, 0.5));

            if (i == selectedqorkiIndex)
            {
                DrawCircle(qorkiCenterX, qorkiCenterY, radius + 10, Fade(YELLOW, 0.3));
                DrawCircleLines(qorkiCenterX, qorkiCenterY, radius + 5, YELLOW);
            }

            if (qorkis[i].isKing)
            {
                DrawCircle(qorkiCenterX, qorkiCenterY, radius / 4, GOLD);
            }
        }
    }
}
void DrawButtons()
{
    // Draw New Game button
    DrawRectangleRec(newGameButton, LIGHTGRAY);
    DrawText("New Game", newGameButton.x + 20, newGameButton.y + 15, 20, DARKGRAY);

    // Draw Load Game button
    DrawRectangleRec(loadGameButton, LIGHTGRAY);
    DrawText("Load Game", loadGameButton.x + 20, loadGameButton.y + 15, 20, DARKGRAY);

    // Draw Save Game button
    DrawRectangleRec(saveGameButton, LIGHTGRAY);
    DrawText("Save Game", saveGameButton.x + 20, saveGameButton.y + 15, 20, DARKGRAY);
}

// Function to get the index of a Qorki entity at a specific position (x, y)
int GetqorkiIndexAt(int x, int y)
{
    for (int i = 0; i < qorkiCount; i++) // Iterate through the qorkis array, which contains qorkiCount elements
    {
        if (!qorkis[i].isCaptured && qorkis[i].x == x && qorkis[i].y == y)// Check if the current Qorki entity is not captured and is at the specified position (x, y)
        {
            return i;// If a match is found, return the index of the Qorki entity
        }
    }
    return -1;// If no match is found, return -1 to indicate that no Qorki is at the specified position
}

bool IsMoveValid(qorki qorki, int x, int y) {
    if (x < 0 || x >= gridSize || y < 0 || y >= gridSize) return false;
    if (abs(qorki.x - x) != abs(qorki.y - y)) return false;

    // Check if the destination square is occupied by another piece
    if (GetqorkiIndexAt(x, y) != -1) return false;

    if (qorki.isKing) {
        int dx = (x > qorki.x) ? 1 : -1;//moves right it is 1 and moves left if it is -1
        int dy = (y > qorki.y) ? 1 : -1;;//moves up it is 1 and moves down if it is -1
        int consecutivePieces = 0;//is used to keep track of the number of consecutive pieces in the path of the move.

        for (int i = 1; i < abs(x - qorki.x); i++) {
            int px = qorki.x + i * dx;
            int py = qorki.y + i * dy;
            int index = GetqorkiIndexAt(px, py);
            if (index != -1) {
                consecutivePieces++;
                // If there are two consecutive pieces in the path, move is invalid
                if (consecutivePieces > 1) return false;
            }
        }
    } else {
        if (abs(qorki.x - x) != 1) return false;
        if ((qorki.isWhite && y <= qorki.y) || (!qorki.isWhite && y >= qorki.y)) return false;
    }

    return true;
}

bool IsCaptureValid(qorki qorki, int x, int y)
{
    // Check if destination is within bounds and on a dark square
    if (x < 0 || x >= gridSize || y < 0 || y >= gridSize || (x + y) % 2 != 0)
        return false;

    // Capture logic for kings
    if (qorki.isKing)
    {
        int dx = (x > qorki.x) ? 1 : -1;
        int dy = (y > qorki.y) ? 1 : -1;
        int dist = abs(x - qorki.x);
        bool opponentFound = false;

        for (int i = 1; i < dist; i++)
        {
            int px = qorki.x + i * dx;
            int py = qorki.y + i * dy;
            int index = GetqorkiIndexAt(px, py);

            if (index != -1)
            {
                if (qorkis[index].isWhite != qorki.isWhite && !qorkis[index].isCaptured && !opponentFound)
                {
                    opponentFound = true;
                }
                else
                {
                    return false;
                }
            }
        }

        if (GetqorkiIndexAt(x, y) != -1 || !opponentFound)
            return false;
        return true;
    }
    else
    {
        // Regular pieces capture logic
        if ((qorki.isWhite && y <= qorki.y) || (!qorki.isWhite && y >= qorki.y))
            return false;
        if (abs(qorki.x - x) != 2 || abs(qorki.y - y) != 2)
            return false;

        int midX = (qorki.x + x) / 2;
        int midY = (qorki.y + y) / 2;

        for (int i = 0; i < qorkiCount; i++)
        {
            if (!qorkis[i].isCaptured && qorkis[i].x == midX && qorkis[i].y == midY && qorkis[i].isWhite != qorki.isWhite)
            {
                if (GetqorkiIndexAt(x, y) != -1)
                    return false;
                return true;
            }
        }
    }
    return false;
}

bool CanqorkiCaptureAgain(int index)
{
    qorki qorki = qorkis[index];
    int dx[] = {-2, 2};
    int dy[] = {-2, 2};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int newX = qorki.x + dx[i];
            int newY = qorki.y + dy[j];

            if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize && IsCaptureValid(qorki, newX, newY))
            {
                return true;
            }
        }
    }
    return false;
}

void Captureqorki(qorki qorki, int x, int y)
{
    if (qorki.isKing)
    {
        int dx = (x > qorki.x) ? 1 : -1;
        int dy = (y > qorki.y) ? 1 : -1;
        int dist = abs(x - qorki.x);

        for (int i = 1; i < dist; i++)
        {
            int px = qorki.x + i * dx;
            int py = qorki.y + i * dy;
            int index = GetqorkiIndexAt(px, py);
            if (index != -1 && qorkis[index].isWhite != qorki.isWhite)
            {
                qorkis[index].isCaptured = true;

                if (qorki.isWhite)
                {
                    whiteCapturedCount++;
                }
                else
                {
                    blackCapturedCount++;
                }
                break;
            }
        }
    }
    else
    {
        int midX = (qorki.x + x) / 2;
        int midY = (qorki.y + y) / 2;

        for (int i = 0; i < qorkiCount; i++)
        {
            if (!qorkis[i].isCaptured && qorkis[i].x == midX && qorkis[i].y == midY)
            {
                qorkis[i].isCaptured = true;

                if (qorki.isWhite)
                {
                    whiteCapturedCount++;
                }
                else
                {
                    blackCapturedCount++;
                }
            }
        }
    }
}

bool HasValidMoves(bool isWhite)
{
    for (int i = 0; i < qorkiCount; i++)
    {
        if (qorkis[i].isWhite == isWhite && !qorkis[i].isCaptured)
        {
            for (int dx = -2; dx <= 2; dx++)
            {
                for (int dy = -2; dy <= 2; dy++)
                {
                    int newX = qorkis[i].x + dx;
                    int newY = qorkis[i].y + dy;
                    if (IsMoveValid(qorkis[i], newX, newY) || IsCaptureValid(qorkis[i], newX, newY))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void CheckForWin()
{
    bool whiteHasMoves = HasValidMoves(true);
    bool blackHasMoves = HasValidMoves(false);

    if (!whiteHasMoves || !blackHasMoves)
    {
        gameOver = true;

        if (!whiteHasMoves && !blackHasMoves)
        {
            winnerMessage = "It's a draw!";
        }
        else if (!whiteHasMoves)
        {
            winnerMessage = player1IsWhite ? (player2Name + " Wins!") : (player1Name + " Wins!");
        }
        else
        {
            winnerMessage = player1IsWhite ? (player1Name + " Wins!") : (player2Name + " Wins!");
        }
    }
}

void Moveqorki(int index, int x, int y)
{
    bool captured = IsCaptureValid(qorkis[index], x, y);
    if (captured)
    {
        Captureqorki(qorkis[index], x, y);
    }

    qorkis[index].x = x;
    qorkis[index].y = y;

    if ((qorkis[index].isWhite && y == gridSize - 1) || (!qorkis[index].isWhite && y == 0))
    {
        qorkis[index].isKing = true;
    }

    if (captured && CanqorkiCaptureAgain(index))
    {
        selectedqorkiIndex = index;
    }
    else
    {
        isWhiteTurn = !isWhiteTurn;
        selectedqorkiIndex = -1;
        CheckForWin(); // Check for game over after the move
    }
}
void DrawMessages()
{
    if (gameSavedMessage)
    {
        DrawText("Game Saved!", screenWidth / 2 - MeasureText("Game Saved!", 30) / 2, screenHeight / 2 - 200, 70, GREEN);
    }

    if (gameLoadedMessage)
    {
        DrawText("Game Loaded!", screenWidth / 2 - MeasureText("Game Loaded!", 30) / 2, screenHeight / 2 - 200, 70, GREEN);
    }
}

void UpdateGame()
{
    // Decrement the game saved and loaded timers
    if (gameSavedMessage)
    {
        gameSavedMessageTimer -= 0.01;
        if (gameSavedMessageTimer <= 0)
        {
            gameSavedMessage = false;
        }
    }

    if (gameLoadedMessage)
    {
        gameLoadedMessageTimer -= 0.01;
        if (gameLoadedMessageTimer <= 0)
        {
            gameLoadedMessage = false;
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !gameOver)
    {
        Vector2 mousePos = GetMousePosition();
        int x = (mousePos.x - 200) / cellSize; // Adjusted for board offset
        int y = (mousePos.y - 50) / cellSize;

        int clickedqorkiIndex = GetqorkiIndexAt(x, y);

        if (selectedqorkiIndex == -1)
        {
            if (clickedqorkiIndex != -1 && qorkis[clickedqorkiIndex].isWhite == isWhiteTurn)
            {
                selectedqorkiIndex = clickedqorkiIndex;
            }
        }
        else
        {
            if (clickedqorkiIndex == selectedqorkiIndex)
            {
                selectedqorkiIndex = -1;
            }
            else if (clickedqorkiIndex != -1 && qorkis[clickedqorkiIndex].isWhite == isWhiteTurn)
            {
                selectedqorkiIndex = clickedqorkiIndex;
            }
            else if (IsMoveValid(qorkis[selectedqorkiIndex], x, y) || IsCaptureValid(qorkis[selectedqorkiIndex], x, y))
            {
                Moveqorki(selectedqorkiIndex, x, y);
                CheckForWin();
            }
            else
            {
                selectedqorkiIndex = -1;
            }
        }
    }

    // Check if buttons are clicked
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();

        // Handle New Game button click
        if (CheckCollisionPointRec(mousePos, newGameButton))
        {
            Initqorkis(); // Start a new game
            gameOver = false;
            isWhiteTurn = player1IsWhite;
        }

        // Handle Load Game button click
        if (CheckCollisionPointRec(mousePos, loadGameButton))
        {
            LoadGame(); // Load game state
            gameOver = false;
        }

        // Handle Save Game button click
        if (CheckCollisionPointRec(mousePos, saveGameButton))
        {
            SaveGame(); // Save game state
        }
    }
}



int main()
{
    InitWindow(screenWidth, screenHeight, "DAMA");
    // SetTargetFPS(80);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        if (gameOptionMenu)
        {
            DrawGameOptionMenu(); // Show the initial menu for New Game or Load Game
        }
        else if (!gameStarted)
        {
            if (choosingColor)
            {
                DrawColorSelectionMenu();
            }
            else
            {
                DrawMenu();
            }
        }
        else
        {
            if (!gameOver)
            {
                UpdateGame();
            }
            ClearBackground({191, 119, 14, 1});

            string currentPlayer = isWhiteTurn ? (player1IsWhite ? player1Name : player2Name) + "'s Turn" : (player1IsWhite ? player2Name : player1Name) + "'s Turn";
            DrawText(currentPlayer.c_str(), screenWidth / 2 - MeasureText(currentPlayer.c_str(), 30) / 2, 10, 25, BLACK);

            DrawBoard();
            Drawqorkis();
            DrawButtons(); // Show buttons on the left side during the game

            if (gameOver)
            {
                // Draw winner message
                DrawText(winnerMessage.c_str(), screenWidth / 2 - MeasureText(winnerMessage.c_str(), 60) / 2, screenHeight / 2 - 30, 60, GREEN);

                // Draw New Game button after game ends
                DrawRectangleRec(newGameAfterEndButton, LIGHTGRAY);
                DrawText("New Game", newGameAfterEndButton.x + 20, newGameAfterEndButton.y + 15, 30, DARKGRAY);

                // Check if New Game button is clicked
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    Vector2 mousePos = GetMousePosition();
                    if (CheckCollisionPointRec(mousePos, newGameAfterEndButton))
                    {
                        Initqorkis();                 // Reset the qorkis
                        gameOver = false;             // Reset the game over state
                        isWhiteTurn = player1IsWhite; // Set the turn to the starting player
                        selectedqorkiIndex = -1;      // Reset selected qorki
                    }
                }
            }

            DrawMessages();
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}