﻿#include "SceneManager.h"
#include"../Helper/KazHelper.h"
#include"../Sound/SoundManager.h"
#include"../Sound/SoundManager.h"
#include"../Helper/ResourceFilePass.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Fps/FPSManager.h"


SceneManager::SceneManager() {
	scene.emplace_back(new TitleScene());
	scene.emplace_back(new GameScene());
	scene.emplace_back(new DebugScene());

	nowScene = 0;
	nextScene = 1;
	scene[nowScene]->Init();
	itisInArrayFlag = true;
}

SceneManager::~SceneManager() {
	for (int i = 0; i < scene.size(); i++) {
		if (scene[i] != nullptr) {
			delete scene[i];
			scene[i] = nullptr;
		}
	}

	SoundManager::Instance()->Finalize();
}

void SceneManager::Update() {
	//�V�[���J�ڂ̋N��
	if (nextScene != nowScene) {
		change.Start();
	}

	//��ʂ��B�ꂽ�u��
	if (change.AllHiden()) {
		scene[nowScene]->Finalize();
		//�ʏ�̃V�[���؂�ւ�
		if (nextScene != -2) {
			nowScene = nextScene;
		}
		//���V�[���̃��Z�b�g
		else if (nextScene == -2) {
			nextScene = nowScene;
		}

		if (KazHelper::IsitInAnArray(nowScene, scene.size())) {
			scene[nowScene]->Init();
			itisInArrayFlag = true;
		}
		else {
			itisInArrayFlag = false;
		}
	}

	if (itisInArrayFlag) {
		scene[nowScene]->Input();
		scene[nowScene]->Update();

		int sceneNum = scene[nowScene]->SceneChange();
		if (sceneNum != SCENE_NONE) {
			nextScene = sceneNum;
		}
	}

	change.Update();

	// fpsを制限(今回は60fps)
	FpsManager::RegulateFps(60);
}

void SceneManager::Draw() {
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	if (itisInArrayFlag) {
		scene[nowScene]->Draw();
	}
	change.Draw();
}
