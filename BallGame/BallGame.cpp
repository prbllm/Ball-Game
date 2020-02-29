#include "BallGame.h"

// Self
#include "BFS.h"
#include "Constants.h"

// C++
#include <fstream>
#include <iostream>
#include <sstream>

namespace nsGame
{
	using std::cout;
	using namespace nsNames;
	
	BallGame::BallGame() = default;

	BallGame::~BallGame() = default;

	void BallGame::Start()
	{
		Initialize();

		nsSearch::BFS search;
		while (!cases.empty())
		{
			search.run(std::move(cases.front()));
			cases.pop();
		}
	}

	void BallGame::Initialize()
	{
		ifstream file(FILE_NAME, ios_base::in);

		if (!file.is_open())
		{
			cout << "File " << FILE_NAME << " failed to open.";
			return;
		}

		auto size{ 0 };
		auto balls{ 0 };
		auto walls{ 0 };
		auto row{ 0 };
		auto col{ 0 };
		auto row2{ 0 };
		auto col2{ 0 };

		while (true)
		{
			string line;
			getline(file, line);
			istringstream stream(line);
			
			size = 0;
			balls = 0;
			walls = 0;

			stream >> size >> balls >> walls;
			if (size == 0 && balls == 0 && walls == 0)
				break;

			nsCondition::Condition cond(balls, walls, size);

			for (auto i = 0; i < balls; ++i)
			{
				line.clear();
				getline(file, line);
				row = 0;
				col = 0;
				istringstream stream_ball(line);
				stream_ball >> row >> col;
				cond.setBall(i, row, col);
			}

			for (auto i = 0; i < balls; ++i)
			{
				line.clear();
				getline(file, line);
				row = 0;
				col = 0;
				istringstream stream_hole(line);
				stream_hole >> row >> col;
				cond.setHole(i, row, col);
			}

			for (auto i = 0; i < walls; ++i)
			{
				line.clear();
				getline(file, line);
				row = 0;
				col = 0;
				row2 = 0;
				col2 = 0;
				istringstream stream_wall(line);
				stream_wall >> row >> col >> row2 >> col2;
				cond.setWall(
					row > row2 ? row2 : row
					, col > col2 ? col2 : col
					, row > row2 ? row : row2
					, col > col2 ? col : col2);
			}
			cases.push(std::make_unique<nsCondition::Condition>(cond));
		}
	}
}
