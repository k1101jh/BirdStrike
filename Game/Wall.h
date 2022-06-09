#pragma once
#include "GameObject.h"

class Wall: public GameObject
{
public:
	Wall(GameScene& game_scene, Vector2<int> position, Vector2<int> scale);
	virtual ~Wall(void);

private:
	Sprite sprite_;
};
