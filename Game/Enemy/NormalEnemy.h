#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

/// <summary>
/// �T���v���p�̓G
/// </summary>
class NormalEnemy:public IEnemy
{
public:
	NormalEnemy();

	void Init(const XMVECTOR &POS);
	void Finalize();
	void Update();
	void Draw();

private:
	int generateTimer;
};
