#include "BFS.h"

// Self
#include "Condition.h"

#include <iostream>

using namespace std;

namespace nsGame::nsSearch
{

void BFS::GetNextPoints(const nsCondition::Condition & state, list<nsCondition::Condition> & states)
{
	states.clear();
	
	// north
	Condition north(state);
	north.setCameFrom(&state);
	if (north.goNorth())
		states.push_back(north);

	// south
	Condition south(state);
	south.setCameFrom(&state);
	if (south.goSouth())
		states.push_back(south);

	// east
	Condition east(state);
	east.setCameFrom(&state);
	if (east.goEast())
		states.push_back(east);

	// west
	Condition west(state);
	west.setCameFrom(&state);
	if (west.goWest())
		states.push_back(west);
}

void BFS::run(std::shared_ptr<Condition> start)
{
	list<Condition> willCheck;
	list<Condition> neigh;
	willCheck.push_back(start);

	// пока лист состояний, ожидающих рассмотрения не пуст, ищем состояние
	while (willCheck.size() > 0)
	{
		// забираем первое значение
		Condition curr = Condition(willCheck.front());

		// проверяем количество шаров
		if (curr.getBallsAndHolesCount() == 0)
		{
			cout << "Case " << cases_count << ": " << curr.getAnswer().length() << " moves " << curr.getAnswer() << ".\n\n";
			++cases_count;
			return;
		}

		// проверяем невозможность нахождения решения
		if (curr.isFinish())
		{
			cout << "Case " << cases_count << ": " << curr.getAnswer() << ".\n\n";
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

}