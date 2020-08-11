//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

// CRTによるメモリーリーク検出
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC	// メモリリーク時にメモリリークを起こしたファイル名や行などが分かる
#include <stdlib.h>
#include <crtdbg.h>		// Debug用の追跡可能な_malloc_dbgや_free_dbgが使える
// new 演算子の再定義
#ifndef DBG_NEW
// new演算子をブロックの種類、ファイル、行番号を表示するデバッグ用に置き換え
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Graphics.h"

// アプリケーション全体を表します。
class Game {
public:
	// アプリケーションを初期化します。
	static void Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// メッセージループを実行します。
	static int Run();
	// アプリケーションを終了します。
	static void Quit();

private:
	// このクラスのシングルトンインスタンスを取得します。
	static Game& GetInstance();

	// すでに初期化済みの場合にtrue、それ以外はfalse
	bool isInitialized = false;
	// メイン ウィンドウ
	GameWindow* window = nullptr;
	// グラフィックス機能
	Graphics* graphics = nullptr;


	DirectX::XMVECTORF32 clearColor = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// ビューポート
	D3D11_VIEWPORT viewport = {};

	// 初期化の際に呼び出されます。
	void OnInitialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// メッセージループを実行する際に呼び出されます。
	int OnRun();

};

//// 位置座標のみを頂点情報に持つデータを表します。
//struct VertexPosition
//{
//	DirectX::XMFLOAT3 position;	// 位置座標
//
//	// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
//	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
//	// GetInputElementDescs()関数で取得される配列の要素数を取得します。
//	static UINT GetInputElementDescsLength();
//};
//
//// 位置座標と法線ベクトルを頂点情報に持つデータを表します。
//struct VertexPositionNormal
//{
//	DirectX::XMFLOAT3 position;	// 位置座標
//	DirectX::XMFLOAT3 normal;	// 法線ベクトル
//
//	// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
//	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
//	// GetInputElementDescs()関数で取得される配列の要素数を取得します。
//	static UINT GetInputElementDescsLength();
//};
//
//// 位置座標と法線ベクトルとテクスチャー座標を頂点情報に持つデータを表します
//struct VertexPositionNormalTexture
//{
//	DirectX::XMFLOAT3 position;	// 位置座標
//	DirectX::XMFLOAT3 normal;	// 法線ベクトル
//	DirectX::XMFLOAT2 texCoord;	// テクスチャー座標(UV)
//
//	// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
//	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
//	// GetInputElementDescs()関数で取得される配列の要素数を取得します。
//	static UINT GetInputElementDescsLength();
//
//};
//
//// 頂点シェーダ―を表します。
//class BasicVertexShader
//{
//private:
//	// D3D11のインターフェース
//	ID3D11VertexShader* shader = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します
//	static BasicVertexShader* Create(ID3D11Device* graphicsDevice);
//	// リソースを開放します
//	void Release();
//	// D3D11のネイティブポインターを取得します
//	ID3D11VertexShader* GetNativePointer();
//	// このシェーダーのバイトコードを取得します。
//	const BYTE* GetBytecode();
//	// バイトコードのサイズを取得します。
//	SIZE_T GetBytecodeLength();
//};
//
//// ジオメトリーシェーダーを表します。
//class BasicGeometryShader
//{
//private:
//	// D3D11のインターフェース
//	ID3D11GeometryShader* shader = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します
//	static BasicGeometryShader* Create(ID3D11Device* graphicsDevice);
//	// リソースを開放します
//	void Release();
//	// D3D11のネイティブポインターを取得します
//	ID3D11GeometryShader* GetNativePointer();
//};
//
//// ピクセルシェーダ―を表します。
//class BasicPixelShader
//{
//private:
//	// D3D11のインターフェース
//	ID3D11PixelShader* shader = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します
//	static BasicPixelShader* Create(ID3D11Device* graphicsDevice);
//	// リソースを開放します
//	void Release();
//	// D3D11のネイティブポインターを取得します
//	ID3D11PixelShader* GetNativePointer();
//};
//
//// 頂点バッファーを表します。
//class VertexBuffer
//{
//	// D3D11リソース
//	ID3D11Buffer* buffer = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します。
//	static VertexBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
//	// リソースを開放します。
//	void Release();
//	// D3D11のネイティブポインターを取得します。
//	ID3D11Buffer* GetNativePointer();
//
//	// バッファーにデータを設定します。
//	void SetData(void* data);
//};
//
//// インデックスバッファーを表します。
//class IndexBuffer
//{
//	// D3D11リソース
//	ID3D11Buffer* buffer = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します。
//	static IndexBuffer* Create(ID3D11Device* graphicsDevice, UINT indexCount);
//	// リソースを解放します。
//	void Release();
//	// D3D11のネイティブポインターを取得します。
//	ID3D11Buffer* GetNativePointer();
//
//	// バッファーにデータを設定します。
//	void SetData(UINT32* data);
//};
//
//// 定数バッファーを表します。
//class ConstantBuffer
//{
//	// D3D11リソース
//	ID3D11Buffer* buffer = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します。
//	static ConstantBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
//	// リソースを解放します。
//	void Release();
//	// D3D11のネイティブポインターを取得します。
//	ID3D11Buffer* GetNativePointer();
//
//	// バッファーにデータを設定します。
//	void SetData(void* data);
//};
//
//// 入力レイアウトを表します。
//class InputLayout
//{
//	// D3D11リソース
//	ID3D11InputLayout* inputLayout = nullptr;
//
//public:
//	// このクラスの新しいインスタンスを作成します。
//	static InputLayout* Create(
//		ID3D11Device* graphicsDevice,
//		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
//		const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength);
//	// リソースを開放します。
//	void Release();
//	// D3D11のネイティブポインターを取得します。
//	ID3D11InputLayout* GetNativePointer();
//};
//
//class Texture2D
//{
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
//	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
//	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
//public:
//	UINT width;
//	// このクラスの新しいインスタンスを作成します。
//	static Texture2D* Create(
//		ID3D11Device* graphicsDevice,
//		UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);
//	// リソースを解放します。
//	void Release();
//
//	// テクスチャーのピクセルを変更します。
//	void SetData(const void* data);
//
//	// D3D11のネイティブポインターを取得します。
//	ID3D11Texture2D* GetNativePointer();
//	ID3D11SamplerState* GetSamplerState();
//	ID3D11ShaderResourceView* GetShaderResourceView();
//};
//
//class ReadBitMap
//{
//	uint32_t* source;
//public:
//	UINT width;
//	UINT height;
//	// bmpからデータを読み込みます
//	uint32_t* ReadFromBitMap(char* path);
//	// リソースを開放します。
//	void Release();
//};