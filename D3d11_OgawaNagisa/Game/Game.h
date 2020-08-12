//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <Windows.h>
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
	std::shared_ptr<GameWindow> window;
	// グラフィックス機能
	std::shared_ptr<Graphics> graphics;

	// 初期化の際に呼び出されます。
	void OnInitialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// メッセージループを実行する際に呼び出されます。
	int OnRun();

};

// トランスフォーム
class Transform final
{
public:
	// このクラスの新しいインスタンスを作成します。
	static std::shared_ptr<Transform> Create();

	DirectX::XMVECTOR scale = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR position = { 0.0f, 0.0f, 0.0f, 1.0f };

	DirectX::XMMATRIX GetWorldMatrix() const;

private:
	// インスタンス生成を禁止
	Transform();
	// コピーと代入演算を禁止
	Transform(const Transform&) {}
	Transform& operator=(const Transform&) { return *this; }
};

// カメラを表します。
class Camera
{
public:
	// このクラスの新しいインスタンスを作成します。
	static std::shared_ptr<Camera> Create(std::shared_ptr<GameWindow> window);

	// 画面クリアーに使用するカラー
	DirectX::XMVECTORF32 clearColor = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// 視点の位置座標
	DirectX::XMVECTOR eyePosition = { 0.0f, 1.0f, -10.0f, 1.0f };
	// 注視点
	DirectX::XMVECTOR focusPosition = { 0.0f, 1.0f, 0.0f, 1.0f };
	// カメラの UP ベクトル
	DirectX::XMVECTOR upDirection = { 0.0f, 1.0f, 0.0f, 0.0f };
	// プロジェクション パラメーター
	float fov = DirectX::XM_PIDIV4;
	float aspectHeightByWidth = 640.0f / 480;
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	// ビューポート
	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f };

	// ビュー変換行列を取得します。
	DirectX::XMMATRIX GetViewMatrix() const;
	// プロジェクション変換行列を取得します。
	DirectX::XMMATRIX GetProjectionMatrix() const;

	// このオブジェクトを初期化する際に一度呼び出されます。
	void Start();

private:
	std::shared_ptr<GameWindow> window;

	// インスタンス生成を禁止
	Camera(std::shared_ptr<GameWindow> window);
	// コピーと代入演算を禁止
	Camera(const Camera&) {}
	Camera& operator=(const Camera&) { return *this; }
};

// ゲーム画面を表します。
class Scene
{
public:
	// このクラスの新しいインスタンスを作成します。
	static Scene* Create(
		std::shared_ptr<GameWindow> window, std::shared_ptr<Graphics> graphics);

	// シーンを初期化する際に呼び出されます。
	void Start();
	// フレームを更新する際に呼び出されます。
	void Update(float time, float elapsedTime);
	// フレームを描画する際に呼び出されます。
	void Draw(float time, float elapsedTime);

private:
	// このシーンと関連付けられたウィンドウ
	std::shared_ptr<GameWindow> window;
	// このシーンと関連付けられたグラフィックス機能
	std::shared_ptr<Graphics> graphics;

	// カメラ パラメーターのための定数バッファーの定義
	struct ConstantBufferDescForCamera
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	// カメラ用の定数バッファー
	std::shared_ptr<ConstantBuffer> constantBufferForCamera;

	// フレームごとに更新される定数バッファーの定義
	struct ConstantBufferDescForPerFrame
	{
		DirectX::XMMATRIX world;
	};
	// フレームごとに更新されるの定数バッファー
	std::shared_ptr<ConstantBuffer> constantBufferForPerFrame;

	// カメラ オブジェクト
	std::shared_ptr<Camera> camera;

	// トランスフォーム
	std::shared_ptr<Transform> transform;
	// メッシュ
	std::shared_ptr<Mesh> mesh;
	// レンダリング
	std::shared_ptr<MeshRenderer> renderer;

	// インスタンス生成を禁止
	Scene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics);
	// コピーと代入演算を禁止
	Scene(const Scene&) {}
	Scene& operator=(const Scene&) { return *this; }
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