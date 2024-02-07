#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>

//https://github.com/kuyuri-iroha/Draw-PMX/blob/master/DrawPMX/pmx_loader.h
//���ꕔ����

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
	// ���_���W
	Float3 position;
	// �@��
	Float3 normal;
	// UV���W
	Float2 uv;
	// �ǉ�UV���W
	std::vector<Float4> additionalUV;

	// �{�[���E�F�C�g
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

	// �G�b�W�{��
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
	// (�X�t�B�A�e�N�X�`���͔�Ή�)

	// �ގ����Ƃ̒��_��
	int vertexNum;
};

struct Bone
{
	// �{�[����
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
	// �Œ莲�����x�N�g��
	Float3 fixedAxis;
	// ���[�J����X�������x�N�g��
	Float3 localAxisX;
	// ���[�J����Z�������x�N�g��
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