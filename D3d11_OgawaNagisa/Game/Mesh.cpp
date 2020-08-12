//=============================================================================
// Mesh.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
std::shared_ptr<Mesh> Mesh::Create(
	UINT numBuffers,
	std::shared_ptr<VertexBuffer>* vertexBuffers, UINT* strides,
	std::shared_ptr<IndexBuffer> indexBuffer,
	UINT numElements,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs)
{
	return shared_ptr<Mesh>(
		new Mesh(
			numBuffers, vertexBuffers, strides,
			indexBuffer,
			numElements, inputElementDescs));
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
Mesh::Mesh(
	UINT numBuffers,
	std::shared_ptr<VertexBuffer>* vertexBuffers, UINT* strides,
	std::shared_ptr<IndexBuffer> indexBuffer,
	UINT numElements,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs)
{
	this->numBuffers = numBuffers;
	for (UINT index = 0; index < numBuffers; ++index) {
		this->vertexBuffers[index] = vertexBuffers[index];
		this->nativePointers[index] = vertexBuffers[index]->GetNativePointer().Get();
		this->strides[index] = strides[index];
		this->offsets[index] = 0;
	}
	this->indexBuffer = indexBuffer;
	this->numElements = numElements;
	for (UINT index = 0; index < numElements; ++index) {
		this->inputElementDescs[index] = inputElementDescs[index];
	}
}

// ���_�o�b�t�@�[�̐����擾���܂��B
UINT Mesh::GetNumBuffers() const
{
	return numBuffers;
}

// ���̃��b�V���Ɋ܂܂��VertexBuffer�̔z����擾���܂��B
const std::shared_ptr<VertexBuffer>* Mesh::GetVertexBuffers() const
{
	return vertexBuffers;
}

// ���̃��b�V���Ɋ܂܂��ID3D11Buffer�̔z����擾���܂��B
ID3D11Buffer* const* Mesh::GetNativePointers() const
{
	return nativePointers;
}

// ID3D11Buffer�̔z��̊e�v�f�ɂ��Ē��_�f�[�^�̃o�C�g�T�C�Y���擾���܂��B
const UINT* Mesh::GetStrides() const
{
	return strides;
}

// ID3D11Buffer�̔z��̊e�v�f�ɂ��Đ擪����̃I�t�Z�b�g���擾���܂��B
const UINT* Mesh::GetOffsets() const
{
	return offsets;
}

// ���̃��b�V���Ɋ܂܂��IndexBuffer���擾���܂��B
std::shared_ptr<const IndexBuffer> Mesh::GetIndexBuffer() const
{
	return indexBuffer;
}

// �v���~�e�B�u�̎�ނ��擾���܂��B
D3D11_PRIMITIVE_TOPOLOGY Mesh::GetPrimitiveTopology() const
{
	return primitiveTopology;
}

// ���_�f�[�^�ɂ��ē��͗v�f���擾���܂��B
const D3D11_INPUT_ELEMENT_DESC* Mesh::GetInputElementDescs() const
{
	return inputElementDescs;
}
