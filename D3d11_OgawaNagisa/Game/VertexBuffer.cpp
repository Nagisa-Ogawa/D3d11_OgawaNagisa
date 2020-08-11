//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "VertexBuffer.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
VertexBuffer* VertexBuffer::Create(
	GraphicsDevice* graphicsDevice, const void* initialData, UINT byteWidth)
{
	VertexBuffer* retVal = nullptr;
	ID3D11Buffer* nativePointer = nullptr;
	try {
		// ���_�o�b�t�@�[�ɂ��Ă̋L�q
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = byteWidth;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		// �������f�[�^�ɂ��Ă̋L�q
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = initialData;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		// ���_�o�b�t�@�[���쐬
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateBuffer(
				&vertexBufferDesc, &subresourceData, &nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new VertexBuffer(graphicsDevice, nativePointer);
		SafeRelease(nativePointer);

		return retVal;
	}
	catch (...) {
		SafeRelease(nativePointer);
		SafeDelete(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
VertexBuffer::VertexBuffer(
	GraphicsDevice* graphicsDevice, ID3D11Buffer* nativePointer)
	: GraphicsResource(graphicsDevice)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// �f�X�g���N�^�[
VertexBuffer::~VertexBuffer()
{
	SafeRelease(nativePointer);
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return nativePointer;
}
