//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DX;

// ���̃N���X�̃C���X�^���X���쐬���܂��B
std::shared_ptr<InputLayout> InputLayout::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
{
	ComPtr<ID3D11InputLayout> nativePointer;
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateInputLayout(
			inputElementDescs, numElements,
			shaderBytecodeWithInputSignature->GetBufferPointer(),
			shaderBytecodeWithInputSignature->GetBufferSize(),
			&nativePointer));
	// ���̃N���X�̃C���X�^���X��������
	return shared_ptr<InputLayout>(
		new InputLayout(graphicsDevice, nativePointer));
}

// ���̃N���X�̃C���X�^���X�����������܂��B
InputLayout::InputLayout(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	Microsoft::WRL::ComPtr<ID3D11InputLayout> nativePointer)
	: GraphicsResource(graphicsDevice)
{
	this->nativePointer = nativePointer;
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout::GetNativePointer()
{
	return nativePointer;
}
