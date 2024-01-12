#include "Structures.h"

namespace ball_game {

Ball::Ball(int x, int y, int num)
{
	m_row = x;
	m_col = y;
	m_number = num;
}

void Ball::SetRow(int x) noexcept
{
	m_row = x;
}

int Ball::GetRow() const noexcept
{
	return m_row;
}

void Ball::SetCol(int y) noexcept
{
	m_col = y;
}

int Ball::GetCol() const noexcept
{
	return m_col;
}

void Ball::SetNumber(int num) noexcept
{
	m_number = num;
}

int Ball::GetNumber() const noexcept
{
	return m_number;
}

Wall::Wall(int row, int col, int row2, int col2)
{
	m_rowFirst = row;
	m_colFirst = col;
	m_rowSec = row2;
	m_colSec = col2;
}

int Wall::GetRowFirst() const noexcept
{
	return m_rowFirst;
}

int Wall::GetRowSec() const noexcept
{
	return m_rowSec;
}

int Wall::GetColFirst() const noexcept
{
	return m_colFirst;
}

int Wall::GetColSec() const noexcept
{
	return m_colSec;
}
}
