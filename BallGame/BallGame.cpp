#include "BallGame.h"

// Self
#include "BFS.h"
#include "Constants.h"

// C++
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

namespace nsGame
{
	using std::cout;
	using std::vector;
	using std::thread;
	
	using namespace nsCondition;
	using namespace nsNames;
	using namespace nsSearch;

	void BallGame::Start()
	{
		_exceptions.clear();
		_cases.clear();

		StartWithExceptions();

		for (const auto& exc : _exceptions)
		{
			try
			{
				if (exc != nullptr)
					std::rethrow_exception(exc);
			}
			catch (const std::exception & e)
			{
				cout << e.what() << endl;
			}
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

		int size{ 0 };
		int balls{ 0 };
		int walls{ 0 };
		int row{ 0 };
		int col{ 0 };
		int row2{ 0 };
		int col2{ 0 };

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

			Condition cond(balls, walls, size);

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
			_cases.push_back(std::make_shared<Condition>(cond));
		}
	}

	void BallGame::StartWithExceptions()
	{
		try
		{
			Initialize();
			const auto size = _cases.size();
			_exceptions.reserve(size);
			
			vector<thread> threads;
			threads.reserve(size);

			vector<BFS> search;
			search.reserve(size);

			for (auto i = 0; i < size; ++i)
			{
				search.emplace_back();
				threads.emplace_back();
				threads[i] = thread(&BFS::Run, search[i], std::cref(_cases[i]));
			}

			for (const auto& i : search)
			{
				// TODO дождаться потоков и вывести результат
			}
		}
		catch (...)
		{
			std::lock_guard<std::mutex> lock(_mtx);
			_exceptions.push_back(std::current_exception());
		}
	}
}
