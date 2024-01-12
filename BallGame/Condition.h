#pragma once

// Self
#include "Structures.h"

// C++
#include <list>
#include <memory>
#include <string>

namespace ball_game
{

/**
 * \class Condition
 * \brief Класс хранения состояний
 */
class Condition
{
public:

	/**
	 * \brief Тип направления
	 */
	enum class DirectionType
	{
		North,
		South,
		East,
		West,
	};

	/**
	 * \brief Конструктор
	 * \param nSize размер поля
	 */
	explicit Condition(int nSize);

	/**
	 * \brief Конструктор копирования
	 */
	Condition(const Condition& cond);

	/**
	 * \brief Оператор копирования
	 */
	Condition& operator=(const Condition& rhs);

	Condition() = default;
	~Condition() = default;
	Condition(Condition&& other) noexcept = default;
	Condition& operator=(Condition&& other) noexcept = default;

	/**
	 * \brief функция получения пути
	 * \return путь
	 */
	[[nodiscard]] std::string GetAnswer() const;

	/**
	 * \brief функция установки предыдущего состояния
	 * \param cf предыдущее состояние
	 */
	void SetCameFrom(const Condition& cf) noexcept;

	/**
	 * \brief функция получения предыдущего состояния
	 * \return предыдущее состояние
	 */
	[[nodiscard]] std::shared_ptr<Condition> GetCameFrom() const noexcept;

	/**
	 * \brief функция установки шары
	 * \param num номер шара
	 * \param row строка
	 * \param col столбец
	 */
	void SetBall(int num, int row, int col) noexcept;

	/**
	 * \brief функция установки лунки
	 * \param num номер лунки
	 * \param row строка
	 * \param col столбец
	 */
	void SetHole(int num, int row, int col) noexcept;

	/**
	 * \brief функция установки стены
	 * \param row строка
	 * \param col столбец
	 * \param row2 строка 2
	 * \param col2 столбец 2
	 */
	void SetWall(int row, int col, int row2, int col2) noexcept;

	/**
	 * \brief удаление мяча и лунки по номеру
	 * \param number номер мяча/лунки
	 */
	void DeleteDataByNumber(int number) noexcept;

	/**
	 * \brief функция возврата количества мячей и лунок
	 * \return количество мячей и лунок
	 */
	[[nodiscard]] int GetBallsAndHolesCount() const noexcept;

	/**
	 * \brief функция возврата размера поля
	 * \return размер поля
	 */
	[[nodiscard]] int GetSize() const noexcept;

	/**
	 * \brief движение на север
	 * \return да/нет
	 */
	[[nodiscard]] bool GoNorth();

	/**
	 * \brief движение на юг
	 * \return да/нет
	 */
	[[nodiscard]] bool GoSouth();

	/**
	 * \brief движение на восток
	 * \return да/нет
	 */
	[[nodiscard]] bool GoEast();

	/**
	 * \brief движение на запад
	 * \return да/нет
	 */
	[[nodiscard]] bool GoWest();

	/**
	 * \brief функция проверки выхода из цикла
	 * \return да/нет
	 */
	[[nodiscard]] bool IsFinish();

private:
	std::list<std::shared_ptr<Ball>> m_holes;	///< лунки
	std::list<std::shared_ptr<Wall>> m_walls;	///< стены
	std::list<std::shared_ptr<Ball>> m_balls;	///< шары

	int m_size{ 0 };			///< размер поля
	std::string m_answer;		///< путь

	std::shared_ptr<Condition> m_cameFrom{ nullptr }; ///< указатель на предыдущее состояние

	/**
	 * \brief функция добавления текущего направления
	 * \param type тип направления
	 */
	void AddToAnswer(DirectionType type) noexcept;
};
} // namespace ball_game
