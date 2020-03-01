#pragma once

// Self
#include "Condition.h"

// C++
#include <exception>
#include <memory>
#include <mutex>
#include <vector>

namespace nsGame
{
	class BallGame
	{
	public:
		
		/**
		 * \brief Конструктор по умолчанию
		 */
		explicit BallGame() = default;
		
		/**
		 * \brief Деструктор по умолчанию
		 */
		~BallGame() = default;
		
		/**
		 * \brief Запуск поиска решений
		 */
		void Start();
	private:
		
		/**
		 * \brief Загрузка первоначальных данных из файла
		 */
		void Initialize();

		/**
		 * \brief Запуск поиска решений с отловом исключений
		 */
		void StartWithExceptions();

		std::vector<std::shared_ptr<nsCondition::Condition>> _cases; ///< количество проверяемых вариантов
		std::vector<std::exception_ptr> _exceptions; ///< набор исключений 

		std::mutex _mtx; ///< мьютекс для синхронизации обращения к консоли
	};
}