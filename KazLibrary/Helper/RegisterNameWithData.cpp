#include "RegisterNameWithData.h"
#include"../Helper/KazHelper.h"

RegisterNameWithData::RegisterNameWithData()
{
}

RegisterNameWithData::~RegisterNameWithData()
{
}

bool RegisterNameWithData::RegisterName(std::string NAME)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] == NAME)
		{
			std::string dangerString = "危険:" + NAME + "を登録しようとしましたが、同じ名前があった為登録できませんでした";
			ErrorCheck(dangerString.c_str());
			return false;
		}
	}

	name.push_back(NAME);
	return true;
}

bool RegisterNameWithData::UnRegisterName(std::string NAME)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] == NAME)
		{
			name.erase(name.begin() + i);
			return true;
		}
	}

	std::string dangerString = "危険:" + NAME + "を削除しようとしましたが、無かった為削除に失敗しました";
	ErrorCheck(dangerString.c_str());
	return false;
}

int RegisterNameWithData::IsNameExists(std::string NAME)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] == NAME)
		{
			return i;
		}
	}
	std::string dangerString = "危険:" + NAME + "が登録されていません";
	ErrorCheck(dangerString.c_str());
	return -1;
}

void RegisterNameWithData::UnRegisterName(int HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, name.size()))
	{
		name[HANDLE] = "";
	}
}
