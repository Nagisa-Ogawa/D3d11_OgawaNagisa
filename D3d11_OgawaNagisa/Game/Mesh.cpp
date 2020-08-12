//=============================================================================
// Mesh.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;

// このクラスの新しいインスタンスを作成します。
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

// このクラスの新しいインスタンスを初期化します。
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

// 頂点バッファーの数を取得します。
UINT Mesh::GetNumBuffers() const
{
	return numBuffers;
}

// このメッシュに含まれるVertexBufferの配列を取得します。
const std::shared_ptr<VertexBuffer>* Mesh::GetVertexBuffers() const
{
	return vertexBuffers;
}

// このメッシュに含まれるID3D11Bufferの配列を取得します。
ID3D11Buffer* const* Mesh::GetNativePointers() const
{
	return nativePointers;
}

// ID3D11Bufferの配列の各要素について頂点データのバイトサイズを取得します。
const UINT* Mesh::GetStrides() const
{
	return strides;
}

// ID3D11Bufferの配列の各要素について先頭からのオフセットを取得します。
const UINT* Mesh::GetOffsets() const
{
	return offsets;
}

// このメッシュに含まれるIndexBufferを取得します。
std::shared_ptr<const IndexBuffer> Mesh::GetIndexBuffer() const
{
	return indexBuffer;
}

// プリミティブの種類を取得します。
D3D11_PRIMITIVE_TOPOLOGY Mesh::GetPrimitiveTopology() const
{
	return primitiveTopology;
}

// 頂点データについて入力要素を取得します。
const D3D11_INPUT_ELEMENT_DESC* Mesh::GetInputElementDescs() const
{
	return inputElementDescs;
}
