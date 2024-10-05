#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() : AdjacencyList(50,50)
{

}

AdjacencyList::AdjacencyList(int row, int column) : row(row), column(column)
{
	neighbor.resize(row * column);
	surround.resize(row * column);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			int index = i * column + j;
			if (i > 0)
			{
				surround[index].push_back(index - column);
			}
			if (i < row - 1)
			{
				surround[index].push_back(index + column);
			}
			if (j > 0)
			{
				surround[index].push_back(index - 1);
			}
			if (j < column - 1)
			{
				surround[index].push_back(index + 1);
			}
		}
	}

}

AdjacencyList::~AdjacencyList()
{
}

void AdjacencyList::connect(int x, int y)
{
}

void AdjacencyList::disconnect(int x, int y)
{
}


