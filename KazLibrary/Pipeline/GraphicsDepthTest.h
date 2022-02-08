#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"

//一個しか生成できないので注意
class GraphicsDepthTest {
public:
	GraphicsDepthTest();
	~GraphicsDepthTest();

	UINT CreateBuffer();
	void Clear(short HANDLE);

	vector<D3D12_CPU_DESCRIPTOR_HANDLE> dsvH;
private:
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	vector<ComPtr<ID3D12Resource>> depthBuffer;

	HandleMaker handle;
};

