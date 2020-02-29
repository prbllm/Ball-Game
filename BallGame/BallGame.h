#pragma once

// Self
#include "Condition.h"

// C++
#include <queue>
#include <memory>

namespace nsGame
{
	class BallGame
	{
		using queue = std::queue<std::unique_ptr<nsCondition::Condition>>;
	public:
		
		/**
		 * \brief Конструктор по умолчанию
		 */
		explicit BallGame();
		
		/**
		 * \brief Деструктор по умолчанию
		 */
		~BallGame();
		
		/**
		 * \brief Запуск поиска решений
		 */
		void Start();
	private:
		
		/**
		 * \brief Загрузка первоначальных данных из файла
		 */
		void Initialize();

		queue cases; // количество проверяемых вариантов
	};
}