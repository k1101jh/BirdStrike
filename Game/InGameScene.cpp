#include "stdafx.h"
#include "InGameScene.h"


InGameScene::InGameScene(void) {
	
}

InGameScene::~InGameScene(void) {

}

void InGameScene::OnInitialUpdate(void) {
	GameScene::OnInitialUpdate();

	// 추후 GameManager나 BackgroundManager에서 생성하기
	AddGameObject(new BackGround(0, 0));

	for (auto ground : Ground::CreateGround(100, 600, 10)) {
		AddGameObject(ground);
	}
	
}