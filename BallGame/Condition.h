#pragma once

// Self
#include "Structures.h"

// C++
#include <list>
#include <string>

namespace BallGame
{

/**
 * \class Condition
 * \brief Класс хранения состояний
 */
class Condition
{
public:

	enum class DirectionType
	{
		North,
		South,
		East,
		West,
	};

	/**
	 * \brief Конструктор по умолчанию
	 */
	explicit Condition() = default;

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
	void addToAnswer(DirectionType type) noexcept;

	/**
	 * \brief функция установки пути
	 * \param ans путь
	 */
	void setAnswer(std::string ans);

	/**
	 * \brief функция получения пути
	 * \return путь
	 */
	[[nodiscard]] std::string getAnswer() const;

	/**
	 * \brief функция установки предыдущего состояния
	 * \param cf предыдущее состояние
	 */
	void setCameFrom(const Condition * cf);

	/**
	 * \brief функция получения предыдущего состояния
	 * \return предыдущее состояние
	 */
	[[nodiscard]] Condition * getCameFrom() const;

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
	[[nodiscard]] int getBallsAndHolesCount() const;

	/**
	 * \brief функция возврата количества стен
	 * \return количество стен
	 */
	[[nodiscard]] int getWallsCount() const;

	/**
	 * \brief функция возврата размера поля
	 * \return размер поля
	 */
	[[nodiscard]] int getSize() const;

	/**
	 * \brief движение на север
	 * \return да/нет
	 */
	[[nodiscard]] bool goNorth();

	/**
	 * \brief движение на юг
	 * \return да/нет
	 */
	[[nodiscard]] bool goSouth();

	/**
	 * \brief движение на восток
	 * \return да/нет
	 */
	[[nodiscard]] bool goEast();

	/**
	 * \brief движение на запад
	 * \return да/нет
	 */
	[[nodiscard]] bool goWest();

	/**
	 * \brief функция проверки выхода из цикла
	 * \return да/нет
	 */
	[[nodiscard]] bool isFinish();

private:

	std::list<Ball*> holes;	///< лунки
	std::list<Wall*> walls;	///< стены
	std::list<Ball*> balls;	///< шары

	int holes_count{ 0 };	///< количество лунок
	int walls_count{ 0 };	///< количество стен
	int balls_count{ 0 };	///< количество шаров

	Condition* came_from{ nullptr }; ///< указатель на предыдущее состояние
	std::string answer;				 ///< путь
	int size{ 0 };				     ///< размер поля
};
} // namespace BallGame
