//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include "GameWindow.h"

// グラフィックスデバイスを表します。
class GraphicsDevice final
{
public:
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<GraphicsDevice> Create();

	// IDXGIFactory1オブジェクトを取得します。
	Microsoft::WRL::ComPtr<IDXGIFactory1> GetDXGIFactory();
	// IDXGIAdapter1オブジェクトを取得します。
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetDXGIAdapter();
	// IDXGIDevice1オブジェクトを取得します。
	Microsoft::WRL::ComPtr<IDXGIDevice1> GetDXGIDevice();
	// Direct3D 11のデバイスを取得します。
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	// Direct3D 11の機能レベルを取得します。
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11のデバイス コンテキストを取得します。
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext();

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

	// インスタンス生成を禁止
	GraphicsDevice(
		Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory,
		Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter,
		Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		D3D_FEATURE_LEVEL featureLevel,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext);
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
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<SwapChain> Create(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		DXGI_FORMAT format, const DXGI_SAMPLE_DESC& sampleDesc);

	// ディスプレイにレンダリングイメージを表示します。
	void Present(UINT syncInterval, UINT flags);
	// スワップチェーンを取得します。
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetNativePointer();

private:
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> nativePointer;

	// インスタンス生成を禁止
	SwapChain(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain);
	// コピーを禁止
	SwapChain(const SwapChain& other) : GraphicsResource(other) {}
	// 代入演算を禁止
	SwapChain& operator=(const SwapChain&) { return *this; }
};

// レンダーターゲットを表します。
class RenderTarget final : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<RenderTarget> Create(
		std::shared_ptr<SwapChain> swapChain);

	// バックバッファーを取得します。
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer();
	// レンダーターゲットを取得します。
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetView();
	// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView();

private:
	// バックバッファー
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// レンダーターゲット
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view;
	// バックバッファーをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// インスタンス生成を禁止
	RenderTarget(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView);
	// コピーと代入演算を禁止
	RenderTarget(const RenderTarget& other) : GraphicsResource(other) {}
	RenderTarget& operator=(const RenderTarget&) { return *this; }
};

// 深度ステンシルを表します。
class DepthStencil final : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<DepthStencil> Create(
		std::shared_ptr<RenderTarget> renderTarget, DXGI_FORMAT format);

	// 深度ステンシルバッファーを取得します。
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer();
	// レンダーターゲットを取得します。
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetView();
	// バックバッファーをシェーダーで利用するためのリソース ビューを取得します。
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView();

private:
	// バッファー
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// 深度ステンシル
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view;
	// 深度ステンシルをシェーダーで利用するためのリソース ビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// インスタンス生成を禁止
	DepthStencil(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView);
	// コピーと代入演算を禁止
	DepthStencil(const DepthStencil& other) : GraphicsResource(other) {}
	DepthStencil& operator=(const DepthStencil&) { return *this; }
};

// グラフィックス機能を表します。
class Graphics final
{
public:
	// グラフィックス機能を初期化します。
	static std::shared_ptr<Graphics> Create(
		std::shared_ptr<GameWindow> gameWindow,
		DXGI_FORMAT backBufferFormt, DXGI_FORMAT depthStencilFormt);

	// グラフィックス デバイスを取得します。
	std::shared_ptr<GraphicsDevice> GetGraphicsDevice();
	// スワップチェーンを取得します。
	std::shared_ptr<SwapChain> GetSwapChain();
	// レンダーターゲットを取得します。
	std::shared_ptr<RenderTarget> GetRenderTarget();
	// 深度ステンシルを取得します。
	std::shared_ptr<DepthStencil> GetDepthStencil();

private:
	// グラフィックス デバイス
	std::shared_ptr<GraphicsDevice> graphicsDevice;
	// スワップチェーン
	std::shared_ptr<SwapChain> swapChain;
	// レンダーターゲット
	std::shared_ptr<RenderTarget> renderTarget;
	// 深度ステンシル
	std::shared_ptr<DepthStencil> depthStencil;

	// インスタンス生成を禁止
	Graphics(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<SwapChain> swapChain,
		std::shared_ptr<RenderTarget> renderTarget,
		std::shared_ptr<DepthStencil> depthStencil);
	// コピーと代入演算を禁止
	Graphics(const Graphics&) {}
	Graphics& operator=(const Graphics&) { return *this; }
};

// 頂点シェーダーを表します。
class VertexShader : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static std::shared_ptr<VertexShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<VertexShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<VertexShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetNativePointer();
	// このシェーダーのバイトコードを取得します。
	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBytecode();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode;
	// このクラスのインスタンスを初期化します。
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer,
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode);
	// コピー コンストラクター
	VertexShader(const VertexShader&);
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static std::shared_ptr<VertexShader> VertexShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}

// ジオメトリー シェーダーを表します。
class GeometryShader : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static std::shared_ptr<GeometryShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<GeometryShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<GeometryShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer;
	// このクラスのインスタンスを初期化します。
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer);
	// コピー コンストラクター
	GeometryShader(const GeometryShader&);
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static std::shared_ptr<GeometryShader> GeometryShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}

// ピクセル シェーダーを表します。
class PixelShader : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <SIZE_T _Size>
	static std::shared_ptr<PixelShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<PixelShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<PixelShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> nativePointer;
	// このクラスのインスタンスを初期化します。
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> nativePointer);
	// コピー コンストラクター
	PixelShader(const PixelShader&);
};

// このクラスのインスタンスを作成します。
template <SIZE_T _Size>
static std::shared_ptr<PixelShader> PixelShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}

// マテリアルを表します。
class Material
{
public:
	// このクラスの新しいインスタンスを作成します。
	static std::shared_ptr<Material> Create(
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<GeometryShader> geometryShader,
		std::shared_ptr<PixelShader> pixelShader);

	// 頂点シェーダー
	std::shared_ptr<VertexShader> GetVertexShader();
	// ジオメトリー シェーダー
	std::shared_ptr<GeometryShader> GetGeometryShader();
	// ピクセル シェーダー
	std::shared_ptr<PixelShader> GetPixelShader();

private:
	// 頂点シェーダー
	std::shared_ptr<VertexShader> vertexShader;
	// ジオメトリー シェーダー
	std::shared_ptr<GeometryShader> geometryShader;
	// ピクセル シェーダー
	std::shared_ptr<PixelShader> pixelShader;

	// このクラスの新しいインスタンスを初期化します。
	Material(
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<GeometryShader> geometryShader,
		std::shared_ptr<PixelShader> pixelShader);
	// コピーと代入演算を禁止
	Material(const Material&);
	Material& operator=(const Material&) { return *this; }
};

// 頂点バッファーを表します。
class VertexBuffer : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <class T, UINT _Size>
	static std::shared_ptr<VertexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&initialData)[_Size]);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<VertexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	// このクラスのインスタンスを初期化します。
	VertexBuffer(std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer);
	// コピーと代入演算を禁止
	VertexBuffer(const VertexBuffer&);
	VertexBuffer& operator=(const VertexBuffer&) { return *this; }
};

// このクラスのインスタンスを作成します。
template <class T, UINT _Size>
static std::shared_ptr<VertexBuffer> VertexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const T(&initialData)[_Size])
{
	return Create(graphicsDevice, initialData, sizeof(T) * _Size);
}

// インデックス バッファーを表します。
class IndexBuffer : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <UINT _Size>
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint16_t(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	template <UINT _Size>
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint32_t(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	template <class T, UINT _Size>
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&shaderBytecode)[_Size]);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth, DXGI_FORMAT format);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;
	// インデックス バッファー内の要素のフォーマットを取得します。
	DXGI_FORMAT GetFormat() const;
	// インデックス バッファー先頭からのオフセットを取得します。
	UINT GetOffset() const;
	// インデックス バッファー内の要素の数を取得します。
	UINT GetCount() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;
	UINT offset = 0;
	UINT count = 0;
	// このクラスのインスタンスを初期化します。
	IndexBuffer(std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer,
		DXGI_FORMAT format, UINT count);
	// コピーと代入演算を禁止
	IndexBuffer(const IndexBuffer&);
	IndexBuffer& operator=(const IndexBuffer&) { return *this; }
};

// このクラスのインスタンスを作成します。
template <UINT _Size>
static std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint16_t(&initialData)[_Size])
{
	return Create(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R16_UINT);
}

// このクラスのインスタンスを作成します。
template <UINT _Size>
static std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint32_t(&initialData)[_Size])
{
	return Create(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R32_UINT);
}

// このクラスのインスタンスを作成します。
template <class T, UINT _Size>
static std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const T(&initialData)[_Size])
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
		throw DX::com_exception(E_INVALIDARG);
		return nullptr;
	}
	return Create(graphicsDevice, initialData, sizeof(T) * _Size, format);
}

// 定数バッファーを表します。
class ConstantBuffer : public GraphicsResource
{
public:
	// このクラスの新しいインスタンスを作成します。
	static std::shared_ptr<ConstantBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;

	// このクラスの新しいインスタンスを初期化します。
	ConstantBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer);
	// コピーと代入演算を禁止
	ConstantBuffer(const ConstantBuffer&);
	ConstantBuffer& operator=(const ConstantBuffer&) { return *this; }
};

// 入力レイアウトを表します。
class InputLayout : public GraphicsResource
{
public:
	// このクラスのインスタンスを作成します。
	template <UINT _Size>
	static std::shared_ptr<InputLayout> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);
	// このクラスのインスタンスを作成します。
	static std::shared_ptr<InputLayout> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);

	// ネイティブ実装のポインターを取得します。
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer;
	// このクラスのインスタンスを初期化します。
	InputLayout(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer);
	// コピー コンストラクター
	InputLayout(const InputLayout&);
};

// このクラスのインスタンスを作成します。
template <UINT _Size>
static std::shared_ptr<InputLayout> InputLayout::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
{
	return Create(graphicsDevice, shaderBytecode, _Size, shaderBytecodeWithInputSignature);
}

// モデルのメッシュデータを表します。
class Mesh final
{
public:
	// このクラスの新しいインスタンスを作成します。
	template <UINT _NumBuffers, UINT _NumElements>
	static std::shared_ptr<Mesh> Create(
		std::shared_ptr<VertexBuffer>(&vertexBuffers)[_NumBuffers],
		UINT(&strides)[_NumBuffers],
		std::shared_ptr<IndexBuffer> indexBuffer,
		const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_NumElements]);
	// このクラスの新しいインスタンスを作成します。
	static std::shared_ptr<Mesh> Create(
		UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);

	// 頂点バッファーの数を取得します。
	UINT GetNumBuffers() const;
	// このメッシュに含まれるVertexBufferの配列を取得します。
	const std::shared_ptr<VertexBuffer>* GetVertexBuffers() const;
	// このメッシュに含まれるID3D11Bufferの配列を取得します。
	ID3D11Buffer* const* GetNativePointers() const;
	// ID3D11Bufferの配列の各要素について頂点データのバイトサイズを取得します。
	const UINT* GetStrides() const;
	// ID3D11Bufferの配列の各要素について先頭からのオフセットを取得します。
	const UINT* GetOffsets() const;
	// このメッシュに含まれるIndexBufferを取得します。
	std::shared_ptr<const IndexBuffer> GetIndexBuffer() const;
	// プリミティブの種類を取得します。
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
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

	// インスタンス生成を禁止
	Mesh(UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);
	// コピーと代入演算を禁止
	Mesh(const Mesh&) {}
	Mesh& operator=(const Mesh&) { return *this; }
};

// このクラスの新しいインスタンスを作成します。
template <UINT _NumBuffers, UINT _NumElements>
static std::shared_ptr<Mesh> Mesh::Create(
	std::shared_ptr<VertexBuffer>(&vertexBuffers)[_NumBuffers],
	UINT(&strides)[_NumBuffers],
	std::shared_ptr<IndexBuffer> indexBuffer,
	const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_NumElements])
{
	return Create(
		_NumBuffers, vertexBuffers, strides,
		indexBuffer,
		_NumElements, inputElementDescs);
}

// メッシュをレンダリングする機能を表します。
class MeshRenderer final
{
public:
	// このクラスの新しいインスタンスを作成します。
	static std::shared_ptr<MeshRenderer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material);

	// レンダリングに使用するメッシュを取得します。
	std::shared_ptr<Mesh> GetMesh();
	// レンダリングに使用するマテリアルを取得します。
	std::shared_ptr<Material> GetMaterial();

	// このレンダラーによる描画を実行します。
	void Draw(
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext);

private:
	// メッシュ
	std::shared_ptr<Mesh> mesh;
	// マテリアル
	std::shared_ptr<Material> material;
	// 入力レイアウト
	std::shared_ptr<InputLayout> inputLayout;

	// インスタンス生成を禁止
	MeshRenderer(
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material,
		std::shared_ptr<InputLayout> inputLayout);
	// コピーと代入演算を禁止
	MeshRenderer(const MeshRenderer&) {}
	MeshRenderer& operator=(const MeshRenderer&) { return *this; }
};