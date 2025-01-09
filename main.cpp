#include <iostream>
#include <raylib.h>
#include <string>
using namespace std;

#define LEFT "LEFT"
#define RIGHT "RIGHT"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int padleWidth = 10;
const int padleHieght = 80;
const int padleSpeed = 10;


class Player {

    string pos;    
    float padleX;
    float padleY;
    int score;

    public:

        Player(string pos): score(0) {
            this->padleY = (SCREEN_HEIGHT / 2) - (padleHieght / 2);
            this->pos = pos;
            if(this->pos == LEFT) {
                padleX = 0;
            } else if(this->pos == RIGHT) {
                padleX = SCREEN_WIDTH - padleWidth;
            }
        }

        int getScore() {
            return this->score;
        }

        float getPadleXCord() {
            return this->padleX;
        }

        float getPadleYCord() {
            return this->padleY;
        }

        void updatePadleYCord(int y) {
            if(y + (padleHieght / 2) < 0 || y + (padleHieght / 2) > SCREEN_HEIGHT) {
                return;
            }
            this->padleY = y;
        }

        void increaseScore() {
            this->score += 1;
        }

        void DrawPadle() {
            Rectangle rec = {this->padleX, this->padleY, padleWidth, padleHieght};
            // DrawRectangle(this->padleX, this->padleY, padleWidth, padleHieght, WHITE);
            DrawRectangleRounded(rec, 1, 50, WHITE);
        }

        void printScore() {

            int margin = 50;
            int fontSize = 50;

            int x, y = margin;
            if(this->pos == LEFT) {
                x = margin;
            } else if(this->pos == RIGHT) {
                x = SCREEN_WIDTH - margin - fontSize/2;
            }

            DrawText(to_string(this->score).c_str(), x, y, fontSize, WHITE);
        }

        void DrawPlayerDetails() {
            this->printScore();
            this->DrawPadle();
        }

        void keyBind() {
            KeyboardKey up;
            KeyboardKey down;
            int nextPos = 0;


            if(this->pos == LEFT) {
                up = KEY_W;
                down = KEY_S;
            } else if(this->pos == RIGHT) {
                up = KEY_UP;
                down = KEY_DOWN;
            }

            if(IsKeyDown(up)) {
                nextPos = this->padleY - padleSpeed;
                this->updatePadleYCord(nextPos);
            }

            if(IsKeyDown(down)) {
                nextPos = this->padleY + padleSpeed;
                this->updatePadleYCord(nextPos);
            }
        }
};

class Ball {
    float x;
    float y;
    float xVelocity;
    float yVelocity;
    int ballReduis;
    float increaseVilocityFactor = 0.5;

    public:

        Ball(): x(SCREEN_WIDTH/2), y(SCREEN_HEIGHT/2), ballReduis(5), xVelocity(2.0),  yVelocity(2.0) { }

        void draw() {
            DrawCircle(this->x - ballReduis, this->y - ballReduis, ballReduis * 2, WHITE);
        }

        void resetBallPos() {
            this->x = SCREEN_WIDTH / 2;
            this->y = SCREEN_HEIGHT / 2;
        }

        void resetVilocity() {
            this->xVelocity = 2.0;
            this->yVelocity = 2.0;
        }

        void setXVelocity(float xVelocity) {
            this->xVelocity = xVelocity;
        }

        void setYVelocity(float yVelocity) {
            this->yVelocity = yVelocity;
        }
        
        void increateVelocity() {

            if(this->xVelocity >= 0) {
                this->setXVelocity(this->xVelocity + this->increaseVilocityFactor);
            } else {
                this->setXVelocity(this->xVelocity - this->increaseVilocityFactor);               
            }

            if(this->yVelocity >= 0) {
                this->setYVelocity(this->yVelocity + this->increaseVilocityFactor);
            } else {
                this->setYVelocity(this->yVelocity - this->increaseVilocityFactor);
            }
        }

        void checkForCollision(Player & p1, Player & p2) {
            if(this->y < p1.getPadleYCord() || this->y > p1.getPadleYCord() + padleHieght) {
                p2.increaseScore();
                this->resetBallPos();
                this->resetVilocity();
            } else {
                this->increateVelocity();
                this->xVelocity = -this->xVelocity;
            }
        }

        void update(Player & p1, Player & p2) {
            
            float nextXCord = this->x + this->xVelocity;
            float nextYCord = this->y + this->yVelocity;

            if(nextXCord < padleWidth || nextXCord > SCREEN_WIDTH - padleWidth){
                if(nextXCord < padleWidth) {
                    checkForCollision(p1, p2);
                } else {
                    checkForCollision(p2, p1);
                }
            }

            if(nextYCord <= 0 || nextYCord > SCREEN_HEIGHT) {
                this->yVelocity = -this->yVelocity;
            }

            this->x += this->xVelocity;
            this->y += this->yVelocity;
        }
};

string checkWinner(Player & p1, Player & p2) {
    if(p1.getScore() == 7)
        return "Player 1";
    if(p2.getScore() == 7)
        return "player 2";
    return "";
}


int main () {


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayLibTuto");
    SetTargetFPS(60); 

    Player p1(LEFT);
    Player p2(RIGHT);
    Ball ball;

    while (!WindowShouldClose()){

        p1.keyBind();
        p2.keyBind();

        Color bgColor = {0, 200, 200, 255};
        ClearBackground(bgColor);

        BeginDrawing();
            p1.DrawPlayerDetails();
            p2.DrawPlayerDetails();
            ball.update(p1, p2);
            ball.draw();
            string res = checkWinner(p1, p2);
            if(res != "") {
                string s = res + " win.";
                DrawText(s.c_str(), SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 50, WHITE);
                break;
            }
        EndDrawing();

    }

    CloseWindow();
}