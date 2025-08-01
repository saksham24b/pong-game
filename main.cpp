#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
    float x,y;
    int speed_x,speed_y;
    int radius;

    void Draw(){
        DrawCircle(x,y,radius,WHITE);
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <=0){
            speed_y *= -1;
        }
        if(x + radius >= GetScreenWidth()){ //Cpu wins
            cpu_score++;
            ResetBall();
        }
        if(x - radius <=0){
            player_score++;
            ResetBall();
        }
    }

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle{
    protected:

    void LimitMovement(){
        if(y <= 0){
            y = 0;
        }
        if(y + height >=GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }
    public:
    float x,y;
    float width, height;
    int speed;
    int keyUp, keyDown;

    void Draw(){
        DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE);
    }

    void Update(){
        if(IsKeyDown(keyUp)){
            y = y - speed;
        }
        if(IsKeyDown(keyDown)){
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle: public Paddle{
    public:
    
    void Update(int ball_y){
        if(y + height/2 > ball_y){
            y = y - speed;
        }
        if(y + height/2 <= ball_y){
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player, player2;
CpuPaddle cpu;

enum GameState{ MENU, PLAYING_1, PLAYING_2};
GameState gameState = MENU;

int main () {

    cout << "Starting the game!" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width -10;
    player.y = screen_height/2 - player.height/2 -10;
    player.speed = 6;
    player.keyUp = KEY_UP;
    player.keyDown = KEY_DOWN;

    player2.height = 120;
    player2.width = 25;
    player2.x = 10;
    player2.y = screen_height/2 - cpu.height/2;
    player2.speed = 6;
    player2.keyUp = KEY_W;
    player2.keyDown = KEY_S;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;

    while(WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(BLACK);
        if(gameState == MENU){
            DrawText("PONG", screen_width/2 - 120, screen_height/2 - 100, 100, WHITE);
            DrawText("Press [SPACE] to Start", screen_width/2 - 220, screen_height/2 + 50, 40, GRAY);
            DrawText("[1] Player", screen_width/2 - 45, screen_height/2 + 100, 30, DARKGRAY);
            DrawText("[2] Player", screen_width/2 - 50, screen_height/2 + 140, 30, DARKGRAY);
            if(IsKeyPressed(KEY_SPACE)){
                gameState = PLAYING_1;
            }
            else if(IsKeyPressed(KEY_ONE)){
                gameState = PLAYING_1;
            }
            else if(IsKeyPressed(KEY_TWO)){
                gameState = PLAYING_2;
            }
            
        }
        else if (gameState == PLAYING_1){
            //Updating
            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            // Checking for collision
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }

            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
            {
                ball.speed_x *= -1;
            }

            // Drawing
            ball.Draw();
            cpu.Draw();
            player.Draw();
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            DrawText(TextFormat("%i", cpu_score), screen_width / 4, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4, 20, 80, WHITE);
            if(IsKeyPressed(KEY_M)){
                gameState = MENU;
                cpu_score = 0;
                player_score = 0;
        }
        }

        else if (gameState == PLAYING_2){
            //Updating
            ball.Update();
            player.Update();
            player2.Update();

            // Checking for collision
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }

            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.height}))
            {
                ball.speed_x *= -1;
            }

            // Drawing
            ball.Draw();
            player2.Draw();
            player.Draw();
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            DrawText(TextFormat("%i", cpu_score), screen_width / 4, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4, 20, 80, WHITE);
            if(IsKeyPressed(KEY_M)){
                gameState = MENU;
                cpu_score = 0;
                player_score = 0;
        }
        }

        

        EndDrawing();
    }

    CloseWindow();
}