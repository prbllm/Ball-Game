// Self
#include "BFS.h"
#include "Condition.h"
#include "stdafx.h"

// C++
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#define FILE_NAME "input.txt"

using namespace nsBallGame;

/*!
 * \brief функция загрузки данных из файла
 * \param cases список кейсов
 */
void openFile(list<Condition> & cases)
{
	ifstream file(FILE_NAME, ios_base::in);

	if (!file.is_open())
	{
		cout << "File " << FILE_NAME << " failed to open.";
		return;
	}
	bool isExit = false;
	
	int size = 0, balls = 0, walls = 0;
	int row = 0, col = 0;
	int row2 = 0, col2 = 0;

	while (!isExit)
	{
		string line;
		getline(file, line);
		istringstream stream(line);
		size = 0;
		balls = 0;
		walls = 0;

		stream >> size >> balls >> walls;
		if (size == 0 && balls == 0 && walls == 0)
		{
			isExit = true;
			break;
		}
		Condition cond(balls, walls, size);

		for (int i = 0; i < balls; ++i)
		{
			line.clear();
			getline(file, line);
			row = 0;
			col = 0;
			istringstream stream(line);
			stream >> row >> col;
			cond.setBall(i, row, col);
		}

		for (int i = 0; i < balls; ++i)
		{
			line.clear();
			getline(file, line);
			row = 0;
			col = 0;
			istringstream stream(line);
			stream >> row >> col;
			cond.setHole(i, row, col);
		}

		for (int i = 0; i < walls; ++i)
		{
			line.clear();
			getline(file, line);
			row = 0;
			col = 0;
			row2 = 0;
			col2 = 0;
			istringstream stream(line);
			stream >> row >> col >> row2 >> col2;
			cond.setWall(row > row2 ? row2 : row, col > col2 ? col2 : col, row > row2 ? row : row2, col > col2 ? col2 : col2);
		}
		cases.push_back(cond);
	}
}

int main()
{
	BFS bfs;
	list<Condition> cases;
	list<Condition>::iterator it;
	openFile(cases);

	for (it = cases.begin(); it != cases.end(); ++it)
		bfs.run(*it);

    return 0;
}

