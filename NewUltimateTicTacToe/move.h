#ifndef MOVE_H
#define MOVE_H

#include <vector>

struct Move {
	int score;
	int dist;
	std::vector<int> bestMovesX;
	std::vector<int> bestMovesY;
	bool maximizing;
	Move* nextMoves[9][9];
	Move()
	{
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 9; ++x)
			{
				nextMoves[x][y] = NULL;
			}
		}
	}
	~Move()
	{
	}
};

void fillMoves(Move* move, int board[9][9], int playableGrid, bool currPlayer, bool maximizingPlayer, int depth, int* count, bool naive);

int gridOwner(int board[9][9], int gridX, int gridY);

int boardWinner(int board[9][9]);

int getScore(int board[9][9], int player);

#endif