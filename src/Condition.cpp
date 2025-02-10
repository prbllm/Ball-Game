#include "Condition.hpp"

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

bool Condition::CheckMovement() const noexcept
{
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
	return true;
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
	return static_cast<int>(m_balls.size());
}

int Condition::GetSize() const noexcept
{
	return m_size;
}

bool Condition::ChangePosition(DirectionType directionType)
{
	const auto checkBalls = [](const Ball& oldBall, const std::shared_ptr<Ball>& ball, DirectionType type)
	{
		switch (type)
		{
		case DirectionType::North:
			return oldBall.GetCol() == ball->GetCol() && oldBall.GetRow() > ball->GetRow();
		case DirectionType::South:
			return oldBall.GetCol() == ball->GetCol() && oldBall.GetRow() < ball->GetRow();
		case DirectionType::East:
			return oldBall.GetRow() == ball->GetRow() && oldBall.GetCol() < ball->GetCol();
		case DirectionType::West:
			return oldBall.GetRow() == ball->GetRow() && oldBall.GetCol() > ball->GetCol();
		default:
			std::cout << "Warning. Unknown direction type." << std::endl;
			return false;
		}
	};

	const auto checkWalls = [](const std::shared_ptr<Wall>& wall, const std::shared_ptr<Ball>& ball, DirectionType type)
	{
		switch (type)
		{
		case DirectionType::North:
			return wall->GetColFirst() == wall->GetColSec() && wall->GetColFirst() == ball->GetCol() && wall->GetRowFirst() >= ball->GetRow();
		case DirectionType::South:
			return wall->GetColFirst() == wall->GetColSec() && wall->GetColFirst() == ball->GetCol() && wall->GetRowSec() <= ball->GetRow();
		case DirectionType::East:
			return wall->GetRowFirst() == wall->GetRowSec() && wall->GetRowFirst() == ball->GetRow() && wall->GetColSec() <= ball->GetCol();
		case DirectionType::West:
			return wall->GetRowFirst() == wall->GetRowSec() && wall->GetRowFirst() == ball->GetRow() && wall->GetColFirst() >= ball->GetCol();
		default:
			std::cout << "Warning. Unknown direction type." << std::endl;
			return false;
		}
	};

	const auto checkBallsWithWall = [checkBalls](const Ball& oldBall, const std::shared_ptr<Ball>& ball, const std::shared_ptr<Wall>& wall, DirectionType type)
	{
		const auto res = checkBalls(oldBall, ball, type);
		switch (type)
		{
		case DirectionType::North:
			return res && oldBall.GetRow() <= wall->GetRowFirst();
		case DirectionType::South:
			return res && oldBall.GetRow() >= wall->GetRowSec();
		case DirectionType::East:
			return res && oldBall.GetCol() >= wall->GetColSec();
		case DirectionType::West:
			return res && oldBall.GetCol() <= wall->GetColFirst();
		default:
			return false;
		}
	};

	const auto calcNewPoint = [this](bool needWall, int ballsNumber, DirectionType type, const std::shared_ptr<Wall>& wall = nullptr)
	{
		switch (type)
		{
		case DirectionType::North:
			return needWall && wall ? wall->GetRowFirst() - ballsNumber : m_size - 1 - ballsNumber;
			case DirectionType::South:
			return needWall && wall ? wall->GetRowSec() + ballsNumber : ballsNumber;
		case DirectionType::East:
			return needWall && wall ? wall->GetColSec() - ballsNumber : ballsNumber;
		case DirectionType::West:
			return needWall && wall ? wall->GetColFirst() + ballsNumber : m_size - 1 - ballsNumber;
		default:
			std::cout << "Warning. Unknown direction type." << std::endl;
			return 0;
		}
	};

	const auto checkHoles = [](const std::shared_ptr<Ball>& hole, const std::shared_ptr<Ball>& ball, int newPoint, DirectionType type)
	{
		switch (type)
		{
		case DirectionType::North:
			return hole->GetCol() == ball->GetCol() && hole->GetRow() <= newPoint && hole->GetRow() > ball->GetRow();
		case DirectionType::South:
			return hole->GetCol() == ball->GetCol() && hole->GetRow() >= newPoint && hole->GetRow() < ball->GetRow();
		case DirectionType::East:
			return hole->GetRow() == ball->GetRow() && hole->GetCol() >= newPoint && hole->GetCol() < ball->GetCol();
		case DirectionType::West:
			return hole->GetRow() == ball->GetRow() && hole->GetCol() <= newPoint && hole->GetCol() > ball->GetCol();
		default:
			std::cout << "Warning. Unknown direction type." << std::endl;
			return false;
		}
	};

	const auto changeData = [](const std::shared_ptr<Ball>& ball, int data, DirectionType type)
	{
		switch (type)
		{
		case DirectionType::North:
		case DirectionType::South:
			ball->SetRow(data);
			break;
		case DirectionType::East:
		case DirectionType::West:
			ball->SetCol(data);
			break;
		default:
			std::cout << "Warning. Unknown direction type." << std::endl;
		}
	};

	// все 4 функции аналогичные, различия только по направлению движения
	int rowNew{ 0 }, ballsCount{ 0 }, delCount{ 0 };
	bool isWall{ false };

	std::list<int> numbers;
	std::list<Ball> oldBalls;
	for (const auto& ball : m_balls)
	{
		oldBalls.emplace_back(*ball);
	}

	// цикл по всем шарам
	for (const auto& ball : m_balls)
	{
		rowNew = 0;
		ballsCount = 0;
		isWall = false;

		// ищутся шары до конца поля
		for (const auto& oldBall : oldBalls)
		{
			if (checkBalls(oldBall, ball, directionType))
			{
				++ballsCount;
			}
		}

		// цикл по стенам
		for (const auto& wall : m_walls)
		{
			// ищутся шары до стены
			if (checkWalls(wall, ball, directionType))
			{
				ballsCount = 0;
				for (const auto& oldBall : oldBalls)
				{
					if (checkBallsWithWall(oldBall, ball, wall, directionType))
					{
						++ballsCount;
					}
				}

				// рассчитывается новая строка
				rowNew = calcNewPoint(true, ballsCount, directionType, wall);
				isWall = true;
			}
		}

		if (!isWall)
		{
			rowNew = calcNewPoint(false, ballsCount, directionType, nullptr);
		}

		// цикл по лункам
		for (const auto& hole : m_holes)
		{
			if (checkHoles(hole, ball, rowNew, directionType))
			{
				// если есть лунка нужная на пути, запоминаем номер
				if (hole->GetNumber() == ball->GetNumber() && ballsCount == 0)
				{
					numbers.push_back(hole->GetNumber());
					++delCount;
				}
				else
				{
					return false;
				}
			}
		}

		if (delCount == 0)
		{
			changeData(ball, rowNew, directionType);
		}
	}

	// удаляем шары и лунки
	for (const auto& i : numbers)
	{
		DeleteDataByNumber(i);
	}

	if (!CheckMovement())
	{
		return false;
	}

	AddToAnswer(directionType);
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
