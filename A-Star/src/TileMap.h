#pragma once
#include <array>
#include <glm/glm.hpp>
#include "Label.h"

enum TileType : int
{
	AIR,
	GRASS,
	WATER,
	MUD,
	COUNT
};

enum DistanceType
{
	MANHATTAN,
	EUCLIDEAN
};

struct Cell
{
	int col, row; // x, y
};

struct Node
{
	// Constructors
	Node() { Init(); }
	Node(Cell cell) { Init(cell); }
	Node(Cell cell, float g, float h) { Init(cell, {}, g, h); }
	Node(Cell cell, Cell parent, float g, float h) { Init(cell, parent, g, h); }

	void Init(Cell cell = {}, Cell parent = {}, float g = 0.0f, float h = 0.0f)
	{
		this->cell = cell;
		this->parent = parent;
		this->g = g;
		this->h = h;
	}
	float F() const { return g + h; }

	Cell cell;
	Cell parent;
	float g, h;
};

struct Tile
{
	Label* label;
	float cost;
};

using Path = std::vector<Cell>;


float Manhattan(Cell a, Cell b);
float Euclidean(Cell a, Cell b);
float Cost(TileType type);
bool Compare(const Node& a, const Node& b);
bool operator==(const Cell& a, const Cell& b);

constexpr int GRID_SIZE = 10;
class TileMap
{
public:
	Cell GridPosition(glm::vec2 pixelPosition);
	glm::vec2 PixelPosition(Cell gridPosition);
	glm::vec2 Lerp(Cell a, Cell b, float t);
	int Index(Cell cell) { return cell.row * GRID_SIZE + cell.col; }

	void Init(Scene* scene);
	void RenderTile(Cell cell, glm::vec3 colour);
	void RenderTile(Cell cell, TileType type);
	void RenderTile(Cell cell, int score);
	void Render();
	void UpdateScores();

	DistanceType& GetDistanceType();
	void SetDistanceType(DistanceType type);
	bool GetColoured();
	void SetColoured(bool val);
	bool GetCostVis();
	void SetCostVis(bool val);
	Path FindPath();
	std::vector<Cell> Neighbours(Cell cell);

	const int tileWidth = Config::SCREEN_WIDTH / GRID_SIZE;
	const int tileHeight = Config::SCREEN_HEIGHT / GRID_SIZE;
	Cell start{ 1, 8 }, end{ 8, 1 };

private:
	bool m_coloured = false, m_costVisibility = false;
	DistanceType m_distanceType = EUCLIDEAN;
	std::array<std::array<Tile, GRID_SIZE>, GRID_SIZE> m_info;
	std::array<std::array<int, GRID_SIZE>, GRID_SIZE> m_tiles
	{
		std::array<int, GRID_SIZE> {3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
		std::array<int, GRID_SIZE> {3, 0, 0, 0, 0, 0, 0, 0, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 1, 0, 1, 1, 1, 1, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 2, 0, 0, 0, 0, 2, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 2, 1, 0, 0, 0, 2, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 0, 0, 0, 0, 0, 2, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 0, 0, 0, 1, 0, 2, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 0, 0, 3, 1, 0, 2, 0, 3},
		std::array<int, GRID_SIZE> {3, 0, 0, 0, 0, 0, 0, 0, 1, 3},
		std::array<int, GRID_SIZE> {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}
	};
};