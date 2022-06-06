#include "SecondStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"

SecondStage::SecondStage()
{
	stageParamLoader.LoadFile(KazFilePathName::StageParamPath + "StageParamData_SecondStage.json");

	for (int i = 0; i < line.size(); ++i)
	{
		line[i].data.color = { 255.0f,255.0f,255.0f,255.0f };
		line[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;

		handle[i].handle = line[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		handle[i].fogColor.fogdata = { 1.0f,1.0f,1.0f,0.4f };

		FogData tmp = handle[i].fogColor;
		line[i].TransData(&tmp, handle[i].handle, typeid(tmp).name());
	}

	if (false)
	{
		std::array <std::array<char, 7>, 50>data;
		for (int i = 0; i < line.size(); ++i)
		{
			data[i][0] = 'L';
			data[i][1] = 'i';
			data[i][2] = 'n';
			data[i][3] = 'e';
			if (i < 10)
			{
				data[i][4] = std::to_string(i)[0];
				data[i][5] = '\0';
			}
			else
			{
				data[i][4] = std::to_string(i)[0];
				data[i][5] = std::to_string(i)[1];
				data[i][6] = '\0';
			}
		}

		for (int i = 0; i < line.size(); ++i)
		{
			XMVECTOR startPos = line[i].data.startPos;
			XMVECTOR endPos = line[i].data.endPos;

			//Line毎のメンバ変数を追加
			rapidjson::Value startPosArray(rapidjson::kArrayType);
			rapidjson::Value endPosArray(rapidjson::kArrayType);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				startPosArray.PushBack(rapidjson::Value(startPos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
				endPosArray.PushBack(rapidjson::Value(endPos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
			}

			//Lineオブジェクトにデータを追加
			rapidjson::Value object(rapidjson::kObjectType);
			object.AddMember("StartPos", startPosArray, stageParamLoader.doc.GetAllocator());
			object.AddMember("EndPos", endPosArray, stageParamLoader.doc.GetAllocator());
			stageParamLoader.doc.AddMember(rapidjson::GenericStringRef<char>(data[i].data()), object, stageParamLoader.doc.GetAllocator());
		}
		stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "StageParamData_SecondStage.json");
	}

	for (int i = 0; i < JSON_ARRAY_NUM; ++i)
	{
		std::string name = "Line" + std::to_string(i);
		for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
		{
			line[i].data.startPos.m128_f32[axisIndex] = stageParamLoader.doc[name.c_str()]["StartPos"][axisIndex].GetFloat();
			line[i].data.endPos.m128_f32[axisIndex] = stageParamLoader.doc[name.c_str()]["EndPos"][axisIndex].GetFloat();
		}
	}


	int index = 0;
	int zIndex = 0;
	for (int i = 25; i < line.size(); i++)
	{
		float z = -100 + zIndex * 50;
		switch (index)
		{
		case 0:
			line[i].data.startPos = { -150.0f,-100.0f,z };
			line[i].data.endPos = { 150.0f,-100.0f,z };
			++index;
			break;

		case 1:
			line[i].data.startPos = { -150.0f,100.0f,z };
			line[i].data.endPos = { 150.0f,100.0f,z };
			++index;
			break;

		case 2:
			line[i].data.startPos = { -150.0f,100.0f,z };
			line[i].data.endPos = { -150.0f,-100.0f,z };
			++index;
			break;

		case 3:
			line[i].data.startPos = { 150.0f,100.0f,z };
			line[i].data.endPos = { 150.0f,-100.0f,z };
			++zIndex;
			index = 0;
			break;
		default:
			break;
		}
	}
}

void SecondStage::Update()
{
#pragma region ImGui
	bool exportFlag = false;
	bool importFlag = false;

	/*ImGui::Begin("Stage");
	for (int i = 0; i < JSON_ARRAY_NUM; ++i)
	{
		string name = "Line" + std::to_string(i);
		if (ImGui::TreeNode(name.c_str()))
		{
			ImGui::InputFloat("StartPosX", &line[i].data.startPos.m128_f32[0]);
			ImGui::InputFloat("StartPosY", &line[i].data.startPos.m128_f32[1]);
			ImGui::InputFloat("StartPosZ", &line[i].data.startPos.m128_f32[2]);
			ImGui::InputFloat("EndPosX", &line[i].data.endPos.m128_f32[0]);
			ImGui::InputFloat("EndPosY", &line[i].data.endPos.m128_f32[1]);
			ImGui::InputFloat("EndPosZ", &line[i].data.endPos.m128_f32[2]);
			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Import"))
	{
		importFlag = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Export"))
	{
		exportFlag = true;
	}
	ImGui::End();*/

	//ファイル読み込み
	if (importFlag)
	{
		for (int i = 0; i < JSON_ARRAY_NUM; ++i)
		{
			std::string name = "Line" + std::to_string(i);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				line[i].data.startPos.m128_f32[axisIndex] = stageParamLoader.doc[name.c_str()]["StartPos"][axisIndex].GetFloat();
				line[i].data.endPos.m128_f32[axisIndex] = stageParamLoader.doc[name.c_str()]["EndPos"][axisIndex].GetFloat();
			}
		}
	}

	//ファイル書き込み
	if (exportFlag)
	{
		for (int i = 0; i < JSON_ARRAY_NUM; ++i)
		{
			std::string name = "Line" + std::to_string(i);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				stageParamLoader.doc[name.c_str()]["StartPos"][axisIndex].SetFloat(line[i].data.startPos.m128_f32[axisIndex]);
				stageParamLoader.doc[name.c_str()]["EndPos"][axisIndex].SetFloat(line[i].data.endPos.m128_f32[axisIndex]);
			}
		}
		stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "StageParamData_SecondStage.json");
	}


#pragma endregion
	for (int i = 0; i < 4; i++)
	{
		line[i].data.startPos.m128_f32[2] += -1.0f;
		line[i].data.endPos.m128_f32[2] += -1.0f;
		if (line[i].data.startPos.m128_f32[2] <= -150.0f)
		{
			line[i].data.startPos.m128_f32[2] = 1400.0f;
			line[i].data.endPos.m128_f32[2] = 1400.0f;
		}
	}

	for (int i = 25; i < line.size(); i++)
	{
		line[i].data.startPos.m128_f32[2] += -1.0f;
		line[i].data.endPos.m128_f32[2] += -1.0f;
		if (line[i].data.startPos.m128_f32[2] <= -150.0f)
		{
			line[i].data.startPos.m128_f32[2] = 1400.0f;
			line[i].data.endPos.m128_f32[2] = 1400.0f;
		}
	}
}

void SecondStage::Draw()
{
	for (int i = 0; i < line.size(); i++)
	{
		line[i].data.cameraIndex = cameraIndex;
	}
	for (int i = 0; i < line.size(); ++i)
	{
		line[i].Draw();
	}
}