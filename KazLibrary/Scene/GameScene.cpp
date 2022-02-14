﻿#include "GameScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Imgui/MyImgui.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();

	hitBox.Init({ 0.0f,0.0f,30.0f }, 1);

	sceneNum = SCENE_NONE;


	responeData[0][0].enemyType = 0;
	responeData[0][0].layerLevel = 0;
	responeData[0][0].flame = 360;
	responeData[0][0].initPos = { 0.0f,10.0f,20.0f };

	//敵を纏めて生成する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			if (responeData[enemyType][enemyCount].enemyType != -1)
			{
				switch (enemyType)
				{
				case 0:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					//子敵の生成(テスト用)
					for (int i = 0; i < 8; ++i)
					{
						int index = enemiesHandle[ENEMY_TYPE_KID];
						enemies[ENEMY_TYPE_KID][index] = std::make_unique<KidEnemy>();
						++enemiesHandle[ENEMY_TYPE_KID];
					}

					break;
				default:
					break;
				}
				++enemiesHandle[enemyType];
			}
		}
	}
	//敵を纏めて生成する処理----------------------------------------------------------------



	//ゲームループの初期化----------------------------------------------------------------
	gameFlame = 0;
	//ゲームループの初期化----------------------------------------------------------------

}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
	//�J�����ړ�
	if (input->InputState(DIK_D))
	{
		debugCameraMove.x = -debugSpeed;
	}
	if (input->InputState(DIK_A))
	{
		debugCameraMove.x = debugSpeed;
	}
	if (input->InputState(DIK_W))
	{
		//y�Ȃ̂ɑO�ɐi��....
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	//�J�����p�x
	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSpeed;
	}
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);

#pragma endregion


	if (input->InputTrigger(DIK_END))
	{
		bool debug = false;

		debug = true;
	}


	cursor.Input
	(
		input->InputState(DIK_T),
		input->InputState(DIK_G),
		input->InputState(DIK_F),
		input->InputState(DIK_H),
		input->InputState(DIK_RETURN),
		input->InputRelease(DIK_RETURN)
	);
}

void GameScene::Update()
{
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });





#pragma region 生成処理
	//敵を追加で初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();
				//追加で生成するデータを検知したら生成する
				if (enemyData->genarateData.enemyType != -1)
				{
					//生成する敵の種類
					int enemyTypeData = enemyData->genarateData.enemyType;
					//最後に生成して次のハンドル
					int nowHandle = addEnemiesHandle[enemyTypeData];
					responeData[enemyTypeData][nowHandle].enemyType = enemyData->genarateData.enemyType;
					//現在のレイヤーレベルに合わせる
					responeData[enemyTypeData][nowHandle].layerLevel = gameLayerLevel;
					//現在のフレーム数+インターバルフレーム*個数で設定する
					responeData[enemyTypeData][nowHandle].flame = gameFlame;
					responeData[enemyTypeData][nowHandle].initPos = enemyData->genarateData.initPos;
					//ハンドルを増やす
					++addEnemiesHandle[enemyTypeData];

					//追加したら終了処理を入れる
					enemies[enemyType][enemyCount]->GetData()->genarateData.Finalize();
				}
			}
		}
	}
	//敵を追加で初期化する処理----------------------------------------------------------------


	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			bool enableToUseThisDataFlag = responeData[enemyType][enemyCount].enemyType != -1;
			bool readyToInitDataFlag = responeData[enemyType][enemyCount].flame == gameFlame &&
				responeData[enemyType][enemyCount].layerLevel == gameLayerLevel;

			if (enableToUseThisDataFlag && readyToInitDataFlag)
			{
				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;

				case ENEMY_TYPE_KID:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;
				default:
					break;
				}
			}
		}
	}
	//敵をどのタイミングで初期化する処理----------------------------------------------------------------
#pragma endregion


#pragma region ロックオン
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				//敵のデータのポインタを代入
				EnemyData *enemyData = enemies[enemyType][enemyCount]->GetData().get();

				//ロックオン判定
				bool enableToLockOnNumFlag = cursor.LockOn();
				bool enableToLockOnEnemyFlag = enemies[enemyType][enemyCount]->AliveOrNot() && !enemies[enemyType][enemyCount]->LockedOrNot();
				if (CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemyData->hitBox) &&
					enableToLockOnNumFlag &&
					enableToLockOnEnemyFlag)
				{
					//カーソルのカウント数を増やす
					cursor.Count();
					//敵が当たった情報を書く
					enemies[enemyType][enemyCount]->Hit();
					//線演出をかける際にどの配列を使用するか決める
				}

				//敵が生きている&&ロックオンできる回数が0以下&&ロックオン入力がリリースされた時、敵は死亡する
				if (enemies[enemyType][enemyCount]->AliveOrNot() && enemies[enemyType][enemyCount]->LockedOrNot() && cursor.Release())
				{
					enemies[enemyType][enemyCount]->Dead();
				}
			}
		}
	}
#pragma endregion


#pragma region 更新処理
	//更新処理----------------------------------------------------------------
	player.Update();
	cursor.Update();
	hitBox.Update();

	//敵の更新処理
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				enemies[enemyType][enemyCount]->Update();
			}
		}
	}
	//更新処理----------------------------------------------------------------
#pragma endregion


	//ゲームループの経過時間----------------------------------------------------------------
	++gameFlame;
	//ゲームループの経過時間----------------------------------------------------------------

}

void GameScene::Draw()
{
	bg.Draw();
	player.Draw();
	cursor.Draw();
	//hitBox.Draw();

	//敵の描画処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている敵のみ描画処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				enemies[enemyType][enemyCount]->Draw();
			}
		}
	}
	//敵の描画処理----------------------------------------------------------------

}

int GameScene::SceneChange()
{
	if (sceneNum != -1) {
		int tmp = sceneNum;
		sceneNum = -1;
		return tmp;
	}

	return SCENE_NONE;
}