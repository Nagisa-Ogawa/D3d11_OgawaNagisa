#pragma once
#include <Windows.h>
#include <DirectXMath.h>

class ReadModel
{

public:
	// �ʒu���W�݂̂������_�f�[�^�̌^
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// �ʒu���W
		DirectX::XMFLOAT3 normal;	// �@���x�N�g��
		DirectX::XMFLOAT2 texCoord;	// UV���W
	};

	// .obj�t�@�C���̓ǂݍ��݂����郁�\�b�h
	void ReadModelData(char* filename);
	// vertices���擾���郁�\�b�h
	VertexPosition* GetVertexData();
	// indices���擾���郁�\�b�h
	UINT32* GetIndexData();
	// vertices�̃T�C�Y���擾���郁�\�b�h
	SIZE_T GetVerticesSize();
	// indices�̃T�C�Y���擾���郁�\�b�h
	SIZE_T GetIndicesSize();
	// �f�X�g���N�^
	~ReadModel();

private:
	// �z��̃T�C�Y
	UINT32 max;
	// ���_�f�[�^
	VertexPosition* vertices;
	// �C���f�b�N�X�f�[�^
	UINT32* indices;

	// �w�肵�������ŕ������؂蕪���郁�\�b�h
	void SplitBuffer(
		const char splitKeyword,
		const char* readBuffer,
		std::vector<std::string>& splitBuffer);
};
