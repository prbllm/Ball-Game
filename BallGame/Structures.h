#pragma once

namespace BallGame
{
class Ball
{
public:
	explicit Ball(int x, int y, int num);
	void setRow(int x) noexcept;
	void setCol(int y) noexcept;
	void setNumber(int num) noexcept;
	[[nodiscard]] int getRow() const noexcept;
	[[nodiscard]] int getCol() const noexcept;
	[[nodiscard]] int getNumber() const noexcept;

private:
	int row{ 0 };
	int col{ 0 };
	int number{ 0 };


};

class Wall
{
public:
	explicit Wall(int row, int col, int row2, int col2);
	[[nodiscard]] int getRowFirst() const noexcept;
	[[nodiscard]] int getRowSec() const noexcept;
	[[nodiscard]] int getColFirst() const noexcept;
	[[nodiscard]] int getColSec() const noexcept;

private:
	int row_first{ 0 };
	int col_first{ 0 };
	int row_sec{ 0 };
	int col_sec{ 0 };
};

} // namespace BallGame
