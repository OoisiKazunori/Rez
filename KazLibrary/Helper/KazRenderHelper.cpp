#include "KazRenderHelper.h"

KazRenderHelper::DrawIndexInstanceCommandData KazRenderHelper::SetDrawIndexInstanceCommandData(const D3D_PRIMITIVE_TOPOLOGY &TOPOLOGY, const D3D12_VERTEX_BUFFER_VIEW &VERTEX_VIEW, const D3D12_INDEX_BUFFER_VIEW &INDEX_VIEW, UINT INDECIES_NUM, UINT INSTANCE_NUM)
{
	KazRenderHelper::DrawIndexInstanceCommandData result;
	result.topology = TOPOLOGY;
	result.vertexBufferDrawData.vertexBufferView = VERTEX_VIEW;
	result.vertexBufferDrawData.numViews = INSTANCE_NUM;
	result.vertexBufferDrawData.slot = 0;
	result.indexBufferView = INDEX_VIEW;
	result.drawIndexInstancedData.indexCountPerInstance = INDECIES_NUM;
	result.drawIndexInstancedData.instanceCount = 1;
	result.drawIndexInstancedData.baseVertexLocation = 0;
	result.drawIndexInstancedData.startIndexLocation = 0;
	result.drawIndexInstancedData.startInstanceLocation = 0;
	return result;
}

KazRenderHelper::DrawInstanceCommandData KazRenderHelper::SetDrawInstanceCommandData(const D3D_PRIMITIVE_TOPOLOGY &TOPOLOGY, const D3D12_VERTEX_BUFFER_VIEW &VERTEX_VIEW, UINT VERTEX_NUM, UINT INSTANCE_NUM)
{
	KazRenderHelper::DrawInstanceCommandData result;
	result.topology = TOPOLOGY;
	result.vertexBufferDrawData.vertexBufferView = VERTEX_VIEW;
	result.vertexBufferDrawData.numViews = INSTANCE_NUM;
	result.vertexBufferDrawData.slot = 0;
	result.drawInstanceData.vertexCountPerInstance = VERTEX_NUM;
	result.drawInstanceData.instanceCount = 1;
	result.drawInstanceData.startVertexLocation = 0;
	result.drawInstanceData.startInstanceLocation = 0;
	return result;
}

array<KazMath::Vec2<float>, 4> KazRenderHelper::ChangePlaneScale(const KazMath::Vec2<float> &LEFTUP_POS, const KazMath::Vec2<float> &RIGHTDOWN_POS, const KazMath::Vec2<float> &SCALE, const KazMath::Vec2<float> &ANCHOR_POINT, const KazMath::Vec2<int> &TEX_SIZE)
{
	//?ǂݍ???
	float width = static_cast<float>(TEX_SIZE.x);
	float height = static_cast<float>(TEX_SIZE.y);


	float left = (LEFTUP_POS.x - ANCHOR_POINT.x) * width * SCALE.x;
	float right = (RIGHTDOWN_POS.x - ANCHOR_POINT.x) * width * SCALE.x;
	float up = (LEFTUP_POS.y - ANCHOR_POINT.y) * height * SCALE.y;
	float down = (RIGHTDOWN_POS.y - ANCHOR_POINT.y) * height * SCALE.y;


	array<KazMath::Vec2<float>, 4>pos;
	pos[0] = { left, down };
	pos[1] = { left, up };
	pos[2] = { right, down };
	pos[3] = { right, up };
	return pos;
}

array<KazMath::Vec2<float>, 4> KazRenderHelper::ChangeModiPlaneScale(const KazMath::Vec2<float> &LEFTUP_POS, const KazMath::Vec2<float> &RIGHTDOWN_POS, const DirectX::XMFLOAT4 &SCALE, const DirectX::XMFLOAT2 &ANCHOR_POINT, const DirectX::XMFLOAT2 &TEX_SIZE)
{
	//?ǂݍ???
	float width = TEX_SIZE.x;
	float height = TEX_SIZE.y;


	float left = (LEFTUP_POS.x - ANCHOR_POINT.x) * width * SCALE.x;
	float right = (RIGHTDOWN_POS.x - ANCHOR_POINT.x) * width * SCALE.y;
	float up = (LEFTUP_POS.y - ANCHOR_POINT.y) * height * SCALE.z;
	float down = (RIGHTDOWN_POS.y - ANCHOR_POINT.y) * height * SCALE.w;


	array<KazMath::Vec2<float>, 4>pos;
	pos[0] = { left, down };
	pos[1] = { left, up };
	pos[2] = { right, down };
	pos[3] = { right, up };
	return pos;
}

void KazRenderHelper::FlipXUv(DirectX::XMFLOAT2 *UV_LEFTUP_POS, DirectX::XMFLOAT2 *UV_LEFTDOWN_POS, DirectX::XMFLOAT2 *UV_RIGHTUP_POS, DirectX::XMFLOAT2 *UV_RIGHTDOWN_POS)
{
	DirectX::XMFLOAT2 leftUp = *UV_LEFTUP_POS;
	DirectX::XMFLOAT2 rightUp = *UV_RIGHTUP_POS;
	DirectX::XMFLOAT2 leftDown = *UV_LEFTDOWN_POS;
	DirectX::XMFLOAT2 rightDown = *UV_RIGHTDOWN_POS;

	UV_LEFTUP_POS->x = rightUp.x;
	UV_RIGHTUP_POS->x = leftUp.x;
	UV_LEFTDOWN_POS->x = rightDown.x;
	UV_RIGHTDOWN_POS->x = leftDown.x;

}

void KazRenderHelper::FlipYUv(DirectX::XMFLOAT2 *UV_LEFTUP_POS, DirectX::XMFLOAT2 *UV_LEFTDOWN_POS, DirectX::XMFLOAT2 *UV_RIGHTUP_POS, DirectX::XMFLOAT2 *UV_RIGHTDOWN_POS)
{
	DirectX::XMFLOAT2 leftUp = *UV_LEFTUP_POS;
	DirectX::XMFLOAT2 rightUp = *UV_RIGHTUP_POS;
	DirectX::XMFLOAT2 leftDown = *UV_LEFTDOWN_POS;
	DirectX::XMFLOAT2 rightDown = *UV_RIGHTDOWN_POS;

	UV_LEFTUP_POS->y = leftDown.y;
	UV_RIGHTUP_POS->y = rightDown.y;
	UV_LEFTDOWN_POS->y = leftUp.y;
	UV_RIGHTDOWN_POS->y = rightUp.y;
}

void KazRenderHelper::VerticesCut(const KazMath::Vec2<int> &DIV_SIZE, const KazMath::Vec2<int> &DIV_LEFTUP_POS, DirectX::XMFLOAT3 *LEFTUP_POS, DirectX::XMFLOAT3 *LEFTDOWN_POS, DirectX::XMFLOAT3 *RIGHTUP_POS, DirectX::XMFLOAT3 *RIGHTDOWN_POS, const KazMath::Vec2<float> &SCALE, const KazMath::Vec2<float> &ANCHOR_POINT)
{
	KazMath::Vec2<int> texSize = DIV_SIZE;
	KazMath::Vec2<int> divGraphPos = DIV_LEFTUP_POS;


	KazMath::Vec2<float> leftUp, rightDown;
	leftUp = { 0.0f,0.0f };
	rightDown = { 1.0f,1.0f };
	array<KazMath::Vec2<float>, 4>tmp = ChangePlaneScale(leftUp, rightDown, SCALE, ANCHOR_POINT, DIV_SIZE);






	*LEFTUP_POS = { tmp[2].x,tmp[2].y,0.0f };
	*LEFTDOWN_POS = { tmp[3].x,tmp[3].y,0.0f };
	*RIGHTUP_POS = { tmp[0].x,tmp[0].y,0.0f };
	*RIGHTDOWN_POS = { tmp[1].x,tmp[1].y,0.0f };
}

void KazRenderHelper::UVCut(const KazMath::Vec2<int> &UV_DIV_LEFTUP_POS, const KazMath::Vec2<int> &DIV_SIZE, const KazMath::Vec2<int> &TEX_SIZE, DirectX::XMFLOAT2 *LEFTUP_POS, DirectX::XMFLOAT2 *LEFTDOWN_POS, DirectX::XMFLOAT2 *RIGHTUP_POS, DirectX::XMFLOAT2 *RIGHTDOWN_POS)
{
	int texX = UV_DIV_LEFTUP_POS.x;
	int texY = UV_DIV_LEFTUP_POS.y;
	int texHeight = DIV_SIZE.y;
	int texWidth = DIV_SIZE.x;

	float tex_left = static_cast<float>(texX) / static_cast<float>(TEX_SIZE.x);
	float tex_right =		static_cast<float>(texX + texWidth) / static_cast<float>(TEX_SIZE.x);
	float tex_top =		static_cast<float>(texY) / static_cast<float>(TEX_SIZE.y);
	float tex_bottom = static_cast<float>(texY + texHeight) / static_cast<float>(TEX_SIZE.y);

	*LEFTUP_POS = { tex_right,tex_top };
	*LEFTDOWN_POS = { tex_right,tex_bottom };
	*RIGHTUP_POS = { tex_left,tex_top };
	*RIGHTDOWN_POS = { tex_left,tex_bottom };
}

void KazRenderHelper::InitUvPos(DirectX::XMFLOAT2 *UV_LEFTUP_POS, DirectX::XMFLOAT2 *UV_LEFTDOWN_POS, DirectX::XMFLOAT2 *UV_RIGHTUP_POS, DirectX::XMFLOAT2 *UV_RIGHTDOWN_POS)
{
	*UV_LEFTUP_POS = { 0.0f,0.0f };
	*UV_LEFTDOWN_POS = { 0.0f,1.0f };
	*UV_RIGHTUP_POS = { 1.0f,0.0f };
	*UV_RIGHTDOWN_POS = { 1.0f,1.0f };
}

void KazRenderHelper::InitVerticesPos(DirectX::XMFLOAT3 *LEFTUP_POS, DirectX::XMFLOAT3 *LEFTDOWN_POS, DirectX::XMFLOAT3 *RIGHTUP_POS, DirectX::XMFLOAT3 *RIGHTDOWN_POS, const DirectX::XMFLOAT2 &ANCHOR_POINT)
{
	*LEFTUP_POS = { (0.0f - ANCHOR_POINT.x), (0.0f - ANCHOR_POINT.y),0.0f };
	*LEFTDOWN_POS = { (0.0f - ANCHOR_POINT.x), (1.0f - ANCHOR_POINT.y),0.0f };
	*RIGHTUP_POS = { (1.0f - ANCHOR_POINT.x), (0.0f - ANCHOR_POINT.y) ,0.0f };
	*RIGHTDOWN_POS = { (1.0f - ANCHOR_POINT.x), (1.0f - ANCHOR_POINT.y) ,0.0f };
}

array<unsigned short, 6> KazRenderHelper::InitIndciesForPlanePolygon()
{
	array<unsigned short, 6>result;
	result[0] = 0;
	result[1] = 1;
	result[2] = 2;
	result[3] = 2;
	result[4] = 1;
	result[5] = 3;
	return result;
}

int KazRenderHelper::SetBufferOnCmdList(const GraphicsRootSignatureParameter &PARAM, const GraphicsRangeType &RANGE, const GraphicsRootParamType &TYPE)
{
	int type = -1;
	int keepParam = -1;
	//???[?g?p?????̔ԍ????ۑ?
	for (int i = 0; i < PARAM.paramMax; i++)
	{
		if (PARAM.range[i] == RANGE && PARAM.paramData[i].type == TYPE)
		{
			type = PARAM.type[i];
			keepParam = PARAM.paramData[i].param;
		}
	}

	return keepParam;
}

DirectX::XMFLOAT4 KazRenderHelper::SendColorDataToGPU(DirectX::XMFLOAT4 COLOR_DATA)
{
	return DirectX::XMFLOAT4(COLOR_DATA.x / 255.0f, COLOR_DATA.y / 255.0f, COLOR_DATA.z / 255.0f, COLOR_DATA.w / 255.0f);
}
