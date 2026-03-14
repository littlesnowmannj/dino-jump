#include <iostream>
#include <vector>
#include <termios.h>
#include <random>
#include <thread>
#include <chrono>
#include <sys/select.h>
#include <unistd.h>

using namespace std;

void initTerminal() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void resetTerminal() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void game() {
    bool dino_type = 0; 
    int jump_frame = 0; 
    int score = 0;
    int nearest_obstacle = 48; 
    vector<char> screen(48, ' '); 
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 9); 

    initTerminal(); 

    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeval timeout{0, 100000}; 

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
            char key;
            read(STDIN_FILENO, &key, 1);
            if (key == ' ' && dino_type == 0) {
                dino_type = 1;
                jump_frame = 4; 
            }
        }
        score++;
        if (dino_type == 1) {
            jump_frame--;
            if (jump_frame <= 0) {
                dino_type = 0;
            }
        }
        if (dis(gen) == 1 && screen.back() == ' ') {
            screen.back() = 'x';
            nearest_obstacle = 47;
        }
        for (int i = 0; i < 47; i++) { 
            screen[i] = screen[i + 1];
        }
        screen[47] = ' ';
        for (int i = 0; i < 48; i++) {
            if (screen[i] == 'x') {
                nearest_obstacle = i;
                break;
            }
        }
        if (nearest_obstacle == 0 && dino_type == 0) {
            break;
        }
        system("clear");
        cout << "Score: " << score << endl;
        cout << "-----------------------------------------------" << endl;
        if (dino_type == 0) {
            cout <<endl<< "O" ;
        } else {; 
            cout <<"O" << endl;
        }
        for (char c : screen) {
            cout << c;
        }
        cout << endl;
        cout << "-----------------------------------------------" << endl;
        this_thread::sleep_for(chrono::milliseconds(40));
    }
    resetTerminal();
    system("clear");
    cout << "Game Over!!!" << endl;
    cout << "Score: " << score << endl;
    cout << "Version 1.0 for Linux" << endl;
    cout << "by littlesnowman and AI 14/2/2026" << endl;
}

int main() {
    cout << "Welcome to the game, it's like Chrome Dino" << endl;
    cout << "Game will start in 3 seconds..." << endl;
    this_thread::sleep_for(chrono::seconds(3));
    system("clear");
    game();
    return 0;
}