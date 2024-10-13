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
	if (!isSurround(x, y))
	{
		throw std::invalid_argument("x and y are not surround");
	}

	if (isNeighbor(x, y))
	{
		return;
	}

	neighbor[x].push_back(y);
	neighbor[y].push_back(x);
}

void AdjacencyList::disconnect(int x, int y)
{
	if (!isSurround(x, y))
	{
		throw std::invalid_argument("x and y are not surround");
	}

	if (!isNeighbor(x, y))
	{
		return;
	}

	neighbor[x].erase(std::remove(neighbor[x].begin(), neighbor[x].end(), y), neighbor[x].end());
	neighbor[y].erase(std::remove(neighbor[y].begin(), neighbor[y].end(), x), neighbor[y].end());
}
//²¢²é¼¯
UnionFind::UnionFind(int n)
{
	parent.resize(n);
	for (int i = 0; i < n; i++)
	{
		parent[i] = i;
	}
}

UnionFind::~UnionFind()
{

}

int UnionFind::find(int x)
{
	int root = x;
	while (parent[root] != root)
	{
		root = parent[root];
	}

	return root;
}

void UnionFind::connect(int x, int y)
{
	int root_x = find(x);
	int root_y = find(y);

	if (root_x != root_y)
	{
		parent[root_x] = root_y;
	}
}

bool UnionFind::isConnected(int x, int y)
{
	return find(x) == find(y);
}
