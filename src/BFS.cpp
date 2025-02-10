#include "BFS.hpp"

// Self
#include "Condition.hpp"

// C++
#include <future>
#include <iostream>
#include <vector>

namespace ball_game
{

void Bfs::GetNextPoints(const Condition& state, std::list<Condition>& states) const noexcept
{
	try
	{
		static const std::vector directions{
		Condition::DirectionType::North
		, Condition::DirectionType::South
		, Condition::DirectionType::East
		, Condition::DirectionType::West };

		static const auto size = directions.size();
		std::vector<std::pair<Condition, std::future<bool>>> futurePoints;
		futurePoints.reserve(size);

		for (size_t i{ 0 }; i < size; ++i)
		{
			Condition newState = state;
			newState.SetCameFrom(state);
			futurePoints.emplace_back(std::move(newState), std::future<bool>{});
		}

		for (size_t i{ 0 }; i < size; ++i)
		{
			futurePoints[i].second = std::async(std::launch::async, &Condition::ChangePosition, &futurePoints[i].first, directions[i]);
		}

		states.clear();
		for (auto& [cond, future] : futurePoints)
		{
			if (future.get())
			{
				states.emplace_back(std::move(cond));
			}
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
