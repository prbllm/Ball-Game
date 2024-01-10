#include "Structures.h"

BallGame::Ball::Ball(int x, int y, int num)
{
	row = x;
	col = y;
	number = num;
}

void BallGame::Ball::setRow(int x) noexcept
{
	row = x;
}

int BallGame::Ball::getRow() const noexcept
{
	return row;
}

void BallGame::Ball::setCol(int y) noexcept
{
	col = y;
}

int BallGame::Ball::getCol() const noexcept
{
	return col;
}

void BallGame::Ball::setNumber(int num) noexcept
{
	number = num;
}

int BallGame::Ball::getNumber() const noexcept
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

int BallGame::Wall::getRowFirst() const noexcept
{
	return row_first;
}

int BallGame::Wall::getRowSec() const noexcept
{
	return row_sec;
}

int BallGame::Wall::getColFirst() const noexcept
{
	return col_first;
}

int BallGame::Wall::getColSec() const noexcept
{
	return col_sec;
}
