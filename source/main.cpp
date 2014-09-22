#include "AIE.h"
#include <iostream>

int screenWidth = 800;
int screenHeight = 600;
const float X_CENTER = screenWidth / 2;
const float Y_CENTER = screenHeight / 2;

//Initialize ball speed
float speed = 210.f;
float xBallSpeed = speed;
float yBallSpeed = speed;


//Psuedo-Functions
void UpdateMainMenu();
void UpdateGamePlay(float a_deltaTime);
void UpdateEndGame();

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
	};

};

struct Ball {
	unsigned int spriteID;
	float xPos;
	float yPos;
	int width;
	int height;
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
	Initialise(screenWidth, screenHeight, false, "My Awesome Game");

	SetBackgroundColour(SColour(0, 0, 0, 255));

	//Initialize paddle
	player1.SetPostion(screenWidth * 0.1f, screenHeight / 2);
	player1.SetSize(20, 120);
	player1.spriteID = CreateSprite("./images/crate_sideup.png", player1.width, player1.height, true);
	player1.score = 0;

	//Initialize paddle 2
	player2.SetPostion(screenWidth * 0.9f, screenHeight / 2);
	player2.SetSize(20, 120);
	player2.spriteID = CreateSprite("./images/crate_sideup.png", player2.width, player2.height, true);
	player2.score = 0;

	//Initialize ball
	ball.xPos = X_CENTER;
	ball.yPos = Y_CENTER;
	ball.width = 32;
	ball.height = 32;
	ball.spriteID = CreateSprite("./images/crate_sideup.png", ball.width, ball.height, true);


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
	DrawString("Press Enter to Start!", (screenWidth / 2) - 120, screenHeight / 2);
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
	DrawString("Player 1 wins", (screenWidth / 2) - 100, screenHeight / 2);
	DrawString("Press Enter to Play Again!", (screenWidth / 2) - 170, (screenHeight / 2) - 40);

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
	DrawString("VS", screenWidth / 2, screenHeight * 0.9f);
	DrawString(itoa(player1.score, score1, 10), screenWidth * 0.3f, screenHeight * 0.9f);
	DrawString(itoa(player2.score, score2, 10), screenWidth * 0.7f, screenHeight * 0.9f);


	//Player1 Movement
	if (IsKeyDown('W')){
		if (player1.yPos >= screenHeight - (player1.height / 2)){
			player1.yPos = screenHeight - (player1.height / 2);
		}
		player1.yPos += a_deltaTime * 200.f;
	}
	if (IsKeyDown('S')){
		if (player1.yPos <= (player1.height / 2)){
			player1.yPos = (player1.height / 2);
		}
		player1.yPos -= a_deltaTime * 200.f;
	}

	//Player2 Movement / Collision
	if (IsKeyDown(265)){
		if (player2.yPos >= screenHeight - (player2.height / 2)){
			player2.yPos = screenHeight - (player2.height / 2);
		}
		player2.yPos += a_deltaTime * 200.f;
	}
	if (IsKeyDown(264)){
		if (player2.yPos <= (player2.height / 2)){
			player2.yPos = (player2.height / 2);
		}
		player2.yPos -= a_deltaTime * 200.f;
	}

	//Ball collision with top and bottom screen
	if (ball.yPos >= screenHeight - (ball.height / 2) || ball.yPos <= 0 + (ball.height / 2)){
		yBallSpeed *= -1;
	}

	//Ball reset when out of bounds
	if (ball.xPos >= screenWidth || ball.xPos <= 0){
		if (ball.xPos >= screenWidth){
			player1.score++;
		}
		else{
			player2.score++;
		}
		ball.xPos = X_CENTER;
		ball.yPos = Y_CENTER;
		xBallSpeed *= -1;
	}

	//Ball collision with paddles
	//X Collision
	if (ball.xPos - (a_deltaTime * xBallSpeed) < player2.xPos - (ball.width / 2)){
		if (ball.yPos - (ball.height / 2) <= player2.yPos + (player2.height / 2) && ball.yPos + (ball.height / 2 ) >= player2.yPos - (player2.height / 2)){
			if (ball.xPos >= player2.xPos - (ball.width / 2)){
				xBallSpeed *= -1;
			}
		}
	}
	if (ball.xPos - (a_deltaTime * xBallSpeed) > player1.xPos + (ball.width / 2)){
		if (ball.yPos - (ball.height / 2) <= player1.yPos + (player1.height / 2) && ball.yPos + (ball.height / 2 ) >= player1.yPos - (player1.height / 2)){
			if (ball.xPos <= player1.xPos + (ball.width / 2)){
				xBallSpeed *= -1;
			}
		}
	}

	//Y Collision Top
	if (ball.yPos - (ball.height / 2) - (a_deltaTime * yBallSpeed) > player2.yPos + (player2.height / 2)){
		if (ball.xPos - (ball.width / 2) <= player2.xPos + (player2.width / 2) && ball.xPos + (ball.width / 2) >= player2.xPos - (player2.width / 2)){
			if ((ball.yPos - (ball.height / 2)) <= player2.yPos + (player2.height / 2)){
				yBallSpeed *= -1;
			}
		}
	}
	if (ball.yPos - (ball.height / 2) - (a_deltaTime * yBallSpeed) > player1.yPos + (player1.height / 2)){
		if (ball.xPos - (ball.width / 2) <= player1.xPos + (player1.width / 2) && ball.xPos + (ball.width / 2) >= player1.xPos - (player1.width / 2)){
			if ((ball.yPos - (ball.height / 2)) <= player1.yPos + (player1.height / 2)){
				yBallSpeed *= -1;
			}
		}
	}

	//Y Collision Bottom
	if (ball.yPos + (ball.height / 2) - (a_deltaTime * yBallSpeed) < player2.yPos - (player2.height / 2)){
		if (ball.xPos - (ball.width / 2) <= player2.xPos + (player2.width / 2) && ball.xPos + (ball.width / 2) >= player2.xPos - (player2.width / 2)){
			if ((ball.yPos + (ball.height / 2)) >= player2.yPos - (player2.height / 2)){
				yBallSpeed *= -1;
			}
		}
	}

	if (ball.yPos + (ball.height / 2) - (a_deltaTime * yBallSpeed) < player1.yPos - (player1.height / 2)){
		if (ball.xPos - (ball.width / 2) <= player1.xPos + (player1.width / 2) && ball.xPos + (ball.width / 2) >= player1.xPos - (player1.width / 2)){
			if ((ball.yPos + (ball.height / 2)) >= player1.yPos - (player1.height / 2)){
				yBallSpeed *= -1;
			}
		}
	}


	//Ball standard movement
	ball.yPos += a_deltaTime * yBallSpeed;
	ball.xPos += a_deltaTime * xBallSpeed;

	if (player1.score >= 10 || player2.score >= 10){
		currentState = END;
	}
}

void Collision(){

}