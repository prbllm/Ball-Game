#pragma once

// Self
#include "Structures.h"

// C++
#include <list>
#include <memory>
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
	 * \brief Оператор копирования
	 */
	Condition& operator=(const Condition& rhs);

	/**
	 * \brief Конструктор
	 * \param n_balls количество шаров
	 * \param n_size размер поля
	 */
	explicit Condition(int n_size);

	/**
	 * \brief Деструктор
	 */
	~Condition() = default;

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
	void setCameFrom(const Condition& cf) noexcept;

	/**
	 * \brief функция получения предыдущего состояния
	 * \return предыдущее состояние
	 */
	[[nodiscard]] std::shared_ptr<Condition> getCameFrom() const noexcept;

	/**
	 * \brief функция установки шары
	 * \param num номер шара
	 * \param row строка
	 * \param col столбец
	 */
	void setBall(int num, int row, int col) noexcept;

	/**
	 * \brief функция установки лунки
	 * \param num номер лунки
	 * \param row строка
	 * \param col столбец
	 */
	void setHole(int num, int row, int col) noexcept;

	/**
	 * \brief функция установки стены
	 * \param row строка
	 * \param col столбец
	 * \param row2 строка 2
	 * \param col2 столбец 2
	 */
	void setWall(int row, int col, int row2, int col2) noexcept;

	/**
	 * \brief удаление мяча и лунки по номеру
	 * \param number номер мяча/лунки
	 */
	void DeleteDataByNumber(int number) noexcept;

	/**
	 * \brief функция возврата количества мячей и лунок
	 * \return количество мячей и лунок
	 */
	[[nodiscard]] int getBallsAndHolesCount() const noexcept;

	/**
	 * \brief функция возврата размера поля
	 * \return размер поля
	 */
	[[nodiscard]] int getSize() const noexcept;

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
	[[nodiscard]] bool IsFinish();

private:
	std::list<std::shared_ptr<Ball>> holes;	///< лунки
	std::list<std::shared_ptr<Wall>> walls;	///< стены
	std::list<std::shared_ptr<Ball>> balls;	///< шары

	int size{ 0 };			///< размер поля
	std::string answer;		///< путь

	std::shared_ptr<Condition> came_from{ nullptr }; ///< указатель на предыдущее состояние
};
} // namespace BallGame
