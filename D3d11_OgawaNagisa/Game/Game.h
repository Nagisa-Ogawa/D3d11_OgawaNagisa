//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#define SAFE_RELEASE(p) if ((p) != nullptr) { (p)->Release(); (p) = nullptr; }

// �A�v���P�[�V�����S�̂�\���܂��B
class Game {
public:
	// �A�v���P�[�V���������������܂��B
	void Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// ���b�Z�[�W���[�v�����s���܂��B
	int Run();

private:
	// �E�B���h�E�̃^�C�g��
	LPCWSTR WindowTitle = L"�^�C�g��";
	// �E�B���h�E�̕�
	int ScreenWidth = 640;
	// �E�B���h�E�̍���
	int ScreenHeight = 480;
	// �E�B���h�E�̃n���h��
	HWND hWnd = NULL;

	// �E�B���h�E���쐬
	bool InitWindow();

	// Direct3D 11�̃f�o�C�X
	ID3D11Device* graphicsDevice = nullptr;
	// Direct3D 11�̃f�o�C�X �R���e�L�X�g
	ID3D11DeviceContext* immediateContext = nullptr;
	// Direct3D 11�̋@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// �쐬����@�\���x���B��]���Ɏw�肷��
	const D3D_FEATURE_LEVEL featureLevels[6] = {
		//D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	// �X���b�v�`�F�[��
	IDXGISwapChain* swapChain = nullptr;
	// �����_�[�^�[�Q�b�g
	ID3D11RenderTargetView* renderTargetViews[1];
	// �o�b�N�o�b�t�@�|���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X�r���[
	ID3D11ShaderResourceView* renderTargetResourceView = nullptr;
	// �[�x�X�e���V���̃t�H�[�}�b�g
	//const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// �[�x�X�e���V��
	ID3D11DepthStencilView* depthStencilView = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�Ŏg�p���邽�߂̃��\�[�X�r���[
	ID3D11ShaderResourceView* depthStencilResourceView = nullptr;
	// ��ʃN���A�[�Ɏg�p����J���[
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f };
	// �r���[�|�[�g
	D3D11_VIEWPORT viewports[1] = {};

	// �O���t�B�b�N�f�o�C�X���쐬���܂�
	bool InitGraphicsDevice();
	// �O���t�B�b�N���\�[�X���J�����܂�
	void ReleaseGraphicsDevice();

};

// �ʒu���W�݂̂𒸓_���Ɏ��f�[�^��\���܂��B
struct VertexPosition
{
	DirectX::XMFLOAT3 position;	// �ʒu���W

	// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
	// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
	static UINT GetInputElementDescsLength();
};

// �ʒu���W�Ɩ@���x�N�g���𒸓_���Ɏ��f�[�^��\���܂��B
struct VertexPositionNormal
{
	DirectX::XMFLOAT3 position;	// �ʒu���W
	DirectX::XMFLOAT3 normal;	// �@���x�N�g��

	// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
	// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
	static UINT GetInputElementDescsLength();
};

// �ʒu���W�Ɩ@���x�N�g���ƃe�N�X�`���[���W�𒸓_���Ɏ��f�[�^��\���܂�
struct VertexPositionNormalTexture
{
	DirectX::XMFLOAT3 position;	// �ʒu���W
	DirectX::XMFLOAT3 normal;	// �@���x�N�g��
	DirectX::XMFLOAT2 texCoord;	// �e�N�X�`���[���W(UV)

	// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
	// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
	static UINT GetInputElementDescsLength();

};

// ���_�V�F�[�_�\��\���܂��B
class BasicVertexShader
{
private:
	// D3D11�̃C���^�[�t�F�[�X
	ID3D11VertexShader* shader = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
	static BasicVertexShader* Create(ID3D11Device* graphicsDevice);
	// ���\�[�X���J�����܂�
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂�
	ID3D11VertexShader* GetNativePointer();
	// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
	const BYTE* GetBytecode();
	// �o�C�g�R�[�h�̃T�C�Y���擾���܂��B
	SIZE_T GetBytecodeLength();
};

// �W�I���g���[�V�F�[�_�[��\���܂��B
class BasicGeometryShader
{
private:
	// D3D11�̃C���^�[�t�F�[�X
	ID3D11GeometryShader* shader = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
	static BasicGeometryShader* Create(ID3D11Device* graphicsDevice);
	// ���\�[�X���J�����܂�
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂�
	ID3D11GeometryShader* GetNativePointer();
};

// �s�N�Z���V�F�[�_�\��\���܂��B
class BasicPixelShader
{
private:
	// D3D11�̃C���^�[�t�F�[�X
	ID3D11PixelShader* shader = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
	static BasicPixelShader* Create(ID3D11Device* graphicsDevice);
	// ���\�[�X���J�����܂�
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂�
	ID3D11PixelShader* GetNativePointer();
};

// ���_�o�b�t�@�[��\���܂��B
class VertexBuffer
{
	// D3D11���\�[�X
	ID3D11Buffer* buffer = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static VertexBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
	// ���\�[�X���J�����܂��B
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
	ID3D11Buffer* GetNativePointer();

	// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
	void SetData(void* data);
};

// �C���f�b�N�X�o�b�t�@�[��\���܂��B
class IndexBuffer
{
	// D3D11���\�[�X
	ID3D11Buffer* buffer = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static IndexBuffer* Create(ID3D11Device* graphicsDevice, UINT indexCount);
	// ���\�[�X��������܂��B
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
	ID3D11Buffer* GetNativePointer();

	// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
	void SetData(UINT32* data);
};

// �萔�o�b�t�@�[��\���܂��B
class ConstantBuffer
{
	// D3D11���\�[�X
	ID3D11Buffer* buffer = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static ConstantBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
	// ���\�[�X��������܂��B
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
	ID3D11Buffer* GetNativePointer();

	// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
	void SetData(void* data);
};

// ���̓��C�A�E�g��\���܂��B
class InputLayout
{
	// D3D11���\�[�X
	ID3D11InputLayout* inputLayout = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static InputLayout* Create(
		ID3D11Device* graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength);
	// ���\�[�X���J�����܂��B
	void Release();
	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
	ID3D11InputLayout* GetNativePointer();
};

class Texture2D
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
public:
	UINT width;
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	static Texture2D* Create(
		ID3D11Device* graphicsDevice,
		UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);
	// ���\�[�X��������܂��B
	void Release();

	// �e�N�X�`���[�̃s�N�Z����ύX���܂��B
	void SetData(const void* data);

	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
	ID3D11Texture2D* GetNativePointer();
	ID3D11SamplerState* GetSamplerState();
	ID3D11ShaderResourceView* GetShaderResourceView();
};

class ReadBitMap
{
	uint32_t* source;
public:
	UINT width;
	UINT height;
	// bmp����f�[�^��ǂݍ��݂܂�
	uint32_t* ReadFromBitMap(char* path);
	// ���\�[�X���J�����܂��B
	void Release();
};