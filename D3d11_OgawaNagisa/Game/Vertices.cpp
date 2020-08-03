//=============================================================================
// Vertices.cpp
// 
//=============================================================================
#include "Game.h"

namespace 
{
	const D3D11_INPUT_ELEMENT_DESC VertexPosition_InputElementDescs[1] =
	{
		{
			"POSITION",						// ���̗v�f�̃Z�}���e�B�b�N��
			0,								// ���̗v�f�̃Z�}���e�B�b�N�C���f�b�N�X�B�����Z�}���e�B�b�N�����v�f����������Ƃ��Ɏg�p
			DXGI_FORMAT_R32G32B32_FLOAT,	// �f�[�^�̌`��
			0,								// ���̓A�Z���u���[�����ʂ��鐮���l
			0,								// ���_�̎n�_����̃I�t�Z�b�g(�o�C�g�P��)
			D3D11_INPUT_PER_VERTEX_DATA,	// ���͗v�f�̃f�[�^�^�C�v���w�肷��\����
			0								// �o�b�t�@����1�v�f���i�ޑO�ɁA�����C���X�^���X���Ƃ̃f�[�^���g�p���ĕ`�悷��C���X�^���X�̐��B
											// ���_�f�[�^���܂ޗv�f�̏ꍇ�A���̒l��0�łȂ���΂Ȃ�
		},
	};
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormal_InputElementDescs[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTexture_InputElementDescs[3] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}

// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
const D3D11_INPUT_ELEMENT_DESC* VertexPosition::GetInputElementDescs()
{
	return VertexPosition_InputElementDescs;
}

// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
UINT VertexPosition::GetInputElementDescsLength()
{
	return ARRAYSIZE(VertexPosition_InputElementDescs);
}

// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormal::GetInputElementDescs()
{
	return VertexPositionNormal_InputElementDescs;
}

// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
UINT VertexPositionNormal::GetInputElementDescsLength()
{
	return ARRAYSIZE(VertexPositionNormal_InputElementDescs);
}

// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormalTexture::GetInputElementDescs()
{
	return VertexPositionNormalTexture_InputElementDescs;
}

// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
UINT VertexPositionNormalTexture::GetInputElementDescsLength()
{
	return ARRAYSIZE(VertexPositionNormalTexture_InputElementDescs);
}
