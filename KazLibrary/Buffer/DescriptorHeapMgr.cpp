#include "DescriptorHeapMgr.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"

DescriptorHeapMgr::DescriptorHeapMgr()
{
	//4096を最大のバッファサイズとし、256個ずつ領域を渡すようにする
	short heapMemoryMax = DESCRIPTOR_HEAP_SIZE_MAX / DESCRIPTORHEAP_MEMORY_MAX;

	for (short i = 0; i < DESCRIPTORHEAP_MEMORY_MAX; i++)
	{
		heapMaxSizes[i].startSize = heapMemoryMax * i;
		heapMaxSizes[i].endSize = heapMemoryMax * (i + 1);

		if (1 <= i)
		{
			++heapMaxSizes[i].startSize;
		}
	}

	for (int i = 0; i < DESCRIPTOR_HEAP_SIZE_MAX; i++)
	{
		usedHandle[i] = false;
	}


	//インクリメントのサイズ
	shaderResourceHeapIncreSize = DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	rtvHeapIncreSize = DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	{
		//デスクリプタヒープ作成
		D3D12_DESCRIPTOR_HEAP_DESC srbDescHeap{};
		srbDescHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//シェーダーから見える
		srbDescHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//バッファの数
		srbDescHeap.NumDescriptors = DESCRIPTOR_HEAP_SIZE_MAX;
		DirectX12Device::Instance()->dev->CreateDescriptorHeap(&srbDescHeap, IID_PPV_ARGS(&heaps));
	}
}

DescriptorHeapMgr::~DescriptorHeapMgr()
{
}

void DescriptorHeapMgr::CreateBufferView(const short &HANDLE, const D3D12_SHADER_RESOURCE_VIEW_DESC &BUFFER_VIEW, ID3D12Resource *RESOURCE)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	if (isSafeToUseThisHandle(HANDLE))
	{
		heapHandle = heaps->GetCPUDescriptorHandleForHeapStart();
		heapHandle.ptr += shaderResourceHeapIncreSize * static_cast<UINT64>(HANDLE);
		DirectX12Device::Instance()->dev->CreateShaderResourceView(RESOURCE, &BUFFER_VIEW, heapHandle);
		usedHandle[HANDLE] = true;
	}
	else
	{
		ErrorCheck("既に使われた、もしくはデスクリプタヒープの範囲外の場所でシェーダーリソースビューを作成しようとしています");
	}
}

void DescriptorHeapMgr::Release(const short &HANDLE)
{
	if (isSafeToReadThisHandle(HANDLE))
	{
		usedHandle[HANDLE] = false;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapMgr::GetGpuDescriptorView(const short &HANDLE)
{
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	if (isSafeToReadThisHandle(HANDLE))
	{
		gpuDescHandle = heaps->GetGPUDescriptorHandleForHeapStart();
		gpuDescHandle.ptr += shaderResourceHeapIncreSize * HANDLE;
		return gpuDescHandle;
	}
	else
	{
		gpuDescHandle.ptr = UINT64(-1);
		ErrorCheck("作成されていないビューを取ろうとしています");
		return gpuDescHandle;
	}
}

const D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapMgr::GetCpuDescriptorView(const short &HANDLE)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle;
	if (isSafeToReadThisHandle(HANDLE))
	{
		cpuDescHandle = heaps->GetCPUDescriptorHandleForHeapStart();
		cpuDescHandle.ptr += shaderResourceHeapIncreSize *HANDLE;
		return cpuDescHandle;
	}
	else
	{
		cpuDescHandle.ptr = UINT64(-1);
		ErrorCheck("作成されていないビューを取ろうとしています");
		return cpuDescHandle;
	}
}

const BufferMemorySize DescriptorHeapMgr::GetSize(const BufferMemory &TYPE)
{
	if (TYPE != DESCRIPTORHEAP_MEMORY_MAX)
	{
		return heapMaxSizes[TYPE];
	}
};

void DescriptorHeapMgr::SetDescriptorHeap()
{
	ID3D12DescriptorHeap *ppHeap[] = { heaps.Get() };
	DirectX12CmdList::Instance()->cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
}

int DescriptorHeapMgr::GetType(const short &HANDLE)
{
	for (int i = 0; i < heapMaxSizes.size(); i++)
	{
		if (heapMaxSizes[i].startSize <= HANDLE && HANDLE <= heapMaxSizes[i].endSize)
		{
			return i;
		}
	}
}
