#pragma once

// C++
#include <list>

namespace BallGame
{

class Condition;

/**
 * \class BFS
 * \brief Класс поиска в ширину (breadth-first search)
 */
class BFS
{
public:

	/**
	 * \brief Конструктор по умолчанию
	 */
	explicit BFS() = default;

	/**
	 * \brief Деструктор
	 */
	~BFS() = default;

	/**
	 * \brief основная функция запуска алгоритма поиска
	 * \param start начальное состояние
	 */
	void run(const Condition & start) noexcept;

private:

	int cases_count{ 1 };

	/**
	 * \brief функция поиска следующих точек
	 * \param state текущее состояние
	 * \param states список возможных состояний
	 */
	void getNextPoints(const Condition& state, std::list<Condition>& states) noexcept;

};

} // namespace BallGame
