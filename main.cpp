#include <iostream>
#include <vector>
#include <conio.h>   // For _getch()
#include <cstdlib>   // For system("cls")
#include <ctime>     // For seeding randomness
#include <string>
#include <thread> // for animation
#include <chrono> // for animation

using namespace std;
using namespace this_thread;
using namespace chrono;

// ANSI escape sequence for setting color
#define SET_COLOR(r, g, b) "\033[38;2;" << r << ";" << g << ";" << b << "m"
#define RESET_COLOR "\033[0m"

// Game settings
const int gridSize = 9;
const int numOfBombs = gridSize;
int spawnX = gridSize / 2, spawnY = gridSize / 2; // Player spawn position
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

Tile bomb = Tile(true, 255, 255, 255, true, "#");
Tile tile = Tile(true, 255, 255, 255, false, "#");
Tile flag = Tile(false, 255, 0, 0, false, "#");

// Game map
vector<vector<Tile>> gameMap(gridSize, vector<Tile>(gridSize));
vector<vector<Tile>> gameMapDupe(gridSize, vector<Tile>(gridSize));

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
bool movePlayer(char input) {
    bool gameOver = false;
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
            gameOver = true;
        }
    }
    return gameOver;
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
        if (randX == spawnX && randY == spawnY) {
            gameMap[randX][randY] = tile;
            gameMapDupe[randX][randY] = tile;
        }
        else {
            gameMap[randX][randY] = bomb;
            gameMapDupe[randX][randY] = bomb;
        }

    }
}

int checkFlagPlace(char input, int bombsLeft) {
    if (input == '7') {
        if (playerY != 0 && playerX != 0) {
            if (gameMap[playerY - 1][playerX - 1] == bomb)
                bombsLeft--;
            if (gameMap[playerY - 1][playerX - 1] == flag) {
                if (gameMapDupe[playerY - 1][playerX - 1] == bomb)
                    bombsLeft++;
                gameMap[playerY - 1][playerX - 1] = gameMapDupe[playerY - 1][playerX - 1];
            }
            else
                gameMap[playerY - 1][playerX - 1] = flag;
        }
    }
    if (input == '8') {
        if (playerY != 0) {
            if (gameMap[playerY - 1][playerX] == bomb)
                bombsLeft--;
            if (gameMap[playerY - 1][playerX] == flag) {
                if (gameMapDupe[playerY - 1][playerX] == bomb)
                    bombsLeft++;
                gameMap[playerY - 1][playerX] = gameMapDupe[playerY - 1][playerX];
            }
            else
                gameMap[playerY - 1][playerX] = flag;
        }
    }
    if (input == '9') {
        if (playerY != 0 && playerX != gridSize - 1) {
            if (gameMap[playerY - 1][playerX + 1] == bomb)
                bombsLeft--;
            if (gameMap[playerY - 1][playerX + 1] == flag) {
                if (gameMapDupe[playerY - 1][playerX + 1] == bomb)
                    bombsLeft++;
                gameMap[playerY - 1][playerX + 1] = gameMapDupe[playerY - 1][playerX + 1];
            }
            else
                gameMap[playerY - 1][playerX + 1] = flag;
        }
    }
    if (input == '4') {
        if (playerX != 0) {
            if (gameMap[playerY][playerX - 1] == bomb)
                bombsLeft--;
            if (gameMap[playerY][playerX - 1] == flag) {
                if (gameMapDupe[playerY][playerX - 1] == bomb)
                    bombsLeft++;
                gameMap[playerY][playerX - 1] = gameMapDupe[playerY][playerX - 1];
            }
            else
                gameMap[playerY][playerX - 1] = flag;
        }
    }
    if (input == '6') {
        if (playerX != gridSize - 1) {
            if (gameMap[playerY][playerX + 1] == bomb)
                bombsLeft--;
            if (gameMap[playerY][playerX + 1] == flag) {
                if (gameMapDupe[playerY][playerX + 1] == bomb)
                    bombsLeft++;
                gameMap[playerY][playerX + 1] = gameMapDupe[playerY][playerX + 1];
            }
            else
                gameMap[playerY][playerX + 1] = flag;
        }
    }
    if (input == '1') {
        if (playerY != gridSize - 1 && playerX != 0) {
            if (gameMap[playerY + 1][playerX - 1] == bomb)
                bombsLeft--;
            if (gameMap[playerY + 1][playerX - 1] == flag) {
                if (gameMapDupe[playerY + 1][playerX - 1] == bomb)
                    bombsLeft++;
                gameMap[playerY + 1][playerX - 1] = gameMapDupe[playerY + 1][playerX - 1];
            }
            else
                gameMap[playerY + 1][playerX - 1] = flag;
        }
    }
    if (input == '2') {
        if (playerY != gridSize - 1) {
            if (gameMap[playerY + 1][playerX] == bomb)
                bombsLeft--;
            if (gameMap[playerY + 1][playerX] == flag) {
                if (gameMapDupe[playerY + 1][playerX] == bomb)
                    bombsLeft++;
                gameMap[playerY + 1][playerX] = gameMapDupe[playerY + 1][playerX];
            }
            else
                gameMap[playerY + 1][playerX] = flag;
        }
    }
    if (input == '3') {
        if (playerY != gridSize - 1 && playerX != gridSize - 1) {
            if (gameMap[playerY + 1][playerX + 1] == bomb)
                bombsLeft--;
            if (gameMap[playerY + 1][playerX + 1] == flag) {
                if (gameMapDupe[playerY + 1][playerX + 1] == bomb)
                    bombsLeft++;
                gameMap[playerY + 1][playerX + 1] = gameMapDupe[playerY + 1][playerX + 1];
            }
            else
                gameMap[playerY + 1][playerX + 1] = flag;
        }
    }
    return bombsLeft;
}


int getSurroundingBombs(int y, int x) {
    int surroundingBombs = 0;
    if (y != 0 && x != 0) {
        if (gameMapDupe[y - 1][playerX - 1] == bomb)
            surroundingBombs++;
    }
    if (y != 0) {
        if (gameMapDupe[y - 1][playerX] == bomb)
            surroundingBombs++;
    }
    if (y != 0 && playerX != gridSize - 1) {
        if (gameMapDupe[y - 1][playerX + 1] == bomb)
            surroundingBombs++;
    }
    if (playerX != 0) {
        if (gameMapDupe[y][playerX - 1] == bomb)
            surroundingBombs++;
    }
    if (playerX != gridSize - 1) {
        if (gameMapDupe[y][playerX + 1] == bomb)
            surroundingBombs++;
    }
    if (y != gridSize - 1 && playerX != 0) {
        if (gameMapDupe[y + 1][playerX - 1] == bomb)
            surroundingBombs++;
    }
    if (y != gridSize - 1) {
        if (gameMapDupe[y + 1][playerX] == bomb)
            surroundingBombs++;
    }
    if (y != gridSize - 1 && playerX != gridSize - 1) {
        if (gameMapDupe[y + 1][playerX + 1] == bomb)
            surroundingBombs++;
    }
    return surroundingBombs;
}

void explosionAnimation() {
    int waitTime = 100;

    cout << "    )\n     (\n    .-`-.\n    :   :\n    :TNT:\n    :___:" << endl;
    sleep_for(milliseconds(1000));
    system("cls");

    cout << "    \\|/\n   - o -\n    /-`-.\n    :   :\n    :TNT:\n    :___:" << endl;
    sleep_for(milliseconds(1500));
    system("cls");

    cout << "\n\n    .---.\n    : | :\n    :-o-:\n    :_|_:" << endl;
    sleep_for(milliseconds(waitTime));
    system("cls");

    cout << "\n\n    .---.\n    (\\|/)\n    --0--\n    (/|\\)" << endl;
    sleep_for(milliseconds(waitTime));
    system("cls");

    cout << "\n\n   '.\\|/.'\n   (\\   /)\n   - -O- -\n   (/   \\)\n   ,'/'|\\'." << endl;
    sleep_for(milliseconds(waitTime));
    system("cls");

    cout << "\n'.  \\ | /  ,'\n  `. `.' ,'\n ( .`.|,' .)\n - ~ -0- ~ -\n (\n','|'.` )\n  .' .'. '.\n,\\'  / | \\  '.\n" << endl;
    sleep_for(milliseconds(waitTime));
    system("cls");

    cout << "\n    \\ '  \"  \n ` . `.' ,'\n . `` ,'. \"\n   ~ (   ~ -\n'\n. ','|` ` .\n  .'  \"  '\n,   ' , '  `" << endl;
    sleep_for(milliseconds(waitTime));
    system("cls");

    cout << "\n\n   (  ) (\n    ) ( )\n    (  )\n     ) /\n    ,---." << endl;
    sleep_for(milliseconds(waitTime));
}


int main() {
    initializeGame();
    
    int bombsLeft = 0;
    int totalBombs = 0;
    for (int i = 0; i <= gridSize - 1; i++) {
        for (int j = 0; j <= gridSize - 1; j++) {
            if (gameMap[i][j] == bomb) {
                bombsLeft++;
                totalBombs++; 
            }
        }
    }

    while (true) {

        printGrid();
        cout << "Use WASD to move. Press 'q' to quit.\n";
        cout << "There are " << totalBombs << " bombs in this round.\n";


        gameMap[playerY][playerX] = Tile(true, 255, 255 - (getSurroundingBombs(playerY, playerX) * 31), 255 - (getSurroundingBombs(playerY, playerX) * 31), false, to_string(getSurroundingBombs(playerY, playerX)));


        char input = _getch(); // Get user input


        if (input == 'q') break;
        bombsLeft = checkFlagPlace(input, bombsLeft);


        if (movePlayer(input)) {
            system("cls");
            explosionAnimation();
            cout << "You hit a bomb!!";
            break;
        }

        if (bombsLeft == 0) {
            cout << "You did it!!\nYou flagged all the bombs on this board!!";
            break;
        }
    }

    return 0;
}
