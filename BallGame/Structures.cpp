#include "Structures.h"

nsBallGame::Ball::Ball(int x, int y, int num)
{
	row = x;
	col = y;
	number = num;
}

void nsBallGame::Ball::setRow(int x)
{
	row = x;
}

int nsBallGame::Ball::getRow() const
{
	return row;
}

void nsBallGame::Ball::setCol(int y)
{
	col = y;
}

int nsBallGame::Ball::getCol() const
{
	return col;
}

void nsBallGame::Ball::setNumber(int num)
{
	number = num;
}

int nsBallGame::Ball::getNumber() const
{
	return number;
}

nsBallGame::Wall::Wall(int row, int col, int row2, int col2)
{
	row_first = row;
	col_first = col;
	row_sec = row2;
	col_sec = col2;
}

int nsBallGame::Wall::getRowFirst() const
{
	return row_first;
}

int nsBallGame::Wall::getRowSec() const
{
	return row_sec;
}

int nsBallGame::Wall::getColFirst() const
{
	return col_first;
}

int nsBallGame::Wall::getColSec() const
{
	return col_sec;
}
