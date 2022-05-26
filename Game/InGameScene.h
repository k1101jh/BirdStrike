#pragma once
#include "GameScene.h"
#include "GameObject.h"
#include "BackGround.h"
#include "Ground.h"
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"

class GameScene;

class InGameScene: public GameScene
{
public:
	InGameScene(void);
	virtual ~InGameScene(void);
	virtual void OnInitialUpdate(void);

private:
	
};
