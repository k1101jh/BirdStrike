#pragma once
#include <cstdlib>
#include "Game.h"
#include "IGameScene.h"
#include "GameObject.h"
#include "GroundPatternID.h"
#include "Vector2.h"
#include "SpriteRenderer.h"
#include "Rigidbody.h"


class GameManager: public GameObject
{
public:
	GameManager(IGameScene& game_scene);
	virtual ~GameManager(void);
	virtual void Update(const float& delta_time);
	std::shared_ptr<IGameObject> CreateGround(Vector2<int> position);
	std::shared_ptr<IGameObject> CreateCoin(Vector2<int> position);
	void CreateStraightGround(int num_ground, Vector2<int> position, int interval=100);
	void CreateStraightCoin(int num_ground, Vector2<int> position, int interval=100);
	void CreateGroundPattern(GroundPatternID pattern_id);

private:
	static bool instantiated_;
	ULONGLONG last_spawn_time_;
	ULONGLONG time_between_spawn_;
	int remain_ground_length_;
	int cur_ground_height_;
	bool ground_in_high_pos_;
	const int kNumInitGrounds = 12;
	const float kGroundSpeed = -300.0f;
	const Vector2<int> kGroundScale = Vector2<int>(100, 100);
	const Vector2<int> kCoinScale = Vector2<int>(100, 100);
};
