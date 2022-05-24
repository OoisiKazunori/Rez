#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/Sprite2DRender.h"

/// <summary>
/// それぞれのステージが切り替わった際に現在のステージ数を知らせるUI
/// </summary>
class AnnounceStageUI
{
public:
	AnnounceStageUI();

	void Init();
	void Update();
	void Draw();

	void AnnounceStage(int STAGE_NUM);
private:
	bool startFlag;				//アナウンスを開始します
	int stageNum;				//現在のステージ番号を表示します

	XMVECTOR basePos, larpScale;

	std::array<int, 10> numberHandle;
	Sprite2DRender flameTex;	//ウィンドウの描画
	Sprite2DRender numberTex;	//数字の描画
	Sprite2DRender areaTex;		//エリア文字の描画
};

