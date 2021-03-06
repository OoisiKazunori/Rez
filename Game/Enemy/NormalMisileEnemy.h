#pragma once
#include"../Interface/IEnemy.h"

class NormalMisileEnemy:public IEnemy
{
public:
	NormalMisileEnemy();

	void Init(const KazMath::Vec3<float> &POS);
	void Finalize();
	void Update();
	void Draw();

private:
	int shotTimer;//ミサイル発射までの時間
	bool initShotFlag;

	int shotSoundHandle;
};
