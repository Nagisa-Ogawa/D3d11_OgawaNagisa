//=============================================================================
// InputLayout.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// ���̃N���X�̃C���X�^���X�����������܂��B
InputLayout::InputLayout(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
	: GraphicsResource(graphicsDevice)
{
	OnInitialize(
		graphicsDevice,
		inputElementDescs, numElements,
		shaderBytecodeWithInputSignature);
}

// ���̃N���X�̃C���X�^���X���쐬���܂��B
void InputLayout::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
	const Microsoft::WRL::ComPtr<ID3DBlob> shaderBytecodeWithInputSignature)
{
	ThrowIfFailed(
		graphicsDevice->GetDevice()->CreateInputLayout(
			inputElementDescs, numElements,
			shaderBytecodeWithInputSignature->GetBufferPointer(),
			shaderBytecodeWithInputSignature->GetBufferSize(),
			&nativePointer));
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout::GetNativePointer() const
{
	return nativePointer;
}
