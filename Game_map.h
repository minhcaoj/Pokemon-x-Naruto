#pragma once
#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_MAP_TILES 20
class tileMap : public BaseObject
{
public:
	tileMap() { ; }
	~tileMap() { ; }
};

class GameMap
{
public: 
	GameMap() { ; }
	~GameMap() { ; }

	void loadMap(char* name);
	void loadTiles(SDL_Renderer* render);
	void DrawMap(SDL_Renderer* render);

private:
	Map game_map;
	tileMap tile_mat[MAX_MAP_TILES];
};
