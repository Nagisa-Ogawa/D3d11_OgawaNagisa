//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// このクラスのインスタンスを初期化します。
VertexBuffer::VertexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, byteWidth);
}

// 初期化の際に呼び出されます。
void VertexBuffer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth)
{
	// バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 初期化データについての記述
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = initialData;
	// バッファーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// このクラスのインスタンスを初期化します。
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth, DXGI_FORMAT format)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, byteWidth, format);
}

// 初期化の際に呼び出されます。
void IndexBuffer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const void* initialData, UINT byteWidth, DXGI_FORMAT format)
{
	// バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 初期化データについての記述
	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = initialData;
	// バッファーを作成
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateBuffer(
			&bufferDesc, &subresourceData, &nativePointer));

	this->format = format;
	this->offset = 0;

	switch (format) {
	case DXGI_FORMAT_R32_UINT:
		count = byteWidth / sizeof(UINT);
		break;
	case DXGI_FORMAT_R16_UINT:
		count = byteWidth / sizeof(USHORT);
		break;
	default:
		throw DX::ComException(E_INVALIDARG);
		break;
	}
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer::GetNativePointer() const
{
	return nativePointer;
}

// インデックス バッファー内の要素のフォーマットを取得します。
DXGI_FORMAT IndexBuffer::GetFormat() const
{
	return format;
}

// インデックス バッファー先頭からのオフセットを取得します。
UINT IndexBuffer::GetOffset() const
{
	return offset;
}

// インデックス バッファー内の要素の数を取得します。
UINT IndexBuffer::GetCount() const
{
	return count;
}

// このクラスの新しいインスタンスを初期化します。
ConstantBuffer::ConstantBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, byteWidth);
}

// 初期化の際に呼び出されます。
void ConstantBuffer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth)
{
	// バッファーについての記述
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = byteWidth;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;
	// バッファーを作成
	ThrowIfFailed(graphicsDevice->GetDevice()->CreateBuffer(
		&constantBufferDesc, NULL, &nativePointer));
}

// ネイティブ実装のポインターを取得します。
Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer::GetNativePointer() const
{
	return nativePointer;
}
