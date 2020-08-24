//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include "GameWindow.h"

class GraphicsDevice;

/// <summary>
/// ビデオアダプターからの出力（通常はモニター）を表します。
/// </summary>
class Output final
{
public:
	/// <summary>
	/// このクラスの新しいインスタンスを初期化します。
	/// </summary>
	explicit Output(Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput);
	/// <summary>
	/// ネイティブ実装のポインターを取得します。
	/// </summary>
	Microsoft::WRL::ComPtr<IDXGIOutput> GetNativePointer();
	DXGI_OUTPUT_DESC GetDesc() const;
	HMONITOR GetHMonitor() const;
	/// <summary>
	/// モニター出力の推奨設定を検索します。
	/// </summary>
	DXGI_MODE_DESC FindClosestMatchingMode(
		UINT width, UINT height, std::shared_ptr<GraphicsDevice> graphicsDevice) const;

private:
	Microsoft::WRL::ComPtr<IDXGIOutput> nativePointer;

	// コピーと代入演算を禁止
	Output(const Output&);
	Output& operator=(const Output&) { return *this; }
};

/// <summary>
/// ビデオアダプターを表します。
/// </summary>
class Adapter final
{
public:
	/// <summary>
	/// このクラスの新しいインスタンスを初期化します。
	/// </summary>
	Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter);
	/// <summary>
	/// ネイティブ実装のポインターを取得します。
	/// </summary>
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetNativePointer();
	DXGI_ADAPTER_DESC1 GetDesc() const;
	std::shared_ptr<Output> GetOutput(int output);
	std::vector<std::shared_ptr<Output>>& GetOutputs();

private:
	Microsoft::WRL::ComPtr<IDXGIAdapter1> nativePointer;
	DXGI_ADAPTER_DESC1 desc = {};
	std::vector<std::shared_ptr<Output>> outputs;

	// コピーと代入演算を禁止
	Adapter(const Adapter&);
	Adapter& operator=(const Adapter&) { return *this; }
};

// グラフィックスデバイスを表します。
class GraphicsDevice final
{
public:
	// IDXGIFactory1オブジェクトを取得します。
	Microsoft::WRL::ComPtr<IDXGIFactory1> GetDXGIFactory() const;
	// IDXGIAdapter1オブジェクトを取得します。
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetDXGIAdapter() const;
	// IDXGIDevice1オブジェクトを取得します。
	Microsoft::WRL::ComPtr<IDXGIDevice1> GetDXGIDevice() const;
	// Direct3D 11のデバイスを取得します。
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	// Direct3D 11の機能レベルを取得します。
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11のデバイス コンテキストを取得します。
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext() const;

	// このクラスの新しいインスタンスを初期化します。
	GraphicsDevice(std::shared_ptr<Adapter> adapter, UINT creationFlags);

private:
	// IDXGIFactory1オブジェクト
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	// IDXGIAdapter1オブジェクト
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
	// IDXGIDevice1オブジェクト
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
	// ID3D11Deviceオブジェクト
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	// Direct3D 11の機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// Direct3D 11のデバイス コンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;

	// コピーと代入演算を禁止
	GraphicsDevice(const GraphicsDevice&) {}
	GraphicsDevice& operator=(const GraphicsDevice&) { return *this; }
};

// グラフィックス リソースの基本クラスを表します。
class GraphicsResource
{
public:
	// Direct3D 11のデバイスを取得します。
	virtual std::shared_ptr<GraphicsDevice> GetGraphicsDevice() final;
	// Direct3D 11のデバイスを取得します。
	virtual std::shared_ptr<const GraphicsDevice> GetGraphicsDevice() const final;

protected:
	// インスタンス生成を禁止
	GraphicsResource(std::shared_ptr<GraphicsDevice> graphicsDevice);
	// コピーを禁止
	GraphicsResource(const GraphicsResource&) {}
private:
	// 代入演算を禁止
	GraphicsResource& operator=(const GraphicsResource&) { return *this; }

	// このリソースを作成したID3D11Deviceオブジェクト
	std::shared_ptr<GraphicsDevice> graphicsDevice;
};

// スワップチェーンを表します。
class SwapChain : public GraphicsResource
{
public:
	// このクラスの新しいインスタンスを初期化します。
	SwapChain(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Output> output);

	// ディスプレイにレンダリングイメージを表示します。
	void Present(UINT syncInterval, UINT flags);
	// スワップチェーンを取得します。
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetNativePointer() const;

private:
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> nativePointer;

	// コピーを禁止
	SwapChain(const SwapChain& other) : GraphicsResource(other) {}
	// 代入演算を禁止
	SwapChain& operator=(const SwapChain&) { return *this; }
};

// レンダーターゲットを表します。
class RenderTarget final : public GraphicsResource
{
public:
	// バックバッファーを取得します。
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer() const;
	// レンダーターゲットを取得します。
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetView() const;
	// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView() const;

	// このクラスの新しいインスタンスを初期化します。
	RenderTarget(std::shared_ptr<SwapChain> swapChain);

private:
	// バックバッファー
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// レンダーターゲット
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view;
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// コピーと代入演算を禁止
	RenderTarget(const RenderTarget& other) : GraphicsResource(other) {}
	RenderTarget& operator=(const RenderTarget&) { return *this; }
};

// 深度ステンシルを表します。
class DepthStencil final : public GraphicsResource
{
public:
	// 深度ステンシルバッファーを取得します。
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer() const;
	// 深度ステンシル ビューを取得します。
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetView() const;
	// 深度ステンシルをシェーダーで利用するためのリソース ビューを取得します。
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView() const;

	// このクラスの新しいインスタンスを初期化します。
	DepthStencil(
		std::shared_ptr<RenderTarget> renderTarget, DXGI_FORMAT format);

private:
	// バッファー
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// 深度ステンシル
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view;
	// 深度ステンシルをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// コピーと代入演算を禁止
	DepthStencil(const DepthStencil& other) : GraphicsResource(other) {}
	DepthStencil& operator=(const DepthStencil&) { return *this; }
};

// グラフィックス初期化についての記述を表します。
struct GraphicsSettings
{
	UINT creationFlags = 0;
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
};

// グラフィックス機能を表します。
class Graphics final
{
public:
	// 描画デバイスを取得します。
	std::shared_ptr<GraphicsDevice> GetGraphicsDevice();
	// 描画デバイスを取得します。
	std::shared_ptr<const GraphicsDevice> GetGraphicsDevice() const;
	// スワップチェーンを取得します。
	std::shared_ptr<SwapChain> GetSwapChain();
	// スワップチェーンを取得します。
	std::shared_ptr<const SwapChain> GetSwapChain() const;
	// レンダーターゲットを取得します。
	std::shared_ptr<RenderTarget> GetRenderTarget();
	// レンダーターゲットを取得します。
	std::shared_ptr<const RenderTarget> GetRenderTarget() const;
	// 深度ステンシルを取得します。
	std::shared_ptr<DepthStencil> GetDepthStencil();
	// 深度ステンシルを取得します。
	std::shared_ptr<const DepthStencil> GetDepthStencil() const;

	// このクラスの新しいインスタンスを初期化します。
	Graphics(
		const GraphicsSettings& settings, std::shared_ptr<GameWindow> window);

private:
	// IDXGIFactory1インターフェイス
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	// 使用可能なすべてのビデオアダプター
	std::vector<std::shared_ptr<Adapter>> adapters;
	// ビデオアダプター
	std::shared_ptr<Adapter> adapter;
	// ビデオアダプター出力
	std::shared_ptr<Output> output;
	// 描画デバイス
	std::shared_ptr<GraphicsDevice> graphicsDevice;
	// スワップチェーン
	std::shared_ptr<SwapChain> swapChain;
	// レンダーターゲット
	std::shared_ptr<RenderTarget> renderTarget;
	// 深度ステンシル
	std::shared_ptr<DepthStencil> depthStencil;

	// コピーと代入演算を禁止
	Graphics(const Graphics&) {}
	Graphics& operator=(const Graphics&) { return *this; }
};

// 頂点シェーダーを表します。
class VertexShader : public GraphicsResource
{
public:
	// このクラスの新しいインスタンスを初期化します。
	template <SIZE_T _Size>
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスの新しいインスタンスを初期化します。
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetNativePointer() const;
	// このシェーダーのバイトコードを取得します。
	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBytecode() const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);
	// コピーと代入演算を禁止
	VertexShader(const VertexShader&);
	VertexShader& operator=(const VertexShader&) { return *this; }
};

// このクラスの新しいインスタンスを初期化します。
template <SIZE_T _Size>
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, _Size);
}

// ジオメトリー シェーダーを表します。
class GeometryShader : public GraphicsResource
{
public:
	// このクラスの新しいインスタンスを初期化します。
	template <SIZE_T _Size>
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスの新しいインスタンスを初期化します。
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetNativePointer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);
	// コピーと代入演算を禁止
	GeometryShader(const GeometryShader&);
	GeometryShader& operator=(const GeometryShader&) { return *this; }
};

// このクラスの新しいインスタンスを初期化します。
template <SIZE_T _Size>
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, _Size);
}

// ピクセル シェーダーを表します。
class PixelShader : public GraphicsResource
{
public:
	// このクラスの新しいインスタンスを初期化します。
	template <SIZE_T _Size>
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスの新しいインスタンスを初期化します。
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetNativePointer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> nativePointer;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);
	// コピーと代入演算を禁止
	PixelShader(const PixelShader&);
	PixelShader& operator=(const PixelShader&) { return *this; }
};

// このクラスの新しいインスタンスを初期化します。
template <SIZE_T _Size>
PixelShader::PixelShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, _Size);
}

// マテリアルを表します。
class Material
{
public:
	// 頂点シェーダーを取得します。
	std::shared_ptr<VertexShader> GetVertexShader();
	// 頂点シェーダーを取得します。
	std::shared_ptr<const VertexShader> GetVertexShader() const;
	// ジオメトリー シェーダーを取得します。
	std::shared_ptr<GeometryShader> GetGeometryShader();
	// ジオメトリー シェーダーを取得します。
	std::shared_ptr<const GeometryShader> GetGeometryShader() const;
	// ピクセル シェーダーを取得します。
	std::shared_ptr<PixelShader> GetPixelShader();
	// ピクセル シェーダーを取得します。
	std::shared_ptr<const PixelShader> GetPixelShader() const;

	// このクラスの新しいインスタンスを初期化します。
	Material(
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<GeometryShader> geometryShader,
		std::shared_ptr<PixelShader> pixelShader);

private:
	// 頂点シェーダー
	std::shared_ptr<VertexShader> vertexShader;
	// ジオメトリー シェーダー
	std::shared_ptr<GeometryShader> geometryShader;
	// ピクセル シェーダー
	std::shared_ptr<PixelShader> pixelShader;

	// コピーと代入演算を禁止
	Material(const Material&);
	Material& operator=(const Material&) { return *this; }
};

// 頂点バッファーを表します。
class VertexBuffer : public GraphicsResource
{
public:
	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;

	// このクラスの新しいインスタンスを初期化します。
	template <class T, UINT _Size>
	VertexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&initialData)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	VertexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth);
	// コピーと代入演算を禁止
	VertexBuffer(const VertexBuffer&);
	VertexBuffer& operator=(const VertexBuffer&) { return *this; }
};

// このクラスの新しいインスタンスを作成します。
template <class T, UINT _Size>
VertexBuffer::VertexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const T(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, sizeof(T) * _Size);
}

// インデックス バッファーを表します。
class IndexBuffer : public GraphicsResource
{
public:
	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;
	// インデックス バッファー内の要素のフォーマットを取得します。
	DXGI_FORMAT GetFormat() const;
	// インデックス バッファー先頭からのオフセットを取得します。
	UINT GetOffset() const;
	// インデックス バッファー内の要素の数を取得します。
	UINT GetCount() const;

	// このクラスの新しいインスタンスを初期化します。
	template <UINT _Size>
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint16_t(&shaderBytecode)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	template <UINT _Size>
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint32_t(&shaderBytecode)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	template <class T, UINT _Size>
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&shaderBytecode)[_Size]);
	// このクラスの新しいインスタンスを初期化します。
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth, DXGI_FORMAT format);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;
	UINT offset = 0;
	UINT count = 0;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth, DXGI_FORMAT format);
	// コピーと代入演算を禁止
	IndexBuffer(const IndexBuffer&);
	IndexBuffer& operator=(const IndexBuffer&) { return *this; }
};

// このクラスの新しいインスタンスを初期化します。
template <UINT _Size>
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint16_t(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R16_UINT);
}

// このクラスの新しいインスタンスを初期化します。
template <UINT _Size>
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint32_t(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R32_UINT);
}

// このクラスの新しいインスタンスを初期化します。
template <class T, UINT _Size>
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const T(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	auto format = DXGI_FORMAT_R32_UINT;
	switch (sizeof(T)) {
	case 4:
		format = DXGI_FORMAT_R32_UINT;
		break;
	case 2:
		format = DXGI_FORMAT_R16_UINT;
		break;
	default:
		throw DX::ComException(E_INVALIDARG);
		return nullptr;
	}
	OnInitialize(graphicsDevice, initialData, sizeof(T) * _Size, format);
}

// 定数バッファーを表します。
class ConstantBuffer : public GraphicsResource
{
public:
	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;

	// このクラスの新しいインスタンスを初期化します。
	ConstantBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth);
	// コピーと代入演算を禁止
	ConstantBuffer(const ConstantBuffer&);
	ConstantBuffer& operator=(const ConstantBuffer&) { return *this; }
};

// 入力レイアウトを表します。
class InputLayout : public GraphicsResource
{
public:
	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetNativePointer() const;

	// このクラスの新しいインスタンスを初期化します。
	template <UINT _Size>
	InputLayout(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_Size],
		const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);
	// このクラスの新しいインスタンスを初期化します。
	InputLayout(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);
	// コピー コンストラクター
	InputLayout(const InputLayout&);
};

// このクラスの新しいインスタンスを作成します。
template <UINT _Size>
InputLayout::InputLayout(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_Size],
	const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(
		graphicsDevice,
		inputElementDescs, _Size,
		shaderBytecodeWithInputSignature);
}

// 
class Image final
{
public:
	// このクラスの新しいインスタンスを初期化します。
	Image(LPCWSTR fileName);

	UINT GetWidth() const;
	UINT GetHeight() const;
	UINT GetStride() const;
	UINT GetPixelSize() const;
	UINT GetSize() const;
	const BYTE* GetPixels() const;

private:
	UINT width = 0;
	UINT height = 0;
	UINT pixelSize = 4;
	std::shared_ptr<BYTE> data;

	// コピーと代入演算を禁止
	Image(const Image&);
	Image& operator=(const Image&) { return *this; }
};

// 2D のテクスチャーを表します。
class Texture2D : public GraphicsResource
{
public:
	// このクラスの新しいインスタンスを初期化します。
	Texture2D(std::shared_ptr<GraphicsDevice> graphicsDevice, LPCWSTR fileName);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSamplerState() const;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	// コピーと代入演算を禁止
	Texture2D(const Texture2D&);
	Texture2D& operator=(const Texture2D&) { return *this; }
};

// モデルのメッシュデータを表します。
class Mesh final
{
public:
	// このクラスの新しいインスタンスを初期化します。
	template <UINT _NumBuffers, UINT _NumElements>
	Mesh(
		std::shared_ptr<VertexBuffer>(&vertexBuffers)[_NumBuffers],
		const UINT(&strides)[_NumBuffers],
		std::shared_ptr<IndexBuffer> indexBuffer,
		const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_NumElements]);
	// このクラスの新しいインスタンスを初期化します。
	Mesh(
		UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, const UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);

	// 頂点バッファーの数を取得します。
	UINT GetNumBuffers() const;

	// このメッシュに含まれるVertexBufferの配列を取得します。
	const std::shared_ptr<VertexBuffer>* GetVertexBuffers();
	// このメッシュに含まれるVertexBufferの配列を取得します。
	const std::shared_ptr<VertexBuffer>* GetVertexBuffers() const;

	// このメッシュに含まれるID3D11Bufferの配列を取得します。
	ID3D11Buffer* const* GetNativePointers();
	// このメッシュに含まれるID3D11Bufferの配列を取得します。
	const ID3D11Buffer* const* GetNativePointers() const;

	// ID3D11Bufferの配列の各要素について頂点データのバイトサイズを取得します。
	const UINT* GetStrides() const;
	// ID3D11Bufferの配列の各要素について先頭からのオフセットを取得します。
	const UINT* GetOffsets() const;

	// このメッシュに含まれるIndexBufferを取得します。
	std::shared_ptr<IndexBuffer> GetIndexBuffer();
	// このメッシュに含まれるIndexBufferを取得します。
	std::shared_ptr<const IndexBuffer> GetIndexBuffer() const;

	// プリミティブの種類を取得します。
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
	// 入力要素の数を取得します。
	UINT GetNumElements() const;
	// 頂点データについて入力要素を取得します。
	const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs() const;

private:
	UINT numBuffers = 0;
	std::shared_ptr<VertexBuffer> vertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
	ID3D11Buffer* nativePointers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
	UINT strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
	UINT offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
	std::shared_ptr<IndexBuffer> indexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT] = {};
	UINT numElements = 0;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, const UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);
	// コピーと代入演算を禁止
	Mesh(const Mesh&) {}
	Mesh& operator=(const Mesh&) { return *this; }
};

// このクラスの新しいインスタンスを初期化します。
template <UINT _NumBuffers, UINT _NumElements>
Mesh::Mesh(
	std::shared_ptr<VertexBuffer>(&vertexBuffers)[_NumBuffers],
	const UINT(&strides)[_NumBuffers],
	std::shared_ptr<IndexBuffer> indexBuffer,
	const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_NumElements])
{
	OnInitialize(
		_NumBuffers, vertexBuffers, strides,
		indexBuffer,
		_NumElements, inputElementDescs);
}

// メッシュをレンダリングする機能を表します。
class MeshRenderer final
{
public:
	// レンダリングに使用するメッシュを取得します。
	std::shared_ptr<Mesh> GetMesh();
	// レンダリングに使用するメッシュを取得します。
	std::shared_ptr<const Mesh> GetMesh() const;
	// レンダリングに使用するマテリアルを取得します。
	std::shared_ptr<Material> GetMaterial();
	// レンダリングに使用するマテリアルを取得します。
	std::shared_ptr<const Material> GetMaterial() const;

	// このクラスの新しいインスタンスを初期化します。
	MeshRenderer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material,
		std::shared_ptr<Texture2D> texture2D);

	// このレンダラーによる描画を実行します。
	void Draw(
		ID3D11DeviceContext* deviceContext);

private:
	// メッシュ
	std::shared_ptr<Mesh> mesh;
	// マテリアル
	std::shared_ptr<Material> material;
	// 入力レイアウト
	std::shared_ptr<InputLayout> inputLayout;
	// テクスチャー
	std::shared_ptr<Texture2D> texture2D;

	// 初期化の際に呼び出されます。
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material,
		std::shared_ptr<Texture2D> texture2D);
	// コピーと代入演算を禁止
	MeshRenderer(const MeshRenderer&);
	MeshRenderer& operator=(const MeshRenderer&) { return *this; }
};