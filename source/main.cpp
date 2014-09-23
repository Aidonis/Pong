/*
const char* GAME_NAME = Super Pang
const char* NAME = Aidan Nabass
const char* EMAIL = aidan.nabass@students.aie.edu.au

//Description time:
	Super Pang is a game created to be a clone of Pong.
	With 2 paddles and 1 ball the game resembles table tennis; pitting 2 players against each other in an attempt to score a point on the other.
	Player 1 is on the left and is controlled by the 'W' and 'S' keys.
	Player 2 is on the right and is controlled by the 'Up_Arrow' and 'Down_Arrow' keys.
	The main menu can be accessed at any time with 'F1'
	The biggest loser screen can be accessed from the main menu with 'F2'
*/

#include "AIE.h"
#include <iostream>
#include <fstream>


//Screen values
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
const float X_CENTER = SCREEN_WIDTH / 2;
const float Y_CENTER = SCREEN_HEIGHT / 2;

//Keycodes
const int ESC_KEYCODE = 256;
const int ENTER_KEYCODE = 257;
const int F1_KEYCODE = 290;
const int F2_KEYCODE = 291;
const int UP_ARROW_KEYCODE = 265;
const int DOWN_ARROW_KEYCODE = 264;
const int MAX_CHAR_COUNT = 10;

//Speeds
const int PADDLE_SPEED = 200;
const float BALL_SPEED = 260.f;

//Main Menu
const char* MAIN_MENU_START_TEXT = "Press <Enter> to Start";
const char* MAIN_MENU_WELCOME = "Welcome to Super Pang!";
const char* F1_TO_BIG_LOSER = "Press <F2> to check out biggest loser";
const char* ESC_TO_QUIT = "Press <Esc> to Quit";

//End Game Screen
const char* PLAYER_1_WINS = "Player 1 wins!";
const char* PLAYER_2_WINS = "Player 2 wins!";

//High Score Menu
const char* HIGH_SCORE_TEXT = "The biggest loser with a score of :";
const char* HIGH_SCORE_LOG = "Biggestloser.pong";
const char* HIGH_SCORE_TO_MAIN = "Press <F1> to go to Main Menu";
const char* HIGH_SCORE_RESET = "Press <R> to reset biggest loser";

//Game sprite location
const char* SPRITE = "./images/purple.png";

//Initialize highscore
int highScore;

//Initialize char buffer
char buffer[MAX_CHAR_COUNT];

//Psuedo-Functions
void UpdateMainMenu();
void UpdateGamePlay(float a_deltaTime);
void UpdateEndGame();
void UpdateHighScore();
void loadFile();
void writeFile();
void InitializeGameObjects();
float GetLeft(float a_x, int a_width);
float GetRight(float a_x, int a_width);
float GetTop(float a_y, int a_height);
float GetBottom(float a_y, int a_height);
float GetDistance(float a_position1, float a_position2);

//Create paddle structure
struct Paddle {
	unsigned int spriteID;
	float xPos;
	float yPos;
	int width;
	int height;
	int score;
	unsigned int moveUp;
	unsigned int moveDown;
	unsigned int topBorder;
	unsigned int bottomBorder;

	void SetSize(int a_width, int a_height){
		width = a_width;
		height = a_height;
	};

	void SetMoveKeys(unsigned int a_moveUp, unsigned int a_moveDown){
		moveUp = a_moveUp;
		moveDown = a_moveDown;
	};

	void SetPostion(float a_x, float a_y){
		xPos = a_x;
		yPos = a_y;
	};

	//Paddle movement and border collision
	void Move(float a_deltaTime){
		if (IsKeyDown(moveUp)){
			if (yPos >= SCREEN_HEIGHT - (height / 2)){
				yPos = SCREEN_HEIGHT - (height / 2);
			}
			yPos += a_deltaTime * PADDLE_SPEED;
		}
		if (IsKeyDown(moveDown)){
			if (yPos <= (height / 2)){
				yPos = (height / 2);
			}
			yPos -= a_deltaTime * PADDLE_SPEED;
		}
	}

	//Helper functions
	float GetLeft(){
		return xPos - (width / 2);
	}

	float GetRight(){
		return xPos + (width / 2);
	}
	float GetTop(){
		return yPos + (height / 2);
	}

	float GetBottom(){
		return yPos - (height / 2);
	}

};

//Initialize players
Paddle player1;
Paddle player2;

//Create Ball structure
struct Ball {
	unsigned int spriteID;
	float xPos;
	float yPos;
	int width;
	int height;
	float xSpeed;
	float ySpeed;

	//Increment score
	void Score(){
		if (xPos >= SCREEN_WIDTH){
			player1.score++;
		}
		else {
			player2.score++;
		}
	}

	//Reset the ball position when out of bounds
	void Reset(){
		if (xPos >= SCREEN_WIDTH || xPos <= 0 || yPos > SCREEN_HEIGHT || yPos < 0){
			Score();
			xPos = X_CENTER;
			yPos = Y_CENTER;
			xSpeed *= -1;
		}
	}

	//Top-Bottom screen collision
	void yScreenCollision(){
		if (yPos >= SCREEN_HEIGHT - (height / 2) || yPos <= 0 + (height / 2)){
			ySpeed *= -1;
		}
	}

	//Helper functions
	float GetLeft(){
		return xPos - (width / 2);
	}

	float GetRight(){
		return xPos + (width / 2);
	}

	float GetTop(){
		return yPos + (height / 2);
	}

	float GetBottom(){
		return yPos - (height / 2);
	}

	float xChange(float a_deltaTime){
		return (a_deltaTime * xSpeed);
	}
	float yChange(float a_deltaTime){
		return (a_deltaTime * ySpeed);
	}
};

enum GAMESTATES{
	MAIN_MENU,
	GAMEPLAY,
	END,
	HIGH_SCORE,
};

//Initialize Ball
Ball ball;

//Initialize gamestate
GAMESTATES currentState = MAIN_MENU;

int main(int argc, char* argv[])
{
	Initialise(SCREEN_WIDTH, SCREEN_HEIGHT, false, "Super Pang");

	SetBackgroundColour(SColour(0, 0, 0, 255));

	InitializeGameObjects();

	//Game Loop
	do
	{

		ClearScreen();
		//Initialize time
		float deltaT = GetDeltaTime();
		//Load Highscores
		loadFile();

		switch (currentState){

		case MAIN_MENU:

			UpdateMainMenu();
			break;

		case GAMEPLAY:
			UpdateGamePlay(deltaT);
			break;

		case END:
			UpdateEndGame();
			break;

		case HIGH_SCORE:
			UpdateHighScore();

		default:
			break;
		}

	} while (!FrameworkUpdate());

	Shutdown();

	return 0;
}
void UpdateMainMenu(){
	// Draw UI elements
	DrawString(MAIN_MENU_WELCOME, X_CENTER - 150, Y_CENTER + 200);
	DrawString(MAIN_MENU_START_TEXT, X_CENTER - 200, Y_CENTER);
	DrawString(ESC_TO_QUIT, X_CENTER - 200, Y_CENTER - 40);
	DrawString(F1_TO_BIG_LOSER, X_CENTER - 200, Y_CENTER - 80);
	if (IsKeyDown(ENTER_KEYCODE)){
		currentState = GAMEPLAY;
	}
	if (IsKeyDown(F2_KEYCODE)){
		currentState = HIGH_SCORE;
	}
	if (IsKeyDown(ESC_KEYCODE)){
		Shutdown();
		exit(0);
	}

}

void UpdateHighScore(){
	
	DrawString(HIGH_SCORE_TEXT, X_CENTER - 190, Y_CENTER);
	DrawString(itoa(highScore, buffer, 10), X_CENTER + 225, Y_CENTER);
	DrawString(HIGH_SCORE_TO_MAIN, X_CENTER - 190, Y_CENTER - 40);
	DrawString(HIGH_SCORE_RESET, X_CENTER - 190, Y_CENTER - 80);
	
	//Change to Main_Menu
	if (IsKeyDown(F1_KEYCODE)){
		currentState = MAIN_MENU;
	}

	//Reset the score
	if (IsKeyDown('R')){
		highScore = 0;
		writeFile();
		loadFile();
	}
}

void UpdateEndGame(){
	if (player1.score > player2.score){
		if (player2.score >= highScore){
			highScore = player2.score;
		}
		DrawString(PLAYER_1_WINS, X_CENTER - 100, Y_CENTER);
	}
	else{
		if (player1.score >= highScore){
			highScore = player1.score;
		}
		DrawString(PLAYER_2_WINS, X_CENTER - 100, Y_CENTER);
	}
	DrawString(ESC_TO_QUIT, X_CENTER - 140, Y_CENTER - 40);
	DrawString(HIGH_SCORE_TEXT, X_CENTER - 190, Y_CENTER - 80);
	DrawString(itoa(highScore, buffer, 10), X_CENTER + 225, Y_CENTER - 80);
	
	//Track biggest loser
	writeFile();
	loadFile();

	//Change to Main_Menu
	if (IsKeyDown(F1_KEYCODE)){
		currentState = MAIN_MENU;
	}

	//Exit game
	if (IsKeyDown(ESC_KEYCODE)){
		Shutdown();
		exit(0);
	}
}

void UpdateGamePlay(float a_deltaTime){
	//Change to Main_Menu
	if (IsKeyDown(F1_KEYCODE)){
		currentState = MAIN_MENU;
	}

	//Draw initial sprites
	MoveSprite(ball.spriteID, ball.xPos, ball.yPos);
	DrawSprite(ball.spriteID);
	MoveSprite(player1.spriteID, player1.xPos, player1.yPos);
	DrawSprite(player1.spriteID);
	MoveSprite(player2.spriteID, player2.xPos, player2.yPos);
	DrawSprite(player2.spriteID);

	//Draw UI
	DrawString("VS", X_CENTER, SCREEN_HEIGHT * 0.9f);
	DrawString(itoa(player1.score, buffer, 10), SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.9f);
	DrawString(itoa(player2.score, buffer, 10), SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.9f);

	//Player Movement
	player1.Move(a_deltaTime);
	player2.Move(a_deltaTime);

	//Ball collision with top and bottom screen
	ball.yScreenCollision();

	//Ball X collision sides of paddle
		//Player 2
	if (ball.xPos - ball.xChange(a_deltaTime) < player2.GetLeft()){
		if (ball.GetBottom() <= player2.GetTop() && ball.GetTop() >= player2.GetBottom()){
			if (ball.GetRight() >= player2.GetLeft()){
				ball.xPos = player2.GetLeft() - (ball.width / 2);
				ball.xSpeed *= -1;
			}
		}
	}
		//Player 1
	if (ball.xPos - ball.xChange(a_deltaTime) > player1.GetRight()){
		if (ball.GetBottom() <= player1.GetTop() && ball.GetTop() >= player1.GetBottom()){
			if (ball.GetLeft() <= player1.GetRight()){
				ball.xPos = player1.GetRight() + (ball.width / 2);
				ball.xSpeed *= -1;
			}
		}
	}

	//Y Collision top of paddle
		//Player 2 Collision
	if (ball.GetBottom() - ball.yChange(a_deltaTime) > player2.GetTop()){
		if (ball.GetLeft() <= player2.GetRight() && ball.GetRight() >= player2.GetLeft()){
			if (ball.GetBottom() <= player2.GetTop()){
				ball.yPos = player2.GetTop() + (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}
		//Player 1 Collision
	if (ball.GetBottom() - ball.yChange(a_deltaTime) > player1.GetTop()){
		if (ball.GetLeft() <= player1.GetRight() && ball.GetRight() >= player1.GetLeft()){
			if (ball.GetBottom() <= player1.GetTop()){
				ball.yPos = player1.GetTop() + (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}

	//Y Collision bottom of paddle
		//Player 2 Collision
	if (ball.GetTop() - ball.yChange(a_deltaTime) < player2.GetBottom()){
		if (ball.GetLeft() <= player2.GetRight() && ball.GetRight() >= player2.GetLeft()){
			if (ball.GetTop() >= player2.GetBottom()){
				ball.yPos = player2.GetBottom() - (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}
		//Player 1 Collision
	if (ball.GetTop() - ball.yChange(a_deltaTime) < player1.GetBottom()){
		if (ball.GetLeft() <= player1.GetRight() && ball.GetRight() >= player1.GetLeft()){
			if (ball.GetTop() >= player1.GetBottom()){
				ball.yPos = player1.GetBottom() - (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}

	//Ball reset when out of bounds
	ball.Reset();

	//Ball standard movement
	ball.yPos += a_deltaTime * ball.ySpeed;
	ball.xPos += a_deltaTime * ball.xSpeed;

	if (player1.score >= 10 || player2.score >= 10){
		currentState = END;
	}
}

void writeFile(){
	std::fstream file;
	file.open(HIGH_SCORE_LOG, std::ios::out);
	file << highScore;
	file.close();
}

void loadFile(){

	std::fstream file;
	file.open(HIGH_SCORE_LOG, std::ios::in);

	//character buffer
	file.getline(buffer, 10);
	highScore = atoi(buffer);
	file.close();
}

void InitializeGameObjects(){
	//Initialize paddle
	player1.SetPostion(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT / 2);
	player1.SetSize(28, 120);
	player1.spriteID = CreateSprite(SPRITE, player1.width, player1.height, true);
	player1.score = 0;
	player1.SetMoveKeys('W', 'S');

	//Initialize paddle 2
	player2.SetPostion(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT / 2);
	player2.SetSize(28, 120);
	player2.spriteID = CreateSprite(SPRITE, player2.width, player2.height, true);
	player2.score = 0;
	player2.SetMoveKeys(UP_ARROW_KEYCODE, DOWN_ARROW_KEYCODE);

	//Initialize ball
	ball.xPos = X_CENTER;
	ball.yPos = Y_CENTER;
	ball.width = 20;
	ball.height = 20;
	ball.xSpeed = BALL_SPEED;
	ball.ySpeed = BALL_SPEED;
	ball.spriteID = CreateSprite(SPRITE, ball.width, ball.height, true);
}