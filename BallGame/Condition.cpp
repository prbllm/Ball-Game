#include "Condition.h"

// C++
#include <iostream>
#include <iterator>

namespace BallGame
{

Condition::Condition(const Condition& cond)
{
	this->balls_count = cond.balls_count;
	this->walls_count = cond.walls_count;
	this->holes_count = cond.holes_count;
	this->size = cond.size;

	std::list<Ball*>::const_iterator it_h;
	for (it_h = cond.holes.begin(); it_h != cond.holes.end(); ++it_h)
		setHole((*it_h)->getNumber(), (*it_h)->getRow(), (*it_h)->getCol());

	std::list<Ball*>::const_iterator it_b;
	for (it_b = cond.balls.begin(); it_b != cond.balls.end(); ++it_b)
		setBall((*it_b)->getNumber(), (*it_b)->getRow(), (*it_b)->getCol());

	this->walls = cond.walls;
	this->came_from = cond.came_from;
	this->answer = cond.answer;
}

Condition& Condition::operator=(const Condition& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	this->balls_count = rhs.balls_count;
	this->walls_count = rhs.walls_count;
	this->holes_count = rhs.holes_count;
	this->size = rhs.size;

	std::list<Ball*>::const_iterator it_h;
	for (it_h = rhs.holes.begin(); it_h != rhs.holes.end(); ++it_h)
		setHole((*it_h)->getNumber(), (*it_h)->getRow(), (*it_h)->getCol());

	std::list<Ball*>::const_iterator it_b;
	for (it_b = rhs.balls.begin(); it_b != rhs.balls.end(); ++it_b)
		setBall((*it_b)->getNumber(), (*it_b)->getRow(), (*it_b)->getCol());

	this->walls = rhs.walls;
	this->came_from = rhs.came_from;
	this->answer = rhs.answer;

	return *this;
}

Condition::Condition(int n_balls, int n_walls, int n_size)
{
	this->holes_count = n_balls;
	this->walls_count = n_walls;
	this->balls_count = n_balls;

	this->came_from = nullptr;
	this->size = n_size;
}

Condition::~Condition()
{
	std::list<Ball*>::iterator hole;
	for (hole = holes.begin(); hole != holes.end(); ++hole)
		delete *hole;

	std::list<Ball*>::iterator ball;
	for (ball = balls.begin(); ball != balls.end(); ++ball)
		delete *ball;
}

void Condition::addToAnswer(DirectionType type) noexcept
{
	switch (type)
	{
	case DirectionType::North:
		answer += 'N';
		break;
	case DirectionType::South:
		answer += 'S';
		break;
	case DirectionType::East:
		answer += 'E';
		break;
	case DirectionType::West:
		answer += 'W';
		break;
	default:
		std::cout << "Unknown direction type" << std::endl;
		break;
	}
}

void Condition::setAnswer(std::string ans)
{
	answer = ans;
}

std::string Condition::getAnswer() const
{
	return answer;
}

void Condition::setCameFrom(const Condition& cf) noexcept
{
	try
	{
		came_from = std::make_shared<Condition>(cf);
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create previous step condition. Exception: " << exception.what() << std::endl;
		came_from = nullptr;
	}
}

std::shared_ptr<Condition> Condition::getCameFrom() const noexcept
{
	return came_from;
}

void Condition::setBall(int num, int row, int col)
{
	Ball * ball = new Ball(row, col, num);
	balls.push_back(ball);
}

void Condition::setHole(int num, int row, int col)
{
	Ball * hole = new Ball(row, col, num);
	holes.push_back(hole);
}

void Condition::setWall(int row, int col, int row2, int col2) noexcept
{
	try
	{
		walls.emplace_back(std::make_shared<Wall>(row, col, row2, col2));
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create wall. Exception: " << exception.what() << std::endl;
	}
}

void Condition::deleteBall(int number)
{
	std::list<Ball*>::iterator ball;

	for (ball = balls.begin(); ball != balls.end(); ball++)
	{
		if ((*ball)->getNumber() == number)
		{
			delete *ball;
			ball = balls.erase(ball);
			--this->balls_count;
			if (ball == balls.end())
				break;
		}
	}
}

void Condition::deleteHole(int number)
{
	std::list<Ball*>::iterator hole;
	for (hole = holes.begin(); hole != holes.end(); ++hole)
		if ((*hole)->getNumber() == number)
		{
			delete *hole;
			hole = holes.erase(hole);
			--this->holes_count;
			if (hole == holes.end())
				break;
		}
}

int Condition::getBallsAndHolesCount() const noexcept
{
	return balls_count;
}

int Condition::getWallsCount() const noexcept
{
	return walls_count;
}

int Condition::getSize() const noexcept
{
	return size;
}

bool Condition::goNorth()
{
	// все 4 функции аналогичные, различия только по направлению движения
	std::list<Ball*>::iterator it_b;
	std::list<Ball>::const_iterator it_b_new;
	std::list<Ball*>::iterator it_h;
	int row_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> balls_old;
	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		balls_old.push_back(*(*it_b));

	// цикл по всем шарам
	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
	{
		row_new = 0;
		balls_count = 0;
		isWall = false;

		// ищутся шары до конца поля
		for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
		{
			if (((*it_b_new).getCol() == (*it_b)->getCol())
				&& ((*it_b_new).getRow() > (*it_b)->getRow()))
				++balls_count;
		}

		// цикл по стенам
		for (const auto& wall : walls)
		{
			// ищутся шары до стены
			if ((wall->getColFirst() == wall->getColSec()) && (wall->getColFirst() == (*it_b)->getCol())
				&& (wall->getRowFirst() >= (*it_b)->getRow()))
			{
				balls_count = 0;
				for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
				{
					if (((*it_b_new).getCol() == (*it_b)->getCol())
						&& ((*it_b_new).getRow() > (*it_b)->getRow())
						&& ((*it_b_new).getRow() <= wall->getRowFirst()))
					{
						++balls_count;
					}
				}
				// рассчитывается новая строка
				row_new = wall->getRowFirst() - balls_count;
				isWall = true;
			}
		}

		if (!isWall)
			row_new = size - 1 - balls_count;

		// цикл по лункам
		for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
		{
			if (((*it_h)->getCol() == (*it_b)->getCol()) && ((*it_h)->getRow() <= row_new) && ((*it_h)->getRow() > (*it_b)->getRow()))
			{
				// если есть лунка нужная на пути, запоминаем номер
				if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
				{
					numbers.push_back((*it_h)->getNumber());
					++del_count;
				}
				else
					return false;
			}
			if (it_h == holes.end())
				break;
		}
		if (it_b == balls.end())
			break;
		if (del_count == 0)
			(*it_b)->setRow(row_new);
	}

	// удаляем шары и лунки
	for (int i : numbers)
	{
		deleteBall(i);
		deleteHole(i);
	}

	// отсеивание лишних движений, которые не изменяют позицию
	if ((came_from != nullptr) &&
		(came_from->came_from != nullptr))
	{
		std::list<Ball*>::const_iterator iter;
		std::list<Ball*>::const_iterator iter_3;
		std::list<Ball*>::const_iterator iter_2;
		int count_3 = 0, count_2 = 0;
		for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
		{
			for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
			{
				if ((*iter)->getNumber() == (*iter_3)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_3)->getRow()) &&
						((*iter)->getCol() == (*iter_3)->getCol()))
						++count_3;
				}
			}
			for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
			{
				if ((*iter)->getNumber() == (*iter_2)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_2)->getRow()) &&
						((*iter)->getCol() == (*iter_2)->getCol()))
						++count_2;
				}
			}
		}
		if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
			((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
			return false;
	}
	addToAnswer(DirectionType::North);
	return true;
}

bool Condition::goSouth()
{
	std::list<Ball*>::iterator it_b;
	std::list<Ball>::const_iterator it_b_new;
	std::list<Ball*>::iterator it_h;
	int row_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> balls_old;
	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		balls_old.push_back(*(*it_b));

	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
	{
		row_new = 0;
		balls_count = 0;
		isWall = false;

		for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
		{
			if (((*it_b_new).getCol() == (*it_b)->getCol())
				&& ((*it_b_new).getRow() < (*it_b)->getRow()))
				++balls_count;
		}

		for (const auto& wall : walls)
		{
			if ((wall->getColFirst() == wall->getColSec()) && (wall->getColFirst() == (*it_b)->getCol())
				&& (wall->getRowSec() <= (*it_b)->getRow()))
			{
				balls_count = 0;
				for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
				{
					if (((*it_b_new).getCol() == (*it_b)->getCol())
						&& ((*it_b_new).getRow() < (*it_b)->getRow())
						&& ((*it_b_new).getRow() >= wall->getRowSec()))
					{
						++balls_count;
					}
				}
				row_new = wall->getRowSec() + balls_count;
				isWall = true;
			}
		}

		if (!isWall)
			row_new = balls_count;

		for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
		{
			if (((*it_h)->getCol() == (*it_b)->getCol()) && ((*it_h)->getRow() >= row_new) && ((*it_h)->getRow() < (*it_b)->getRow()))
			{
				if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
				{
					numbers.push_back((*it_h)->getNumber());
					++del_count;
				}
				else
					return false;
			}
			if (it_h == holes.end())
				break;
		}
		if (it_b == balls.end())
			break;
		if (del_count == 0)
			(*it_b)->setRow(row_new);
	}

	for (int i : numbers)
	{
		deleteBall(i);
		deleteHole(i);
	}

	if ((came_from != nullptr) &&
		(came_from->came_from != nullptr))
	{
		std::list<Ball*>::const_iterator iter;
		std::list<Ball*>::const_iterator iter_3;
		std::list<Ball*>::const_iterator iter_2;
		int count_3 = 0, count_2 = 0;
		for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
		{
			for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
			{
				if ((*iter)->getNumber() == (*iter_3)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_3)->getRow()) &&
						((*iter)->getCol() == (*iter_3)->getCol()))
						++count_3;
				}
			}

			for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
			{
				if ((*iter)->getNumber() == (*iter_2)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_2)->getRow()) &&
						((*iter)->getCol() == (*iter_2)->getCol()))
						++count_2;
				}
			}
		}
		if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
			((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
			return false;
	}
	addToAnswer(DirectionType::South);
	return true;
}

bool Condition::goEast()
{
	std::list<Ball*>::iterator it_b;
	std::list<Ball>::const_iterator it_b_new;
	std::list<Ball*>::iterator it_h;
	int col_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> balls_old;
	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		balls_old.push_back(*(*it_b));

	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
	{
		col_new = 0;
		balls_count = 0;
		isWall = false;

		for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
		{
			if (((*it_b_new).getRow() == (*it_b)->getRow())
				&& ((*it_b_new).getCol() < (*it_b)->getCol()))
				++balls_count;
		}

		for (const auto& wall : walls)
		{
			if ((wall->getRowFirst() == wall->getRowSec()) && (wall->getRowFirst() == (*it_b)->getRow())
				&& (wall->getColSec() <= (*it_b)->getCol()))
			{
				balls_count = 0;
				for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
				{
					if (((*it_b_new).getRow() == (*it_b)->getRow())
						&& ((*it_b_new).getCol() < (*it_b)->getCol())
						&& ((*it_b_new).getCol() >= wall->getColSec()))
					{
						++balls_count;
					}
				}
				col_new = wall->getColSec() - balls_count;
				isWall = true;
			}
		}

		if (!isWall)
			col_new = balls_count;

		for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
		{
			if (((*it_h)->getRow() == (*it_b)->getRow()) && ((*it_h)->getCol() >= col_new) && ((*it_h)->getCol() < (*it_b)->getCol()))
			{
				if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
				{
					numbers.push_back((*it_h)->getNumber());
					++del_count;
				}
				else
					return false;
			}
			if (it_h == holes.end())
				break;
		}
		if (it_b == balls.end())
			break;
		if (del_count == 0)
			(*it_b)->setCol(col_new);
	}

	for (int i : numbers)
	{
		deleteBall(i);
		deleteHole(i);
	}

	if ((came_from != nullptr) &&
		(came_from->came_from != nullptr))
	{
		std::list<Ball*>::const_iterator iter;
		std::list<Ball*>::const_iterator iter_3;
		std::list<Ball*>::const_iterator iter_2;
		int count_3 = 0, count_2 = 0;
		for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
		{
			for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
			{
				if ((*iter)->getNumber() == (*iter_3)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_3)->getRow()) &&
						((*iter)->getCol() == (*iter_3)->getCol()))
						++count_3;
				}
			}
			for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
			{
				if ((*iter)->getNumber() == (*iter_2)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_2)->getRow()) &&
						((*iter)->getCol() == (*iter_2)->getCol()))
						++count_2;
				}
			}
		}
		if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
			((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
			return false;
	}
	addToAnswer(DirectionType::East);
	return true;
}

bool Condition::goWest()
{
	std::list<Ball*>::iterator it_b;
	std::list<Ball>::const_iterator it_b_new;
	std::list<Ball*>::iterator it_h;
	int col_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> balls_old;
	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		balls_old.push_back(*(*it_b));

	for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
	{
		col_new = 0;
		balls_count = 0;
		isWall = false;

		for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
		{
			if (((*it_b_new).getRow() == (*it_b)->getRow())
				&& ((*it_b_new).getCol() > (*it_b)->getCol()))
				++balls_count;
		}

		for (const auto& wall : walls)
		{
			if ((wall->getRowFirst() == wall->getRowSec()) && (wall->getRowFirst() == (*it_b)->getRow())
				&& (wall->getColFirst() >= (*it_b)->getCol()))
			{
				balls_count = 0;
				for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
				{
					if (((*it_b_new).getRow() == (*it_b)->getRow())
						&& ((*it_b_new).getCol() > (*it_b)->getCol())
						&& ((*it_b_new).getCol() <= wall->getColFirst()))
					{
						++balls_count;
					}
				}
				col_new = wall->getColFirst() + balls_count;
				isWall = true;
			}
		}

		if (!isWall)
			col_new = size - 1 - balls_count;

		for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
		{
			if (((*it_h)->getRow() == (*it_b)->getRow()) && ((*it_h)->getCol() <= col_new) && ((*it_h)->getCol() > (*it_b)->getCol()))
			{
				if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
				{
					numbers.push_back((*it_h)->getNumber());
					++del_count;
				}
				else
					return false;
			}
			if (it_h == holes.end())
				break;
		}
		if (it_b == balls.end())
			break;
		if (del_count == 0)
			(*it_b)->setCol(col_new);
	}

	for (int i : numbers)
	{
		deleteBall(i);
		deleteHole(i);
	}

	if ((came_from != nullptr) &&
		(came_from->came_from != nullptr))
	{
		std::list<Ball*>::const_iterator iter;
		std::list<Ball*>::const_iterator iter_3;
		std::list<Ball*>::const_iterator iter_2;
		int count_3 = 0, count_2 = 0;
		for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
		{
			for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
			{
				if ((*iter)->getNumber() == (*iter_3)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_3)->getRow()) &&
						((*iter)->getCol() == (*iter_3)->getCol()))
						++count_3;
				}
			}
			for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
			{
				if ((*iter)->getNumber() == (*iter_2)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_2)->getRow()) &&
						((*iter)->getCol() == (*iter_2)->getCol()))
						++count_2;
				}
			}
		}
		if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
			((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
			return false;
	}
	addToAnswer(DirectionType::West);
	return true;
}

bool Condition::isFinish()
{
	if (balls_count != 1)
	{
		return false;
	}

	if ((came_from == nullptr) ||
		(came_from->came_from == nullptr) ||
		(came_from->came_from->came_from == nullptr) ||
		(came_from->came_from->came_from->came_from == nullptr))
	{
		return false;
	}

	if ((balls_count != came_from->balls_count) ||
		((balls_count != came_from->came_from->balls_count)) ||
		(balls_count != came_from->came_from->came_from->balls_count) ||
		(balls_count != came_from->came_from->came_from->came_from->balls_count))
	{
		return false;
	}

	std::list<Ball*>::const_iterator iter;
	std::list<Ball*>::const_iterator iter_3;
	std::list<Ball*>::const_iterator iter_4;
	int count_3 = 0, count_4 = 0;

	for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
	{
		for (iter_3 = came_from->came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->came_from->balls.cend(); ++iter_3)
		{
			if ((*iter)->getNumber() == (*iter_3)->getNumber())
			{
				if (((*iter)->getRow() == (*iter_3)->getRow()) &&
					((*iter)->getCol() == (*iter_3)->getCol()))
					++count_3;
			}
		}

		for (iter_4 = came_from->came_from->came_from->came_from->balls.cbegin(); iter_4 != came_from->came_from->came_from->came_from->balls.cend(); ++iter_4)
		{
			if ((*iter)->getNumber() == (*iter_4)->getNumber())
			{
				if (((*iter)->getRow() == (*iter_4)->getRow()) &&
					((*iter)->getCol() == (*iter_4)->getCol()))
					++count_4;
			}
		}
	}

	if ((count_3 == balls_count) || (count_4 == balls_count))
	{
		answer = "Impossible";
		return true;
	}
	else
	{
		return false;
	}
}

}