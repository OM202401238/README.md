#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

#define MAX_LENGTH 1000
const char DIR_UP = 'W';
const char DIR_DOWN = 'S';
const char DIR_RIGHT = 'D';
const char DIR_LEFT = 'A';

int height, width;
HANDLE hCONSOLE;
COORD bufferSize;
CHAR_INFO* consoleBuffer;

void initialize_screen() {
    hCONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCONSOLE, &csbi);
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    bufferSize = {static_cast<SHORT>(width), static_cast<SHORT>(height)};
    consoleBuffer = new CHAR_INFO[width * height];
    memset(consoleBuffer, 0, sizeof(CHAR_INFO) * width * height);
}

class Point {
public:
    int xcoord;
    int ycoord;
    Point() {}

    Point(int x, int y) {
        xcoord = x;
        ycoord = y;
    }
};

class Snake {
    int length;
    char direction;

public:
    Point body[MAX_LENGTH];

    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength() {
        return length;
    }

    void changeDirection(char newDirection) {
        if (direction == DIR_UP && newDirection != DIR_DOWN) {
            direction = newDirection;
        } else if (direction == DIR_DOWN && newDirection != DIR_UP) {
            direction = newDirection;
        } else if (direction == DIR_RIGHT && newDirection != DIR_LEFT) {
            direction = newDirection;
        } else if (direction == DIR_LEFT && newDirection != DIR_RIGHT) {
            direction = newDirection;
        }
    }

    bool move(Point food) {
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        if (direction == DIR_UP) {
            body[0].ycoord -= 1;
        } else if (direction == DIR_DOWN) {
            body[0].ycoord += 1;
        } else if (direction == DIR_RIGHT) {
            body[0].xcoord += 1;
        } else if (direction == DIR_LEFT) {
            body[0].xcoord -= 1;
        }

        for (int i = 1; i < length; i++) {
            if (body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord) {
                return false;
            }
        }

        if (body[0].xcoord < 1 || body[0].xcoord >= width - 1 || body[0].ycoord < 1 || body[0].ycoord >= height - 1) {
            return false;
        }

        if (food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord) {
            body[length] = Point(body[length - 1].xcoord, body[length - 1].ycoord);
            length++;
        }

        return true;
    }
};

class Board {
    Snake* snake;
    const char snake_body = 'O';
    Point food;
    const char FOOD = 'o';
    int score;

public:
    Board() {
        spawnFood();
        snake = new Snake(10, 10);
        score = 0;
    }

    ~Board() {
        delete snake;
    }

    int getScore() {
        return score;
    }

    void spawnFood() {
        int x = rand() % (width - 2) + 1;
        int y = rand() % (height - 2) + 1;
        food = Point(x, y);
    }

    void drawBorder() {
        for (int i = 0; i < width; i++) {
            consoleBuffer[i].Char.AsciiChar = '#';
            consoleBuffer[i].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            consoleBuffer[(height - 1) * width + i].Char.AsciiChar = '#';
            consoleBuffer[(height - 1) * width + i].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }

        for (int i = 0; i < height; i++) {
            consoleBuffer[i * width].Char.AsciiChar = '#';
            consoleBuffer[i * width].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            consoleBuffer[i * width + (width - 1)].Char.AsciiChar = '#';
            consoleBuffer[i * width + (width - 1)].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }
    }

    void draw() {
        memset(consoleBuffer, 0, sizeof(CHAR_INFO) * width * height);
        drawBorder();

        for (int i = 0; i < snake->getLength(); i++) {
            consoleBuffer[snake->body[i].ycoord * width + snake->body[i].xcoord].Char.AsciiChar = snake_body;
            consoleBuffer[snake->body[i].ycoord * width + snake->body[i].xcoord].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }

        consoleBuffer[food.ycoord * width + food.xcoord].Char.AsciiChar = FOOD;
        consoleBuffer[food.ycoord * width + food.xcoord].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

        char scoreText[50];
        snprintf(scoreText, sizeof(scoreText), "Current Score: %d", score);
        for (int i = 0; scoreText[i] != '\0'; i++) {
            consoleBuffer[i + (width / 2)].Char.AsciiChar = scoreText[i];
            consoleBuffer[i + (width / 2)].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }

        SMALL_RECT writeArea = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};
        WriteConsoleOutput(hCONSOLE, consoleBuffer, bufferSize, {0, 0}, &writeArea);
    }

    bool update() {
        bool isAlive = snake->move(food);
        if (!isAlive) {
            return false;
        }
        if (food.xcoord == snake->body[0].xcoord && food.ycoord == snake->body[0].ycoord) {
            score++;
            spawnFood();
        }
        return true;
    }

    void getInput() {
        if (kbhit()) {
            int key = getch();
            if (key == 'w' || key == 'W') {
                snake->changeDirection(DIR_UP);
            } else if (key == 's' || key == 'S') {
                snake->changeDirection(DIR_DOWN);
            } else if (key == 'a' || key == 'A') {
                snake->changeDirection(DIR_LEFT);
            } else if (key == 'd' || key == 'D') {
                snake->changeDirection(DIR_RIGHT);
            }
        }
    }
};

int main() {
    srand(time(0));
    initialize_screen();
    Board* board = new Board();
    while (board->update()) {
        board->getInput();
        board->draw();
        Sleep(100);
    }
    cout << endl << "GAME OVER" << endl;
    cout << "Final Score: " << board->getScore() << endl;
    return 0;
}