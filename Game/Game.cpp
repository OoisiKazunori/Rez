#include "Game.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Easing/easing.h"
#include<cmath>
#include<iostream>
#include"../KazLibrary/Sound/SoundManager.h"


Game::Game()
{
	besidePoly = std::make_unique<BoxPolygonRender>();
	verticlaPoly = std::make_unique<BoxPolygonRender>();
	cameraPoly = std::make_unique<BoxPolygonRender>();

	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model->data.transform.pos = { 0.0f,0.0f,20.0f };
	model->data.transform.scale = { 5.0f,5.0f,5.0f };
	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");
	model->data.color = { 255,0,0,255 };

	//mainRenderTarget.data.handle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;

	std::vector<MultiRenderTargetData> data;
	data.push_back(MultiRenderTargetData());
	data.push_back(MultiRenderTargetData());
	data[0].graphSize = { WIN_X,WIN_Y };
	data[0].backGroundColor = BG_COLOR;
	data[1].graphSize = { WIN_X,WIN_Y };
	data[1].backGroundColor = { 0.0f,0.0f,0.0f };

	handles =
		RenderTargetStatus::Instance()->CreateMultiRenderTarget(data, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.handleData = handles[0];

	addHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	addRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	addRenderTarget.data.pipelineName = PIPELINE_NAME_BLOOM;
	addRenderTarget.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX;
	addRenderTarget.data.addHandle.paramType[1] = GRAPHICS_PRAMTYPE_TEX2;
	addRenderTarget.data.addHandle.paramType[2] = GRAPHICS_PRAMTYPE_TEX3;
	addRenderTarget.data.addHandle.paramType[3] = GRAPHICS_PRAMTYPE_TEX4;


	buler[0] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X, WIN_Y));
	buler[1] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X / 2, WIN_Y / 2));
	buler[2] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X / 3, WIN_Y / 3));
	buler[3] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X / 4, WIN_Y / 4));


	luminaceTex.data.pipelineName = PIPELINE_NAME_SPRITE_LUMI;
	luminaceTex.data.handleData = handles[0];
	luminaceTex.data.addHandle.handle[0] = handles[1];
	luminaceTex.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX2;
	luminaceTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };


	lineStartPoly[0].data.transform.pos = { 0.5f,0.15f,0.0f };
	lineStartPoly[1].data.transform.pos = { -0.5f,0.0f,-0.5f };
	lineStartPoly[2].data.transform.pos = { 0.5f,0.6f,0.5f };
	lineStartPoly[3].data.transform.pos = { 0.5f,1.0f,0.0f };
	lineStartPoly[4].data.transform.pos = { 0.5f,-1.0f,-1.0f };
	lineStartPoly[5].data.transform.pos = { -0.5f,-1.0f,0.0f };
	lineStartPoly[6].data.transform.pos = { 0.5f,0.3f,-0.3f };
	lineStartPoly[7].data.transform.pos = { 0.0f,-0.3f,-1.0f };

	CameraMgr::Instance()->CameraSetting(60.0f, 1200.0f);

	stages[0] = std::make_unique<FirstStage>();
	stages[1] = std::make_unique<GpuParticleStage>();
	stages[2] = std::make_unique<ThridStage>();


	blackTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Black.png");
	blackTex.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	blackTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	blackTex.data.transform.scale = { 2000.0f,2000.0f };
	blackTex.data.colorData.color.a = 0;


	pressAButtonTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "PressAButton.png");
	pressAButtonTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f + 100.0f };
	pressAButtonTex.data.transform.scale = { 0.4f,0.4f };

	gameOverTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::GameOverPath + "GameOver.png");
	gameOverTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	gameOverTex.data.transform.scale = { 1.2f,1.2f };


	potalTexHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);


	bgmSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Bgm.wav");
	lockSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Lock.wav", false);
	doneSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Done.wav");
	SoundManager::Instance()->StopSoundMem(bgmSoundHandle);
	//SoundManager::Instance()->PlaySoundMem(bgmSoundHandle, 1, true);

}

Game::~Game()
{
	//レンダーターゲットの破棄
	RenderTargetStatus::Instance()->DeleteRenderTarget(addHandle);
	RenderTargetStatus::Instance()->DeleteMultiRenderTarget(handles);
	RenderTargetStatus::Instance()->DeleteRenderTarget(potalTexHandle);
	SoundManager::Instance()->StopSoundMem(bgmSoundHandle);
}

void Game::Init(const array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &RESPONE_DATA)
{
	player.Init(KazMath::Transform3D().pos);
	cursor.Init();


	stageUI.Init();

	sceneNum = SCENE_NONE;

	const int NO_ENEMY_NUM = 0;
	BufferMemorySize enemySize;
	enemySize.startSize = NO_ENEMY_NUM;
	enemySize.endSize = ENEMY_NUM_MAX;

	BufferMemorySize eventSize;
	enemySize.startSize = ENEMY_NUM_MAX;
	enemySize.endSize = ENEMY_NUM_MAX + 50;

	responeData = RESPONE_DATA;

	//操作可能OBJを纏めて生成する処理----------------------------------------------------------------
	for (int enemyType = 0; enemyType < responeData.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < responeData[enemyType].size(); ++enemyCount)
		{
			if (responeData[enemyType][enemyCount].layerLevel != -1)
			{
				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					break;

				case ENEMY_TYPE_MOTHER:
					enemies[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					//子敵の生成(テスト用)
					for (int i = 0; i < 8; ++i)
					{
						int index = enemiesHandle[ENEMY_TYPE_KID];
						enemies[ENEMY_TYPE_KID][index] = std::make_unique<KidEnemy>();
						++enemiesHandle[ENEMY_TYPE_KID];
					}
					break;

				case ENEMY_TYPE_MISILE:
					enemies[enemyType][enemyCount] = std::make_unique<NormalMisileEnemy>();
					enemies[ENEMY_TYPE_MISILE_SPLINE][enemiesHandle[ENEMY_TYPE_MISILE_SPLINE]] = std::make_unique<SplineMisile>();
					++enemiesHandle[ENEMY_TYPE_MISILE_SPLINE];
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
	gameStartFlag = false;
	gameFlame = 0;
	//ゴールに触れ無かった場合に次のステージに移動する際の最大フレーム数
	for (int i = 0; i < changeLayerLevelMaxTime.size(); ++i)
	{
		changeLayerLevelMaxTime[i] = 1800;
	}
	changeLayerLevelMaxTime[0] = 60 * 28;
	changeLayerLevelMaxTime[1] = 60 * 39;
	gameStageLevel = 0;
	stageNum = gameStageLevel;
	//ゲームループの初期化----------------------------------------------------------------


	eyePos = { 0.0f,5.0f,-10.0f };
	trackingTargetPos = { 0.0f,0.0f,0.0f };
	nowTargerPos = { 0.0f,0.0f,0.0f };
	leftRightAngleVel = { -90.0f,-90.0f };
	upDownAngleVel = { 0.0f,0.0f };

	trackLeftRightAngleVel = leftRightAngleVel;
	trackUpDownAngleVel = upDownAngleVel;

	baseEyePos = { 0.0f,5.0f,-10.0f };
	baseTargetPos = { 0.0f,3.0f,0.0f };

	centralPos = { 0.0f,3.0f,0.0f };
	centralPos2 = centralPos;


	besidePoly->data.color = { 255,255,255,255 };
	verticlaPoly->data.color = { 255,255,0,255 };
	cameraPoly->data.color = { 255,0,0,255 };
	r = 8.0f;
	r2 = 8.0f;


	testEnemyPos = { 0.0f,0.0f,100.0f };
	mulValue = { 10.0f,30.0f };
	mulValue2 = { 60.0f,60.0f };
	cameraChangeFlag = true;

	forceCameraDirVel.x = -90.0f;

	appearGoalBoxPos = { -10.0f,5.0f,40.0f };
	responeGoalBoxPos = { -10.0f,-100.0f,40.0f };
	goalBox.Init(responeGoalBoxPos);
	initAppearFlag = false;

	movieEffect.Init();


	gameOverFlag = false;
	readyToBlackOutFlag = false;
	readyToBlackOutToGoTitleFlag = false;
	flashFlag = false;
	flashTimer = 0;

	layerLevelEyePos = { -4.15f,3.0f,40.0f };
	layerLevelTargetPos = { -4.15f,3.0f,45.0f };


	titleLogoTex.data.colorData.color.a = 255;
	titleLogoTex.data.transform.pos = { 0.0f,50.0f,500.0f };
	titleLogoTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "TitleName.png");
	doneSprite.Init({ 0.0f,0.0f,500.0f }, TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "Start.png"));


	tutorialWindow.Init();

	startFlag = false;

	portal.Init(KazMath::Vec3<float>(0.0f, 3.0f, 50.0f));
}

void Game::Finalize()
{
}

void Game::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	ControllerInputManager *inputController = ControllerInputManager::Instance();

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

#pragma endregion


	bool upFlag = false;
	bool downFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;
	bool doneFlag = false;
	bool releaseFlag = false;

	const int DEAD_ZONE = 3000;
	if (inputController->InputState(XINPUT_GAMEPAD_A))
	{
		doneFlag = true;
	}
	if (inputController->InputRelease(XINPUT_GAMEPAD_A))
	{
		releaseFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, UP_SIDE, DEAD_ZONE))
	{
		upFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, DOWN_SIDE, DEAD_ZONE))
	{
		downFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, LEFT_SIDE, DEAD_ZONE))
	{
		leftFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, RIGHT_SIDE, DEAD_ZONE))
	{
		rightFlag = true;
	}

	//ゲーム終了処理
	//三ステージ目で6秒後にタイトル画面に戻る許可を出す
	if (inputController->InputTrigger(XINPUT_GAMEPAD_A) && 2 <= gameStageLevel)
	{
		sceneNum = 0;
	}

	//ゲームオーバー画面でタイトル画面に戻る処理
	if (inputController->InputTrigger(XINPUT_GAMEPAD_A) && gameOverFlag)
	{
		readyToBlackOutToGoTitleFlag = true;
	}


	if (input->InputTrigger(DIK_O))
	{
		sceneNum = 0;
	}


	KazMath::Vec2<float> joyStick;
	joyStick.x = inputController->GetJoyStickLXNum(0) / 32767.0f;
	joyStick.y = inputController->GetJoyStickLYNum(0) / 32767.0f;

	cursor.Input
	(
		upFlag,
		downFlag,
		leftFlag,
		rightFlag,
		doneFlag,
		releaseFlag,
		joyStick
	);


	//カメラの前後左右強制に向かせる処理
	if (input->InputState(DIK_Z))
	{
		forceCameraDirVel.x = 0.0f;
	}
	if (input->InputState(DIK_X))
	{
		forceCameraDirVel.x = -90.0f;
	}
	if (input->InputState(DIK_C))
	{
		forceCameraDirVel.x = -180.0f;
	}
	if (input->InputState(DIK_V))
	{
		forceCameraDirVel.x = -270.0f;
	}


	KazMath::Vec3<float> vel = {};
	KazMath::Vec3<float> verticalVel = {};
	KazMath::Vec3<float> besideVel = {};

	float speed = 1.0f;
	if (upFlag)
	{
		vel.y = -speed;
		verticalVel = { -speed,-speed,0.0f };
	}
	if (downFlag)
	{
		vel.y = speed;
		verticalVel = { speed,speed,0.0f };
	}
	if (leftFlag)
	{
		vel.x = -speed;
		besideVel = { speed,speed,0.0f };
	}
	if (rightFlag)
	{
		vel.x = speed;
		besideVel = { -speed,-speed,0.0f };
	}


	KazMath::Vec2<float> cursorValue = cursor.GetValue();
	nowTargerPos += vel;

	upDownAngleVel =
	{
		mulValue.x * -cursorValue.y,
		mulValue.y * -cursorValue.y
	};
	leftRightAngleVel =
	{
		forceCameraDirVel.x + mulValue2.x * -cursorValue.x,
		forceCameraDirVel.x + mulValue2.y * -cursorValue.x
	};

	//if (KeyBoradInputManager::Instance()->InputTrigger(DIK_R))
	//{
	//	gameFlame = 0;
	//	for (int i = 0; i < addEnemiesHandle.size(); ++i)
	//	{
	//		addEnemiesHandle[i] = 0;;
	//	}
	//}
}

void Game::Update()
{
	for (int i = 0; i < lineStartPoly.size(); ++i)
	{
		lineStartPoly[i].data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
		lineStartPoly[i].data.transform.scale = { 0.1f,0.1f,0.1f };
		lineStartPoly[i].data.color = { 255,0,0,255 };
		lineEffectArrayData[i].startPos = lineStartPoly[i].data.transform.pos + player.pos;
	}
	{
		int lineArrayNum = static_cast<int>(lineStartPoly.size());
		for (int i = lineArrayNum; i < lineArrayNum * 2; ++i)
		{
			lineEffectArrayData[i].startPos = lineStartPoly[i - lineArrayNum].data.transform.pos + player.pos;
		}
	}


#pragma region カメラ挙動

	//左右の角度変更のイージング
	{
		KazMath::Vec2<float> distance = leftRightAngleVel - trackLeftRightAngleVel;
		trackLeftRightAngleVel += distance * 0.1f;
	}
	//上下の角度変更のイージング
	{
		KazMath::Vec2<float> distance = upDownAngleVel - trackUpDownAngleVel;
		trackUpDownAngleVel += distance * 0.1f;
	}
	//左右の回転
	besidePoly->data.transform.pos =
	{
		cosf(KazMath::AngleToRadian(trackLeftRightAngleVel.x)) * r,
		0.0f,
		sinf(KazMath::AngleToRadian(trackLeftRightAngleVel.y)) * r
	};
	//上下の回転
	verticlaPoly->data.transform.pos =
	{
		0.0f,
		trackUpDownAngleVel.x,
		0.0f,
	};


	//注視点の自動
	//baseTargetPos.z = 0.0f + 1.0f * -cursor.GetValue().y;
	//if (baseTargetPos.z <= 0.0f)
	//{
	//	baseTargetPos.z = 0.0f;
	//}

	//centralPos.z = 0.0f + 3.0f * -cursor.GetValue().y;




	{
		float mul = 1.0f;
		float limitValue = 3.0f;
		//上下でプレイヤーのいる位置を変える
		if (0.1f <= cursor.GetValue().y)
		{
			mul *= -1.0f;
			limitValue = 2.5f;
		}

		//ポータル演出開始したらプレイヤーの動きを固定する
		DirectX::XMFLOAT3 rate = { cursor.GetValue().x ,cursor.GetValue().y,0.0f };
		honraiPlayerCameraPos.x = 0.0f + (2.0f * -rate.x);
		honraiPlayerCameraPos.z = 0.0f + (limitValue * rate.y) * mul;

		if (0.0f <= honraiPlayerCameraPos.z)
		{
			honraiPlayerCameraPos.z = 0.0f;
		}
		KazMath::Vec3<float> distance = honraiPlayerCameraPos - player.pos;
		player.pos += distance * 0.1f;
	}


	//上下左右の回転
	cameraPoly->data.transform.pos = centralPos + (besidePoly->data.transform.pos + verticlaPoly->data.transform.pos);



	if (cameraChangeFlag)
	{
		eyePos = cameraPoly->data.transform.pos;
		targetPos = baseTargetPos;
	}
	else
	{
		//デバック用
		eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
		targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	}

	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);

#pragma endregion



	//敵が一通り生成終わった際に登場させる----------------------------------------------------------------
	//if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame && !initAppearFlag)
	if (100 <= gameFlame && !initAppearFlag)
	{
		goalBox.Appear(appearGoalBoxPos);
		initAppearFlag = true;
	}
	//敵が一通り生成終わった際に登場させる----------------------------------------------------------------

	changeStageFlag = false;


	portal.CheckCameraPos(eyePos.z);
	portal.portalTexHandle = potalTexHandle;
	portal.Update();

	//全部隠れたら次ステージの描画をする
	if (portal.AllHidden())
	{
		//ゲームループの初期化----------------------------------------------
		++gameStageLevel;
		gameFlame = 0;
		//ゲームループの初期化----------------------------------------------

		//ステージの切り替え----------------------------------------------
		++stageNum;
		changeStageFlag = true;
		//ステージの切り替え----------------------------------------------

		//ゴールボックスの初期化----------------------------------------------
		initAppearFlag = false;
		goalBox.Init(responeGoalBoxPos);
		//ゴールボックスの初期化----------------------------------------------


		//leftRightAngleVel = { -90.0f,-90.0f };
		//trackLeftRightAngleVel = leftRightAngleVel;
		//upDownAngleVel = { 0.0f,0.0f };
		//trackUpDownAngleVel = upDownAngleVel;
		//movieEffect.startFlag = false;

		stageUI.Init();
		stageUI.AnnounceStage(stageNum + 1);
		//cursor.Appear();
	}


	if (goalBox.startPortalEffectFlag)
	{
		portal.Start();
	}

	if (portal.Reset())
	{
		portal.Init(KazMath::Vec3<float>(0.0f, 3.0f, 50.0f));
		goalBox.startPortalEffectFlag = false;
	}


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
					//現在のレイヤーレベルに合わせる
					responeData[enemyTypeData][nowHandle].layerLevel = gameStageLevel;
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
			bool enableToUseThisDataFlag = responeData[enemyType][enemyCount].layerLevel != -1;
			bool readyToInitDataFlag = responeData[enemyType][enemyCount].flame == gameFlame &&
				responeData[enemyType][enemyCount].layerLevel == gameStageLevel;

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

				case ENEMY_TYPE_MISILE:
					enemies[enemyType][enemyCount]->Init(responeData[enemyType][enemyCount].initPos);
					break;

				case ENEMY_TYPE_MISILE_SPLINE:
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


#pragma region 敵ロックオン
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
				bool enableToLockOnEnemyFlag = enemies[enemyType][enemyCount]->IsAlive() && !enemies[enemyType][enemyCount]->LockedOrNot();
				bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, enemyData->hitBox);
				if (hitFlag &&
					enableToLockOnNumFlag &&
					enableToLockOnEnemyFlag &&
					!cursor.releaseFlag)
				{
					SoundManager::Instance()->PlaySoundMem(lockSoundHandle, 1);


					//カーソルのカウント数を増やす
					cursor.Hit(enemyData->hitBox.center);
					//敵が当たった情報を書く
					enemies[enemyType][enemyCount]->Hit();

					//線演出をかける際にどの配列を使用するか決める
					for (int i = 0; i < lineEffectArrayData.size(); ++i)
					{
						if (!lineEffectArrayData[i].usedFlag)
						{
							lineLevel[i].Attack2(lineEffectArrayData[i].startPos, *enemyData->hitBox.center);
							lineEffectArrayData[i].usedFlag = true;
							lineEffectArrayData[i].lineIndex = i;
							lineEffectArrayData[i].enemyTypeIndex = enemyType;
							lineEffectArrayData[i].enemyIndex = enemyCount;
							break;
						}
					}
				}
			}
		}
	}


#pragma region イベントロックオン
	{
		//ロックオン判定
		bool enableToLockOnNumFlag = cursor.LockOn();
		bool enableToLockOnEnemyFlag = goalBox.IsAlive() && !goalBox.LockedOrNot();
		bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, goalBox.hitBox);
		if (hitFlag &&
			enableToLockOnNumFlag &&
			enableToLockOnEnemyFlag)
		{
			SoundManager::Instance()->PlaySoundMem(lockSoundHandle, 1);

			//カーソルのカウント数を増やす
			cursor.Hit(goalBox.hitBox.center);
			goalBox.Hit();

			//線演出をかける際にどの配列を使用するか決める
			for (int i = 0; i < lineEffectArrayData.size(); ++i)
			{
				if (!lineEffectArrayData[i].usedFlag)
				{
					lineLevel[i].Attack2(lineEffectArrayData[i].startPos, *goalBox.hitBox.center);
					lineEffectArrayData[i].usedFlag = true;
					lineEffectArrayData[i].lineIndex = i;
					lineEffectArrayData[i].eventType = 0;
					break;
				}
			}
		}
	}

	{
		//ロックオン判定
		bool enableToLockOnNumFlag = cursor.LockOn();
		bool enableToLockOnEnemyFlag = doneSprite.IsAlive() && !doneSprite.LockedOrNot();
		bool hitFlag = CollisionManager::Instance()->CheckRayAndSphere(cursor.hitBox, doneSprite.hitBox);
		if (hitFlag &&
			enableToLockOnEnemyFlag &&
			enableToLockOnNumFlag && !startFlag)
		{
			doneSprite.Hit();
			SoundManager::Instance()->PlaySoundMem(doneSoundHandle, 1);
			startFlag = true;
		}
		else if (hitFlag)
		{
			doneSprite.selectingFlag = true;
		}
		else
		{
			doneSprite.selectingFlag = false;
		}
	}
#pragma endregion


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

				for (int i = 0; i < lineEffectArrayData.size(); ++i)
				{
					bool sameEnemyFlag = lineEffectArrayData[i].enemyTypeIndex == enemyType && lineEffectArrayData[i].enemyIndex == enemyCount;
					if (lineEffectArrayData[i].usedFlag && sameEnemyFlag)
					{
						enemyData->oprationObjData->lockOnFlag = true;
						break;
					}
					else
					{
						enemyData->oprationObjData->lockOnFlag = false;
					}
				}
			}
		}
	}

	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		bool sameEventFlag = lineEffectArrayData[i].eventType == 0 && lineEffectArrayData[i].lineIndex == i;
		if (lineEffectArrayData[i].usedFlag && sameEventFlag)
		{
			goalBox.iOperationData.lockOnFlag = true;
			break;
		}
		else
		{
			goalBox.iOperationData.lockOnFlag = false;
		}
	}





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

				for (int i = 0; i < lineEffectArrayData.size(); ++i)
				{
					bool sameEnemyFlag = lineEffectArrayData[i].enemyTypeIndex == enemyType && lineEffectArrayData[i].enemyIndex == enemyCount;
					bool releaseFlag = enemyData->outOfStageFlag || enemyData->timer <= 0;

					if (lineEffectArrayData[i].usedFlag && sameEnemyFlag && releaseFlag)
					{
						int lineIndex = lineEffectArrayData[i].lineIndex;
						lineLevel[lineIndex].Release();
						cursor.SubCount(1);
						break;
					}
				}
			}
		}
	}



#pragma region 線のロックオン解放

	//線がたどり着いたら敵を死亡させる
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
			int enemyIndex = lineEffectArrayData[i].enemyIndex;
			int eventIndex = lineEffectArrayData[i].eventType;

			if (eventIndex == -1)
			{
				//演出を合わせてダメージと死亡をやる
				if (lineLevel[lineIndex].lineReachObjFlag && enemies[enemyTypeIndex][enemyIndex]->IsAlive())
				{
					//lineEffectArrayData[i].Reset();
					//lineLevel[lineIndex].lineReachObjFlag = false;
				}
				else if (lineLevel[lineIndex].lineReachObjFlag && !enemies[enemyTypeIndex][enemyIndex]->IsAlive() && !lineEffectArrayData[i].hitFlag)
				{
					enemies[enemyTypeIndex][enemyIndex]->Dead();
					lineEffectArrayData[i].hitFlag = true;
					//lineEffectArrayData[i].Reset();
					//lineLevel[lineIndex].lineReachObjFlag = false;
					for (int hitEffectIndex = 0; hitEffectIndex < hitEffect.size(); ++hitEffectIndex)
					{
						if (!hitEffect[hitEffectIndex].IsAlive())
						{
							hitEffect[hitEffectIndex].Start(*enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center);
							break;
						}
					}
				}
			}
			else
			{
				//演出を合わせて死亡
				if (lineLevel[lineIndex].lineReachObjFlag && goalBox.IsAlive())
				{
					//lineEffectArrayData[i].Reset();
				}
				else if (lineLevel[lineIndex].lineReachObjFlag && !goalBox.IsAlive())
				{
					//lineEffectArrayData[i].Reset();
				}
			}
		}
	}

	//線演出が終わった際にリンクのデータをリセットする
	for (int i = 0; i < lineEffectArrayData.size(); ++i)
	{
		if (lineEffectArrayData[i].usedFlag)
		{
			int lineIndex = lineEffectArrayData[i].lineIndex;
			if (lineLevel[lineIndex].allFinishFlag)
			{
				lineEffectArrayData[i].Reset();
			}
		}
	}

#pragma endregion


#pragma endregion


	//攻撃----------------------------------------------
	for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->enableToHitFlag;
			if (enableToUseDataFlag)
			{
				//時間が0秒以下ならプレイヤーに当たったと判断する
				if (enemies[enemyType][enemyCount]->GetData()->timer <= 0)
				{
					player.Hit();
					//enemies[enemyType][enemyCount]->Dead();
				}
			}
		}
	}
	//攻撃----------------------------------------------


	//ゲームオーバー----------------------------------------------
	if (!player.isAlive() && !gameOverFlag)
	{
		readyToBlackOutFlag = true;
	}

	if (readyToBlackOutFlag || readyToBlackOutToGoTitleFlag)
	{
		//暗転
		if (blackTex.data.colorData.color.a <= 255.0f)
		{
			blackTex.data.colorData.color.a += 5;
		}
		//タイトル画面に戻る
		else if (readyToBlackOutToGoTitleFlag && 255 <= blackTex.data.colorData.color.a)
		{
			sceneNum = 0;
		}
		//ゲームオーバー画面を表示
		else
		{
			gameOverFlag = true;
			readyToBlackOutFlag = false;
		}
	}
	else
	{
		//明転
		if (0 <= blackTex.data.colorData.color.a)
		{
			blackTex.data.colorData.color.a -= 5;
		}
	}
	if (gameOverFlag)
	{
		++flashTimer;

		if (60 <= flashTimer)
		{
			flashFlag = !flashFlag;
			flashTimer = 0;
		}
	}
	//ゲームオーバー----------------------------------------------


	if (!gameOverFlag)
	{
#pragma region 更新処理

		goalBox.releaseFlag = cursor.releaseFlag;

		//更新処理----------------------------------------------------------------
		player.Update();
		cursor.Update();
		goalBox.Update();
		movieEffect.Update();
		stageUI.Update();
		stages[stageNum]->Update();
		doneSprite.Update();
		tutorialWindow.Update();

		for (int i = 0; i < hitEffect.size(); ++i)
		{
			hitEffect[i].Update();
		}


		//配列外を超えない処理
		if (stageNum + 1 < stages.size())
		{
			stages[stageNum + 1]->Update();
		}
		//ロックオンのリリース処理
		if (cursor.releaseFlag)
		{
			for (int i = 0; i < lineEffectArrayData.size(); ++i)
			{
				if (lineEffectArrayData[i].usedFlag)
				{
					int lineIndex = lineEffectArrayData[i].lineIndex;
					lineLevel[lineIndex].ReleaseShot();
				}
			}
		}

		//ロックオン中に必要なデータ
		for (int i = 0; i < lineEffectArrayData.size(); ++i)
		{
			if (lineEffectArrayData[i].usedFlag)
			{
				int lineIndex = lineEffectArrayData[i].lineIndex;
				//イベントブロックかどうか判断する
				if (lineEffectArrayData[i].eventType != -1)
				{
					KazMath::Vec3<float> pos = *goalBox.hitBox.center;
					lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
				}
				else
				{
					int enemyTypeIndex = lineEffectArrayData[i].enemyTypeIndex;
					int enemyIndex = lineEffectArrayData[i].enemyIndex;
					KazMath::Vec3<float> pos = *enemies[enemyTypeIndex][enemyIndex]->GetData()->hitBox.center;
					lineLevel[lineIndex].CalucurateDistance(player.pos, pos);
				}
			}
		}


		for (int i = 0; i < lineLevel.size(); ++i)
		{
			if (lineEffectArrayData[i].usedFlag)
			{
				lineLevel[i].playerPos = lineEffectArrayData[i].startPos;
			}

			if (!lineLevel[i].initFlag)
			{
				lineEffectArrayData[i].Reset();
			}

			lineLevel[i].Update();
		}

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
	}



	if (1.0f <= titleT)
	{
		titleTReversFlag = false;
	}
	else if (titleT <= 0.0f)
	{
		titleTReversFlag = true;
	}

	float rate = 1.0f / 30.0f;
	if (titleTReversFlag)
	{
		titleT += rate;
	}
	else
	{
		titleT += -rate;
	}

	titleLogoTex.data.transform.pos = { 0.0f,100.0f,500.0f };
	titleLogoTex.data.transform.scale = { 0.8f,0.8f,1.0f };
	baseTitlePosY = 100.0f;
	titleLogoTex.data.transform.pos.y = baseTitlePosY + EasingMaker(Out, Cubic, titleT) * 50.0f;



	//ゲームループの経過時間----------------------------------------------------------------
	titleLogoTex.data.colorData = doneSprite.spriteRender.data.colorData;
	if (titleLogoTex.data.colorData.color.a <= 0 && !stages[0]->startFlag)
	{
		//ゲーム開始
		gameStartFlag = true;
		//ステージ番号知らせる
		stageUI.Init();
		stageUI.AnnounceStage(stageNum + 1);
		//ゲーム開始
		stages[0]->startFlag = true;
	}

	if (gameStartFlag)
	{
		++gameFlame;
	}

	if (60 * 4 <= gameFlame)
	{
		//チュートリアル用のUI非表示
		tutorialWindow.End();
	}

	//ゲームループの経過時間----------------------------------------------------------------


}

void Game::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);

	//ポータル演出
	//if (goalBox.startPortalEffectFlag)
	{
		int lStageNum = -1;
		if (portal.DrawPrevStageFlag())
		{
			lStageNum = stageNum - 1;
			if (lStageNum < 0)
			{
				lStageNum = 0;
			}
		}
		else
		{
			lStageNum = stageNum + 1;
		}
		RenderTargetStatus::Instance()->PrepareToChangeBarrier(potalTexHandle);
		RenderTargetStatus::Instance()->ClearRenderTarget(potalTexHandle);

		CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 1);
		player.Draw();
		stages[lStageNum]->SetCamera(1);
		stages[lStageNum]->Draw();
		RenderTargetStatus::Instance()->PrepareToCloseBarrier(potalTexHandle);
		RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	}


	if (!gameOverFlag)
	{
		RenderTargetStatus::Instance()->PrepareToChangeBarrier(handles[0]);
		RenderTargetStatus::Instance()->ClearRenderTarget(handles[0]);

		if (lineDebugFlag)
		{
			bg.Draw();
		}
		player.Draw();

		stages[stageNum]->SetCamera(0);
		stages[stageNum]->Draw();
		portal.DrawPortal();
		portal.DrawFlame();

		//if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame)
		if (100 <= gameFlame)
		{
			goalBox.Draw();
		}

		//敵の描画処理----------------------------------------------------------------
		for (int enemyType = 0; enemyType < enemies.size(); ++enemyType)
		{
			for (int enemyCount = 0; enemyCount < enemies[enemyType].size(); ++enemyCount)
			{
				//生成されている敵のみ描画処理を通す
				bool enableToUseDataFlag = enemies[enemyType][enemyCount] != nullptr && enemies[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
				if (enableToUseDataFlag)
				{
					//enemies[enemyType][enemyCount]->Draw();
				}
			}
		}

		for (int i = 0; i < hitEffect.size(); ++i)
		{
			hitEffect[i].Draw();
		}

		for (int i = 0; i < lineLevel.size(); ++i)
		{
			lineLevel[i].Draw();
		}

		if (changeLayerLevelMaxTime[gameStageLevel] <= gameFlame)
		{
			goalBox.lightEffect.Draw();
		}
		doneSprite.Draw();
		titleLogoTex.Draw();
		tutorialWindow.Draw();
		stageUI.Draw();

		RenderTargetStatus::Instance()->PrepareToChangeBarrier(addHandle, handles[0]);
		RenderTargetStatus::Instance()->ClearRenderTarget(addHandle);
		//輝度抽出
		luminaceTex.Draw();
		RenderTargetStatus::Instance()->PrepareToCloseBarrier(addHandle);
		RenderTargetStatus::Instance()->SetDoubleBufferFlame();
		//ゲーム画面描画


		mainRenderTarget.Draw();

		addRenderTarget.data.handleData = addHandle;
		addRenderTarget.data.addHandle.handle[0] = buler[0]->BlurImage(addHandle);
		addRenderTarget.data.addHandle.handle[1] = buler[1]->BlurImage(addRenderTarget.data.addHandle.handle[0]);
		addRenderTarget.data.addHandle.handle[2] = buler[2]->BlurImage(addRenderTarget.data.addHandle.handle[1]);
		addRenderTarget.data.addHandle.handle[3] = buler[3]->BlurImage(addRenderTarget.data.addHandle.handle[2]);

		PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "AddRenderTarget");
		addRenderTarget.Draw();
		PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

		cursor.Draw();
		movieEffect.Draw();

	}
	else
	{
		gameOverTex.Draw();
		if (flashFlag)
		{
			pressAButtonTex.Draw();
		}
	}

	blackTex.Draw();

}

int Game::SceneChange()
{
	if (sceneNum != -1) {
		int tmp = sceneNum;
		sceneNum = -1;
		return tmp;
	}

	return SCENE_NONE;
}
