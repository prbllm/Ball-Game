#pragma once

// C++
#include <list>

namespace ball_game
{

class Condition;

/**
 * \class Bfs
 * \brief Класс поиска в ширину (breadth-first search)
 */
class Bfs
{
public:

	/**
	 * \brief Конструктор по умолчанию
	 */
	explicit Bfs() = default;

	/**
	 * \brief Деструктор
	 */
	~Bfs() = default;

	/**
	 * \brief основная функция запуска алгоритма поиска
	 * \param start начальное состояние
	 */
	void Run(const Condition & start) noexcept;

private:

	int m_casesCount{ 1 };

	/**
	 * \brief функция поиска следующих точек
	 * \param state текущее состояние
	 * \param states список возможных состояний
	 */
	void GetNextPoints(const Condition& state, std::list<Condition>& states) noexcept;

};

} // namespace ball_game
