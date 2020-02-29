#pragma once

// C++
#include <list>
#include <memory>

namespace nsGame
{

	namespace nsSearch
	{

		class Condition;

		/**
		 * \class BFS
		 * \brief Класс поиска в ширину (breadth-first search)
		 */
		class BFS
		{
			int cases_count; ///< количество кейсов

			/**
			 * \brief функция поиска следующих точек
			 * \param state текущее состояние
			 * \param states список возможных состояний
			 */
			void getNextPoints(const Condition& state, std::list<Condition>& states);

		public:

			/**
			 * \brief Конструктор по умолчанию
			 */
			explicit BFS();

			/**
			 * \brief Деструктор
			 */
			~BFS() = default;

			/**
			 * \brief основная функция запуска алгоритма поиска
			 * \param start начальное состояние
			 */
			void run(std::unique_ptr<Condition> start);

		};
	}
}