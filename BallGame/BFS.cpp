#include "BFS.h"

// Self
#include "Condition.h"

#include <iostream>

using namespace std;

namespace nsBallGame
{

void BFS::getNextPoints(const Condition & state, list<Condition> & states)
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

BFS::BFS()
	: cases_count(1)
{}

BFS::~BFS()
{}

void BFS::run(const Condition & start)
{
	list<Condition> willCheck;
	list<Condition> neigh;
	willCheck.push_back(start);

	// ���� ���� ���������, ��������� ������������ �� ����, ���� ���������
	while (willCheck.size() > 0)
	{
		// �������� ������ ��������
		Condition curr = Condition(willCheck.front());

		// ��������� ���������� �����
		if (curr.getBallsAndHolesCount() == 0)
		{
			cout << "Case " << cases_count << ": " << curr.getAnswer().length() << " moves " << curr.getAnswer() << ".\n\n";
			++cases_count;
			return;
		}

		// ��������� ������������� ���������� �������
		if (curr.isFinish())
		{
			cout << "Case " << cases_count << ": " << curr.getAnswer() << ".\n\n";
			++cases_count;
			return;
		}
		// ������� ��������� �� ������ ��������� ������������
		willCheck.pop_front();
		
		// �������� ��������� ��������� � ��������� �� � ������ �� ������������
		getNextPoints(curr, neigh);
		willCheck.insert(willCheck.end(), neigh.begin(), neigh.end());
	}
}

}