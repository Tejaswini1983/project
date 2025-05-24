#include <iostream>
#include <vector>
#include <unistd.h>   // usleep
#include <termios.h>  // terminal input
#include <fcntl.h>    // non-blocking input
#include <cstdlib>    // rand
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;
const char PLAYER = 'A';
const char OBSTACLE = '#';
const char ROAD = ' ';
int score = 0;
bool gameOver = false;

struct Position {
    int x, y;
};

// Terminal Input Setup
void enableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

void disableRawMode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char getch() {
    char buf = 0;
    read(0, &buf, 1);
    return buf;
}

class CarGame {
private:
    Position player;
    vector<Position> obstacles;

public:
    CarGame() {
        player.x = WIDTH / 2;
        player.y = HEIGHT - 2;
        srand(time(0));
    }

    void Draw() {
        system("clear"); // use 'cls' for Windows
        for (int i = 0; i < WIDTH + 2; i++) cout << "=";
        cout << "\n";
        for (int y = 0; y < HEIGHT; y++) {
            cout << "|";
            for (int x = 0; x < WIDTH; x++) {
                if (x == player.x && y == player.y) cout << PLAYER;
                else {
                    bool printed = false;
                    for (auto& ob : obstacles) {
                        if (ob.x == x && ob.y == y) {
                            cout << OBSTACLE;
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << ROAD;
                }
            }
            cout << "|\n";
        }
        for (int i = 0; i < WIDTH + 2; i++) cout << "=";
        cout << "\nScore: " << score << "\n";
    }

    void Input() {
        if (kbhit()) {
            char ch = getch();
            switch (ch) {
            case 'a':
                if (player.x > 0) player.x--;
                break;
            case 'd':
                if (player.x < WIDTH - 1) player.x++;
                break;
            case 'x':
                gameOver = true;
                break;
            }
        }
    }

    void Logic() {
        if (rand() % 10 < 2) {
            Position ob;
            ob.x = rand() % WIDTH;
            ob.y = 0;
            obstacles.push_back(ob);
        }

        for (int i = 0; i < obstacles.size(); i++) {
            obstacles[i].y++;
            if (obstacles[i].y >= HEIGHT) {
                obstacles.erase(obstacles.begin() + i);
                score++;
                i--;
            } else if (obstacles[i].x == player.x && obstacles[i].y == player.y) {
                gameOver = true;
            }
        }
    }

    void Run() {
        enableRawMode();
        while (!gameOver) {
            Draw();
            Input();
            Logic();
            usleep(100000); // 100 ms
        }
        disableRawMode();
        cout << "\nGame Over! Final Score: " << score << endl;
    }
};

int main() {
    CarGame game;
    game.Run();
    return 0;
}

