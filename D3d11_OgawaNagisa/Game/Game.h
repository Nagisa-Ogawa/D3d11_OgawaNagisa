//=============================================================================
// Game.h
// 
//=============================================================================
#pragma once

// CRT�ɂ�郁�����[���[�N���o
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC	// ���������[�N���Ƀ��������[�N���N�������t�@�C������s�Ȃǂ�������
#include <stdlib.h>
#include <crtdbg.h>		// Debug�p�̒ǐՉ\��_malloc_dbg��_free_dbg���g����
// new ���Z�q�̍Ē�`
#ifndef DBG_NEW
// new���Z�q���u���b�N�̎�ށA�t�@�C���A�s�ԍ���\������f�o�b�O�p�ɒu������
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "Graphics.h"

// �A�v���P�[�V�����S�̂�\���܂��B
class Game {
public:
	// �A�v���P�[�V���������������܂��B
	static void Initialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// ���b�Z�[�W���[�v�����s���܂��B
	static int Run();
	// �A�v���P�[�V�������I�����܂��B
	static void Quit();

private:
	// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
	static Game& GetInstance();

	// ���łɏ������ς݂̏ꍇ��true�A����ȊO��false
	bool isInitialized = false;
	// ���C�� �E�B���h�E
	GameWindow* window = nullptr;
	// �O���t�B�b�N�X�@�\
	Graphics* graphics = nullptr;


	DirectX::XMVECTORF32 clearColor = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// �r���[�|�[�g
	D3D11_VIEWPORT viewport = {};

	// �������̍ۂɌĂяo����܂��B
	void OnInitialize(LPCWSTR windowTitle, int screenWidth, int screenHeight);
	// ���b�Z�[�W���[�v�����s����ۂɌĂяo����܂��B
	int OnRun();

};

//// �ʒu���W�݂̂𒸓_���Ɏ��f�[�^��\���܂��B
//struct VertexPosition
//{
//	DirectX::XMFLOAT3 position;	// �ʒu���W
//
//	// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
//	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
//	// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
//	static UINT GetInputElementDescsLength();
//};
//
//// �ʒu���W�Ɩ@���x�N�g���𒸓_���Ɏ��f�[�^��\���܂��B
//struct VertexPositionNormal
//{
//	DirectX::XMFLOAT3 position;	// �ʒu���W
//	DirectX::XMFLOAT3 normal;	// �@���x�N�g��
//
//	// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
//	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
//	// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
//	static UINT GetInputElementDescsLength();
//};
//
//// �ʒu���W�Ɩ@���x�N�g���ƃe�N�X�`���[���W�𒸓_���Ɏ��f�[�^��\���܂�
//struct VertexPositionNormalTexture
//{
//	DirectX::XMFLOAT3 position;	// �ʒu���W
//	DirectX::XMFLOAT3 normal;	// �@���x�N�g��
//	DirectX::XMFLOAT2 texCoord;	// �e�N�X�`���[���W(UV)
//
//	// ���̒��_����D3D11_INPUT_ELEMENT_DESC�ŕ\�����z����擾���܂��B
//	static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDescs();
//	// GetInputElementDescs()�֐��Ŏ擾�����z��̗v�f�����擾���܂��B
//	static UINT GetInputElementDescsLength();
//
//};
//
//// ���_�V�F�[�_�\��\���܂��B
//class BasicVertexShader
//{
//private:
//	// D3D11�̃C���^�[�t�F�[�X
//	ID3D11VertexShader* shader = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
//	static BasicVertexShader* Create(ID3D11Device* graphicsDevice);
//	// ���\�[�X���J�����܂�
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂�
//	ID3D11VertexShader* GetNativePointer();
//	// ���̃V�F�[�_�[�̃o�C�g�R�[�h���擾���܂��B
//	const BYTE* GetBytecode();
//	// �o�C�g�R�[�h�̃T�C�Y���擾���܂��B
//	SIZE_T GetBytecodeLength();
//};
//
//// �W�I���g���[�V�F�[�_�[��\���܂��B
//class BasicGeometryShader
//{
//private:
//	// D3D11�̃C���^�[�t�F�[�X
//	ID3D11GeometryShader* shader = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
//	static BasicGeometryShader* Create(ID3D11Device* graphicsDevice);
//	// ���\�[�X���J�����܂�
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂�
//	ID3D11GeometryShader* GetNativePointer();
//};
//
//// �s�N�Z���V�F�[�_�\��\���܂��B
//class BasicPixelShader
//{
//private:
//	// D3D11�̃C���^�[�t�F�[�X
//	ID3D11PixelShader* shader = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
//	static BasicPixelShader* Create(ID3D11Device* graphicsDevice);
//	// ���\�[�X���J�����܂�
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂�
//	ID3D11PixelShader* GetNativePointer();
//};
//
//// ���_�o�b�t�@�[��\���܂��B
//class VertexBuffer
//{
//	// D3D11���\�[�X
//	ID3D11Buffer* buffer = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
//	static VertexBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
//	// ���\�[�X���J�����܂��B
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
//	ID3D11Buffer* GetNativePointer();
//
//	// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
//	void SetData(void* data);
//};
//
//// �C���f�b�N�X�o�b�t�@�[��\���܂��B
//class IndexBuffer
//{
//	// D3D11���\�[�X
//	ID3D11Buffer* buffer = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
//	static IndexBuffer* Create(ID3D11Device* graphicsDevice, UINT indexCount);
//	// ���\�[�X��������܂��B
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
//	ID3D11Buffer* GetNativePointer();
//
//	// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
//	void SetData(UINT32* data);
//};
//
//// �萔�o�b�t�@�[��\���܂��B
//class ConstantBuffer
//{
//	// D3D11���\�[�X
//	ID3D11Buffer* buffer = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
//	static ConstantBuffer* Create(ID3D11Device* graphicsDevice, UINT byteWidth);
//	// ���\�[�X��������܂��B
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
//	ID3D11Buffer* GetNativePointer();
//
//	// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
//	void SetData(void* data);
//};
//
//// ���̓��C�A�E�g��\���܂��B
//class InputLayout
//{
//	// D3D11���\�[�X
//	ID3D11InputLayout* inputLayout = nullptr;
//
//public:
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
//	static InputLayout* Create(
//		ID3D11Device* graphicsDevice,
//		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
//		const void* shaderBytecodeWithInputSignature, SIZE_T bytecodeLength);
//	// ���\�[�X���J�����܂��B
//	void Release();
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
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
//	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
//	static Texture2D* Create(
//		ID3D11Device* graphicsDevice,
//		UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);
//	// ���\�[�X��������܂��B
//	void Release();
//
//	// �e�N�X�`���[�̃s�N�Z����ύX���܂��B
//	void SetData(const void* data);
//
//	// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
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
//	// bmp����f�[�^��ǂݍ��݂܂�
//	uint32_t* ReadFromBitMap(char* path);
//	// ���\�[�X���J�����܂��B
//	void Release();
//};