//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "InputLayout.h"
#include "Game.h"
#include "DirectXHelper.h"

// ���̃N���X�̃C���X�^���X���쐬���܂��B
InputLayout* InputLayout::Create(
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	ID3DBlob* shaderBytecodeWithInputSignature)
{
	InputLayout* retVal = nullptr;
	ID3D11InputLayout* nativePointer = nullptr;
	try {
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateInputLayout(
				inputElementDescs, numElements,
				shaderBytecodeWithInputSignature->GetBufferPointer(),
				shaderBytecodeWithInputSignature->GetBufferSize(),
				&nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new InputLayout(nativePointer);
		SAFE_RELEASE(nativePointer);

		return retVal;
	}
	catch (...) {
		SAFE_RELEASE(nativePointer);
		SAFE_DELETE(retVal);
		throw;
	}
	return nullptr;
}

// ���̃N���X�̃C���X�^���X�����������܂��B
InputLayout::InputLayout(ID3D11InputLayout* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// �f�X�g���N�^�[
InputLayout::~InputLayout()
{
	SAFE_RELEASE(nativePointer);
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return nativePointer;
}
