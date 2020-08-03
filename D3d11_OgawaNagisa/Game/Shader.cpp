//=============================================================================
// Shaders.cpp
// 
//=============================================================================
#include "Game.h"
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
#include "BasicGeometryShader.h"

// このクラスの新しいインスタンスを作成します。
BasicVertexShader* BasicVertexShader::Create(ID3D11Device* graphicsDevice)
{
	// このクラスのメモリーを確保
	auto result = new BasicVertexShader();
	if (result == nullptr)
	{
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// 頂点シェーダ―の作成
	auto hr = graphicsDevice->CreateVertexShader(
		g_BasicVertexShader,	// コンパイルされたシェーダーのポインター
		ARRAYSIZE(g_BasicVertexShader),		// コンパイルされた頂点シェーダ―のサイズ
		NULL,	// クラスリンケージインターフェイスへのポインター
		&result->shader);
	if (FAILED(hr)) {
		OutputDebugString(L"頂点シェーダーを作成できませんでした。");
		delete result;
		return nullptr;
	}

	return result;	// 正常に初期化が完了した場合

}

// リソースを開放します。
void BasicVertexShader::Release()
{
	SAFE_RELEASE(shader);
	delete this;
}

// D3D11のネイティブポインターを取得します。
ID3D11VertexShader* BasicVertexShader::GetNativePointer()
{
	return shader;
}

// このシェーダーのバイトコードを取得します。
const BYTE* BasicVertexShader::GetBytecode()
{
	return g_BasicVertexShader;
}

// バイトコードのサイズを取得します。
SIZE_T BasicVertexShader::GetBytecodeLength()
{
	return ARRAYSIZE(g_BasicVertexShader);
}

// このクラスの新しいインスタンスを作成します。
BasicGeometryShader* BasicGeometryShader::Create(ID3D11Device* graphicsDevice)
{
	// このクラスのメモリーを確保
	auto result = new BasicGeometryShader();
	if (result == nullptr) {
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// ジオメトリーシェーダーの作成
	auto hr = graphicsDevice->CreateGeometryShader(
		g_BasicGeometryShader,
		ARRAYSIZE(g_BasicGeometryShader),
		NULL,
		&result->shader);
	if (FAILED(hr)) {
		OutputDebugString(L"ジオメトリーシェーダーを作成できませんでした。");
		delete result;
		return nullptr;
	}

	return result;
}

// リソースを解放します。
void BasicGeometryShader::Release()
{
	SAFE_RELEASE(shader);
	delete this;
}

// D3D11のネイティブポインターを取得します。
ID3D11GeometryShader* BasicGeometryShader::GetNativePointer()
{
	return shader;
}

// このクラスの新しいインスタンスを作成します。
BasicPixelShader* BasicPixelShader::Create(ID3D11Device* graphicsDevice)
{
	// このクラスのメモリーを確保
	auto result = new BasicPixelShader();
	if (result == nullptr) {
		OutputDebugString(L"メモリーを確保できませんでした。");
		return nullptr;
	}

	// ピクセルシェーダーの作成
	auto hr = graphicsDevice->CreatePixelShader(
		g_BasicPixelShader, ARRAYSIZE(g_BasicPixelShader), NULL,
		&result->shader);
	if (FAILED(hr)) {
		OutputDebugString(L"ピクセルシェーダーを作成できませんでした。");
		delete result;
		return nullptr;
	}

	return result;
}

// リソースを解放します。
void BasicPixelShader::Release()
{
	SAFE_RELEASE(shader);
	delete this;
}

// D3D11のネイティブポインターを取得します。
ID3D11PixelShader* BasicPixelShader::GetNativePointer()
{
	return shader;
}
