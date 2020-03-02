#pragma once

// C++
#include <list>
#include <memory>
#include <string>

// Self
#include "Structures.h"

using namespace std;

namespace nsGame
{
	
	namespace nsCondition
	{

		/**
		 * \class Condition
		 * \brief Класс хранения состояний
		 */
		class Condition
		{
		private:

			list<Ball*> holes;	///< лунки

			list<Wall*> walls;	///< стены

			list<Ball*> balls;	///< шары

			int holes_count;	///< количество лунок

			int walls_count;	///< количество стен

			int balls_count;	///< количество шаров

			Condition* came_from;	///< указатель на предыдущее состояние

			string answer;	///< путь

			int size;	///< размер поля

		public:

			/**
			 * \brief Конструктор по умолчанию
			 */
			explicit Condition();

			/**
			 * \brief Конструктор копирования
			 */
			Condition(const Condition& cond);

			/**
			 * \brief Конструктор
			 * \param n_balls количество шаров
			 * \param n_walls количество стен
			 * \param n_size размер поля
			 */
			Condition(int n_balls, int n_walls, int n_size);

			/**
			 * \brief Деструктор
			 */
			~Condition();

			/**
			 * \brief функция добавления текущего направления
			 * \param symbol направление
			 */
			void addToAnswer(char symbol);

			/**
			 * \brief функция установки пути
			 * \param ans путь
			 */
			void setAnswer(string ans);

			/**
			 * \brief функция получения пути
			 * \return путь
			 */
			string getAnswer() const;

			/**
			 * \brief функция установки предыдущего состояния
			 * \param cf предыдущее состояние
			 */
			void setCameFrom(const std::shared_ptr<Condition>& cf);

			/**
			 * \brief функция получения предыдущего состояния
			 * \return предыдущее состояние
			 */
			Condition* getCameFrom() const;

			/**
			 * \brief функция установки шары
			 * \param num номер шара
			 * \param row строка
			 * \param col столбец
			 */
			void setBall(int num, int row, int col);

			/**
			 * \brief функция установки лунки
			 * \param num номер лунки
			 * \param row строка
			 * \param col столбец
			 */
			void setHole(int num, int row, int col);

			/**
			 * \brief функция установки стены
			 * \param row строка
			 * \param col столбец
			 * \param row2 строка 2
			 * \param col2 столбец 2
			 * \return
			 */
			void setWall(int row, int col, int row2, int col2);

			/**
			 * \brief удаление мяча
			 * \param number номер мяча
			 */
			void deleteBall(int number);

			/**
			 * \brief удаление лунки
			 * \param number номер лунки
			 */
			void deleteHole(int number);

			/**
			 * \brief функция возврата количества мячей и лунок
			 * \return количество мячей и лунок
			 */
			int getBallsAndHolesCount() const;

			/**
			 * \brief функция возврата количества стен
			 * \return количество стен
			 */
			int getWallsCount() const;

			/**
			 * \brief функция возврата размера поля
			 * \return размер поля
			 */
			int getSize() const;

			/**
			 * \brief движение на север
			 * \return да/нет
			 */
			bool goNorth();

			/**
			 * \brief движение на юг
			 * \return да/нет
			 */
			bool goSouth();

			/**
			 * \brief движение на восток
			 * \return да/нет
			 */
			bool goEast();

			/**
			 * \brief движение на запад
			 * \return да/нет
			 */
			bool goWest();

			/**
			 * \brief функция проверки выхода из цикла
			 * \return да/нет
			 */
			bool isFinish();
		};

	}
};