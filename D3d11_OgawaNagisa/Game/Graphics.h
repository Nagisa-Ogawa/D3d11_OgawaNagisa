//=============================================================================
// Graphics.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include "GameWindow.h"

// �O���t�B�b�N�X�f�o�C�X��\���܂��B
class GraphicsDevice final
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<GraphicsDevice> Create();

	// IDXGIFactory1�I�u�W�F�N�g���擾���܂��B
	Microsoft::WRL::ComPtr<IDXGIFactory1> GetDXGIFactory();
	// IDXGIAdapter1�I�u�W�F�N�g���擾���܂��B
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetDXGIAdapter();
	// IDXGIDevice1�I�u�W�F�N�g���擾���܂��B
	Microsoft::WRL::ComPtr<IDXGIDevice1> GetDXGIDevice();
	// Direct3D 11�̃f�o�C�X���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	// Direct3D 11�̋@�\���x�����擾���܂��B
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext();

private:
	// IDXGIFactory1�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	// IDXGIAdapter1�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
	// IDXGIDevice1�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
	// ID3D11Device�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	// Direct3D 11�̋@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;

	// �C���X�^���X�������֎~
	GraphicsDevice(
		Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory,
		Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter,
		Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		D3D_FEATURE_LEVEL featureLevel,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext);
	// �R�s�[�Ƒ�����Z���֎~
	GraphicsDevice(const GraphicsDevice&) {}
	GraphicsDevice& operator=(const GraphicsDevice&) { return *this; }
};

// �O���t�B�b�N�X ���\�[�X�̊�{�N���X��\���܂��B
class GraphicsResource
{
public:
	// Direct3D 11�̃f�o�C�X���擾���܂��B
	virtual std::shared_ptr<GraphicsDevice> GetGraphicsDevice() final;

protected:
	// �C���X�^���X�������֎~
	GraphicsResource(std::shared_ptr<GraphicsDevice> graphicsDevice);
	// �R�s�[���֎~
	GraphicsResource(const GraphicsResource&) {}
private:
	// ������Z���֎~
	GraphicsResource& operator=(const GraphicsResource&) { return *this; }

	// ���̃��\�[�X���쐬����ID3D11Device�I�u�W�F�N�g
	std::shared_ptr<GraphicsDevice> graphicsDevice;
};

// �X���b�v�`�F�[����\���܂��B
class SwapChain : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<SwapChain> Create(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		DXGI_FORMAT format, const DXGI_SAMPLE_DESC& sampleDesc);

	// �f�B�X�v���C�Ƀ����_�����O�C���[�W��\�����܂��B
	void Present(UINT syncInterval, UINT flags);
	// �X���b�v�`�F�[�����擾���܂��B
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetNativePointer();

private:
	// �X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain> nativePointer;

	// �C���X�^���X�������֎~
	SwapChain(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain);
	// �R�s�[���֎~
	SwapChain(const SwapChain& other) : GraphicsResource(other) {}
	// ������Z���֎~
	SwapChain& operator=(const SwapChain&) { return *this; }
};

// �����_�[�^�[�Q�b�g��\���܂��B
class RenderTarget final : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<RenderTarget> Create(
		std::shared_ptr<SwapChain> swapChain);

	// �o�b�N�o�b�t�@�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetView();
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView();

private:
	// �o�b�N�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// �����_�[�^�[�Q�b�g
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view;
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// �C���X�^���X�������֎~
	RenderTarget(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer,
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView);
	// �R�s�[�Ƒ�����Z���֎~
	RenderTarget(const RenderTarget& other) : GraphicsResource(other) {}
	RenderTarget& operator=(const RenderTarget&) { return *this; }
};

// �[�x�X�e���V����\���܂��B
class DepthStencil final : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<DepthStencil> Create(
		std::shared_ptr<RenderTarget> renderTarget, DXGI_FORMAT format);

	// �[�x�X�e���V���o�b�t�@�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetView();
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView();

private:
	// �o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// �[�x�X�e���V��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// �C���X�^���X�������֎~
	DepthStencil(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer,
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView);
	// �R�s�[�Ƒ�����Z���֎~
	DepthStencil(const DepthStencil& other) : GraphicsResource(other) {}
	DepthStencil& operator=(const DepthStencil&) { return *this; }
};

// �O���t�B�b�N�X�@�\��\���܂��B
class Graphics final
{
public:
	// �O���t�B�b�N�X�@�\�����������܂��B
	static std::shared_ptr<Graphics> Create(
		std::shared_ptr<GameWindow> gameWindow,
		DXGI_FORMAT backBufferFormt, DXGI_FORMAT depthStencilFormt);

	// �O���t�B�b�N�X �f�o�C�X���擾���܂��B
	std::shared_ptr<GraphicsDevice> GetGraphicsDevice();
	// �X���b�v�`�F�[�����擾���܂��B
	std::shared_ptr<SwapChain> GetSwapChain();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	std::shared_ptr<RenderTarget> GetRenderTarget();
	// �[�x�X�e���V�����擾���܂��B
	std::shared_ptr<DepthStencil> GetDepthStencil();

private:
	// �O���t�B�b�N�X �f�o�C�X
	std::shared_ptr<GraphicsDevice> graphicsDevice;
	// �X���b�v�`�F�[��
	std::shared_ptr<SwapChain> swapChain;
	// �����_�[�^�[�Q�b�g
	std::shared_ptr<RenderTarget> renderTarget;
	// �[�x�X�e���V��
	std::shared_ptr<DepthStencil> depthStencil;

	// �C���X�^���X�������֎~
	Graphics(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<SwapChain> swapChain,
		std::shared_ptr<RenderTarget> renderTarget,
		std::shared_ptr<DepthStencil> depthStencil);
	// �R�s�[�Ƒ�����Z���֎~
	Graphics(const Graphics&) {}
	Graphics& operator=(const Graphics&) { return *this; }
};

// ���_�V�F�[�_�[��\���܂��B
class VertexShader : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static std::shared_ptr<VertexShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<VertexShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<VertexShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetNativePointer();
	// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBytecode();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer,
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode);
	// �R�s�[ �R���X�g���N�^�[
	VertexShader(const VertexShader&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static std::shared_ptr<VertexShader> VertexShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}

// �W�I���g���[ �V�F�[�_�[��\���܂��B
class GeometryShader : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static std::shared_ptr<GeometryShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<GeometryShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<GeometryShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	GeometryShader(const GeometryShader&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static std::shared_ptr<GeometryShader> GeometryShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}

// �s�N�Z�� �V�F�[�_�[��\���܂��B
class PixelShader : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <SIZE_T _Size>
	static std::shared_ptr<PixelShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<PixelShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<PixelShader> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> nativePointer;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	PixelShader(const PixelShader&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <SIZE_T _Size>
static std::shared_ptr<PixelShader> PixelShader::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
{
	return 	Create(graphicsDevice, shaderBytecode, _Size);
}

// �}�e���A����\���܂��B
class Material
{
public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static std::shared_ptr<Material> Create(
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<GeometryShader> geometryShader,
		std::shared_ptr<PixelShader> pixelShader);

	// ���_�V�F�[�_�[
	std::shared_ptr<VertexShader> GetVertexShader();
	// �W�I���g���[ �V�F�[�_�[
	std::shared_ptr<GeometryShader> GetGeometryShader();
	// �s�N�Z�� �V�F�[�_�[
	std::shared_ptr<PixelShader> GetPixelShader();

private:
	// ���_�V�F�[�_�[
	std::shared_ptr<VertexShader> vertexShader;
	// �W�I���g���[ �V�F�[�_�[
	std::shared_ptr<GeometryShader> geometryShader;
	// �s�N�Z�� �V�F�[�_�[
	std::shared_ptr<PixelShader> pixelShader;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Material(
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<GeometryShader> geometryShader,
		std::shared_ptr<PixelShader> pixelShader);
	// �R�s�[�Ƒ�����Z���֎~
	Material(const Material&);
	Material& operator=(const Material&) { return *this; }
};

// ���_�o�b�t�@�[��\���܂��B
class VertexBuffer : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <class T, UINT _Size>
	static std::shared_ptr<VertexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&initialData)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<VertexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	VertexBuffer(std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer);
	// �R�s�[�Ƒ�����Z���֎~
	VertexBuffer(const VertexBuffer&);
	VertexBuffer& operator=(const VertexBuffer&) { return *this; }
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <class T, UINT _Size>
static std::shared_ptr<VertexBuffer> VertexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const T(&initialData)[_Size])
{
	return Create(graphicsDevice, initialData, sizeof(T) * _Size);
}

// �C���f�b�N�X �o�b�t�@�[��\���܂��B
class IndexBuffer : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <UINT _Size>
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint16_t(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <UINT _Size>
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint32_t(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <class T, UINT _Size>
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<IndexBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth, DXGI_FORMAT format);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;
	// �C���f�b�N�X �o�b�t�@�[���̗v�f�̃t�H�[�}�b�g���擾���܂��B
	DXGI_FORMAT GetFormat() const;
	// �C���f�b�N�X �o�b�t�@�[�擪����̃I�t�Z�b�g���擾���܂��B
	UINT GetOffset() const;
	// �C���f�b�N�X �o�b�t�@�[���̗v�f�̐����擾���܂��B
	UINT GetCount() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;
	UINT offset = 0;
	UINT count = 0;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	IndexBuffer(std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer,
		DXGI_FORMAT format, UINT count);
	// �R�s�[�Ƒ�����Z���֎~
	IndexBuffer(const IndexBuffer&);
	IndexBuffer& operator=(const IndexBuffer&) { return *this; }
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <UINT _Size>
static std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint16_t(&initialData)[_Size])
{
	return Create(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R16_UINT);
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <UINT _Size>
static std::shared_ptr<IndexBuffer> IndexBuffer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint32_t(&initialData)[_Size])
{
	return Create(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R32_UINT);
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
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

// �萔�o�b�t�@�[��\���܂��B
class ConstantBuffer : public GraphicsResource
{
public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static std::shared_ptr<ConstantBuffer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	ConstantBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer);
	// �R�s�[�Ƒ�����Z���֎~
	ConstantBuffer(const ConstantBuffer&);
	ConstantBuffer& operator=(const ConstantBuffer&) { return *this; }
};

// ���̓��C�A�E�g��\���܂��B
class InputLayout : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <UINT _Size>
	static std::shared_ptr<InputLayout> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static std::shared_ptr<InputLayout> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetNativePointer();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	InputLayout(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	InputLayout(const InputLayout&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <UINT _Size>
static std::shared_ptr<InputLayout> InputLayout::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
{
	return Create(graphicsDevice, shaderBytecode, _Size, shaderBytecodeWithInputSignature);
}

// ���f���̃��b�V���f�[�^��\���܂��B
class Mesh final
{
public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	template <UINT _NumBuffers, UINT _NumElements>
	static std::shared_ptr<Mesh> Create(
		std::shared_ptr<VertexBuffer>(&vertexBuffers)[_NumBuffers],
		UINT(&strides)[_NumBuffers],
		std::shared_ptr<IndexBuffer> indexBuffer,
		const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_NumElements]);
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static std::shared_ptr<Mesh> Create(
		UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);

	// ���_�o�b�t�@�[�̐����擾���܂��B
	UINT GetNumBuffers() const;
	// ���̃��b�V���Ɋ܂܂��VertexBuffer�̔z����擾���܂��B
	const std::shared_ptr<VertexBuffer>* GetVertexBuffers() const;
	// ���̃��b�V���Ɋ܂܂��ID3D11Buffer�̔z����擾���܂��B
	ID3D11Buffer* const* GetNativePointers() const;
	// ID3D11Buffer�̔z��̊e�v�f�ɂ��Ē��_�f�[�^�̃o�C�g�T�C�Y���擾���܂��B
	const UINT* GetStrides() const;
	// ID3D11Buffer�̔z��̊e�v�f�ɂ��Đ擪����̃I�t�Z�b�g���擾���܂��B
	const UINT* GetOffsets() const;
	// ���̃��b�V���Ɋ܂܂��IndexBuffer���擾���܂��B
	std::shared_ptr<const IndexBuffer> GetIndexBuffer() const;
	// �v���~�e�B�u�̎�ނ��擾���܂��B
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
	// ���_�f�[�^�ɂ��ē��͗v�f���擾���܂��B
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

	// �C���X�^���X�������֎~
	Mesh(UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);
	// �R�s�[�Ƒ�����Z���֎~
	Mesh(const Mesh&) {}
	Mesh& operator=(const Mesh&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
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

// ���b�V���������_�����O����@�\��\���܂��B
class MeshRenderer final
{
public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static std::shared_ptr<MeshRenderer> Create(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material);

	// �����_�����O�Ɏg�p���郁�b�V�����擾���܂��B
	std::shared_ptr<Mesh> GetMesh();
	// �����_�����O�Ɏg�p����}�e���A�����擾���܂��B
	std::shared_ptr<Material> GetMaterial();

	// ���̃����_���[�ɂ��`������s���܂��B
	void Draw(
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext);

private:
	// ���b�V��
	std::shared_ptr<Mesh> mesh;
	// �}�e���A��
	std::shared_ptr<Material> material;
	// ���̓��C�A�E�g
	std::shared_ptr<InputLayout> inputLayout;

	// �C���X�^���X�������֎~
	MeshRenderer(
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material,
		std::shared_ptr<InputLayout> inputLayout);
	// �R�s�[�Ƒ�����Z���֎~
	MeshRenderer(const MeshRenderer&) {}
	MeshRenderer& operator=(const MeshRenderer&) { return *this; }
};