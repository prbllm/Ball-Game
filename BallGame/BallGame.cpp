// Self
#include "BFS.h"
#include "Condition.h"

// C++
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>

void OpenFile(std::list<ball_game::Condition>& cases)
{
	static const std::filesystem::path fileName{ std::filesystem::current_path() / "BallGame" / "data" / "input.txt"};
	std::ifstream file(fileName, std::ios_base::in);

	if (!file.is_open())
	{
		std::cout << "File " << fileName << " failed to open.";
		return;
	}
	bool isExit{ false };

	int size{ 0 }, balls{ 0 }, walls{ 0 };
	int row{ 0 }, col{ 0 };
	int row2{ 0 }, col2{ 0 };
	std::string line;

	while (!isExit)
	{
		line.clear();
		std::getline(file, line);
		std::istringstream stream(line);
		size = 0;
		balls = 0;
		walls = 0;

		stream >> size >> balls >> walls;
		if (size == 0 && balls == 0 && walls == 0)
		{
			isExit = true;
			break;
		}
		ball_game::Condition cond(size);

		for (int i = 0; i < balls; ++i)
		{
			line.clear();
			getline(file, line);
			row = 0;
			col = 0;
			std::istringstream stream(line);
			stream >> row >> col;
			cond.SetBall(i, row, col);
		}

		for (int i = 0; i < balls; ++i)
		{
			line.clear();
			getline(file, line);
			row = 0;
			col = 0;
			std::istringstream stream(line);
			stream >> row >> col;
			cond.SetHole(i, row, col);
		}

		for (int i = 0; i < walls; ++i)
		{
			line.clear();
			getline(file, line);
			row = 0;
			col = 0;
			row2 = 0;
			col2 = 0;
			std::istringstream stream(line);
			stream >> row >> col >> row2 >> col2;
			cond.SetWall(row > row2 ? row2 : row, col > col2 ? col2 : col, row > row2 ? row : row2, col > col2 ? col2 : col2);
		}
		cases.push_back(cond);
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
