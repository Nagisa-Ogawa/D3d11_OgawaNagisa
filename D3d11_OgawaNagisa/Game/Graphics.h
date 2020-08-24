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
/// �r�f�I�A�_�v�^�[����̏o�́i�ʏ�̓��j�^�[�j��\���܂��B
/// </summary>
class Output final
{
public:
	/// <summary>
	/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	/// </summary>
	explicit Output(Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput);
	/// <summary>
	/// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	/// </summary>
	Microsoft::WRL::ComPtr<IDXGIOutput> GetNativePointer();
	DXGI_OUTPUT_DESC GetDesc() const;
	HMONITOR GetHMonitor() const;
	/// <summary>
	/// ���j�^�[�o�͂̐����ݒ���������܂��B
	/// </summary>
	DXGI_MODE_DESC FindClosestMatchingMode(
		UINT width, UINT height, std::shared_ptr<GraphicsDevice> graphicsDevice) const;

private:
	Microsoft::WRL::ComPtr<IDXGIOutput> nativePointer;

	// �R�s�[�Ƒ�����Z���֎~
	Output(const Output&);
	Output& operator=(const Output&) { return *this; }
};

/// <summary>
/// �r�f�I�A�_�v�^�[��\���܂��B
/// </summary>
class Adapter final
{
public:
	/// <summary>
	/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	/// </summary>
	Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter);
	/// <summary>
	/// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	/// </summary>
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetNativePointer();
	DXGI_ADAPTER_DESC1 GetDesc() const;
	std::shared_ptr<Output> GetOutput(int output);
	std::vector<std::shared_ptr<Output>>& GetOutputs();

private:
	Microsoft::WRL::ComPtr<IDXGIAdapter1> nativePointer;
	DXGI_ADAPTER_DESC1 desc = {};
	std::vector<std::shared_ptr<Output>> outputs;

	// �R�s�[�Ƒ�����Z���֎~
	Adapter(const Adapter&);
	Adapter& operator=(const Adapter&) { return *this; }
};

// �O���t�B�b�N�X�f�o�C�X��\���܂��B
class GraphicsDevice final
{
public:
	// IDXGIFactory1�I�u�W�F�N�g���擾���܂��B
	Microsoft::WRL::ComPtr<IDXGIFactory1> GetDXGIFactory() const;
	// IDXGIAdapter1�I�u�W�F�N�g���擾���܂��B
	Microsoft::WRL::ComPtr<IDXGIAdapter1> GetDXGIAdapter() const;
	// IDXGIDevice1�I�u�W�F�N�g���擾���܂��B
	Microsoft::WRL::ComPtr<IDXGIDevice1> GetDXGIDevice() const;
	// Direct3D 11�̃f�o�C�X���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	// Direct3D 11�̋@�\���x�����擾���܂��B
	D3D_FEATURE_LEVEL GetFeatureLevel() const;
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	GraphicsDevice(std::shared_ptr<Adapter> adapter, UINT creationFlags);

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
	// Direct3D 11�̃f�o�C�X���擾���܂��B
	virtual std::shared_ptr<const GraphicsDevice> GetGraphicsDevice() const final;

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
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	SwapChain(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Output> output);

	// �f�B�X�v���C�Ƀ����_�����O�C���[�W��\�����܂��B
	void Present(UINT syncInterval, UINT flags);
	// �X���b�v�`�F�[�����擾���܂��B
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetNativePointer() const;

private:
	// �X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain> nativePointer;

	// �R�s�[���֎~
	SwapChain(const SwapChain& other) : GraphicsResource(other) {}
	// ������Z���֎~
	SwapChain& operator=(const SwapChain&) { return *this; }
};

// �����_�[�^�[�Q�b�g��\���܂��B
class RenderTarget final : public GraphicsResource
{
public:
	// �o�b�N�o�b�t�@�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer() const;
	// �����_�[�^�[�Q�b�g���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetView() const;
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	RenderTarget(std::shared_ptr<SwapChain> swapChain);

private:
	// �o�b�N�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// �����_�[�^�[�Q�b�g
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view;
	// �o�b�N�o�b�t�@�[���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// �R�s�[�Ƒ�����Z���֎~
	RenderTarget(const RenderTarget& other) : GraphicsResource(other) {}
	RenderTarget& operator=(const RenderTarget&) { return *this; }
};

// �[�x�X�e���V����\���܂��B
class DepthStencil final : public GraphicsResource
{
public:
	// �[�x�X�e���V���o�b�t�@�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Texture2D> GetBuffer() const;
	// �[�x�X�e���V�� �r���[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetView() const;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResourceView() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	DepthStencil(
		std::shared_ptr<RenderTarget> renderTarget, DXGI_FORMAT format);

private:
	// �o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
	// �[�x�X�e���V��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X �r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resourceView;

	// �R�s�[�Ƒ�����Z���֎~
	DepthStencil(const DepthStencil& other) : GraphicsResource(other) {}
	DepthStencil& operator=(const DepthStencil&) { return *this; }
};

// �O���t�B�b�N�X�������ɂ��Ă̋L�q��\���܂��B
struct GraphicsSettings
{
	UINT creationFlags = 0;
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
};

// �O���t�B�b�N�X�@�\��\���܂��B
class Graphics final
{
public:
	// �`��f�o�C�X���擾���܂��B
	std::shared_ptr<GraphicsDevice> GetGraphicsDevice();
	// �`��f�o�C�X���擾���܂��B
	std::shared_ptr<const GraphicsDevice> GetGraphicsDevice() const;
	// �X���b�v�`�F�[�����擾���܂��B
	std::shared_ptr<SwapChain> GetSwapChain();
	// �X���b�v�`�F�[�����擾���܂��B
	std::shared_ptr<const SwapChain> GetSwapChain() const;
	// �����_�[�^�[�Q�b�g���擾���܂��B
	std::shared_ptr<RenderTarget> GetRenderTarget();
	// �����_�[�^�[�Q�b�g���擾���܂��B
	std::shared_ptr<const RenderTarget> GetRenderTarget() const;
	// �[�x�X�e���V�����擾���܂��B
	std::shared_ptr<DepthStencil> GetDepthStencil();
	// �[�x�X�e���V�����擾���܂��B
	std::shared_ptr<const DepthStencil> GetDepthStencil() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Graphics(
		const GraphicsSettings& settings, std::shared_ptr<GameWindow> window);

private:
	// IDXGIFactory1�C���^�[�t�F�C�X
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	// �g�p�\�Ȃ��ׂẴr�f�I�A�_�v�^�[
	std::vector<std::shared_ptr<Adapter>> adapters;
	// �r�f�I�A�_�v�^�[
	std::shared_ptr<Adapter> adapter;
	// �r�f�I�A�_�v�^�[�o��
	std::shared_ptr<Output> output;
	// �`��f�o�C�X
	std::shared_ptr<GraphicsDevice> graphicsDevice;
	// �X���b�v�`�F�[��
	std::shared_ptr<SwapChain> swapChain;
	// �����_�[�^�[�Q�b�g
	std::shared_ptr<RenderTarget> renderTarget;
	// �[�x�X�e���V��
	std::shared_ptr<DepthStencil> depthStencil;

	// �R�s�[�Ƒ�����Z���֎~
	Graphics(const Graphics&) {}
	Graphics& operator=(const Graphics&) { return *this; }
};

// ���_�V�F�[�_�[��\���܂��B
class VertexShader : public GraphicsResource
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <SIZE_T _Size>
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	VertexShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetNativePointer() const;
	// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBytecode() const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> nativePointer;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecode;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);
	// �R�s�[�Ƒ�����Z���֎~
	VertexShader(const VertexShader&);
	VertexShader& operator=(const VertexShader&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
template <SIZE_T _Size>
VertexShader::VertexShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, _Size);
}

// �W�I���g���[ �V�F�[�_�[��\���܂��B
class GeometryShader : public GraphicsResource
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <SIZE_T _Size>
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	GeometryShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetNativePointer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> nativePointer;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);
	// �R�s�[�Ƒ�����Z���֎~
	GeometryShader(const GeometryShader&);
	GeometryShader& operator=(const GeometryShader&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
template <SIZE_T _Size>
GeometryShader::GeometryShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, _Size);
}

// �s�N�Z�� �V�F�[�_�[��\���܂��B
class PixelShader : public GraphicsResource
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <SIZE_T _Size>
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const BYTE(&shaderBytecode)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	PixelShader(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);

	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetNativePointer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> nativePointer;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		LPCWSTR fileName, LPCSTR entryPoint, LPCSTR target);
	// �R�s�[�Ƒ�����Z���֎~
	PixelShader(const PixelShader&);
	PixelShader& operator=(const PixelShader&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
template <SIZE_T _Size>
PixelShader::PixelShader(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const BYTE(&shaderBytecode)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, shaderBytecode, _Size);
}

// �}�e���A����\���܂��B
class Material
{
public:
	// ���_�V�F�[�_�[���擾���܂��B
	std::shared_ptr<VertexShader> GetVertexShader();
	// ���_�V�F�[�_�[���擾���܂��B
	std::shared_ptr<const VertexShader> GetVertexShader() const;
	// �W�I���g���[ �V�F�[�_�[���擾���܂��B
	std::shared_ptr<GeometryShader> GetGeometryShader();
	// �W�I���g���[ �V�F�[�_�[���擾���܂��B
	std::shared_ptr<const GeometryShader> GetGeometryShader() const;
	// �s�N�Z�� �V�F�[�_�[���擾���܂��B
	std::shared_ptr<PixelShader> GetPixelShader();
	// �s�N�Z�� �V�F�[�_�[���擾���܂��B
	std::shared_ptr<const PixelShader> GetPixelShader() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Material(
		std::shared_ptr<VertexShader> vertexShader,
		std::shared_ptr<GeometryShader> geometryShader,
		std::shared_ptr<PixelShader> pixelShader);

private:
	// ���_�V�F�[�_�[
	std::shared_ptr<VertexShader> vertexShader;
	// �W�I���g���[ �V�F�[�_�[
	std::shared_ptr<GeometryShader> geometryShader;
	// �s�N�Z�� �V�F�[�_�[
	std::shared_ptr<PixelShader> pixelShader;

	// �R�s�[�Ƒ�����Z���֎~
	Material(const Material&);
	Material& operator=(const Material&) { return *this; }
};

// ���_�o�b�t�@�[��\���܂��B
class VertexBuffer : public GraphicsResource
{
public:
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <class T, UINT _Size>
	VertexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&initialData)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	VertexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth);
	// �R�s�[�Ƒ�����Z���֎~
	VertexBuffer(const VertexBuffer&);
	VertexBuffer& operator=(const VertexBuffer&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
template <class T, UINT _Size>
VertexBuffer::VertexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const T(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, sizeof(T) * _Size);
}

// �C���f�b�N�X �o�b�t�@�[��\���܂��B
class IndexBuffer : public GraphicsResource
{
public:
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;
	// �C���f�b�N�X �o�b�t�@�[���̗v�f�̃t�H�[�}�b�g���擾���܂��B
	DXGI_FORMAT GetFormat() const;
	// �C���f�b�N�X �o�b�t�@�[�擪����̃I�t�Z�b�g���擾���܂��B
	UINT GetOffset() const;
	// �C���f�b�N�X �o�b�t�@�[���̗v�f�̐����擾���܂��B
	UINT GetCount() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <UINT _Size>
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint16_t(&shaderBytecode)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <UINT _Size>
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const uint32_t(&shaderBytecode)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <class T, UINT _Size>
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const T(&shaderBytecode)[_Size]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	IndexBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth, DXGI_FORMAT format);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;
	DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;
	UINT offset = 0;
	UINT count = 0;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const void* initialData, UINT byteWidth, DXGI_FORMAT format);
	// �R�s�[�Ƒ�����Z���֎~
	IndexBuffer(const IndexBuffer&);
	IndexBuffer& operator=(const IndexBuffer&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
template <UINT _Size>
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint16_t(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R16_UINT);
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
template <UINT _Size>
IndexBuffer::IndexBuffer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const uint32_t(&initialData)[_Size])
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(graphicsDevice, initialData, sizeof(uint32_t) * _Size, DXGI_FORMAT_R32_UINT);
}

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
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

// �萔�o�b�t�@�[��\���܂��B
class ConstantBuffer : public GraphicsResource
{
public:
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetNativePointer() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	ConstantBuffer(
		std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativePointer;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice, UINT byteWidth);
	// �R�s�[�Ƒ�����Z���֎~
	ConstantBuffer(const ConstantBuffer&);
	ConstantBuffer& operator=(const ConstantBuffer&) { return *this; }
};

// ���̓��C�A�E�g��\���܂��B
class InputLayout : public GraphicsResource
{
public:
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetNativePointer() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <UINT _Size>
	InputLayout(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_Size],
		const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	InputLayout(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature);
	// �R�s�[ �R���X�g���N�^�[
	InputLayout(const InputLayout&);
};

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
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
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
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

	// �R�s�[�Ƒ�����Z���֎~
	Image(const Image&);
	Image& operator=(const Image&) { return *this; }
};

// 2D �̃e�N�X�`���[��\���܂��B
class Texture2D : public GraphicsResource
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Texture2D(std::shared_ptr<GraphicsDevice> graphicsDevice, LPCWSTR fileName);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSamplerState() const;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	// �R�s�[�Ƒ�����Z���֎~
	Texture2D(const Texture2D&);
	Texture2D& operator=(const Texture2D&) { return *this; }
};

// ���f���̃��b�V���f�[�^��\���܂��B
class Mesh final
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	template <UINT _NumBuffers, UINT _NumElements>
	Mesh(
		std::shared_ptr<VertexBuffer>(&vertexBuffers)[_NumBuffers],
		const UINT(&strides)[_NumBuffers],
		std::shared_ptr<IndexBuffer> indexBuffer,
		const D3D11_INPUT_ELEMENT_DESC(&inputElementDescs)[_NumElements]);
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Mesh(
		UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, const UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);

	// ���_�o�b�t�@�[�̐����擾���܂��B
	UINT GetNumBuffers() const;

	// ���̃��b�V���Ɋ܂܂��VertexBuffer�̔z����擾���܂��B
	const std::shared_ptr<VertexBuffer>* GetVertexBuffers();
	// ���̃��b�V���Ɋ܂܂��VertexBuffer�̔z����擾���܂��B
	const std::shared_ptr<VertexBuffer>* GetVertexBuffers() const;

	// ���̃��b�V���Ɋ܂܂��ID3D11Buffer�̔z����擾���܂��B
	ID3D11Buffer* const* GetNativePointers();
	// ���̃��b�V���Ɋ܂܂��ID3D11Buffer�̔z����擾���܂��B
	const ID3D11Buffer* const* GetNativePointers() const;

	// ID3D11Buffer�̔z��̊e�v�f�ɂ��Ē��_�f�[�^�̃o�C�g�T�C�Y���擾���܂��B
	const UINT* GetStrides() const;
	// ID3D11Buffer�̔z��̊e�v�f�ɂ��Đ擪����̃I�t�Z�b�g���擾���܂��B
	const UINT* GetOffsets() const;

	// ���̃��b�V���Ɋ܂܂��IndexBuffer���擾���܂��B
	std::shared_ptr<IndexBuffer> GetIndexBuffer();
	// ���̃��b�V���Ɋ܂܂��IndexBuffer���擾���܂��B
	std::shared_ptr<const IndexBuffer> GetIndexBuffer() const;

	// �v���~�e�B�u�̎�ނ��擾���܂��B
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
	// ���͗v�f�̐����擾���܂��B
	UINT GetNumElements() const;
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

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		UINT numBuffers,
		std::shared_ptr<VertexBuffer>* vertexBuffers, const UINT* strides,
		std::shared_ptr<IndexBuffer> indexBuffer,
		UINT numElements,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs);
	// �R�s�[�Ƒ�����Z���֎~
	Mesh(const Mesh&) {}
	Mesh& operator=(const Mesh&) { return *this; }
};

// ���̃N���X�̐V�����C���X�^���X�����������܂��B
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

// ���b�V���������_�����O����@�\��\���܂��B
class MeshRenderer final
{
public:
	// �����_�����O�Ɏg�p���郁�b�V�����擾���܂��B
	std::shared_ptr<Mesh> GetMesh();
	// �����_�����O�Ɏg�p���郁�b�V�����擾���܂��B
	std::shared_ptr<const Mesh> GetMesh() const;
	// �����_�����O�Ɏg�p����}�e���A�����擾���܂��B
	std::shared_ptr<Material> GetMaterial();
	// �����_�����O�Ɏg�p����}�e���A�����擾���܂��B
	std::shared_ptr<const Material> GetMaterial() const;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	MeshRenderer(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material,
		std::shared_ptr<Texture2D> texture2D);

	// ���̃����_���[�ɂ��`������s���܂��B
	void Draw(
		ID3D11DeviceContext* deviceContext);

private:
	// ���b�V��
	std::shared_ptr<Mesh> mesh;
	// �}�e���A��
	std::shared_ptr<Material> material;
	// ���̓��C�A�E�g
	std::shared_ptr<InputLayout> inputLayout;
	// �e�N�X�`���[
	std::shared_ptr<Texture2D> texture2D;

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(
		std::shared_ptr<GraphicsDevice> graphicsDevice,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material,
		std::shared_ptr<Texture2D> texture2D);
	// �R�s�[�Ƒ�����Z���֎~
	MeshRenderer(const MeshRenderer&);
	MeshRenderer& operator=(const MeshRenderer&) { return *this; }
};