#include "Structures.h"

namespace nsGame::nsParameters
{
	Ball::Ball(int x, int y, int num)
	{
		row = x;
		col = y;
		number = num;
	}

	void Ball::setRow(int x)
	{
		row = x;
	}

	int Ball::getRow() const
	{
		return row;
	}

	void Ball::setCol(int y)
	{
		col = y;
	}

	int Ball::getCol() const
	{
		return col;
	}

	void Ball::setNumber(int num)
	{
		number = num;
	}

	int Ball::getNumber() const
	{
		return number;
	}

	Wall::Wall(int row, int col, int row2, int col2)
	{
		row_first = row;
		col_first = col;
		row_sec = row2;
		col_sec = col2;
	}

	int Wall::getRowFirst() const
	{
		return row_first;
	}

	int Wall::getRowSec() const
	{
		return row_sec;
	}

	int Wall::getColFirst() const
	{
		return col_first;
	}

	int Wall::getColSec() const
	{
		return col_sec;
	}
}
