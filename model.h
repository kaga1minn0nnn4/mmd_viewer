#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>

//https://github.com/kuyuri-iroha/Draw-PMX/blob/master/DrawPMX/pmx_loader.h
//より一部改変

struct Float2
{
	float x;
	float y;
};

struct Float3
{
	float x;
	float y;
	float z;
};

struct Float4
{
	float x;
	float y;
	float z;
	float w;
};


struct Vertex
{
	// 頂点座標
	Float3 position;
	// 法線
	Float3 normal;
	// UV座標
	Float2 uv;
	// 追加UV座標
	std::vector<Float4> additionalUV;

	// ボーンウェイト
	struct Weight
	{
		enum Type
		{
			BDEF1,
			BDEF2,
			BDEF4,
			SDEF,
		};

		Type type;
		int born1;
		int	born2;
		int	born3;
		int	born4;
		float weight1;
		float weight2;
		float weight3;
		float weight4;
		Float3 c;
		Float3 r0;
		Float3 r1;
	} weight;

	// エッジ倍率
	float edgeMagnif;
};

struct Surface
{
	int vertexIndex;
};

struct Material
{
	Float4 diffuse;
	Float3 specular;
	float specularity;
	Float3 ambient;

	int colorMapTextureIndex;
	int toonTextureIndex;
	// (スフィアテクスチャは非対応)

	// 材質ごとの頂点数
	int vertexNum;
};

struct Bone
{
	// ボーン名
	std::wstring name;
	// English version
	std::string nameEnglish;
	Float3 position;
	int parentIndex;
	int transformationLevel;
	unsigned short flag;
	Float3 coordOffset;
	int childrenIndex;
	int impartParentIndex;
	float impartRate;
	// 固定軸方向ベクトル
	Float3 fixedAxis;
	// ローカルのX軸方向ベクトル
	Float3 localAxisX;
	// ローカルのZ軸方向ベクトル
	Float3 localAxisZ;
	int externalParentKey;
	int ikTargetIndex;
	int ikLoopCount;
	float ikUnitAngle;
	struct IKLink
	{
		int index;
		bool existAngleLimited;
		Float3 limitAngleMin;
		Float3 limitAngleMax;
	};
	std::vector<IKLink> ikLinks;
};

struct PMXModelData
{
	static constexpr int NO_DATA_FLAG = -1;

	std::vector<Vertex> vertices;
	std::vector<Surface> surfaces;
	std::vector<std::wstring> texturePaths;
	std::vector<std::string> texturePathsString;
	std::vector<Material> materials;
	std::vector<Bone> bones;
};

bool loadPMX(PMXModelData&, const std::string&);