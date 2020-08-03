//=============================================================================
// Buffers.cpp
// 
//=============================================================================
#include "Game.h"

// このクラスの新しいインスタンスを作成します
VertexBuffer* VertexBuffer::Create(
	ID3D11Device* graphicsDevice, UINT byteWidth)
{
	// このクラスのメモリーを確保
	auto result = new VertexBuffer();
	if (result == nullptr)
	{
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// 作成する頂点バッファについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;	// 作成するバッファのサイズ(bytes)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// バッファの使用方法(とりあえずDEFAULT)
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// Vertex Bufferとして利用する
	bufferDesc.CPUAccessFlags = 0;			// CPUからの読み書きに使わない場合は0
	bufferDesc.MiscFlags = 0;				// オプションのフラグ
	bufferDesc.StructureByteStride = 0;		// 頂点バッファとして使うなら0
	// 頂点バッファを作成
	graphicsDevice->CreateBuffer(
		&bufferDesc,
		NULL,	// バッファの初期データ
		&result->buffer);

	return result;

}

// リソースを開放します。
void VertexBuffer::Release()
{
	SAFE_RELEASE(buffer);
	delete this;
}

// ID3D11のネイティブポインターを取得します。
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer;
}

// バッファーにデータを設定します。
void VertexBuffer::SetData(void* data)
{
	// ID3D11Deviceを取得
	ID3D11Device* graphicsDevice = nullptr;
	buffer->GetDevice(&graphicsDevice);
	// ID3D11DeviceContextを取得
	ID3D11DeviceContext* immediateContext = nullptr;
	graphicsDevice->GetImmediateContext(&immediateContext);

	// バッファにデータを転送(コピー)する
	immediateContext->UpdateSubresource(
		buffer, // 送り先のバッファのポインター
		0,	// 送り先のサブリソースを判別するindex
		NULL,
		data,	// 送るデータへのポインタ 
		0,	// 送るデータの一行のサイズ
		0	// 送るデータの一つの深度スライスのサイズ
	);

	// 取得したインターフェースを開放
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}

// このクラスの新しいインスタンスを作成します。
IndexBuffer* IndexBuffer::Create(
	ID3D11Device* graphicsDevice, UINT indexCount)
{
	// このクラスのメモリーを確保
	auto result = new IndexBuffer();
	if (result == nullptr) {
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// 作成するバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = indexCount * sizeof(UINT32);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// インデックスバッファーを作成
	graphicsDevice->CreateBuffer(&bufferDesc, nullptr, &result->buffer);

	return result;
}

// リソースを解放します。
void IndexBuffer::Release()
{
	SAFE_RELEASE(buffer);
	delete this;
}

// D3D11のネイティブポインターを取得します。
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer;
}

// バッファーにデータを設定します。
void IndexBuffer::SetData(UINT32* data)
{
	// ID3D11Deviceを取得
	ID3D11Device* graphicsDevice = nullptr;
	buffer->GetDevice(&graphicsDevice);
	// ID3D11DeviceContextを取得
	ID3D11DeviceContext* immediateContext = nullptr;
	graphicsDevice->GetImmediateContext(&immediateContext);

	// バッファーにdataを転送(コピー)する
	immediateContext->UpdateSubresource(
		buffer, 0, NULL, data, 0, 0);

	// 取得したインターフェイスを解放
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}

// このクラスの新しいインスタンスを作成します。
ConstantBuffer* ConstantBuffer::Create(
	ID3D11Device* graphicsDevice, UINT byteWidth)
{
	// このクラスのメモリーを確保
	auto result = new ConstantBuffer();
	if (result == nullptr) {
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// 作成するバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 定数バッファーを作成
	graphicsDevice->CreateBuffer(&bufferDesc, nullptr, &result->buffer);

	return result;
}

// リソースを解放します。
void ConstantBuffer::Release()
{
	SAFE_RELEASE(buffer);
	delete this;
}

// D3D11のネイティブポインターを取得します。
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer;
}

// バッファーにデータを設定します。
void ConstantBuffer::SetData(void* data)
{
	// ID3D11Deviceを取得
	ID3D11Device* graphicsDevice = nullptr;
	buffer->GetDevice(&graphicsDevice);
	// ID3D11DeviceContextを取得
	ID3D11DeviceContext* immediateContext = nullptr;
	graphicsDevice->GetImmediateContext(&immediateContext);

	// バッファーにdataを転送(コピー)する
	immediateContext->UpdateSubresource(
		buffer, 0, NULL, data, 0, 0);

	// 取得したインターフェイスを解放
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}
