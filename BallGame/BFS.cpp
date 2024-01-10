#include "BFS.h"

// Self
#include "Condition.h"

// C++
#include <iostream>

namespace BallGame
{

void BFS::getNextPoints(const Condition& state, std::list<Condition>& states) noexcept
{
	try
	{
		states.clear();

		// north
		Condition north = state;
		north.setCameFrom(state);
		if (north.goNorth())
		{
			states.emplace_back(north);
		}

		// south
		Condition south = state;
		south.setCameFrom(state);
		if (south.goSouth())
		{
			states.emplace_back(south);
		}

		// east
		Condition east = state;
		east.setCameFrom(state);
		if (east.goEast())
		{
			states.emplace_back(east);
		}

		// west
		Condition west = state;
		west.setCameFrom(state);
		if (west.goWest())
		{
			states.emplace_back(west);
		}
	}
	catch (const std::exception& excpetion)
	{
		std::cout << "Unable to get next points. Exception: " << excpetion.what() << std::endl;
	}
}

void BFS::run(const Condition & start) noexcept
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
			if (curr.getBallsAndHolesCount() == 0)
			{
				std::cout << "Case " << cases_count << ": " << curr.getAnswer().length() << " moves " << curr.getAnswer() << ".\n\n";
				++cases_count;
				return;
			}

			// проверяем невозможность нахождения решения
			if (curr.isFinish())
			{
				std::cout << "Case " << cases_count << ": " << curr.getAnswer() << ".\n\n";
				++cases_count;
				return;
			}
			// удаляем состояние из списка ожидающих рассмотрения
			willCheck.pop_front();

			// получаем возможные состояния и добавляем их в список на рассмотрение
			getNextPoints(curr, neigh);
			willCheck.insert(willCheck.end(), neigh.begin(), neigh.end());
		}
	}
	catch (const std::exception& excpetion)
	{
		std::cout << "Unable to find solution. Exception: " << excpetion.what() << std::endl;
	}
}

} // namespace BallGame
