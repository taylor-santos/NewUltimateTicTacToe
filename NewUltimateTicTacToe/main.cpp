#include <Windows.h>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ctime>
#include "move.h"

#define alpha 150

static sf::Color playerColor0 = sf::Color::Red;
static sf::Color playerColor1 = sf::Color::Blue;
static sf::Color backgroundColor = sf::Color::White;
static sf::Color playableColor = sf::Color(255, 153, 0);
double boardSize = 0.8;
double gapSize = 0.01;

int main()
{
	srand(time(NULL));
	sf::Vector2f screenSize = sf::Vector2f(800, 800);
	sf::RenderWindow window(sf::VideoMode((int)screenSize.x, (int)screenSize.y), "Ultimate Tic-Tac-Toe");
	
	double boxSize = (boardSize - 10 * gapSize) / 9;

	sf::Vertex LongLines[2 * 4];
	int line_index = 0;
	for (int x = 0; x < 2; ++x)
	{
		LongLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x * ((1 - boardSize) / 2 + 3 * boxSize + 3 * gapSize + x * (3 * boxSize + 4 * gapSize)), screenSize.y * ((1 - boardSize) / 2 - gapSize*3)), sf::Color::Black);
		LongLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x * ((1 - boardSize) / 2 + 3 * boxSize + 3 * gapSize + x * (3 * boxSize + 4 * gapSize)), screenSize.y * (boardSize + (1 - boardSize) / 2 + gapSize*3)),sf::Color::Black);
	}
	for (int y = 0; y < 2; ++y)
	{
		LongLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x * ((1 - boardSize) / 2 - gapSize*3), screenSize.y * ((1 - boardSize) / 2 + 3 * boxSize + 3 * gapSize + y * (3 * boxSize + 4 * gapSize))), sf::Color::Black);
		LongLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x * (boardSize + (1 - boardSize) / 2 + gapSize*3), screenSize.y * ((1 - boardSize) / 2 + 3 * boxSize + 3 * gapSize + y * (3 * boxSize + 4 * gapSize))), sf::Color::Black);
	}

	sf::Vertex ShortLines[2 * 9 * 4];
	line_index = 0;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			sf::Vector2f corner = sf::Vector2f((1 - boardSize) / 2 - gapSize / 2 + gridX * (3*boxSize + 4 * gapSize), (1 - boardSize) / 2 - gapSize / 2 + +gridY * (3 * boxSize + 4 * gapSize));
			for (int x = 1; x < 3; ++x)
			{
				ShortLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x*(corner.x + x * (boxSize + gapSize)), screenSize.y*(corner.y)), sf::Color::Black);
				ShortLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x*(corner.x + x * (boxSize + gapSize)), screenSize.y*(corner.y + 3 * boxSize + 3 * gapSize)), sf::Color::Black);
			}
			for (int y = 1; y < 3; ++y)
			{
				ShortLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x*(corner.x), screenSize.y*(corner.y + y * (boxSize + gapSize))), sf::Color::Black);
				ShortLines[line_index++] = sf::Vertex(sf::Vector2f(screenSize.x*(corner.x + 3 * boxSize + 3 * gapSize), screenSize.y*(corner.y + y * (boxSize + gapSize))), sf::Color::Black);
			}
		}
	}

	sf::RectangleShape boxes[9][9];
	sf::Vector2<double> corner{ (1 - boardSize) / 2,(1 - boardSize) / 2 };
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			float oldCornerY = corner.y;
			for (int y = 0; y < 3; ++y)
			{
				float oldCornerX = corner.x;
				for (int x = 0; x < 3; ++x)
				{
					boxes[3 * gridX + x][3 * gridY + y].setSize(sf::Vector2f(screenSize.x * boxSize, screenSize.y * boxSize));
					boxes[3 * gridX + x][3 * gridY + y].setPosition(sf::Vector2f(corner.x * screenSize.x, corner.y * screenSize.y));
					boxes[3 * gridX + x][3 * gridY + y].setFillColor(playableColor);
					window.draw(boxes[3 * gridX + x][3 * gridY + y]);
					corner.x += boxSize + gapSize;
				}
				corner.y += boxSize + gapSize;
				corner.x = oldCornerX;
			}
			corner.y = oldCornerY;
			corner.x += 3 * boxSize + 4 * gapSize;
		}
		corner.x = (1 - boardSize) / 2;
		corner.y += 3 * boxSize + 4 * gapSize;
	}
	sf::RectangleShape grids[3][3];
	corner = sf::Vector2<double> ((1 - boardSize) / 2 - gapSize/2,(1 - boardSize) / 2 - gapSize/2);
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		float oldCornerX = corner.x;
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			grids[gridX][gridY].setSize(sf::Vector2f(screenSize.x * (3 * boxSize + 3 * gapSize), screenSize.y * (3 * boxSize + 3 * gapSize)));
			grids[gridX][gridY].setPosition(sf::Vector2f(corner.x * screenSize.x, corner.y * screenSize.y));
			grids[gridX][gridY].setFillColor(sf::Color(0, 0, 0, 0));
			corner.x += 3 * boxSize + 4 * gapSize;
		}
		corner.x = oldCornerX;
		corner.y += 3 * boxSize + 4 * gapSize;
	}

	int boxOwners[9][9];
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			boxOwners[i][j] = -1;
		}
	}

	Move* moves = new Move();
	bool player = false;
	bool mouseDown = false;
	int playableGrid = -1;
	int count;
	fillMoves(moves, boxOwners, playableGrid, player, player, 4, &count);
	
	//boxOwners[4][4] = 1;
	//playableGrid = 4;
	int HumanCount = -1;
	while (HumanCount < 0 || HumanCount > 2)
	{
		std::cout << "Number of human players (0-2) : ";
		std::cin >> HumanCount;
	}
	
	int depth = 0;
	if (HumanCount != 2)
	{
		while (depth < 1 || depth > 9)
		{
			std::cout << "Bot difficulty (1-8) : ";
			std::cin >> depth;
		}
	}

	if (HumanCount == 1)
	{
		int input = -1;
		while (input < 0 || input > 1)
		{
			std::cout << "First player (0-Human, 1-Bot) : ";
			std::cin >> input;
		}
		player = input;
	}
	int start_s = clock();
	int timeBank = 10000;
	int prevTime = timeBank;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(backgroundColor);

	/*Player*/
		if (player == false && HumanCount == 1 || HumanCount == 2)
		{
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
			sf::Vector2i mouseGrid = (sf::Vector2i)mousePos;
			mouseGrid.x -= screenSize.x * ((1 - boardSize) / 2);
			mouseGrid.y -= screenSize.y * ((1 - boardSize) / 2);
			mouseGrid.x /= screenSize.x * (3 * boxSize + 4 * gapSize);
			mouseGrid.y /= screenSize.y * (3 * boxSize + 4 * gapSize);

			mousePos.x -= screenSize.x * ((1 - boardSize) / 2);
			mousePos.y -= screenSize.y * ((1 - boardSize) / 2);
			while (mousePos.x > screenSize.x * (3 * boxSize + 4 * gapSize))
			{
				mousePos.x -= screenSize.x * (3 * boxSize + 4 * gapSize);
			}
			while (mousePos.y > screenSize.y * (3 * boxSize + 4 * gapSize))
			{
				mousePos.y -= screenSize.y * (3 * boxSize + 4 * gapSize);
			}
			mousePos.x /= screenSize.x * (boxSize + gapSize);
			mousePos.y /= screenSize.y * (boxSize + gapSize);

			double remainderX = mousePos.x;
			while (remainderX > 1)
			{
				remainderX -= 1;
			}
			double remainderY = mousePos.y;
			while (remainderY > 1)
				remainderY -= 1;
			if (remainderX > boxSize / (boxSize + gapSize))
				mousePos.x = -1;
			if (remainderY > boxSize / (boxSize + gapSize))
				mousePos.y = -1;
			if (mousePos.x < 0 || mousePos.x > 3)
				mousePos.x = -1;
			if (mousePos.y < 0 || mousePos.y > 3)
				mousePos.y = -1;
			if (mouseGrid.x < 0 || mouseGrid.x > 2)
				mouseGrid.x = -1;
			if (mouseGrid.y < 0 || mouseGrid.y > 2)
				mouseGrid.y = -1;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!mouseDown && mousePos.x != -1 && mousePos.y != -1 && mouseGrid.x != -1 && mouseGrid.y != -1)
				{
					if ((playableGrid == -1 && gridOwner(boxOwners,(int)mouseGrid.x,(int)mouseGrid.y) == -1) || playableGrid == 3 * (int)mouseGrid.y + (int)mouseGrid.x)
					{
						if (boxOwners[(3 * (int)mouseGrid.x + (int)mousePos.x)][(int)(3 * (int)mouseGrid.y + (int)mousePos.y)] == -1)
						{
							mouseDown = true;
							boxOwners[(3 * (int)mouseGrid.x + (int)mousePos.x)][(int)(3 * (int)mouseGrid.y + (int)mousePos.y)] = player;
							playableGrid = 3 * (int)mousePos.y + (int)mousePos.x;
							if (gridOwner(boxOwners, (int)mousePos.x, (int)mousePos.y) != -1)
							{
								playableGrid = -1;
							}
							moves = new Move();
							
							if (HumanCount != 2)
							{
								int count = 0;
								//int start_time = clock();
								fillMoves(moves, boxOwners, playableGrid, !player, !player, depth, &count);
								/*int stop_time = clock();
								timeBank -= (stop_time - start_time) / double(CLOCKS_PER_SEC) * 1000;
								timeBank += 500;
								std::cout << timeBank << "ms" << std::endl;
								if (timeBank - prevTime < 0)
								{
									depth--;
								}
								if (timeBank - prevTime > 0 && timeBank >= 20000)
								{
									depth++;
								}
								prevTime = timeBank;*/
								std::cout << "Depth: " << depth << std::endl;
								std::cout << count << " moves calculated." << std::endl;
							}
							//std::cout << (int)mouseGrid.x << "," << (int)mouseGrid.y << " " << (int)mousePos.x << "," << (int)mousePos.y << std::endl;
							//std::cout << "Next Grid: " << playableGrid;
							//std::cout << "Player " << player << " Score: " << getScore(boxOwners, player) << std::endl;
							player = !player;
						}
					}
				}
			}
			else {
				mouseDown = false;
			}
		}
	/*AI*/
		else if (boardWinner(boxOwners) == -1){
			std::cout << "Best score: " << moves->score << std::endl;
			for (int y = 0; y < 9; ++y)
			{
				for (int x = 0; x < 9; ++x)
				{
					std::cout << "[";
					if (moves->nextMoves[x][y] != NULL)
					{
						int score = moves->nextMoves[x][y]->score;
						if (score <= -1000)
						{
							std::cout << " MIN";
						}
						else if (score >= 1000) {
							std::cout << " MAX";
						}
						else {
							if (score >= 0)
							{
								std::cout << " ";
							}
							if (abs(score) < 10)
							{
								std::cout << "  " << score;
							}
							else if (abs(score) < 100)
							{
								std::cout << " " << score;
							}
							else if (abs(score) < 1000)
							{
								std::cout << score;
							}
						}
					}
					else
						std::cout << "    ";
					std::cout << "]";
				}
				std::cout << std::endl;
				if (y % 3 == 2)
					std::cout << std::endl;
			}
			int randIndex = rand() % moves->bestMovesX.size();
			std::cout << "Selected move (" << moves->bestMovesX[randIndex] << "," << moves->bestMovesY[randIndex] << ")" << std::endl;
			boxOwners[moves->bestMovesX[randIndex]][moves->bestMovesY[randIndex]] = player;
			playableGrid = 3 * (moves->bestMovesY[randIndex] % 3) + (moves->bestMovesX[randIndex] % 3);
			if (gridOwner(boxOwners, moves->bestMovesX[randIndex] % 3, moves->bestMovesY[randIndex] % 3) != -1)
			{
				playableGrid = -1;
			}
			delete moves;
			
			int stop_s = clock();
			//std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds" << std::endl;

			moves = new Move();
			if (HumanCount == 0)
			{
				//int start_time = clock();
				int count = 0;
				fillMoves(moves, boxOwners, playableGrid, !player, !player, depth, &count);
				/*int stop_time = clock();
				if (player == true)
				{
					timeBank -= (stop_time - start_time) / double(CLOCKS_PER_SEC) * 1000;
					timeBank += 500;
					std::cout << timeBank << "ms" << std::endl;
					if (timeBank < 10000)
					{
						depth--;
					}
					if (timeBank > 15000)
					{
						depth++;
					}
				}*/
				std::cout << count << " moves calculated." << std::endl;
			}
			
			player = !player;
			
		}

		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				for (int y = 0; y < 3; ++y)
				{
					for (int x = 0; x < 3; ++x)
					{
						switch (boxOwners[3 * gridX + x][3 * gridY + y])
						{
							case 0:
							{
								boxes[3 * gridX + x][3 * gridY + y].setFillColor(playerColor0);
								break;
							}
							case 1:
							{
								boxes[3 * gridX + x][3 * gridY + y].setFillColor(playerColor1);
								break;
							}
							default:
							{
								if (3*gridY+gridX == playableGrid || (playableGrid == -1 && gridOwner(boxOwners,gridX,gridY) == -1))
									boxes[3 * gridX + x][3 * gridY + y].setFillColor(playableColor);
								else
									boxes[3 * gridX + x][3 * gridY + y].setFillColor(sf::Color(0,0,0,0));
								break;
							}
						}
						window.draw(boxes[3 * gridX + x][3 * gridY + y]);
					}
				}
				int gridWinner = gridOwner(boxOwners, gridX, gridY);
				if (gridWinner == 0)
				{
					grids[gridX][gridY].setFillColor(sf::Color(playerColor0.r, playerColor0.g, playerColor0.b, alpha));
				}
				else if (gridWinner == 1)
				{
					grids[gridX][gridY].setFillColor(sf::Color(playerColor1.r, playerColor1.g, playerColor1.b, alpha));
				}
				else {
					grids[gridX][gridY].setFillColor(sf::Color(0, 0, 0, 0));
				}
				window.draw(grids[gridX][gridY]);
			}
		}
		
		window.draw(LongLines, sizeof(LongLines)/sizeof(sf::Vertex), sf::Lines);
		window.draw(ShortLines, sizeof(ShortLines) / sizeof(sf::Vertex), sf::Lines);

		window.display();
	}
}