//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "InputLayout.h"
#include "Game.h"
#include "DirectXHelper.h"

// ���̃N���X�̃C���X�^���X���쐬���܂��B
InputLayout* InputLayout::Create(
	GraphicsDevice* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	ID3DBlob* shaderBytecodeWithInputSignature)
{
	InputLayout* retVal = nullptr;
	ID3D11InputLayout* nativePointer = nullptr;
	try {
		DX::ThrowIfFailed(
			graphicsDevice->GetDevice()->CreateInputLayout(
				inputElementDescs, numElements,
				shaderBytecodeWithInputSignature->GetBufferPointer(),
				shaderBytecodeWithInputSignature->GetBufferSize(),
				&nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new InputLayout(graphicsDevice, nativePointer);
		SafeRelease(nativePointer);

		return retVal;
	}
	catch (...) {
		SafeRelease(nativePointer);
		SafeDelete(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
InputLayout::InputLayout(
	GraphicsDevice* graphicsDevice, ID3D11InputLayout* nativePointer)
	: GraphicsResource(graphicsDevice)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// �f�X�g���N�^�[
InputLayout::~InputLayout()
{
	SafeRelease(nativePointer);
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return nativePointer;
}
