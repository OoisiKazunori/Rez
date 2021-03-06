#include"../Render/IRender.h"

RESOURCE_HANDLE IRender::CreateConstBuffer(const unsigned int &TRANSMISSION_DATA, const string &ID, const GraphicsRangeType &RANGE, const GraphicsRootParamType &ROOTPARAM)
{
	constBufferDataName.push_back(unique_ptr<string>(new string(ID)));
	constBufferDataSize.push_back(unique_ptr<unsigned int>(new unsigned int(TRANSMISSION_DATA)));
	constBufferRootParam.push_back(unique_ptr<GraphicsRootParamType>(new GraphicsRootParamType(ROOTPARAM)));
	constBufferRangeType.push_back(unique_ptr<GraphicsRangeType>(new GraphicsRangeType(RANGE)));

	RESOURCE_HANDLE lConstBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetConstBufferData(TRANSMISSION_DATA)
	);

	constBufferHandles.push_back(std::make_unique<RESOURCE_HANDLE>(lConstBufferHandle));

	return lConstBufferHandle;
}

void IRender::TransData(void *DATA, RESOURCE_HANDLE HANDLE, const string &ID)
{
	bool succeedFlag = false;
	for (int i = 0; i < constBufferDataName.size(); i++)
	{
		if (*constBufferDataName[i] == ID)
		{
			gpuBuffer->TransData(HANDLE, DATA, *constBufferDataSize[i]);
			succeedFlag = true;
		}
	}
	if (!succeedFlag)
	{
		assert(0);
	}
}

void IRender::SetConstBufferOnCmdList(PipeLineNames pipeline, bool REMOVE_DATA_FLAG)
{
	for (int i = 0; i < constBufferRootParam.size(); i++)
	{
		if (i != 1 || !REMOVE_DATA_FLAG)
		{
			UINT num = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(pipeline)), GRAPHICS_RANGE_TYPE_CBV, *constBufferRootParam[i]);
			renderData.cmdListInstance->cmdList->SetGraphicsRootConstantBufferView(num, gpuBuffer->GetBufferData(*constBufferHandles[i]).Get()->GetGPUVirtualAddress());
		}
	}
}

void IRender::DrawIndexInstanceCommand(const KazRenderHelper::DrawIndexInstanceCommandData &DATA)
{
	//?`?施??-----------------------------------------------------------------------------------------------------
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(DATA.topology);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData.slot, DATA.vertexBufferDrawData.numViews, &DATA.vertexBufferDrawData.vertexBufferView);
	renderData.cmdListInstance->cmdList->IASetIndexBuffer(&DATA.indexBufferView);
	renderData.cmdListInstance->cmdList->DrawIndexedInstanced(
		DATA.drawIndexInstancedData.indexCountPerInstance,
		DATA.drawIndexInstancedData.instanceCount,
		DATA.drawIndexInstancedData.startIndexLocation,
		DATA.drawIndexInstancedData.baseVertexLocation,
		DATA.drawIndexInstancedData.startInstanceLocation
	);
	//?`?施??-----------------------------------------------------------------------------------------------------
}

void IRender::DrawInstanceCommand(const KazRenderHelper::DrawInstanceCommandData &DATA)
{
	renderData.cmdListInstance->cmdList->IASetPrimitiveTopology(DATA.topology);
	renderData.cmdListInstance->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData.slot, DATA.vertexBufferDrawData.numViews, &DATA.vertexBufferDrawData.vertexBufferView);
	renderData.cmdListInstance->cmdList->DrawInstanced(
		DATA.drawInstanceData.vertexCountPerInstance,
		DATA.drawInstanceData.instanceCount,
		DATA.drawInstanceData.startVertexLocation,
		DATA.drawInstanceData.startInstanceLocation
	);
}
