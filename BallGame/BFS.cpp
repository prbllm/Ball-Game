#include "BFS.h"

// Self
#include "Condition.h"

// C++
#include <iostream>
#include <list>
#include <queue>

namespace nsGame::nsSearch
{
	using std::shared_ptr;
	
	using namespace nsCondition;
	
void BFS::GetNextPoints(const std::shared_ptr<Condition > & state, std::list<std::shared_ptr<Condition>>&states) const
{
	states.clear();
	
	auto north = state;
	north->setCameFrom(state);
	if (north->goNorth())
		states.push_back(north);

	auto south = state;
	south->setCameFrom(&state);
	if (south->goSouth())
		states.push_back(south);

	auto east = state;
	east->setCameFrom(&state);
	if (east->goEast())
		states.push_back(east);

	auto west = state;
	west->setCameFrom(&state);
	if (west->goWest())
		states.push_back(west);
}

void BFS::Run(const std::shared_ptr<Condition>& start)
{
	queue<shared_ptr<Condition>> willCheck;
	list<shared_ptr<Condition>> neigh;
	willCheck.push(start);

	// пока список состояний, ожидающих рассмотрения не пуст, ищем состояние
	while (!willCheck.empty())
	{
		// забираем первое значение
		auto current = willCheck.front();

		// проверяем количество шаров
		if (current->getBallsAndHolesCount() == 0)
		{
			cout << "Case " << cases_count << ": " << current->getAnswer().length() << " moves " << current->getAnswer() << ".\n\n";
			++cases_count;
			return;
		}

		// проверяем невозможность нахождения решения
		if (current->isFinish())
		{
			cout << "Case " << cases_count << ": " << current->getAnswer() << ".\n\n";
			++cases_count;
			_mtx.unlock();
			return;
		}
		// удаляем состояние из списка ожидающих рассмотрения
		willCheck.pop();
		
		// получаем возможные состояния и добавляем их в список на рассмотрение
		GetNextPoints(current, neigh);

		for (const auto& i : neigh)
			willCheck.push(i);
	}
}

}