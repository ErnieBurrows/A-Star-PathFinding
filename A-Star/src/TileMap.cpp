#include "TileMap.h"
#include "Game.h"
#include "Util.h"
#include <queue>

Cell TileMap::GridPosition(glm::vec2 pixelPosition)
{
	return { (int)pixelPosition.x / tileWidth, (int)pixelPosition.y / tileHeight };
}

glm::vec2 TileMap::PixelPosition(Cell gridPosition)
{
	return { gridPosition.col * tileWidth, gridPosition.row * tileHeight };
}

glm::vec2 TileMap::Lerp(Cell a, Cell b, float t)
{
	return Util::Lerp(PixelPosition(a) + glm::vec2((Config::SCREEN_WIDTH / GRID_SIZE) * 0.5, (Config::SCREEN_HEIGHT / GRID_SIZE) * 0.5), PixelPosition(b) + glm::vec2((Config::SCREEN_WIDTH / GRID_SIZE) * 0.5, (Config::SCREEN_HEIGHT / GRID_SIZE) * 0.5), t);
}

void TileMap::Init(Scene* scene)
{
	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE; col++)
		{
			glm::vec2 labelPosition = PixelPosition({ col, row });
			labelPosition.x += tileWidth * 0.5f;
			labelPosition.y += tileHeight * 0.5f;

			Label* label = new Label("N/A", "Consolas", 14, { 255, 0, 0, 255 }, labelPosition);
			m_info[row][col].label = label;
		}
	}
	UpdateScores();
}

void TileMap::RenderTile(Cell cell, glm::vec3 colour)
{
	glm::vec2 tilePosition{ cell.col * tileWidth, cell.row * tileHeight };
	glm::vec2 adj = glm::vec2((Config::SCREEN_WIDTH / GRID_SIZE) * 0.5, (Config::SCREEN_HEIGHT / GRID_SIZE) * 0.5);
	if (colour == glm::vec3{ 0.65f, 0.79f, 0.34f })
	{
		// For rendering map objects
		TextureManager::Instance().Load("../Assets/textures/bush.png", "bush");
		TextureManager::Instance().Draw("bush", tilePosition + adj, 0.0, 255, true);
	}
	else if (colour == glm::vec3{ 0.66f, 0.52f, 0.4f })
	{
		TextureManager::Instance().Load("../Assets/textures/dirt.png", "dirt");
		TextureManager::Instance().Draw("dirt", tilePosition + adj, 0.0, 255, true);
	}
	else if (colour == glm::vec3{ 0.56f, 0.87f, 0.93f })
	{
		TextureManager::Instance().Load("../Assets/textures/water.png", "water");
		TextureManager::Instance().Draw("water", tilePosition + adj, 0.0, 255, true);
	}
	else Util::DrawFilledRect(tilePosition, tileWidth, tileHeight, glm::vec4(colour, 1.0f));
	if (m_costVisibility)
	{
		Util::DrawFilledRect(tilePosition, tileWidth, tileHeight, glm::vec4(colour, 1.0f));
		m_info[cell.row][cell.col].label->Draw();
	}
}

void TileMap::RenderTile(Cell cell, TileType type)
{
	glm::vec3 tileColour;
	switch (type)
	{
	case AIR:
		tileColour = { 1.0f, 1.0f, 1.0f };
		break;

	case GRASS:
		tileColour = { 0.65f, 0.79f, 0.34f };
		break;

	case WATER:
		tileColour = { 0.56f, 0.87f, 0.93f };
		break;

	case MUD:
		tileColour = { 0.66f, 0.52f, 0.4f };
		break;
	}
	RenderTile(cell, tileColour);
}

void TileMap::RenderTile(Cell cell, int cost)
{
	glm::vec3 tileColour;
	switch (cost)
	{
	case 0:
		tileColour = { 1.0f, 1.0f, 1.0f };
		break;
	case 1:
		tileColour = { 0.50f, 1.0f, 0.88f };
		break;
	case 2:
		tileColour = { 0.45f, 0.94f, 0.87f };
		break;
	case 3:
		tileColour = { 0.39f, 0.87f, 0.87f };
		break;
	case 4:
		tileColour = { 0.34f, 0.81f, 0.88f };
		break;
	case 5:
		tileColour = { 0.28f, 0.75f, 0.89f };
		break;
	case 6:
		tileColour = { 0.31f, 0.66f, 0.87f };
		break;
	case 7:
		tileColour = { 0.33f, 0.56f, 0.85f };
		break;
	case 8:
		tileColour = { 0.37f, 0.38f, 0.81f };
		break;
	case 9:
		tileColour = { 0.41f, 0.19f, 0.76f };
		break;
	case 10:
		tileColour = { 0.45f, 0.0f, 0.68f };
		break;
	default:
		tileColour = { 0.0f, 0.0f, 0.0f };
		break;
	}

	RenderTile(cell, tileColour);
}

void TileMap::Render()
{
	if (!m_coloured)
	{
		for (int row = 0; row < GRID_SIZE; row++)
		{
			for (int col = 0; col < GRID_SIZE; col++)
			{
				RenderTile({ col, row }, (TileType)m_tiles[row][col]);
			}
		}
	}
	else
	{
		for (int row = 0; row < GRID_SIZE; row++)
		{
			for (int col = 0; col < GRID_SIZE; col++)
			{
				RenderTile({ col, row }, m_info[row][col].cost);
			}
		}
	}

	RenderTile(start, { 0.0f, 1.0f, 0.0f });
	RenderTile(end, { 1.0f, 0.0f, 0.0f });
}

void TileMap::UpdateScores()
{
	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE; col++)
		{
			float g = m_distanceType == EUCLIDEAN ? Euclidean({ col, row }, end) : Manhattan({ col, row }, end);
			float h = Cost((TileType)m_tiles[row][col]);
			std::string text = std::to_string(g + h);
			text.resize(4);
			m_info[row][col].label->SetText("H: " + text);
			m_info[row][col].cost = g + h;
		}
	}
}

DistanceType& TileMap::GetDistanceType()
{
	return m_distanceType;
}

void TileMap::SetDistanceType(DistanceType type)
{
	m_distanceType = type;
}

bool TileMap::GetColoured()
{
	return m_coloured;
}

void TileMap::SetColoured(bool val)
{
	m_coloured = val;
}

bool TileMap::GetCostVis()
{
	return m_costVisibility;
}

void TileMap::SetCostVis(bool val)
{
	m_costVisibility = val;
}

Path TileMap::FindPath()
{
	const int tileCount = GRID_SIZE * GRID_SIZE;
	std::vector<Node> tileNodes(tileCount);
	std::priority_queue<Node, std::vector<Node>, decltype(&Compare)> openList(Compare);
	std::vector<bool> closedList(tileCount, false);
	tileNodes[Index(start)].parent = start;
	openList.push({ start });

	while (!openList.empty())
	{
		const Cell currentCell = openList.top().cell;

		// End condition
		if (currentCell == end)
		{
			break;
		}

		// Continue with A* by popping fromt the front of the queue and adding it to the close list so we don't revisit it.
		openList.pop();
		closedList[Index(currentCell)] = true;

		float gNew, hNew;
		for (const Cell& neighbour : Neighbours(currentCell))
		{
			const int neighbourIndex = Index(neighbour);

			// Skip if already visited
			if (closedList[neighbourIndex]) continue;
			// Skip if tile is mud
			else if ((TileType)m_tiles[neighbour.row][neighbour.col] == MUD) continue;

			gNew = m_distanceType == EUCLIDEAN ? Euclidean(currentCell, neighbour) : Manhattan(currentCell, neighbour);
			hNew = m_distanceType == EUCLIDEAN ? Euclidean(neighbour, end) : Manhattan(neighbour, end);
			hNew += Cost((TileType)m_tiles[neighbour.row][neighbour.col]);

			// Append if unvisited or best score
			if (tileNodes[neighbourIndex].F() <= FLT_EPSILON || (gNew + hNew) < tileNodes[neighbourIndex].F())
			{
				openList.push({ neighbour, gNew, hNew });
				tileNodes[neighbourIndex] = { neighbour, currentCell, gNew, hNew };
			}
		}
	}

	Path path;
	Cell currentCell = end;
	int currentIndex = Index(currentCell);

	if (tileNodes[currentIndex].parent == Cell{ -1, -1 })
	{
		return {};
	}

	while (!(tileNodes[currentIndex].parent == currentCell))
	{
		path.push_back(currentCell);
		currentCell = tileNodes[currentIndex].parent;
		currentIndex = Index(currentCell);
	}
	std::reverse(path.begin(), path.end());

	return path;
}

std::vector<Cell> TileMap::Neighbours(Cell cell)
{
	std::vector<Cell> neighbours;
	for (int row = cell.row - 1; row <= cell.row + 1 && row >= 0 && row < GRID_SIZE; row++)
	{
		for (int col = cell.col - 1; col <= cell.col + 1 && col >= 0 && col < GRID_SIZE; col++)
		{
			// Double check Connor's lab for the hint regarding impassable tiles.
			Cell neighbour{ col, row };
			if (!(neighbour == cell)) neighbours.push_back(neighbour);
		}
	}

	return neighbours;
}

float Manhattan(Cell a, Cell b)
{
	return abs(b.col - a.col) + abs(b.row - a.row);
}

float Euclidean(Cell a, Cell b)
{
	float dx = b.col - a.col;
	float dy = b.row - a.row;
	return sqrtf(dx * dx + dy * dy);
}

float Cost(TileType type)
{
	static std::array<float, COUNT> costs
	{
		0.0f,	// Air
		10.0f,	// Grass
		20.0f,	// Water
		50.0f	// Mud
	};
	return costs[type];
}

bool Compare(const Node& a, const Node& b)
{
	return a.F() > b.F();
}

bool operator==(const Cell& a, const Cell& b)
{
	return a.row == b.row && a.col == b.col;
}
