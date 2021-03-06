#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"

/// <summary>
/// デバイスを生成、管理します
/// </summary>
class DirectX12Device:public ISingleton<DirectX12Device> 
{
public:
	DirectX12Device();
	~DirectX12Device();

	/// <summary>
	/// デバイスの生成
	/// </summary>
	/// <param name="adapter">アダプター</param>
	void CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter);
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
private:
	HRESULT result;
	friend ISingleton<DirectX12Device>;
	
};

