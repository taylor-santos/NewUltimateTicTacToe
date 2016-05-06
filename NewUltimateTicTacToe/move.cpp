#include <algorithm>
#include "move.h"

int gridOwner(int board[9][9], int gridX, int gridY)
{
	if (gridX < 0 || gridX > 2 || gridY < 0 || gridY > 2)
		return -1;
	for (int y = 0; y < 3; ++y)	//Horizontal
	{
		int player = board[3 * gridX][3 * gridY + y];
		if (player == 0 || player == 1)
		{
			if (player == board[3 * gridX + 1][3 * gridY + y] && player == board[3 * gridX + 2][3 * gridY + y])
			{
				return player;
			}
		}
	}
	for (int x = 0; x < 3; ++x)	//Vertical
	{
		int player = board[3 * gridX + x][3 * gridY];
		if (player == 0 || player == 1)
		{
			if (player == board[3 * gridX + x][3 * gridY + 1] && player == board[3 * gridX + x][3 * gridY + 2])
			{
				return player;
			}
		}
	}
	int player = board[3 * gridX + 1][3 * gridY + 1];
	if (player == 0 || player == 1)
	{
		if (player == board[3 * gridX][3 * gridY] && player == board[3 * gridX + 2][3 * gridY + 2])
			return player;
		if (player == board[3 * gridX + 2][3 * gridY] && player == board[3 * gridX][3 * gridY + 2])
			return player;
	}
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (board[3 * gridX + x][3 * gridY + y] == -1)
			{
				return -1;
			}
		}
	}
	return -2;
}

int boardWinner(int board[9][9])
{
	for (int y = 0; y < 3; ++y)	//Horizontal
	{
		int player = gridOwner(board,0,y);
		if (player >= 0)
		{
			if (player == gridOwner(board, 1, y) && player == gridOwner(board, 2, y))
			{
				return player;
			}
		}
	}
	for (int x = 0; x < 3; ++x)	//Vertical
	{
		int player = gridOwner(board, x, 0);
		if (player >= 0)
		{
			if (player == gridOwner(board, x, 1) && player == gridOwner(board, x, 2))
			{
				return player;
			}
		}
	}
	int player = gridOwner(board, 1, 1);
	if (player >= 0)
	{
		if (player == gridOwner(board, 0, 0) && player == gridOwner(board, 2, 2))
			return player;
		if (player == gridOwner(board, 0, 2) && player == gridOwner(board, 2, 0))
			return player;
	}
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (gridOwner(board, x, y) == -1)
			{
				return -1;
			}
		}
	}
	return -2;
}

void fillMoves(Move* move, int board[9][9], int playableGrid, bool currPlayer, bool maximizingPlayer, int depth, int* count)
{
	if (move == NULL)
		return;
	move->maximizing = currPlayer == maximizingPlayer;
	if (depth == 0 || boardWinner(board) != -1)
	{
		move->score = getScore(board, maximizingPlayer);
		move->dist = 0;
		return;
	}
	
	int gridXMin = 0, gridXMax = 2, gridYMin = 0, gridYMax = 2;
	if (playableGrid == -1)
	{
		//depth = 1;
	}
	else {
		gridXMin = playableGrid % 3;
		gridXMax = gridXMin;
		gridYMin = (playableGrid - gridXMin) / 3;
		gridYMax = gridYMin;
	}
	move->score = INT_MIN;
	if (currPlayer != maximizingPlayer)
		move->score = INT_MAX;
	move->dist = INT_MIN;
	for (int gridY = gridYMin; gridY <= gridYMax; ++gridY)
	{
		for (int gridX = gridXMin; gridX <= gridXMax; ++gridX)
		{
			if (gridOwner(board,gridX,gridY) == -1)
			{
				for (int y = 0; y < 3; ++y)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (board[3 * gridX + x][3 * gridY + y] == -1)
						{
							int tempGrid[9][9];
							std::memcpy(&tempGrid[0][0], &board[0][0], sizeof(int) * 9 * 9);
							tempGrid[3 * gridX + x][3 * gridY + y] = currPlayer;
							move->nextMoves[3 * gridX + x][3 * gridY + y] = new Move();
							int nextPlayableGrid = 3 * y + x;
							if (gridOwner(tempGrid, x, y) != -1)
								nextPlayableGrid = -1;
							(*count)++;
							//int board_win = boardWinner(tempGrid);
							//if (board_win == -1)
							//{
							fillMoves(move->nextMoves[3 * gridX + x][3 * gridY + y], tempGrid, nextPlayableGrid, !currPlayer, maximizingPlayer, depth - 1, count);
							for (int moveY = 0; moveY < 9; ++moveY)
							{
								for (int moveX = 0; moveX < 9; ++moveX)
								{
									delete (move->nextMoves[3 * gridX + x][3 * gridY + y]->nextMoves[moveX][moveY]);
								}
							}
							int nextScore = move->nextMoves[3 * gridX + x][3 * gridY + y]->score;
							if (currPlayer == maximizingPlayer)
							{
								if (nextScore == move->score)
								{
									move->bestMovesX.push_back(3 * gridX + x);
									move->bestMovesY.push_back(3 * gridY + y);
								}
								else if (nextScore > move->score)
								{
									move->score = nextScore;
									move->bestMovesX.clear();
									move->bestMovesY.clear();
									move->bestMovesX.push_back(3 * gridX + x);
									move->bestMovesY.push_back(3 * gridY + y);
								}
							}
							else {
								if (nextScore == move->score)
								{
									move->bestMovesX.push_back(3 * gridX + x);
									move->bestMovesY.push_back(3 * gridY + y);
								}
								else if (nextScore < move->score)
								{
									move->score = nextScore;
									move->bestMovesX.clear();
									move->bestMovesY.clear();
									move->bestMovesX.push_back(3 * gridX + x);
									move->bestMovesY.push_back(3 * gridY + y);
								}
							}
							//delete move->nextMoves[3 * gridX + x][3 * gridY + y];
							//}
							//else if (board_win == currPlayer) {
							//	move->nextMoves[3 * gridX + x][3 * gridY + y]->score = INT_MAX;
							//}
							//else {
							//	move->nextMoves[3 * gridX + x][3 * gridY + y]->score = INT_MIN;
							//}
						}
					}
				}
			}
		}
	}
}

int getScore(int board[9][9], int player)
{
	int winner = boardWinner(board);
	if (winner == player)
	{
		return INT_MAX;
	}
	else if (winner == !player)
	{
		return INT_MIN;
	}
	else {
		int score = 0;
		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				int gridWinner = gridOwner(board, gridX, gridY);
				if (gridWinner == player)
				{
					score += 20;
				}
				else if (gridWinner == !player)
				{
					score -= 20;
				}
				else if (gridWinner == -1)
				{
					for (int y = 0; y < 3; ++y)
					{
						for (int x = 0; x < 3; ++x)
						{
							if (board[3 * gridX + x][3 * gridY + y] == player)
							{
								score++;
							}else if (board[3 * gridX + x][3 * gridY + y] == !player)
							{
								score--;
							}
						}
					}
					int boxOwners[3][3];
					for (int y = 0; y < 3; ++y)
					{
						for (int x = 0; x < 3; ++x)
						{
							boxOwners[x][y] = board[3 * gridX + x][3 * gridY + y];
						}
					}
					for (int y = 0; y < 3; ++y)//Vertical two-in-a-row
					{
						int currPlayer = boxOwners[1][y];
						if (currPlayer >= 0)
						{
							if ((currPlayer == boxOwners[0][y] && boxOwners[2][y] == -1) ||
								(boxOwners[0][y] == -1 && currPlayer == boxOwners[2][y]))
							{
								if (currPlayer == player)
								{
									score++;
								}
								else {
									score--;
								}
							}
						}
						else {
							if (boxOwners[0][y] == boxOwners[2][y] && boxOwners[0][y] >= 0)
							{
								if (currPlayer == player)
								{
									score++;
								}
								else {
									score--;
								}
							}
						}
					}
					for (int x = 0; x < 3; ++x)//Horizontal two-in-a-row
					{
						int currPlayer = boxOwners[x][1];
						if (currPlayer >= 0)
						{
							if ((currPlayer == boxOwners[x][0] && boxOwners[x][2] == -1) ||
								(boxOwners[x][0] == -1 && currPlayer == boxOwners[x][2]))
							{
								if (currPlayer == player)
								{
									score++;
								}
								else {
									score--;
								}
							}
						}
						else {
							if (boxOwners[x][0] == boxOwners[x][2] && boxOwners[x][0] >= 0)
							{
								if (currPlayer == player)
								{
									score++;
								}
								else {
									score--;
								}
							}
						}
					}
					int currPlayer = boxOwners[1][1];
					if (currPlayer >= 0) //Corner diagonal two-in-a-row
					{
						if ((boxOwners[0][0] == currPlayer && boxOwners[2][2] == -1) ||
							(boxOwners[2][2] == currPlayer && boxOwners[0][0] == -1) ||
							(boxOwners[2][0] == currPlayer && boxOwners[0][2] == -1) ||
							(boxOwners[0][2] == currPlayer && boxOwners[2][0] == -1))
						{
							if (currPlayer == player)
							{
								score++;
							}
							else {
								score--;
							}
						}
					}
				}
			}
		}
		int gridOwners[3][3];
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				gridOwners[x][y] = gridOwner(board,x,y);
			}
		}
		for (int y = 0; y < 3; ++y)//Vertical two-in-a-row
		{
			int currPlayer = gridOwners[1][y];
			if (currPlayer >= 0)
			{
				if ((currPlayer == gridOwners[0][y] && gridOwners[2][y] == -1) ||
					(gridOwners[0][y] == -1 && currPlayer == gridOwners[2][y]))
				{
					if (currPlayer == player)
					{
						score += 10;
					}
					else {
						score -= 10;
					}
				}
			}
			else {
				if (gridOwners[0][y] == gridOwners[2][y] && gridOwners[0][y] >= 0)
				{
					if (currPlayer == player)
					{
						score += 10;
					}
					else {
						score -= 10;
					}
				}
			}
		}
		for (int x = 0; x < 3; ++x)//Horizontal two-in-a-row
		{
			int currPlayer = gridOwners[x][1];
			if (currPlayer >= 0)
			{
				if ((currPlayer == gridOwners[x][0] && gridOwners[x][2] == -1) ||
					(gridOwners[x][0] == -1 && currPlayer == gridOwners[x][2]))
				{
					if (currPlayer == player)
					{
						score+=10;
					}
					else {
						score-=10;
					}
				}
			}
			else {
				if (gridOwners[x][0] == gridOwners[x][2] && gridOwners[x][0] >= 0)
				{
					if (currPlayer == player)
					{
						score+=10;
					}
					else {
						score-=10;
					}
				}
			}
		}
		int currPlayer = gridOwners[1][1];
		if (currPlayer >= 0) //Corner diagonal two-in-a-row
		{
			if ((gridOwners[0][0] == currPlayer && gridOwners[2][2] == -1) ||
				(gridOwners[2][2] == currPlayer && gridOwners[0][0] == -1) ||
				(gridOwners[2][0] == currPlayer && gridOwners[0][2] == -1) ||
				(gridOwners[0][2] == currPlayer && gridOwners[2][0] == -1))
			{
				if (currPlayer == player)
				{
					score+=10;
				}
				else {
					score-=10;
				}
			}
		}

		return score;
	}
}