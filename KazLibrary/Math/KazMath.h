#pragma once
#include"../DirectXCommon/Base.h"

#pragma warning(push)
#pragma warning(disable:4023)
#include"fbxsdk.h"
#pragma warning(pop)

namespace KazMath
{

#pragma region Vec2
	template<typename T>
	struct Vec2
	{
		T x, y;

		Vec2()
		{
			x = 0;
			y = 0;
		};
		Vec2(T X, T Y) :x(X), y(Y) {};

		float Length()const
		{
			return sqrt(pow(x, 2) + pow(y, 2));
		};
		float Distance(const Vec2 &To)const
		{
			return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2));
		};
		Vec2<float> GetNormal()const
		{
			float len = Length();
			if (len == 0.0f)return Vec2<float>(0.0, 0.0);
			return Vec2<float>(x / len, y / len);
		};
		void Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
		};
		XMFLOAT2 ConvertXMFLOAT2()
		{
			return DirectX::XMFLOAT2(x, y);
		};

		Vec2<int>Int()const
		{
			return Vec2<int>(x, y);
		}
		Vec2<float>Float()const
		{
			return Vec2<float>((float)x, (float)y);
		}

		float Dot(const Vec2<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.x + me.y * rhs.y;
		}
		float Cross(const Vec2<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.y - me.y * rhs.x;
		}

#pragma region オペレーター演算子
		Vec2 operator-() const
		{
			return Vec2(-x, -y);
		}
		Vec2 operator+(const Vec2 &rhs) const
		{
			return Vec2(x + rhs.x, y + rhs.y);
		};
		Vec2 operator-(const Vec2 &rhs)const
		{
			return Vec2(x - rhs.x, y - rhs.y);
		};
		Vec2 operator*(const Vec2 &rhs)const
		{
			return Vec2(x * rhs.x, y * rhs.y);
		};
		Vec2 operator*(const float &rhs)const
		{
			return Vec2(x * rhs, y * rhs);
		};
		Vec2 operator/(const Vec2 &rhs)const
		{
			return Vec2(x / rhs.x, y / rhs.y);
		};
		Vec2 operator/(const float &rhs)const
		{
			return Vec2(x / rhs, y / rhs);
		};
		Vec2 operator%(const Vec2 &rhs) const
		{
			return Vec2(fmodf(x, rhs.x), fmodf(y, rhs.y));
		};
		void operator=(const Vec2 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		};
		bool operator==(const Vec2 &rhs)const
		{
			return (x == rhs.x && y == rhs.y);
		};
		bool operator!=(const Vec2 &rhs)const
		{
			return !(*this == rhs);
		};
		void operator+=(const Vec2 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
		};
		void operator-=(const Vec2 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		};
		void operator*=(const Vec2 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
		};
		void operator/=(const Vec2 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
		};
		void operator%=(const Vec2 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
		};

		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
		};
		void operator%=(const float &rhs)
		{
			x = fmodf(x, rhs);
			y = fmodf(y, rhs);
		};

		bool operator<(const Vec2<T> &rhs)
		{
			return x < rhs.x &&y < rhs.y;
		}
#pragma endregion
	};
#pragma endregion

#pragma region Vec3
	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3() 
		{
			x = 0;
			y = 0;
			z = 0;
		};
		Vec3(T X, T Y, T Z) :x(X), y(Y), z(Z) {};
		float Length() const
		{
			return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		};
		Vec3(Vec2<T>XY, T Z) :x(XY.x), y(XY.y), z(Z) {};

		float Distance(const Vec3 &To)const
		{
			return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2) + pow(To.z - z, 2));
		};
		Vec3<float> GetNormal()const
		{
			float len = Length();
			if (len == 0.0f)return Vec3(0, 0, 0);
			return Vec3<float>(x / len, y / len, z / len);
		};
		void Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
			z /= len;
		};
		XMFLOAT3 ConvertXMFLOAT3()
		{
			return DirectX::XMFLOAT3(x, y, z);
		};
		XMVECTOR ConvertXMVECTOR()
		{
			return XMVECTOR(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0f);
		};

		Vec3<int>Int()const
		{
			return Vec3<int>(x, y, z); 
		}
		Vec3<float>Float()const
		{ 
			return Vec3<float>((float)x, (float)y, (float)z);
		}

		float Dot(const Vec3<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.x + me.y * rhs.y + me.z * rhs.z;
		}
		Vec3<float>Cross(const Vec3<float> &rhs)const
		{
			auto me = Float();
			return Vec3<float>(
				me.y * rhs.z - rhs.y * me.z,
				me.z * rhs.x - rhs.z * me.x,
				me.x * rhs.y - rhs.x * me.y);
		}

#pragma region オペレーター演算子
		Vec3 operator-()const
		{
			return Vec3(-x, -y, -z);
		}
		Vec3 operator+(const Vec3 &rhs)const
		{
			return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
		};
		Vec3 operator-(const Vec3 &rhs)const
		{
			return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
		};
		Vec3 operator*(const Vec3 &rhs) const
		{
			return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
		};
		Vec3 operator*(const float &rhs)const
		{
			return Vec3(x * rhs, y * rhs, z * rhs);
		};
		Vec3 operator/(const Vec3 &rhs)const
		{
			return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
		};
		Vec3 operator/(const float &rhs)const
		{
			return Vec3(x / rhs, y / rhs, z / rhs);
		};
		Vec3 operator%(const Vec3 &rhs)const
		{
			return Vec3(fmodf(x, rhs.x), fmodf(y, rhs.y), fmodf(z, rhs.z));
		};
		void operator=(const Vec3 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		};
		bool operator==(const Vec3 &rhs)const
		{
			return (x == rhs.x && y == rhs.y && z == rhs.z);
		};
		bool operator!=(const Vec3 &rhs)const
		{
			return !(*this == rhs);
		};
		void operator+=(const Vec3 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		};
		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
		};
		void operator-=(const Vec3 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
		};
		void operator*=(const Vec3 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
		};
		void operator/=(const Vec3 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
		};
		void operator%=(const Vec3 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
			z = fmodf(z, rhs.z);
		};
#pragma endregion
	};
#pragma endregion




	/// <summary>
	/// 3D空間上でオブジェクトを動かす際のデータです
	/// </summary>
	struct Transform3D
	{
		Vec3<float> pos;
		Vec3<float> scale;
		Vec3<float> rotation;

		Transform3D()
		{
			pos = { 0.0f,0.0f,0.0f };
			scale = { 1.0f,1.0f,1.0f };
			rotation = { 0.0f,0.0f,0.0f };
		};
	};

	/// <summary>
	/// 2D空間上でオブジェクトを動かす際のデータです
	/// </summary>
	struct Transform2D
	{
		Vec2<float> pos;
		Vec2<float> scale;
		float rotation;
		Transform2D()
		{
			pos = { 0.0f,0.0f };
			scale = { 1.0f,1.0f };
			rotation = 0.0f;
		};
	};

	struct BaseMatWorldData
	{
		XMMATRIX matWorld;
		XMMATRIX matScale;
		XMMATRIX matRota;
		XMMATRIX matTrans;
	};


	static const float COLOR_MAX_VALUE = 255.0f;



	static const float PI_2F = 3.1415926540f;
	static const XMMATRIX MAT_IDENTITY = XMMatrixIdentity();


	XMVECTOR LoadFloat3ToVector(XMFLOAT3 NUM_2);
	XMFLOAT3 LoadVecotrToXMFLOAT3(XMVECTOR NUM_1);


	XMVECTOR CalculateScreenToWorld(XMVECTOR pout, XMMATRIX View, XMMATRIX Prj);

	template<typename T>
	Vec2<T> CaluAngle(const Vec2<T> &POS, float Angle, const Vec2<T> &CPos);
	template<typename T>
	Vec3<T> CaluAngle3D(const Vec3<T> &POS, float ANGLE, const Vec3<T> &CENTRAL_POS);

	template<typename T>
	T Rand(T MAX_NUM, T MIN_NUM);

	void ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC);

	/// <summary>
	/// 秒数をフレームに直します
	/// </summary>
	/// <param name="SECOND">秒数</param>
	/// <returns>フレーム</returns>
	int ConvertSecondToFlame(int SECOND);

	/// <summary>
	/// 目標地点に到達するまでの移動量を計算します
	/// </summary>
	/// <param name="A_POINT">A地点</param>
	/// <param name="B_POINT">B地点</param>
	/// <param name="FLAME">一秒あたりの時間</param>
	/// <param name="MAX_FLAME">到達時間</param>
	/// <returns>目標地点までの移動量</returns>
	template <typename T>
	Vec3<T> CaluAPointToBPointVel(const Vec3<T> &A_POINT, const Vec3<T> &B_POINT, int FLAME, int MAX_FLAME);

	/// <summary>
	/// スクリーン座標からワールド座標に変換します
	/// </summary>
	/// <param name="SCREEN_POS">スクリーン座標</param>
	/// <param name="VIEW_MAT">ビュー行列</param>
	/// <param name="PROJECTION_MAT">プロジェクション行列</param>
	/// <returns>ワールド座標</returns>
	template <typename T>
	Vec3<T> ConvertScreenPosToWorldPos(const Vec3<T> &SCREEN_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);

	/// <summary>
	/// ワールド座標からスクリーン座標に変換します
	/// </summary>
	/// <param name="SCREEN_POS">ワールド座標</param>
	/// <param name="VIEW_MAT">ビュー行列</param>
	/// <param name="PROJECTION_MAT">プロジェクション行列</param>
	/// <returns>スクリーン座標</returns>
	template <typename T>
	Vec3<T> ConvertWorldPosToScreenPos(const Vec3<T> &WORLD_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);


	/// <summary>
	/// 回転行列を計算します
	/// </summary>
	/// <param name="ROTATION">角度</param>
	/// <returns>回転行列</returns>
	XMMATRIX CaluRotaMatrix(const Vec3<float> &ROTATION);

	/// <summary>
	/// スケーリング行列を計算します
	/// </summary>
	/// <param name="SCALE">大きさ</param>
	/// <returns>スケーリング行列</returns>
	XMMATRIX CaluScaleMatrix(const Vec3<float> &SCALE);

	/// <summary>
	/// 平行移動行列を計算します
	/// </summary>
	/// <param name="POS">座標</param>
	/// <returns>平行移動行列</returns>
	XMMATRIX CaluTransMatrix(const Vec3<float> &POS);

	/// <summary>
	/// 回転行列を計算します
	/// </summary>
	/// <param name="ROTATION">角度</param>
	/// <returns>回転行列</returns>
	XMMATRIX CaluRotaMatrix(const Vec3<float> &ROTATION);

	/// <summary>
	/// スケーリング行列を計算します
	/// </summary>
	/// <param name="SCALE">大きさ</param>
	/// <returns>スケーリング行列</returns>
	XMMATRIX CaluScaleMatrix(const Vec3<float> &SCALE);

	/// <summary>
	/// 平行移動行列を計算します
	/// </summary>
	/// <param name="POS">座標</param>
	/// <returns>平行移動行列</returns>
	XMMATRIX CaluTransMatrix(const Vec3<float> &POS);


	/// <summary>
	/// ガウシアンの大きさを計算する
	/// </summary>
	/// <param name="count"></param>
	/// <param name="s"></param>
	/// <returns></returns>
	vector<float> GetGaussianWeights(size_t count, float s);

	/// <summary>
	/// バッファのサイズをアライメントする
	/// </summary>
	/// <param name="size">元々のサイズ</param>
	/// <param name="alignment">何バイトにしたいか</param>
	/// <returns>バッファのサイズ</returns>
	unsigned int AligmentedValue(unsigned int size, unsigned int alignment);

	/// <summary>
	/// ガウシアンの大きさを計算する
	/// </summary>
	/// <param name="weightsTbl">配列の先頭アドレス</param>
	/// <param name="sizeOfWeightsTbl">配列の大きさ</param>
	/// <param name="sigma">どれくらいぼかすか。数字が大きければ大きいほどぼかせる</param>
	void CalcWeightsTableFromGaussian(float *weightsTbl, int sizeOfWeightsTbl, float sigma);


	/// <summary>
	/// デバック用の視点座標を計算します
	/// 上下左右自由にカメラを動かす事が可能になります
	/// </summary>
	/// <param name="EYE_POS">視点座標</param>
	/// <param name="ANGLE">角度</param>
	/// <returns>デバック用の視点座標</returns>
	template<typename T>
	Vec3<T> CaluEyePosForDebug(const Vec3<T> &EYE_POS, const Vec3<float> &MOVE, const Vec2<float> &ANGLE);

	/// <summary>
	/// デバック用の注視点を計算します
	/// </summary>
	/// <param name="EYE_POS">視点座標</param>
	/// <param name="ANGLE">角度</param>
	/// <returns>デバック用の注視点</returns>
	template<typename T>
	Vec3<T> CaluTargetPosForDebug(const Vec3<T> &EYE_POS, float ANGLE);


	bool MatrixEqualOrNot(const XMMATRIX &MAT_1, const XMMATRIX &MAT_2);

	template<typename T>
	XMMATRIX CaluSlopeMatrix(const Vec3<T> &Y,const Vec3<T> &Z);

	template<typename T>
	XMMATRIX CaluFrontMatrix(const Vec3<T> &Y, const Vec3<T> &Z);

	int RadianToAngle(float RADIAN);
	float AngleToRadian(int ANGLE);

	template<typename T>
	XMMATRIX CaluMat(const KazMath::Transform3D &TRANSFORM, const Vec3<T> &Y_VEC, const Vec3<T> &Z_VEC, const XMMATRIX &VIEW_MAT, const XMMATRIX &PROJECT_MAT);
	template<typename T>
	XMMATRIX CaluWorld(const KazMath::Transform3D &TRANSFORM, const Vec3<T> &Y_VEC, const Vec3<T> &Z_VEC);


	void Larp(float BASE_TRANSFORM, float *TRANSFORM, float MUL);

	template<typename T>
	float GetSinFloat(T VALUE)
	{
		return static_cast<float>(sin(static_cast<float>VALUE));
	};

	template<typename T>
	float GetCosFloat(T VALUE)
	{
		return static_cast<float>(cos(static_cast<float>VALUE));
	};

	template<typename T>
	Vec3<T>CastXMVECTOR(const XMVECTOR &VALUE)
	{
		Vec3<T> result(static_cast<T>(VALUE.m128_f32[0]), static_cast<T>(VALUE.m128_f32[1]), static_cast<T>(VALUE.m128_f32[2]));
		return result;
	};

	/// <summary>
	/// スプライン曲線
	/// </summary>
	/// <param name="points"></param>
	/// <param name="startIndex"></param>
	/// <param name="t"></param>
	/// <param name="Loop"></param>
	/// <returns></returns>
	template <typename T>
	Vec3<T> SplinePosition(const std::vector<Vec3<T>> &points, size_t startIndex, float t, bool Loop);

}

