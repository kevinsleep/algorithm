#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <stdexcept>
enum Direction
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

class UnionFind;
class AdjacencyList;

class AdjacencyList
{
public:
	AdjacencyList();
	AdjacencyList(int row, int column);
	~AdjacencyList();
	
	void connect(int x, int y);
	void disconnect(int x, int y);

	int getRow() const { return row; }
	int getColumn() const { return column; }

	std::shared_ptr<std::vector<int>> getNeighbor(int i)  { return std::make_shared<std::vector<int>>(neighbor[i]); }
	std::shared_ptr<std::vector<int>> getSurround(int i)  { return std::make_shared<std::vector<int>>(surround[i]); }

	bool isSurround(int i, int j)
	{
		for (auto& k : surround[i])
		{
			if (k == j)
			{
				return true;
			}
		}

		return false;
	}

	bool isNeighbor(int i, int j)
	{
		for (auto& k : neighbor[i])
		{
			if (k == j)
			{
				return true;
			}
		}

		return false;
	}

private:
	int row;
	int column;

	std::vector<std::vector<int>> neighbor;
	std::vector<std::vector<int>> surround;


};

class UnionFind
{
public:
	UnionFind(int n);
	~UnionFind();

	int find(int x);
	void connect(int x, int y);
	bool isConnected(int x, int y);
private:
	std::vector<int> parent;
};

