#include "AIE.h"
#include <iostream>
#include <fstream>



int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
const float X_CENTER = SCREEN_WIDTH / 2;
const float Y_CENTER = SCREEN_HEIGHT / 2;
const char* SPRITE = "./images/purple.png";

//Initialize ball speed
float speed = 150.f;

//Initialize highscore
int highScore = 0;

//Initialize char buffer
char buffer[10];

//Psuedo-Functions
void UpdateMainMenu();
void UpdateGamePlay(float a_deltaTime);
void UpdateEndGame();
void makeFile();
void writeFile();
float GetLeft(float a_x, int a_width);
float GetRight(float a_x, int a_width);
float GetTop(float a_y, int a_height);
float GetBottom(float a_y, int a_height);
float GetDistance(float a_position1, float a_position2);


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

	void SetBorder(unsigned int a_topBorder, unsigned int a_bottomBorder){
		topBorder = a_topBorder;
		bottomBorder = a_bottomBorder;
	};

	void SetMoveKeys(unsigned int a_moveUp, unsigned int a_moveDown){
		moveUp = a_moveUp;
		moveDown = a_moveDown;
	};

	void SetPostion(float a_x, float a_y){
		xPos = a_x;
		yPos = a_y;
	};

	void Move(float a_deltaTime){
		if (IsKeyDown(moveUp)){
			if (yPos >= SCREEN_HEIGHT - (height / 2)){
				yPos = SCREEN_HEIGHT - (height / 2);
			}
			yPos += a_deltaTime * 200.f;
		}
		if (IsKeyDown(moveDown)){
			if (yPos <= (height / 2)){
				yPos = (height / 2);
			}
			yPos -= a_deltaTime * 200.f;
		}
	}

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

	//Reset the ball position and increment score
	void Reset(){
		if (xPos >= SCREEN_WIDTH || xPos <= 0){
			Score();
			xPos = X_CENTER;
			yPos = Y_CENTER;
			xSpeed *= -1;
		}
	}

	void yScreenCollision(){
		if (yPos >= SCREEN_HEIGHT - (height / 2) || yPos <= 0 + (height / 2)){
			ySpeed *= -1;
		}
	}

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
};

//Initialize Ball
Ball ball;

//Initialize gamestate
GAMESTATES currentState = MAIN_MENU;

int main(int argc, char* argv[])
{
	Initialise(SCREEN_WIDTH, SCREEN_HEIGHT, false, "My Awesome Game");

	SetBackgroundColour(SColour(0, 0, 0, 255));

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
	player2.score = 8;
	player2.SetMoveKeys(265, 264);

	//Initialize ball
	ball.xPos = X_CENTER;
	ball.yPos = Y_CENTER;
	ball.width = 20;
	ball.height = 20;
	ball.xSpeed = speed;
	ball.ySpeed = speed;
	ball.spriteID = CreateSprite(SPRITE, ball.width, ball.height, true);


	//Game Loop
	do
	{

		ClearScreen();
		//Initialize time
		float deltaT = GetDeltaTime();

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

		default:
			break;
		}

	} while (!FrameworkUpdate());

	Shutdown();

	return 0;
}
void UpdateMainMenu(){
	DrawString("Press Enter to Start!", X_CENTER - 120, Y_CENTER);
	if (IsKeyDown(257)){
		currentState = GAMEPLAY;
	}
	if (IsKeyDown(290)){
		currentState = END;
	}
	if (IsKeyDown(256)){
		Shutdown();
		exit(0);
	}

}

void UpdateEndGame(){
	if (player1.score > player2.score){
		highScore = player1.score;
		DrawString("Player 1 Wins", X_CENTER - 100, Y_CENTER);
	}
	else{
		highScore = player2.score;
		DrawString("Player 2 Wins", X_CENTER - 100, Y_CENTER);
	}
	DrawString("Press <Esc> to quit!", X_CENTER - 140, Y_CENTER - 40);
	DrawString("The highscore is currently:", X_CENTER - 190, Y_CENTER - 80);
	DrawString(itoa(highScore,buffer,10), X_CENTER + 145, Y_CENTER - 80);

	
	writeFile();
	makeFile();

	if (IsKeyDown(256)){
		currentState = MAIN_MENU;
	}
}

void UpdateGamePlay(float a_deltaTime){
	if (IsKeyDown(256)){
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

	//Ball X collision
	if (ball.xPos - ball.xChange(a_deltaTime) < player2.GetLeft()){
		if (ball.GetBottom() <= player2.GetTop() && ball.GetTop() >= player2.GetBottom()){
			if (ball.GetRight() >= player2.GetLeft()){
				ball.xPos = player2.GetLeft() - (ball.width / 2);
				ball.xSpeed *= -1;
			}
		}
	}
	if (ball.xPos - ball.xChange(a_deltaTime) > player1.GetRight()){
		if (ball.GetBottom() <= player1.GetTop() && ball.GetTop() >= player1.GetBottom()){
			if (ball.GetLeft() <= player1.GetRight()){
				ball.xPos = player1.GetRight() + (ball.width / 2);
				ball.xSpeed *= -1;
			}
		}
	}

	//Y Collision Top
	if (ball.GetBottom() - ball.yChange(a_deltaTime) > player2.GetTop()){
		if (ball.GetLeft() <= player2.GetRight() && ball.GetRight() >= player2.GetLeft()){
			if (ball.GetBottom() <= player2.GetTop()){
				ball.yPos = player2.GetTop() + (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}
	if (ball.GetBottom() - ball.yChange(a_deltaTime) > player1.GetTop()){
		if (ball.GetLeft() <= player1.GetRight() && ball.GetRight() >= player1.GetLeft()){
			if (ball.GetBottom() <= player1.GetTop()){
				ball.yPos = player1.GetTop() + (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}

	//Y Collision Bottom
	if (ball.GetTop() - ball.yChange(a_deltaTime) < player2.GetBottom()){
		if (ball.GetLeft() <= player2.GetRight() && ball.GetRight() >= player2.GetLeft()){
			if (ball.GetTop() >= player2.GetBottom()){
				ball.yPos = player2.GetBottom() - (ball.height / 2);
				ball.ySpeed *= -1;
			}
		}
	}

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

	if (player1.score >= 9 || player2.score >= 9){
		currentState = END;
	}
}

void writeFile(){
	std::fstream file;
	file.open("Highscore.txt", std::ios::out);
	file << highScore;
	file.close();
}

void makeFile(){

	std::fstream file;
	file.open("Highscore.txt", std::ios::in);

	//character buffer
	file.getline(buffer, 10);
	highScore = atoi(buffer);
	file.close();
}