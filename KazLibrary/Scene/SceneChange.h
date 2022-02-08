#pragma once
#include"../DirectXCommon/Base.h"
#include"../Math/KazMath.h"
#include"../Render/KazRender.h"
#include"../Render/KazRender.h"
#include"../Helper/DirtyFlag.h"
namespace ChangeScene {

	class SceneChange {
	public:
		SceneChange();

		void Init();
		void Finalize();
		void Update();
		void Draw();

		/// <summary>
		/// シーン遷移を開始します
		/// </summary>
		void Start();

		/// <summary>
		/// 画面が隠れた瞬間の判定を取ります
		/// 必ずトリガー判定で取る事
		/// </summary>
		/// <returns>true...隠れた,false...隠れてない</returns>
		bool AllHiden();

		/// <summary>
		/// 遷移開始時の変数の初期化
		/// </summary>
		void InitOutbound();

		/// <summary>
		/// 遷移切り返し時の変数の初期化
		/// </summary>
		void InitInbound();

	private:
		bool startFlag, finishFlag;

		KazMath::Transform2D noiseTransform;
		//TextureRender noiseTex;
		short handle;

		int timer;

		short eye_catch_handle_;
		Sprite2DRenderPtr eye_catch_;

		float pos_;

		// 変化率
		float t_;

		// 遷移が始まった瞬間
		bool start_trigger_;

		// 画面が切り替わった瞬間
		bool change_trigger_;

		// 初期位置
		float initial_pos_;

		// 総移動量
		float total_movement_;

		// 遷移
		bool is_inbound_;

		unique_ptr<DirtyFlag<bool>>startDirtyFlag;
	};
}