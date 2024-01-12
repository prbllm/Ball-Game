#include "Condition.h"

// C++
#include <iostream>

namespace ball_game
{

Condition::Condition(const Condition& cond)
{
	for (const auto& ball : cond.m_balls)
	{
		this->m_balls.emplace_back(std::make_shared<Ball>(*ball));
	}

	this->m_size = cond.m_size;
	this->m_holes = cond.m_holes;
	this->m_walls = cond.m_walls;
	this->m_cameFrom = cond.m_cameFrom;
	this->m_answer = cond.m_answer;
}

Condition& Condition::operator=(const Condition& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	for (const auto& ball : rhs.m_balls)
	{
		this->m_balls.emplace_back(std::make_shared<Ball>(*ball));
	}

	this->m_size = rhs.m_size;
	this->m_holes = rhs.m_holes;
	this->m_walls = rhs.m_walls;
	this->m_cameFrom = rhs.m_cameFrom;
	this->m_answer = rhs.m_answer;

	return *this;
}

Condition::Condition(int nSize)
{
	this->m_size = nSize;
}

void Condition::AddToAnswer(DirectionType type) noexcept
{
	switch (type)
	{
	case DirectionType::North:
		m_answer += 'N';
		break;
	case DirectionType::South:
		m_answer += 'S';
		break;
	case DirectionType::East:
		m_answer += 'E';
		break;
	case DirectionType::West:
		m_answer += 'W';
		break;
	default:
		std::cout << "Unknown direction type" << std::endl;
		break;
	}
}

std::string Condition::GetAnswer() const
{
	return m_answer;
}

void Condition::SetCameFrom(const Condition& cf) noexcept
{
	try
	{
		m_cameFrom = std::make_shared<Condition>(cf);
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create previous step condition. Exception: " << exception.what() << std::endl;
		m_cameFrom = nullptr;
	}
}

std::shared_ptr<Condition> Condition::GetCameFrom() const noexcept
{
	return m_cameFrom;
}

void Condition::SetBall(int num, int row, int col) noexcept
{
	try
	{
		m_balls.emplace_back(std::make_shared<Ball>(row, col, num));
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create ball. Exception: " << exception.what() << std::endl;
	}
}

void Condition::SetHole(int num, int row, int col) noexcept
{
	try
	{
		m_holes.emplace_back(std::make_shared<Ball>(row, col, num));
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to create hole. Exception: " << exception.what() << std::endl;
	}
}

void Condition::SetWall(int row, int col, int row2, int col2) noexcept
{
	try
	{
		m_walls.emplace_back(std::make_shared<Wall>(row, col, row2, col2));
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
				if ((*it)->GetNumber() == number)
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

	run(m_balls);
	run(m_holes);
}

int Condition::GetBallsAndHolesCount() const noexcept
{
	return m_balls.size();
}

int Condition::GetSize() const noexcept
{
	return m_size;
}

bool Condition::GoNorth()
{
	// все 4 функции аналогичные, различия только по направлению движения
	int row_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : m_balls)
	{
		oldBalls.emplace_back(*ball);
	}

	// цикл по всем шарам
	for (const auto& ball : m_balls)
	{
		row_new = 0;
		balls_count = 0;
		isWall = false;

		// ищутся шары до конца поля
		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.GetCol() == ball->GetCol() && oldBall.GetRow() > ball->GetRow())
			{
				++balls_count;
			}
		}

		// цикл по стенам
		for (const auto& wall : m_walls)
		{
			// ищутся шары до стены
			if (wall->GetColFirst() == wall->GetColSec() && wall->GetColFirst() == ball->GetCol() && wall->GetRowFirst() >= ball->GetRow())
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.GetCol() == ball->GetCol() && oldBall.GetRow() > ball->GetRow() && oldBall.GetRow() <= wall->GetRowFirst())
					{
						++balls_count;
					}
				}

				// рассчитывается новая строка
				row_new = wall->GetRowFirst() - balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			row_new = m_size - 1 - balls_count;
		}

		// цикл по лункам
		for (const auto& hole : m_holes)
		{
			if (hole->GetCol() == ball->GetCol() && (hole->GetRow() <= row_new) && (hole->GetRow() > ball->GetRow()))
			{
				// если есть лунка нужная на пути, запоминаем номер
				if (hole->GetNumber() == ball->GetNumber() && balls_count == 0)
				{
					numbers.push_back(hole->GetNumber());
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
			ball->SetRow(row_new);
		}
	}

	// удаляем шары и лунки
	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	// отсеивание лишних движений, которые не изменяют позицию
	if (m_cameFrom && m_cameFrom->m_cameFrom)
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : m_balls)
		{
			for (const auto& oldBall : m_cameFrom->m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count2;
					}
				}
			}
		}

		if ((count3 == GetBallsAndHolesCount() && GetBallsAndHolesCount() == m_cameFrom->m_cameFrom->GetBallsAndHolesCount()) ||
			count2 == GetBallsAndHolesCount() && m_cameFrom->GetBallsAndHolesCount() == GetBallsAndHolesCount())
		{
			return false;
		}
	}
	AddToAnswer(DirectionType::North);
	return true;
}

bool Condition::GoSouth()
{
	int row_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : m_balls)
	{
		oldBalls.emplace_back(*ball);
	}

	for (const auto& ball : m_balls)
	{
		row_new = 0;
		balls_count = 0;
		isWall = false;

		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.GetCol() == ball->GetCol() && oldBall.GetRow() < ball->GetRow())
			{
				++balls_count;
			}
		}

		for (const auto& wall : m_walls)
		{
			if (wall->GetColFirst() == wall->GetColSec() && wall->GetColFirst() == ball->GetCol() && wall->GetRowSec() <= ball->GetRow())
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.GetCol() == ball->GetCol() && oldBall.GetRow() < ball->GetRow() && oldBall.GetRow() >= wall->GetRowSec())
					{
						++balls_count;
					}
				}
				row_new = wall->GetRowSec() + balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			row_new = balls_count;
		}

		for (const auto& hole : m_holes)
		{
			if (hole->GetCol() == ball->GetCol() && hole->GetRow() >= row_new && hole->GetRow() < ball->GetRow())
			{
				if (hole->GetNumber() == ball->GetNumber() && balls_count == 0)
				{
					numbers.push_back(hole->GetNumber());
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
			ball->SetRow(row_new);
		}
	}

	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if (m_cameFrom && m_cameFrom->m_cameFrom)
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : m_balls)
		{
			for (const auto& oldBall : m_cameFrom->m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count2;
					}
				}
			}
		}
		if ((count3 == GetBallsAndHolesCount() && GetBallsAndHolesCount() == m_cameFrom->m_cameFrom->GetBallsAndHolesCount()) ||
			count2 == GetBallsAndHolesCount() && m_cameFrom->GetBallsAndHolesCount() == GetBallsAndHolesCount())
		{
			return false;
		}
	}
	AddToAnswer(DirectionType::South);
	return true;
}

bool Condition::GoEast()
{
	int col_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : m_balls)
	{
		oldBalls.emplace_back(*ball);
	}

	for (const auto& ball : m_balls)
	{
		col_new = 0;
		balls_count = 0;
		isWall = false;

		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.GetRow() == ball->GetRow() && oldBall.GetCol() < ball->GetCol())
			{
				++balls_count;
			}
		}

		for (const auto& wall : m_walls)
		{
			if ((wall->GetRowFirst() == wall->GetRowSec()) && (wall->GetRowFirst() == ball->GetRow())
				&& (wall->GetColSec() <= ball->GetCol()))
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.GetRow() == ball->GetRow() && oldBall.GetCol() < ball->GetCol() && oldBall.GetCol() >= wall->GetColSec())
					{
						++balls_count;
					}
				}
				col_new = wall->GetColSec() - balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			col_new = balls_count;
		}

		for (const auto& hole : m_holes)
		{
			if ((hole->GetRow() == ball->GetRow()) && (hole->GetCol() >= col_new) && (hole->GetCol() < ball->GetCol()))
			{
				if ((hole->GetNumber() == ball->GetNumber()) && (balls_count == 0))
				{
					numbers.push_back(hole->GetNumber());
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
			ball->SetCol(col_new);
		}
	}

	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if (m_cameFrom && m_cameFrom->m_cameFrom)
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : m_balls)
		{
			for (const auto& oldBall : m_cameFrom->m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count2;
					}
				}
			}
		}
		if ((count3 == GetBallsAndHolesCount() && GetBallsAndHolesCount() == m_cameFrom->m_cameFrom->GetBallsAndHolesCount()) ||
			count2 == GetBallsAndHolesCount() && m_cameFrom->GetBallsAndHolesCount() == GetBallsAndHolesCount())
		{
			return false;
		}
	}
	AddToAnswer(DirectionType::East);
	return true;
}

bool Condition::GoWest()
{
	int col_new = 0;
	int balls_count = 0;
	int del_count = 0;
	bool isWall = false;

	std::list<int> numbers;

	std::list<Ball> oldBalls;
	for (const auto& ball : m_balls)
	{
		oldBalls.emplace_back(*ball);
	}

	for (const auto& ball : m_balls)
	{
		col_new = 0;
		balls_count = 0;
		isWall = false;

		for (const auto& oldBall : oldBalls)
		{
			if (oldBall.GetRow() == ball->GetRow() && oldBall.GetCol() > ball->GetCol())
			{
				++balls_count;
			}
		}

		for (const auto& wall : m_walls)
		{
			if (wall->GetRowFirst() == wall->GetRowSec() && wall->GetRowFirst() == ball->GetRow() && wall->GetColFirst() >= ball->GetCol())
			{
				balls_count = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (oldBall.GetRow() == ball->GetRow() && oldBall.GetCol() > ball->GetCol() && oldBall.GetCol() <= wall->GetColFirst())
					{
						++balls_count;
					}
				}
				col_new = wall->GetColFirst() + balls_count;
				isWall = true;
			}
		}

		if (!isWall)
		{
			col_new = m_size - 1 - balls_count;
		}

		for (const auto& hole : m_holes)
		{
			if (hole->GetRow() == ball->GetRow() && hole->GetCol() <= col_new && hole->GetCol() > ball->GetCol())
			{
				if (hole->GetNumber() == ball->GetNumber() && balls_count == 0)
				{
					numbers.push_back(hole->GetNumber());
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
			ball->SetCol(col_new);
		}
	}

	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if ((m_cameFrom != nullptr) &&
		(m_cameFrom->m_cameFrom != nullptr))
	{
		int count3{ 0 }, count2{ 0 };
		for (const auto& ball : m_balls)
		{
			for (const auto& oldBall : m_cameFrom->m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count3;
					}
				}
			}

			for (const auto& oldBall : m_cameFrom->m_balls)
			{
				if (ball->GetNumber() == oldBall->GetNumber())
				{
					if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
					{
						++count2;
					}
				}
			}
		}
		if ((count3 == GetBallsAndHolesCount() && GetBallsAndHolesCount() == m_cameFrom->m_cameFrom->GetBallsAndHolesCount()) ||
			count2 == GetBallsAndHolesCount() && m_cameFrom->GetBallsAndHolesCount() == GetBallsAndHolesCount())
		{
			return false;
		}
	}
	AddToAnswer(DirectionType::West);
	return true;
}

bool Condition::IsFinish()
{
	if (GetBallsAndHolesCount() != 1)
	{
		return false;
	}

	if (!m_cameFrom || !m_cameFrom->m_cameFrom || !m_cameFrom->m_cameFrom->m_cameFrom || !m_cameFrom->m_cameFrom->m_cameFrom->m_cameFrom)
	{
		return false;
	}

	if (GetBallsAndHolesCount() != m_cameFrom->GetBallsAndHolesCount() ||
		GetBallsAndHolesCount() != m_cameFrom->m_cameFrom->GetBallsAndHolesCount() ||
		GetBallsAndHolesCount() != m_cameFrom->m_cameFrom->m_cameFrom->GetBallsAndHolesCount() ||
		GetBallsAndHolesCount() != m_cameFrom->m_cameFrom->m_cameFrom->m_cameFrom->GetBallsAndHolesCount())
	{
		return false;
	}

	int count3{ 0 }, count4{ 0 };

	for (const auto& ball : m_balls)
	{
		for (const auto& oldBall : m_cameFrom->m_cameFrom->m_cameFrom->m_balls)
		{
			if (ball->GetNumber() == oldBall->GetNumber())
			{
				if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
				{
					++count3;
				}
			}
		}

		for (const auto& oldBall : m_cameFrom->m_cameFrom->m_cameFrom->m_cameFrom->m_balls)
		{
			if (ball->GetNumber() == oldBall->GetNumber())
			{
				if (ball->GetRow() == oldBall->GetRow() && ball->GetCol() == oldBall->GetCol())
				{
					++count4;
				}
			}
		}
	}

	if (count3 == GetBallsAndHolesCount() || count4 == GetBallsAndHolesCount())
	{
		m_answer = "Impossible";
		return true;
	}
	return false;
}

} // namespace ball_game
