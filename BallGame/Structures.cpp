#include "Structures.h"

BallGame::Ball::Ball(int x, int y, int num)
{
	row = x;
	col = y;
	number = num;
}

void BallGame::Ball::setRow(int x)
{
	row = x;
}

int BallGame::Ball::getRow() const
{
	return row;
}

void BallGame::Ball::setCol(int y)
{
	col = y;
}

int BallGame::Ball::getCol() const
{
	return col;
}

void BallGame::Ball::setNumber(int num)
{
	number = num;
}

int BallGame::Ball::getNumber() const
{
	return number;
}

BallGame::Wall::Wall(int row, int col, int row2, int col2)
{
	row_first = row;
	col_first = col;
	row_sec = row2;
	col_sec = col2;
}

int BallGame::Wall::getRowFirst() const
{
	return row_first;
}

int BallGame::Wall::getRowSec() const
{
	return row_sec;
}

int BallGame::Wall::getColFirst() const
{
	return col_first;
}

int BallGame::Wall::getColSec() const
{
	return col_sec;
}
