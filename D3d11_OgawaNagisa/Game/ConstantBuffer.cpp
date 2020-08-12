//=============================================================================
// ConstantBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth)
{
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = byteWidth;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	ThrowIfFailed(graphicsDevice->GetDevice()->CreateBuffer(
		&constantBufferDesc, NULL, &nativePointer));

	return std::shared_ptr<ConstantBuffer>(
		new ConstantBuffer(graphicsDevice, nativePointer));
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
ConstantBuffer::ConstantBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer::GetNativePointer()
{
	return nativePointer;
}
