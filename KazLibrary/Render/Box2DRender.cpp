#include "Box2DRender.h"

Box2DRender::Box2DRender()
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	vertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(SpriteVertex));
	//頂点バッファ
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);

	drawInstanceCommandData =
		KazRenderHelper::SetDrawInstanceCommandData(D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			KazBufferHelper::SetVertexBufferView(vertByte, vertByte, sizeof(vertices[0])),
			vertices.size(),
			1
		);
}

void Box2DRender::Draw()
{
	//パイプライン設定-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//パイプライン設定-----------------------------------------------------------------------------------------------------

	//位置変更-------------------------
	if (data.leftUpPosDirtyFlag.Dirty() || data.rightDownPosDirtyFlag.Dirty())
	{
		vertices[LEFT_UP].pos = KazMath::Vec3<float>(data.leftUpPos, 0.0f).ConvertXMFLOAT3();
		vertices[LEFT_DOWN].pos = KazMath::Vec3<float>(data.leftUpPos.x, data.rightDownPos.y, 0.0f).ConvertXMFLOAT3();
		vertices[RIGHT_UP].pos = KazMath::Vec3<float>(data.rightDownPos.x, data.leftUpPos.y, 0.0f).ConvertXMFLOAT3();
		vertices[RIGHT_DOWN].pos = KazMath::Vec3<float>(data.rightDownPos, 0.0f).ConvertXMFLOAT3();

		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//位置変更-------------------------



	//バッファの転送-----------------------------------------------------------------------------------------------------
	if (data.color.Dirty() || data.leftUpPosDirtyFlag.Dirty() || data.rightDownPosDirtyFlag.Dirty())
	{
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = DirectX::XMMatrixIdentity();
		constMap.viewproj = renderData.cameraMgrInstance->GetOrthographicMatProjection();
		constMap.color = data.color.ConvertColorRateToXMFLOAT4();
		constMap.mat = constMap.world * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
	}
	//バッファの転送-----------------------------------------------------------------------------------------------------


	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//バッファをコマンドリストに積む-----------------------------------------------------------------------------------------------------

	//描画命令-----------------------------------------------------------------------------------------------------
	DrawIndexInstanceCommand(drawIndexInstanceCommandData);
	//描画命令-----------------------------------------------------------------------------------------------------

	data.Record();
}
