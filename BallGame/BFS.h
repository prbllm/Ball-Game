#pragma once

#include <list>

namespace nsBallGame
{

class Condition;

/*!
 * \class BFS
 * \brief Класс поиска в ширину (breadth-first search)
 * \author Карунный Я.В.
 */
class BFS
{

int cases_count;	///< количество кейсов

/*!
 * \brief функция поиска следующих точек
 * \param state текущее состояние
 * \param states список возможных состояний
 */
void getNextPoints(const Condition & state, std::list<Condition> & states);

public:

/*!
 * \brief Конструктор
 */
BFS();

/*!
 * \brief Деструктор
 */
~BFS();

/*!
 * \brief основная функция запуска алгоритма поиска
 * \param start начальное состояние
 */
void run(const Condition & start);

};
};