#pragma once

// Self
#include "Condition.h"

// C++
#include <memory>

namespace nsGame
{
	namespace nsSearch
	{
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
			 * \brief Запуск алгоритма поиска
			 * \param start начальное состояние
			 */
			void Run(const std::shared_ptr<nsCondition::Condition>& start);

		private:

			/**
			 * \brief Поиск следующих точек
			 * \param state текущее состояние
			 * \param states список возможных состояний
			 */
			void GetNextPoints(const std::shared_ptr<nsCondition::Condition>& state, std::list<std::shared_ptr<nsCondition::Condition>>& states) const;

			int cases_count{ 1 }; ///< количество кейсов
		};
	}
}