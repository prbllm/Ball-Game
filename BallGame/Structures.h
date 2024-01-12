#pragma once

namespace ball_game
{

class Ball
{
public:
	explicit Ball(int x, int y, int num);
	void SetRow(int x) noexcept;
	void SetCol(int y) noexcept;
	void SetNumber(int num) noexcept;
	[[nodiscard]] int GetRow() const noexcept;
	[[nodiscard]] int GetCol() const noexcept;
	[[nodiscard]] int GetNumber() const noexcept;

private:
	int m_row{ 0 };
	int m_col{ 0 };
	int m_number{ 0 };
};

class Wall
{
public:
	explicit Wall(int row, int col, int row2, int col2);
	[[nodiscard]] int GetRowFirst() const noexcept;
	[[nodiscard]] int GetRowSec() const noexcept;
	[[nodiscard]] int GetColFirst() const noexcept;
	[[nodiscard]] int GetColSec() const noexcept;

private:
	int m_rowFirst{ 0 };
	int m_colFirst{ 0 };
	int m_rowSec{ 0 };
	int m_colSec{ 0 };
};

} // namespace ball_game
