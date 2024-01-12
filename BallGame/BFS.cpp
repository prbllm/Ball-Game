#include "BFS.h"

// Self
#include "Condition.h"

// C++
#include <iostream>

namespace ball_game
{

void Bfs::GetNextPoints(const Condition& state, std::list<Condition>& states) noexcept
{
	try
	{
		states.clear();

		// north
		Condition north = state;
		north.SetCameFrom(state);
		if (north.ChangePosition(Condition::DirectionType::North))
		{
			states.emplace_back(north);
		}

		// south
		Condition south = state;
		south.SetCameFrom(state);
		if (south.ChangePosition(Condition::DirectionType::South))
		{
			states.emplace_back(south);
		}

		// east
		Condition east = state;
		east.SetCameFrom(state);
		if (east.ChangePosition(Condition::DirectionType::East))
		{
			states.emplace_back(east);
		}

		// west
		Condition west = state;
		west.SetCameFrom(state);
		if (west.ChangePosition(Condition::DirectionType::West))
		{
			states.emplace_back(west);
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to get next points. Exception: " << exception.what() << std::endl;
	}
}

void Bfs::Run(const Condition & start) noexcept
{
	try
	{
		std::list<Condition> willCheck, neigh;
		willCheck.emplace_back(start);

		// пока лист состояний, ожидающих рассмотрения не пуст, ищем состояние
		while (!willCheck.empty())
		{
			// забираем первое значение
			Condition curr = willCheck.front();

			// проверяем количество шаров
			if (curr.GetBallsAndHolesCount() == 0)
			{
				std::cout << "Case " << m_casesCount << ": " << curr.GetAnswer().length() << " moves " << curr.GetAnswer() << ".\n\n";
				++m_casesCount;
				return;
			}

			// проверяем невозможность нахождения решения
			if (curr.IsFinish())
			{
				std::cout << "Case " << m_casesCount << ": " << curr.GetAnswer() << ".\n\n";
				++m_casesCount;
				return;
			}
			// удаляем состояние из списка ожидающих рассмотрения
			willCheck.pop_front();

			// получаем возможные состояния и добавляем их в список на рассмотрение
			GetNextPoints(curr, neigh);
			willCheck.insert(willCheck.end(), neigh.begin(), neigh.end());
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << "Unable to find solution. Exception: " << exception.what() << std::endl;
	}
}

} // namespace ball_game
