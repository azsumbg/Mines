#include "pch.h"
#include "mines.h"


// RANDIT ******************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937{ sq };
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max <= min)return -1;

	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}

////////////////////////////////////////

// GRID ********************************

dll::GRID::GRID(int rows_number, int cols_number, int level) :rows{ rows_number }, cols{ cols_number }
{
	array = new TILE * [rows] {};
	for (int i = 0; i < rows; ++i)array[i] = new TILE[cols]{};

	switch (level)
	{
	case 1:
		all_mines = LEVEL1_MINES;
		for (int i = 0; i <= all_mines; ++i)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;
				int temp_row{ RandGen(0,LEVEL1_ROWS - 1) };
				int temp_col{ RandGen(0,LEVEL1_COLS - 1) };
				if (array[temp_row][temp_col].content == MINE)ok = false;
				else array[temp_row][temp_col].content = MINE;
			}
		}
		break;

	case 2:
		all_mines = LEVEL2_MINES;
		for (int i = 0; i <= all_mines; ++i)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;
				int temp_row{ RandGen(0,LEVEL2_ROWS - 1) };
				int temp_col{ RandGen(0,LEVEL2_COLS - 1) };
				if (array[temp_row][temp_col].content == MINE)ok = false;
				else array[temp_row][temp_col].content = MINE;
			}
		}
		break;

	case 3:
		all_mines = LEVEL3_MINES;
		for (int i = 0; i <= all_mines; ++i)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;
				int temp_row{ RandGen(0,LEVEL3_ROWS - 1) };
				int temp_col{ RandGen(0,LEVEL3_COLS - 1) };
				if (array[temp_row][temp_col].content == MINE)ok = false;
				else array[temp_row][temp_col].content = MINE;
			}
		}
		break;

	case 4:
		all_mines = LEVEL4_MINES;
		for (int i = 0; i <= all_mines; ++i)
		{
			bool ok = false;
			while (!ok)
			{
				ok = true;
				int temp_row{ RandGen(0,LEVEL4_ROWS - 1) };
				int temp_col{ RandGen(0,LEVEL4_COLS - 1) };
				if (array[temp_row][temp_col].content == MINE)ok = false;
				else array[temp_row][temp_col].content = MINE;
			}
		}
		break;
	}

	float tx{ 0 };
	float ty{ 50.0f };

	for (int i = 0; i < rows; ++i)
	{
		for (int k = 0; k < cols; ++k)
		{
			array[i][k].dims.left = tx;
			array[i][k].dims.right = tx + CELL_DIM - 1.0f;
			array[i][k].dims.up = ty;
			array[i][k].dims.down = ty + CELL_DIM - 1.0f;
			tx += CELL_DIM;
		}

		tx = 0;
		ty += CELL_DIM;
	}

	for (int i = 0; i < rows; ++i)
	{
		for (int k = 0; k < cols; ++k)
		{
			MinesArround(i, k);
		}
	}
}
dll::GRID::~GRID()
{
	if (array)delete[]array;
}

void dll::GRID::MinesArround(int row, int col)
{
	unsigned char my_flags{ 0 };

	constexpr unsigned char up_flag{ 0b00000001 };
	constexpr unsigned char down_flag{ 0b00000010 };
	constexpr unsigned char left_flag{ 0b00000100 };
	constexpr unsigned char right_flag{ 0b00001000 };

	if (row - 1 >= 0)my_flags |= up_flag;
	if (row + 1 < rows)my_flags |= down_flag;
	if (col - 1 >= 0)my_flags |= left_flag;
	if (col + 1 < cols)my_flags |= right_flag;

	if (my_flags & up_flag)
	{
		if (array[row - 1][col].content == MINE)array[row][col].mines_arround++;

		if (my_flags & left_flag)
		{
			if (array[row - 1][col - 1].content == MINE)array[row][col].mines_arround++;
			if (array[row][col - 1].content == MINE)array[row][col].mines_arround++;
		}
		if (my_flags & right_flag)
		{
			if (array[row - 1][col + 1].content == MINE)array[row][col].mines_arround++;
			if (array[row][col + 1].content == MINE)array[row][col].mines_arround++;
		}
	}
	if (my_flags & down_flag)
	{
		if (array[row + 1][col].content == MINE)array[row][col].mines_arround++;

		if (my_flags & left_flag)
		{
			if (array[row + 1][col - 1].content == MINE)array[row][col].mines_arround++;
			if (array[row][col + 1].content == MINE)array[row][col].mines_arround++;
		}
		if (my_flags & right_flag)
		{
			if (array[row + 1][col + 1].content == MINE)array[row][col].mines_arround++;
			if (array[row][col + 1].content == MINE)array[row][col].mines_arround++;
		}
	}
	
	if (!(my_flags & up_flag) && !(my_flags & down_flag))
	{
		if (my_flags & left_flag)
		{
			if (array[row][col - 1].content == MINE)array[row][col].mines_arround++;
		}
		if (my_flags & right_flag)
		{
			if (array[row][col + 1].content == MINE)array[row][col].mines_arround++;
		}
	}

}
int dll::GRID::MinesRemaining()const
{
	return all_mines;
}
void dll::GRID::MineFound(int row, int col)
{
	if (array[row][col].content == MINE)--all_mines;
}
int dll::GRID::SelectTile(int row, int col)
{
	if (!array[row][col].selected)
	{
		if (array[row][col].content == MINE)return MINE;
		else
		{
			array[row][col].selected = true;
		}
	}

	return array[row][col].content;
}
int dll::GRID::ShowTileInfo(int row, int col)
{
	if (!array[row][col].selected)return UNKNOWN_MINE;
	if (!array[row][col].selected && array[row][col].content == MARKED_MINE)return MARKED_MINE;

	return array[row][col].content;
}

void dll::GRID::Release()
{
	delete this;
}

////////////////////////////////////////