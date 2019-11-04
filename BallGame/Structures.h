#pragma once

namespace nsBallGame
{
 /*!
  * \class Ball
  * \brief Модель шара/отверстия:
  * координаты (строка, столбец) и номер
  * \author Карунный Я.В.
  */
class Ball
{
	int row;	///< Строка
	int col;	///< Столбец
	int number;	///< Номер шара/стены

public:

	/*!
	 * \brief Конструктор
	 * \param x Строка
	 * \param y Столбец
	 * \param num Номер шара/стены
	 */
	explicit Ball(int x, int y, int num);

	/*!
	 * \brief Установка строки
	 * \param x Строка
	 */
	void setRow(int x);

	/*!
	 * \brief Установка столбца
	 * \param y строка
	 */
	void setCol(int y);

	/*!
	 * \brief Установка номера
	 * \param num номер
	 */
	void setNumber(int num);

	/*!
	 * \brief Получение строки
	 * \return номер строки
	 */
	int getRow() const;

	/*!
	 * \brief Получение столбца
	 * \return номер столбца
	 */
	int getCol() const;

	/*!
	 * \brief Получение номера шара/отверстия
	 * \return номер шара/отверстия
	*/
	int getNumber() const;
};

/*!
 * \class Wall
 * \brief Модель стены:
 * координаты начала стены (строка, столбец) и координаты конца стены (строка столбец)
 * \author Карунный Я.В.
 */
class Wall
{
	int row_first;	///< координаты строки начала стены
	int col_first;	///< координаты столбца начала стены
	int row_sec;	///< координаты строки конца стены
	int col_sec;	///< координаты столбца конца стены

public:

	/*!
	 * \brief Конструктор
	 * \param row Строка начала
	 * \param col Столбец начала
	 * \param row2 Строка конца
	 * \param col2 Столбец конца
	 */
	explicit Wall(int row, int col, int row2, int col2);

	/*!
	 * \brief Получение координаты (строки) начала стены
	 * \return координата (строка) начала стены
	 */
	int getRowFirst() const;

	/*!
	 * \brief Получение координаты (строки) конца стены
	 * \return координата (строки) конца стены
	 */
	int getRowSec() const;

	/*!
	 * \brief Получение координаты (столбца) начала стены
	 * \return координата (столбец) начала стены
	 */
	int getColFirst() const;

	/*!
	 * \brief Получение координаты (столбца) конца стены
	 * \return координата (столбец) конца стены
	 */
	int getColSec() const;
};
}