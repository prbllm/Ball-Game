// Self
#include "BFS.hpp"
#include "Condition.hpp"

// C++
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>

void OpenFile(std::list<ball_game::Condition>& cases)
{
	static const std::filesystem::path fileName{ std::filesystem::current_path() / ".." / "data" / "input.txt" };
	std::ifstream file(fileName, std::ios_base::in);

	if (!file.is_open())
	{
		std::cout << "File " << fileName << " failed to open.";
		return;
	}

	int size{ 0 }, ballsCount{ 0 }, wallsCount{ 0 }, row{ 0 }, row2{ 0 }, col{ 0 }, col2{ 0 };
	while (true)
	{
		file >> size >> ballsCount >> wallsCount;
		if (!size && !ballsCount && !wallsCount)
		{
			break;
		}

		ball_game::Condition condition{ size };

		for (int i = 0; i < ballsCount; ++i)
		{
			file >> row >> col;
			condition.SetBall(i, row, col);
		}

		for (int i = 0; i < ballsCount; ++i)
		{
			file >> row >> col;
			condition.SetHole(i, row, col);
		}

		for (int i = 0; i < wallsCount; ++i)
		{
			file >> row >> col >> row2 >> col2;
			condition.SetWall(row > row2 ? row2 : row
				, col > col2 ? col2 : col
				, row > row2 ? row : row2
				, col > col2 ? col : col2);
		}

		cases.emplace_back(condition);
	}
}

int main()
{
	ball_game::Bfs bfs;
	std::list<ball_game::Condition> cases;
	OpenFile(cases);

	for (const auto& cond : cases)
	{
		bfs.Run(cond);
	}
    return 0;
}
