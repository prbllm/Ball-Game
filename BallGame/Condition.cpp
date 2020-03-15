#include "Condition.h"

#include <iterator>

using namespace std;
namespace nsGame::nsCondition
{
	Condition::Condition() = default;

	Condition::Condition(const Condition & cond)
	{
		this->balls_count = cond.getBallsAndHolesCount();
		this->walls_count = cond.getWallsCount();
		this->holes_count = cond.getBallsAndHolesCount();
		this->size = cond.getSize();

		list<Ball*>::const_iterator it_h;
		for (it_h = cond.holes.begin(); it_h != cond.holes.end(); ++it_h)
			setHole((*it_h)->getNumber(), (*it_h)->getRow(), (*it_h)->getCol());

		list<Ball*>::const_iterator it_b;
		for (it_b = cond.balls.begin(); it_b != cond.balls.end(); ++it_b)
			setBall((*it_b)->getNumber(), (*it_b)->getRow(), (*it_b)->getCol());

		list<Wall*>::const_iterator it_w;
		for (it_w = cond.walls.begin(); it_w != cond.walls.end(); ++it_w)
			setWall((*it_w)->getRowFirst(), (*it_w)->getColFirst(), (*it_w)->getRowSec(), (*it_w)->getColSec());

		this->came_from = cond.getCameFrom();
		this->answer = cond.getAnswer();
	}

	Condition::Condition(int n_balls, int n_walls, int n_size)
	{
		this->holes_count = n_balls;
		this->walls_count = n_walls;
		this->balls_count = n_balls;

		this->came_from = nullptr;
		this->size = n_size;
	}

	Condition::~Condition()
	{
		list<Ball*>::iterator hole;
		for (hole = holes.begin(); hole != holes.end(); ++hole)
			delete *hole;
		
		list<Ball*>::iterator ball;
		for (ball = balls.begin(); ball != balls.end(); ++ball)
			delete *ball;

		list<Wall*>::iterator wall;
		for (wall = walls.begin(); wall != walls.end(); ++wall)
			delete *wall;
	}

	void Condition::addToAnswer(char symbol)
	{
		answer += symbol;
	}

	void Condition::setAnswer(string ans)
	{
		answer = ans;
	}

	string Condition::getAnswer() const
	{
		return answer;
	}

	void Condition::setCameFrom(const std::shared_ptr<Condition>&* cf)
	{
		came_from = new Condition(*cf);
	}

	Condition * Condition::getCameFrom() const
	{
		return came_from;
	}

	void Condition::setBall(int num, int row, int col)
	{
		Ball * ball = new Ball(row, col, num);
		balls.push_back(ball);
	}

	void Condition::setHole(int num, int row, int col)
	{
		Ball * hole = new Ball(row, col, num);
		holes.push_back(hole);
	}

	void Condition::setWall(int row, int col, int row2, int col2)
	{
		Wall * wall = new Wall(row, col, row2, col2);
		walls.push_back(wall);
	}

	void Condition::deleteBall(int number)
	{
		list<Ball*>::iterator ball;
		
		for (ball = balls.begin(); ball != balls.end(); ball++)
		{
			if ((*ball)->getNumber() == number)
			{
				delete *ball;
				ball = balls.erase(ball);
				--this->balls_count;
				if (ball == balls.end())
					break;
			}
		}
	}

	void Condition::deleteHole(int number)
	{
		list<Ball*>::iterator hole;
		for (hole = holes.begin(); hole != holes.end(); ++hole)
			if ((*hole)->getNumber() == number)
			{
				delete *hole;
				hole = holes.erase(hole);
				--this->holes_count;
				if (hole == holes.end())
					break;
			}
	}

	int Condition::getBallsAndHolesCount() const
	{
		return balls_count;
	}

	int Condition::getWallsCount() const
	{
		return walls_count;
	}

	int Condition::getSize() const
	{
		return size;
	}

	bool Condition::goNorth()
	{
		// все 4 функции аналогичные, различия только по направлению движения
		list<Ball*>::iterator it_b;
		list<Ball>::const_iterator it_b_new;
		list<Ball*>::iterator it_h;
		list<Wall*>::iterator it_w;
		int row_new = 0;
		int balls_count = 0;
		int del_count = 0;
		bool isWall = false;

		list<int> numbers;

		list<Ball> balls_old;
		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
			balls_old.push_back(*(*it_b));

		// цикл по всем шарам
		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		{
			row_new = 0;
			balls_count = 0;
			isWall = false;

			// ищутся шары до конца поля
			for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
			{
				if (((*it_b_new).getCol() == (*it_b)->getCol())
					&& ((*it_b_new).getRow() > (*it_b)->getRow()))
					++balls_count;
			}

			// цикл по стенам
			for (it_w = walls.begin(); it_w != walls.end(); ++it_w)
			{
				// ищутся шары до стены
				if (((*it_w)->getColFirst() == (*it_w)->getColSec()) && ((*it_w)->getColFirst() == (*it_b)->getCol())
					&& ((*it_w)->getRowFirst() >= (*it_b)->getRow()))
				{
					balls_count = 0;
					for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
					{
						if (((*it_b_new).getCol() == (*it_b)->getCol())
							&& ((*it_b_new).getRow() > (*it_b)->getRow())
							&& ((*it_b_new).getRow() <= (*it_w)->getRowFirst()))
							++balls_count;
					}
					// рассчитывается новая строка
					row_new = (*it_w)->getRowFirst() - balls_count;
					isWall = true;
				}
			}

			if (!isWall)
				row_new = size - 1 - balls_count;

			// цикл по лункам
			for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
			{
				if (((*it_h)->getCol() == (*it_b)->getCol()) && ((*it_h)->getRow() <= row_new) && ((*it_h)->getRow() > (*it_b)->getRow()))
				{
					// если есть лунка нужная на пути, запоминаем номер
					if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
					{
						numbers.push_back((*it_h)->getNumber());
						++del_count;
					}
					else
						return false;
				}
				if (it_h == holes.end())
					break;
			}
			if (it_b == balls.end())
				break;
			if (del_count == 0)
				(*it_b)->setRow(row_new);
		}

		// удаляем шары и лунки
		for (int i : numbers)
		{
			deleteBall(i);
			deleteHole(i);
		}

		// отсеивание лишних движений, которые не изменяют позицию
		if ((came_from != nullptr) &&
			(came_from->came_from != nullptr))
		{
			list<Ball*>::const_iterator iter;
			list<Ball*>::const_iterator iter_3;
			list<Ball*>::const_iterator iter_2;
			int count_3 = 0, count_2 = 0;
			for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
			{
				for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
				{
					if ((*iter)->getNumber() == (*iter_3)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_3)->getRow()) &&
							((*iter)->getCol() == (*iter_3)->getCol()))
							++count_3;
					}
				}
				for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
				{
					if ((*iter)->getNumber() == (*iter_2)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_2)->getRow()) &&
							((*iter)->getCol() == (*iter_2)->getCol()))
							++count_2;
					}
				}
			}
			if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
				((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
				return false;
		}
		addToAnswer('N');
		return true;
	}

	bool Condition::goSouth()
	{
		list<Ball*>::iterator it_b;
		list<Ball>::const_iterator it_b_new;
		list<Ball*>::iterator it_h;
		list<Wall*>::iterator it_w;
		int row_new = 0;
		int balls_count = 0;
		int del_count = 0;
		bool isWall = false;

		list<int> numbers;

		list<Ball> balls_old;
		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
			balls_old.push_back(*(*it_b));

		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		{
			row_new = 0;
			balls_count = 0;
			isWall = false;

			for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
			{
				if (((*it_b_new).getCol() == (*it_b)->getCol())
					&& ((*it_b_new).getRow() < (*it_b)->getRow()))
					++balls_count;
			}

			for (it_w = walls.begin(); it_w != walls.end(); ++it_w)
			{
				if (((*it_w)->getColFirst() == (*it_w)->getColSec()) && ((*it_w)->getColFirst() == (*it_b)->getCol())
					&& ((*it_w)->getRowSec() <= (*it_b)->getRow()))
				{
					balls_count = 0;
					for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
					{
						if (((*it_b_new).getCol() == (*it_b)->getCol())
							&& ((*it_b_new).getRow() < (*it_b)->getRow())
							&& ((*it_b_new).getRow() >= (*it_w)->getRowSec()))
							++balls_count;
					}
					row_new = (*it_w)->getRowSec() + balls_count;
					isWall = true;
				}
			}

			if (!isWall)
				row_new = balls_count;

			for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
			{
				if (((*it_h)->getCol() == (*it_b)->getCol()) && ((*it_h)->getRow() >= row_new) && ((*it_h)->getRow() < (*it_b)->getRow()))
				{
					if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
					{
						numbers.push_back((*it_h)->getNumber());
						++del_count;
					}
					else
						return false;
				}
				if (it_h == holes.end())
					break;
			}
			if (it_b == balls.end())
				break;
			if (del_count == 0)
				(*it_b)->setRow(row_new);
		}

		for (int i : numbers)
		{
			deleteBall(i);
			deleteHole(i);
		}

		if ((came_from != nullptr) &&
			(came_from->came_from != nullptr))
		{
			list<Ball*>::const_iterator iter;
			list<Ball*>::const_iterator iter_3;
			list<Ball*>::const_iterator iter_2;
			int count_3 = 0, count_2 = 0;
			for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
			{
				for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
				{
					if ((*iter)->getNumber() == (*iter_3)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_3)->getRow()) &&
							((*iter)->getCol() == (*iter_3)->getCol()))
							++count_3;
					}
				}

				for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
				{
					if ((*iter)->getNumber() == (*iter_2)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_2)->getRow()) &&
							((*iter)->getCol() == (*iter_2)->getCol()))
							++count_2;
					}
				}
			}
			if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) || 
				((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
				return false;
		}
		addToAnswer('S');
		return true;
	}

	bool Condition::goEast()
	{
		list<Ball*>::iterator it_b;
		list<Ball>::const_iterator it_b_new;
		list<Ball*>::iterator it_h;
		list<Wall*>::iterator it_w;
		int col_new = 0;
		int balls_count = 0;
		int del_count = 0;
		bool isWall = false;

		list<int> numbers;

		list<Ball> balls_old;
		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
			balls_old.push_back(*(*it_b));

		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		{
			col_new = 0;
			balls_count = 0;
			isWall = false;

			for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
			{
				if (((*it_b_new).getRow() == (*it_b)->getRow())
					&& ((*it_b_new).getCol() < (*it_b)->getCol()))
					++balls_count;
			}

			for (it_w = walls.begin(); it_w != walls.end(); ++it_w)
			{
				if (((*it_w)->getRowFirst() == (*it_w)->getRowSec()) && ((*it_w)->getRowFirst() == (*it_b)->getRow())
					&& ((*it_w)->getColSec() <= (*it_b)->getCol()))
				{
					balls_count = 0;
					for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
					{
						if (((*it_b_new).getRow() == (*it_b)->getRow())
							&& ((*it_b_new).getCol() < (*it_b)->getCol())
							&& ((*it_b_new).getCol() >= (*it_w)->getColSec()))
							++balls_count;
					}
					col_new = (*it_w)->getColSec() - balls_count;
					isWall = true;
				}
			}

			if (!isWall)
				col_new = balls_count;

			for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
			{
				if (((*it_h)->getRow() == (*it_b)->getRow()) && ((*it_h)->getCol() >= col_new) && ((*it_h)->getCol() < (*it_b)->getCol()))
				{
					if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
					{
						numbers.push_back((*it_h)->getNumber());
						++del_count;
					}
					else
						return false;
				}
				if (it_h == holes.end())
					break;
			}
			if (it_b == balls.end())
				break;
			if (del_count == 0)
				(*it_b)->setCol(col_new);
		}

		for (int i : numbers)
		{
			deleteBall(i);
			deleteHole(i);
		}

		if ((came_from != nullptr) &&
			(came_from->came_from != nullptr))
		{
			list<Ball*>::const_iterator iter;
			list<Ball*>::const_iterator iter_3;
			list<Ball*>::const_iterator iter_2;
			int count_3 = 0, count_2 = 0;
			for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
			{
				for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
				{
					if ((*iter)->getNumber() == (*iter_3)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_3)->getRow()) &&
							((*iter)->getCol() == (*iter_3)->getCol()))
							++count_3;
					}
				}
				for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
				{
					if ((*iter)->getNumber() == (*iter_2)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_2)->getRow()) &&
							((*iter)->getCol() == (*iter_2)->getCol()))
							++count_2;
					}
				}
			}
			if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
				((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
				return false;
		}
		addToAnswer('E');
		return true;
	}

	bool Condition::goWest()
	{
		list<Ball*>::iterator it_b;
		list<Ball>::const_iterator it_b_new;
		list<Ball*>::iterator it_h;
		list<Wall*>::iterator it_w;
		int col_new = 0;
		int balls_count = 0;
		int del_count = 0;
		bool isWall = false;

		list<int> numbers;

		list<Ball> balls_old;
		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
			balls_old.push_back(*(*it_b));

		for (it_b = balls.begin(); it_b != balls.end(); ++it_b)
		{
			col_new = 0;
			balls_count = 0;
			isWall = false;

			for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
			{
				if (((*it_b_new).getRow() == (*it_b)->getRow())
					&& ((*it_b_new).getCol() > (*it_b)->getCol()))
					++balls_count;
			}

			for (it_w = walls.begin(); it_w != walls.end(); ++it_w)
			{
				if (((*it_w)->getRowFirst() == (*it_w)->getRowSec()) && ((*it_w)->getRowFirst() == (*it_b)->getRow())
					&& ((*it_w)->getColFirst() >= (*it_b)->getCol()))
				{
					balls_count = 0;
					for (it_b_new = balls_old.cbegin(); it_b_new != balls_old.cend(); ++it_b_new)
					{
						if (((*it_b_new).getRow() == (*it_b)->getRow())
							&& ((*it_b_new).getCol() > (*it_b)->getCol())
							&& ((*it_b_new).getCol() <= (*it_w)->getColFirst()))
							++balls_count;
					}
					col_new = (*it_w)->getColFirst() + balls_count;
					isWall = true;
				}
			}

			if (!isWall)
				col_new = size - 1 - balls_count;

			for (it_h = holes.begin(); it_h != holes.end(); ++it_h)
			{
				if (((*it_h)->getRow() == (*it_b)->getRow()) && ((*it_h)->getCol() <= col_new) && ((*it_h)->getCol() > (*it_b)->getCol()))
				{
					if (((*it_h)->getNumber() == (*it_b)->getNumber()) && (balls_count == 0))
					{
						numbers.push_back((*it_h)->getNumber());
						++del_count;
					}
					else
						return false;
				}
				if (it_h == holes.end())
					break;
			}
			if (it_b == balls.end())
				break;
			if (del_count == 0)
				(*it_b)->setCol(col_new);
		}

		for (int i : numbers)
		{
			deleteBall(i);
			deleteHole(i);
		}

		if ((came_from != nullptr) &&
			(came_from->came_from != nullptr))
		{
			list<Ball*>::const_iterator iter;
			list<Ball*>::const_iterator iter_3;
			list<Ball*>::const_iterator iter_2;
			int count_3 = 0, count_2 = 0;
			for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
			{
				for (iter_3 = came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->balls.cend(); ++iter_3)
				{
					if ((*iter)->getNumber() == (*iter_3)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_3)->getRow()) &&
							((*iter)->getCol() == (*iter_3)->getCol()))
							++count_3;
					}
				}
				for (iter_2 = came_from->balls.cbegin(); iter_2 != came_from->balls.cend(); ++iter_2)
				{
					if ((*iter)->getNumber() == (*iter_2)->getNumber())
					{
						if (((*iter)->getRow() == (*iter_2)->getRow()) &&
							((*iter)->getCol() == (*iter_2)->getCol()))
							++count_2;
					}
				}
			}
			if (((count_3 == this->balls_count) && (this->balls_count == came_from->came_from->balls_count)) ||
				((count_2 == this->balls_count)) && (came_from->balls_count == this->balls_count))
				return false;
		}
		addToAnswer('W');
		return true;
	}

	bool Condition::isFinish()
	{
		if (balls_count != 1)
			return false;

		if ((came_from == nullptr) ||
			(came_from->came_from == nullptr) ||
			(came_from->came_from->came_from == nullptr) ||
			(came_from->came_from->came_from->came_from == nullptr))
			return false;

		if ((balls_count != came_from->balls_count) ||
			((balls_count != came_from->came_from->balls_count)) ||
			(balls_count != came_from->came_from->came_from->balls_count) ||
			(balls_count != came_from->came_from->came_from->came_from->balls_count))
			return false;

		list<Ball*>::const_iterator iter;
		list<Ball*>::const_iterator iter_3;
		list<Ball*>::const_iterator iter_4;
		int count_3 = 0, count_4 = 0;

		for (iter = balls.cbegin(); iter != balls.cend(); ++iter)
		{
			for (iter_3 = came_from->came_from->came_from->balls.cbegin(); iter_3 != came_from->came_from->came_from->balls.cend(); ++iter_3)
			{
				if ((*iter)->getNumber() == (*iter_3)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_3)->getRow()) &&
						((*iter)->getCol() == (*iter_3)->getCol()))
						++count_3;
				}
			}

			for (iter_4 = came_from->came_from->came_from->came_from->balls.cbegin(); iter_4 != came_from->came_from->came_from->came_from->balls.cend(); ++iter_4)
			{
				if ((*iter)->getNumber() == (*iter_4)->getNumber())
				{
					if (((*iter)->getRow() == (*iter_4)->getRow()) &&
						((*iter)->getCol() == (*iter_4)->getCol()))
						++count_4;
				}
			}
		}

		if ((count_3 == balls_count) || (count_4 == balls_count))
		{
			answer = "Impossible";
			return true;
		}
		else
			return false;
	}

}