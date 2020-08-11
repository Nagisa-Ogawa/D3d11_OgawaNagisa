//=============================================================================
// InputLayout.h
// 
//=============================================================================
#pragma once

#include <d3d11.h>
#include "Graphics.h"

// ���̓��C�A�E�g��\���܂��B
class InputLayout : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <UINT _Size>
	static InputLayout* Create(
		GraphicsDevice* graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
		ID3DBlob* shaderBytecodeWithInputSignature);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static InputLayout* Create(
		GraphicsDevice* graphicsDevice,
		const D3D11_INPUT_ELEMENT_DESC* inputElementDescs, UINT numElements,
		ID3DBlob* shaderBytecodeWithInputSignature);

	// �f�X�g���N�^�[
	~InputLayout();
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	ID3D11InputLayout* GetNativePointer();

private:
	ID3D11InputLayout* nativePointer = nullptr;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	InputLayout(GraphicsDevice* graphicsDevice, ID3D11InputLayout* nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	InputLayout(const InputLayout&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <UINT _Size>
static InputLayout* InputLayout::Create(
	GraphicsDevice* graphicsDevice,
	const D3D11_INPUT_ELEMENT_DESC(&shaderBytecode)[_Size],
	ID3DBlob* shaderBytecodeWithInputSignature)
{
	return Create(graphicsDevice, shaderBytecode, _Size, shaderBytecodeWithInputSignature);
}
