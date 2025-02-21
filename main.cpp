#include <iostream>
#include <vector>
#include <conio.h>   // For _getch()
#include <cstdlib>   // For system("cls")
#include <ctime>     // For seeding randomness

using namespace std;

// ANSI escape sequence for setting color
#define SET_COLOR(r, g, b) "\033[38;2;" << r << ";" << g << ";" << b << "m"
#define RESET_COLOR "\033[0m"

// Game settings
const int gridSize = 9;
const int numOfBombs = 10;
int spawnX = gridSize / 2, spawnY =  gridSize/2; // Player spawn position
int pixelsPerIndex = 1; // Determines the size of each tile when rendered
char playerIcon = '@';

// Struct to define a tile
struct Tile {
    bool canMoveThrough;
    int colorR, colorG, colorB;
    bool killsPlayer;
    string texture; // ASCII texture representation

    Tile(bool walkable = true, int r = 255, int g = 255, int b = 255, bool kills = false, string tex = "#")
        : canMoveThrough(walkable), colorR(r), colorG(g), colorB(b), killsPlayer(kills), texture(tex) {
    }

    // Define the equality operator inside the struct
    bool operator==(const Tile& other) const {
        return (canMoveThrough == other.canMoveThrough &&
            colorR == other.colorR &&
            colorG == other.colorG &&
            colorB == other.colorB &&
            killsPlayer == other.killsPlayer &&
            texture == other.texture);
    }
};

Tile bomb = Tile(true, 0, 255, 255, true, "#");
Tile tile = Tile(true, 255, 255, 255, false, "#");
Tile flag = Tile(false, 255, 0, 0, false, "#");

// Game map
vector<vector<Tile>> gameMap(gridSize, vector<Tile>(gridSize));

// Player position
int playerX = spawnX, playerY = spawnY;

// Function to print the grid
void printGrid() {
    system("cls");
    for (int i = 0; i < gridSize; ++i) {
        for (int px = 0; px < pixelsPerIndex; ++px) { // Scale pixel height
            for (int j = 0; j < gridSize; ++j) {
                for (int py = 0; py < pixelsPerIndex; ++py) { // Scale pixel width
                    if (i == playerY && j == playerX) {
                        cout << playerIcon << playerIcon; // Draw player
                    }
                    else {
                        cout << SET_COLOR(gameMap[i][j].colorR, gameMap[i][j].colorG, gameMap[i][j].colorB)
                            << gameMap[i][j].texture << gameMap[i][j].texture
                            << RESET_COLOR;
                    }
                }
            }
            cout << endl;
        }
    }
}

// Function to handle player movement
void movePlayer(char input) {
    int newX = playerX, newY = playerY;

    switch (input) {
    case 'w': newY--; break;
    case 's': newY++; break;
    case 'a': newX--; break;
    case 'd': newX++; break;
    }

    // Check if movement is possible
    if (newX >= 0 && newX < gridSize && newY >= 0 && newY < gridSize) {
        if (gameMap[newY][newX].canMoveThrough) {
            playerX = newX;
            playerY = newY;
        }

        if (gameMap[newY][newX].killsPlayer) {
            playerX = spawnX;
            playerY = spawnY;
        }
    }
}

// Function to initialize game map
void initializeGame() {
    srand(time(0));
    int randX, randY;

    // Example: Set some tiles as non-walkable and some as deadly
    // gameMap[3][3] = Tile(false, 255, 0, 0, false, "X"); // Wall
    // gameMap[0][0] = Tile(true, 255, 255, 255, true, "#"); settings for bombs
    for (int i = 0; i < numOfBombs; i++) {
        randX = rand() % gridSize;
        randY = rand() % gridSize;
        if (randX == 0 && randY == 0)
            gameMap[randX][randY] = tile;
        else
            gameMap[randX][randY] = bomb;
    }
}

int main() {
    initializeGame();
    int bombsLeft = 1;
    for (int i = 0; i < gridSize - 1; i++) {
        for (int j = 0; j < gridSize - 1; j++) {
            if (gameMap[i][j] == bomb)
                bombsLeft++;
        }
    }

    while (true) {

        printGrid();
        cout << "Use WASD to move. Press 'q' to quit.\n";
        cout << "Number of bombs left:" << bombsLeft << "\n";


        char input = _getch(); // Get user input


        if (input == 'q') break;
        if (input == '7') {
            if (playerY != 0 && playerX != 0) {
                if (gameMap[playerY - 1][playerX - 1] == bomb)
                    bombsLeft--;
                gameMap[playerY - 1][playerX - 1] = flag;
            }
        }
        if (input == '8') {
            if (playerY != 0) {
                if (gameMap[playerY - 1][playerX] == bomb)
                    bombsLeft--;
                gameMap[playerY - 1][playerX] = flag;
            }
        }
        if (input == '9') {
            if (playerY != 0 && playerX != gridSize - 1) {
                if (gameMap[playerY - 1][playerX + 1] == bomb)
                    bombsLeft--;
                gameMap[playerY - 1][playerX + 1] = flag;
            }
        }
        if (input == '4') {
            if (playerX != 0) {
                if (gameMap[playerY][playerX - 1] == bomb)
                    bombsLeft--;
                gameMap[playerY][playerX - 1] = flag;
            }
        }
        if (input == '6') {
            if (playerX != gridSize - 1) {
                if (gameMap[playerY][playerX + 1] == bomb)
                    bombsLeft--;
                gameMap[playerY][playerX + 1] = flag;
            }
        }
        if (input == '1') {
            if (playerY != gridSize - 1 && playerX != 0) {
                if (gameMap[playerY + 1][playerX - 1] == bomb)
                    bombsLeft--;
                gameMap[playerY + 1][playerX - 1] = flag;
            }
        }
        if (input == '2') {
            if (playerY != gridSize - 1) {
                if (gameMap[playerY + 1][playerX] == bomb)
                    bombsLeft--;
                gameMap[playerY + 1][playerX] = flag;
            }
        }
        if (input == '3') {
            if (playerY != gridSize - 1 && playerX != gridSize - 1) {
                if (gameMap[playerY + 1][playerX + 1] == bomb)
                    bombsLeft--;
                gameMap[playerY + 1][playerX + 1] = flag;
            }
        }

        movePlayer(input);
    }

    return 0;
}
