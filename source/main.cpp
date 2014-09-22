#include "AIE.h"
#include <iostream>

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
const float X_CENTER = SCREEN_WIDTH / 2;
const float Y_CENTER = SCREEN_HEIGHT / 2;
const char* SPRITE = "./images/purple.png";

//Initialize ball speed
float speed = 150.f;



//Psuedo-Functions
void UpdateMainMenu();
void UpdateGamePlay(float a_deltaTime);
void UpdateEndGame();
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

struct Ball {
	unsigned int spriteID;
	float xPos;
	float yPos;
	int width;
	int height;
	float xSpeed;
	float ySpeed;

	void Reset(){
		if (xPos >= SCREEN_WIDTH || xPos <= 0){
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

//Initialize players and ball
Paddle player1;
Paddle player2;
Ball ball;

//Initialize gamestate
GAMESTATES currentState = MAIN_MENU;

int main(int argc, char* argv[])
{
	Initialise(SCREEN_WIDTH, SCREEN_HEIGHT, false, "My Awesome Game");

	SetBackgroundColour(SColour(0, 0, 0, 255));

	//Initialize paddle
	player1.SetPostion(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT / 2);
	player1.SetSize(20, 120);
	player1.spriteID = CreateSprite(SPRITE, player1.width, player1.height, true);
	player1.score = 0;
	player1.SetMoveKeys('W', 'S');

	//Initialize paddle 2
	player2.SetPostion(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT / 2);
	player2.SetSize(20, 120);
	player2.spriteID = CreateSprite(SPRITE, player2.width, player2.height, true);
	player2.score = 0;
	player2.SetMoveKeys(265, 264);

	//Initialize ball
	ball.xPos = X_CENTER;
	ball.yPos = Y_CENTER;
	ball.width = 32;
	ball.height = 32;
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
	DrawString("Player 1 Wins", X_CENTER - 100, Y_CENTER);
	DrawString("Press <Enter> to Play Again!", X_CENTER - 170, Y_CENTER - 40);

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

	//Initialize score buffer
	char score1[3];
	char score2[3];

	//Draw UI
	DrawString("VS", X_CENTER, SCREEN_HEIGHT * 0.9f);
	DrawString(itoa(player1.score, score1, 10), SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.9f);
	DrawString(itoa(player2.score, score2, 10), SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.9f);

	//Player Movement
	player1.Move(a_deltaTime);
	player2.Move(a_deltaTime);

	//Ball collision with top and bottom screen
	ball.yScreenCollision();
	/*if (ball.yPos >= SCREEN_HEIGHT - (ball.height / 2) || ball.yPos <= 0 + (ball.height / 2)){
		ball.ySpeed *= -1;
	}*/

	//Ball reset when out of bounds
	ball.Reset();
	if (ball.xPos - ball.xChange(a_deltaTime) < player2.GetLeft()){
		if (ball.GetBottom() <= player2.GetTop() && ball.GetTop() >= player2.GetBottom()){
			if (ball.xPos >= player2.GetLeft()){
				ball.xSpeed *= -1;
			}
		}
	}
	if (ball.xPos - ball.xChange(a_deltaTime) > player1.GetRight()){
		if (ball.GetBottom() <= player1.GetTop() && ball.GetTop() >= player1.GetBottom()){
			if (ball.xPos <= player1.GetRight()){
				ball.xSpeed *= -1;
			}
		}
	}

	//Y Collision Top
	if (ball.GetBottom() - ball.yChange(a_deltaTime) > player2.GetTop()){
		if (ball.GetLeft() <= player2.GetRight() && ball.GetRight() >= player2.GetLeft()){
			if (ball.GetBottom() <= player2.GetTop()){
				ball.ySpeed *= -1;
			}
		}
	}
	if (ball.GetBottom() - ball.yChange(a_deltaTime) > player1.GetTop()){
		if (ball.GetLeft() <= player1.GetRight() && ball.GetRight() >= player1.GetLeft()){
			if (ball.GetBottom() <= player1.GetTop()){
				ball.ySpeed *= -1;
			}
		}
	}

	//Y Collision Bottom
	if (ball.GetTop() - ball.yChange(a_deltaTime) < player2.GetBottom()){
		if (ball.GetLeft() <= player2.GetRight() && ball.GetRight() >= player2.GetLeft()){
			if (ball.GetTop() >= player2.GetBottom()){
				ball.ySpeed *= -1;
			}
		}
	}

	if (ball.GetTop() - ball.yChange(a_deltaTime) < player1.GetBottom()){
		if (ball.GetLeft() <= player1.GetRight() && ball.GetRight() >= player1.GetLeft()){
			if (ball.GetTop() >= player1.GetBottom()){
				ball.ySpeed *= -1;
			}
		}
	}


	//Ball standard movement
	ball.yPos += a_deltaTime * ball.ySpeed;
	ball.xPos += a_deltaTime * ball.xSpeed;

	if (player1.score >= 10 || player2.score >= 10){
		currentState = END;
	}
}


float GetDistance(float a_deltaTime){
	return a_deltaTime * speed;
}

void Collision(){

}