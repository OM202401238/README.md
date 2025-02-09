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
void initialize_screen() {
    HANDLE hCONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hCONSOLE, &csbi);
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
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

    int getlength() {
        return length;
    }

    void Changedirection(char newdirection) {
        if (direction == DIR_UP && newdirection != DIR_DOWN) {
            direction = newdirection;
        } else if (direction == DIR_DOWN && newdirection != DIR_UP) {
            direction = newdirection;
        } else if (direction == DIR_RIGHT && newdirection != DIR_LEFT) {
            direction = newdirection;
        } else if (direction == DIR_LEFT && newdirection != DIR_RIGHT) {
            direction = newdirection;
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
    Snake *snake;
    const char snake_body = 'O';
    Point food;
    const char FOOD = 'o';
    int score;

public:
    Board() {
        spawnFOOD();
        snake = new Snake(10, 10);
        score = 0;
    }

    ~Board() {
        delete snake;
    }

    int getscore() {
        return score;
    }

    void spawnFOOD() {
        int x = rand() % (width - 2) + 1; // avoid placing food on the wall
        int y = rand() % (height - 2) + 1; // avoid placing food on the wall
        food = Point(x, y);
    }

    void displayscore() {
        gotoxy(width / 2, 0);
        cout << "Current Score : " << score;
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void draw() {
        system("cls");

        
        for (int i = 0; i < width; i++) {
            gotoxy(i, 0);
            cout << "#"; 
            gotoxy(i, height - 1);
            cout << "#"; 
        }
        for (int i = 0; i < height; i++) {
            gotoxy(0, i);
            cout << "#"; 
            gotoxy(width - 1, i);
            cout << "#"; 
        }

       
        for (int i = 0; i < snake->getlength(); i++) {
            gotoxy(snake->body[i].xcoord, snake->body[i].ycoord);
            cout << snake_body;
        }

       
        gotoxy(food.xcoord, food.ycoord);
        cout << FOOD;

        
        displayscore();
    }

    bool update() {
        bool isalive = snake->move(food);
        if (isalive == false) {
            return false; 
        }
        if (food.xcoord == snake->body[0].xcoord && food.ycoord == snake->body[0].ycoord) {
            score++;
            spawnFOOD();
        }
        return true;
    }

    void getinput() {
        if (kbhit()) {
            int key = getch();
            if (key == 'w' || key == 'W') {
                snake->Changedirection(DIR_UP);
            } else if (key == 's' || key == 'S') {
                snake->Changedirection(DIR_DOWN);
            } else if (key == 'A' || key == 'a') {
                snake->Changedirection(DIR_LEFT);
            } else if (key == 'd' || key == 'D') {
                snake->Changedirection(DIR_RIGHT);
            }
        }
    }
};

int main() {
    srand(time(0));
    initialize_screen();
    Board* board = new Board();
    while (board->update()) {
        board->getinput();
        board->draw();

        Sleep(100); 
    }
    cout << endl << "GAME OVER" << endl;
    cout << "Final Score: " << board->getscore() << endl;
    return 0;
}