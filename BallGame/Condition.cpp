#include "Condition.h"

// C++
#include <iostream>
#include <iterator>

namespace BallGame
{

Condition::Condition(const Condition& cond)
{
	for (const auto& ball : cond.balls)
	{
		this->balls.emplace_back(std::make_shared<Ball>(*ball));
	}

	this->size = cond.size;
	this->holes = cond.holes;
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

	for (const auto& ball : rhs.balls)
	{
		this->balls.emplace_back(std::make_shared<Ball>(*ball));
	}

	this->size = rhs.size;
	this->holes = rhs.holes;
	this->walls = rhs.walls;
	this->came_from = rhs.came_from;
	this->answer = rhs.answer;

	return *this;
}

Condition::Condition(int n_size)
{
	this->size = n_size;
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

void Condition::setBall(int num, int row, int col) noexcept
{
	try
	{
		balls.emplace_back(std::make_shared<Ball>(row, col, num));
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create ball. Exception: " << exception.what() << std::endl;
	}
}

void Condition::setHole(int num, int row, int col) noexcept
{
	try
	{
		holes.emplace_back(std::make_shared<Ball>(row, col, num));
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create hole. Exception: " << exception.what() << std::endl;
	}
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

void Condition::DeleteDataByNumber(int number) noexcept
{
	const auto run = [number](std::list<std::shared_ptr<Ball>>& list)
	{
		try
		{
			for (auto it = list.cbegin(); it != list.cend(); ++it)
			{
				if ((*it)->getNumber() == number)
				{
					list.erase(it);
					break;
				}
			}
		}
		catch (const std::exception& exception)
		{
			std::cout << "Unable to delete ball and hole by number (" << number << "). Exception: " << exception.what() << std::endl;
		}
	};

	run(balls);
	run(holes);
}

int Condition::getBallsAndHolesCount() const noexcept
{
	return balls.size();
}

int Condition::getSize() const noexcept
{
	return size;
}

bool Condition::goNorth()
{
	// все 4 функции аналогичные, различия только по направлению движения
	int row_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : balls)
	{
		oldBalls.emplace_back(*ball);
	}

	// цикл по всем шарам
	for (const auto& ball : balls)
	{
		row_new = 0;
		balls_count = 0;
		isWall = false;

		// ищутся шары до конца поля
		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.getCol() == ball->getCol() && oldBall.getRow() > ball->getRow())
			{
				++balls_count;
			}
		}

		// цикл по стенам
		for (const auto& wall : walls)
		{
			// ищутся шары до стены
			if (wall->getColFirst() == wall->getColSec() && wall->getColFirst() == ball->getCol() && wall->getRowFirst() >= ball->getRow())
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.getCol() == ball->getCol() && oldBall.getRow() > ball->getRow() && oldBall.getRow() <= wall->getRowFirst())
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
		{
			row_new = size - 1 - balls_count;
		}

		// цикл по лункам
		for (const auto& hole : holes)
		{
			if (hole->getCol() == ball->getCol() && (hole->getRow() <= row_new) && (hole->getRow() > ball->getRow()))
			{
				// если есть лунка нужная на пути, запоминаем номер
				if (hole->getNumber() == ball->getNumber() && balls_count == 0)
				{
					numbers.push_back(hole->getNumber());
					++del_count;
				}
				else
				{
					return false;
				}
			}
		}

		if (del_count == 0)
		{
			ball->setRow(row_new);
		}
	}

	// удаляем шары и лунки
	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	// отсеивание лишних движений, которые не изменяют позицию
	if (came_from && came_from->came_from)
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : balls)
		{
			for (const auto& oldBall : came_from->came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count2;
					}
				}
			}
		}

		if ((count3 == getBallsAndHolesCount() && getBallsAndHolesCount() == came_from->came_from->getBallsAndHolesCount()) ||
			count2 == getBallsAndHolesCount() && came_from->getBallsAndHolesCount() == getBallsAndHolesCount())
		{
			return false;
		}
	}
	addToAnswer(DirectionType::North);
	return true;
}

bool Condition::goSouth()
{
	int row_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : balls)
	{
		oldBalls.emplace_back(*ball);
	}

	for (const auto& ball : balls)
	{
		row_new = 0;
		balls_count = 0;
		isWall = false;

		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.getCol() == ball->getCol() && oldBall.getRow() < ball->getRow())
			{
				++balls_count;
			}
		}

		for (const auto& wall : walls)
		{
			if (wall->getColFirst() == wall->getColSec() && wall->getColFirst() == ball->getCol() && wall->getRowSec() <= ball->getRow())
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.getCol() == ball->getCol() && oldBall.getRow() < ball->getRow() && oldBall.getRow() >= wall->getRowSec())
					{
						++balls_count;
					}
				}
				row_new = wall->getRowSec() + balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			row_new = balls_count;
		}

		for (const auto& hole : holes)
		{
			if (hole->getCol() == ball->getCol() && hole->getRow() >= row_new && hole->getRow() < ball->getRow())
			{
				if (hole->getNumber() == ball->getNumber() && balls_count == 0)
				{
					numbers.push_back(hole->getNumber());
					++del_count;
				}
				else
				{
					return false;
				}
			}
		}

		if (del_count == 0)
		{
			ball->setRow(row_new);
		}
	}

	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if (came_from && came_from->came_from)
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : balls)
		{
			for (const auto& oldBall : came_from->came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count2;
					}
				}
			}
		}
		if ((count3 == getBallsAndHolesCount() && getBallsAndHolesCount() == came_from->came_from->getBallsAndHolesCount()) ||
			count2 == getBallsAndHolesCount() && came_from->getBallsAndHolesCount() == getBallsAndHolesCount())
		{
			return false;
		}
	}
	addToAnswer(DirectionType::South);
	return true;
}

bool Condition::goEast()
{
	int col_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : balls)
	{
		oldBalls.emplace_back(*ball);
	}

	for (const auto& ball : balls)
	{
		col_new = 0;
		balls_count = 0;
		isWall = false;

		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.getRow() == ball->getRow() && oldBall.getCol() < ball->getCol())
			{
				++balls_count;
			}
		}

		for (const auto& wall : walls)
		{
			if ((wall->getRowFirst() == wall->getRowSec()) && (wall->getRowFirst() == ball->getRow())
				&& (wall->getColSec() <= ball->getCol()))
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.getRow() == ball->getRow() && oldBall.getCol() < ball->getCol() && oldBall.getCol() >= wall->getColSec())
					{
						++balls_count;
					}
				}
				col_new = wall->getColSec() - balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			col_new = balls_count;
		}

		for (const auto& hole : holes)
		{
			if ((hole->getRow() == ball->getRow()) && (hole->getCol() >= col_new) && (hole->getCol() < ball->getCol()))
			{
				if ((hole->getNumber() == ball->getNumber()) && (balls_count == 0))
				{
					numbers.push_back(hole->getNumber());
					++del_count;
				}
				else
				{
					return false;
				}
			}
		}

		if (del_count == 0)
		{
			ball->setCol(col_new);
		}
	}

	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if (came_from && came_from->came_from)
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : balls)
		{
			for (const auto& oldBall : came_from->came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count2;
					}
				}
			}
		}
		if ((count3 == getBallsAndHolesCount() && getBallsAndHolesCount() == came_from->came_from->getBallsAndHolesCount()) ||
			count2 == getBallsAndHolesCount() && came_from->getBallsAndHolesCount() == getBallsAndHolesCount())
		{
			return false;
		}
	}
	addToAnswer(DirectionType::East);
	return true;
}

bool Condition::goWest()
{
	int col_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : balls)
	{
		oldBalls.emplace_back(*ball);
	}

	for (const auto& ball : balls)
	{
		col_new = 0;
		balls_count = 0;
		isWall = false;

		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.getRow() == ball->getRow() && oldBall.getCol() > ball->getCol())
			{
				++balls_count;
			}
		}

		for (const auto& wall : walls)
		{
			if (wall->getRowFirst() == wall->getRowSec() && wall->getRowFirst() == ball->getRow() && wall->getColFirst() >= ball->getCol())
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.getRow() == ball->getRow() && oldBall.getCol() > ball->getCol() && oldBall.getCol() <= wall->getColFirst())
					{
						++balls_count;
					}
				}
				col_new = wall->getColFirst() + balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			col_new = size - 1 - balls_count;
		}

		for (const auto& hole : holes)
		{
			if (hole->getRow() == ball->getRow() && hole->getCol() <= col_new && hole->getCol() > ball->getCol())
			{
				if (hole->getNumber() == ball->getNumber() && balls_count == 0)
				{
					numbers.push_back(hole->getNumber());
					++del_count;
				}
				else
				{
					return false;
				}
			}
		}

		if (del_count == 0)
		{
			ball->setCol(col_new);
		}
	}

	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if ((came_from != nullptr) &&
		(came_from->came_from != nullptr))
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : balls)
		{
			for (const auto& oldBall : came_from->came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : came_from->balls)
			{
				if (ball->getNumber() == oldBall->getNumber())
				{
					if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
					{
						++count2;
					}
				}
			}
		}
		if ((count3 == getBallsAndHolesCount() && getBallsAndHolesCount() == came_from->came_from->getBallsAndHolesCount()) ||
			count2 == getBallsAndHolesCount() && came_from->getBallsAndHolesCount() == getBallsAndHolesCount())
		{
			return false;
		}
	}
	addToAnswer(DirectionType::West);
	return true;
}

bool Condition::IsFinish()
{
	if (getBallsAndHolesCount() != 1)
	{
		return false;
	}

	if (!came_from || !came_from->came_from || !came_from->came_from->came_from || !came_from->came_from->came_from->came_from)
	{
		return false;
	}

	if (getBallsAndHolesCount() != came_from->getBallsAndHolesCount() ||
		getBallsAndHolesCount() != came_from->came_from->getBallsAndHolesCount() ||
		getBallsAndHolesCount() != came_from->came_from->came_from->getBallsAndHolesCount() ||
		getBallsAndHolesCount() != came_from->came_from->came_from->came_from->getBallsAndHolesCount())
	{
		return false;
	}

	int count3{ 0 }, count4{ 0 };

	for (const auto& ball : balls)
	{
		for (const auto& oldBall : came_from->came_from->came_from->balls)
		{
			if (ball->getNumber() == oldBall->getNumber())
			{
				if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
				{
					++count3;
				}
			}
		}

		for (const auto& oldBall : came_from->came_from->came_from->came_from->balls)
		{
			if (ball->getNumber() == oldBall->getNumber())
			{
				if (ball->getRow() == oldBall->getRow() && ball->getCol() == oldBall->getCol())
				{
					++count4;
				}
			}
		}
	}

	if (count3 == getBallsAndHolesCount() || count4 == getBallsAndHolesCount())
	{
		answer = "Impossible";
		return true;
	}
	return false;
}

}