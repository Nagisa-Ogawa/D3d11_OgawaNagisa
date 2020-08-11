//=============================================================================
// VertexBuffer.h
// 
//=============================================================================
#pragma once

#include "Graphics.h"

// ���_�o�b�t�@�[��\���܂��B
class VertexBuffer : public GraphicsResource
{
public:
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	template <class T, UINT _Size>
	static VertexBuffer* Create(GraphicsDevice* graphicsDevice, const T(&shaderBytecode)[_Size]);
	// ���̃N���X�̃C���X�^���X���쐬���܂��B
	static VertexBuffer* Create(GraphicsDevice* graphicsDevice, const void* initialData, UINT byteWidth);

	// �f�X�g���N�^�[
	~VertexBuffer();
	// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
	ID3D11Buffer* GetNativePointer();

private:
	ID3D11Buffer* nativePointer = nullptr;
	// ���̃N���X�̃C���X�^���X�����������܂��B
	VertexBuffer(GraphicsDevice* graphicsDevice, ID3D11Buffer* nativePointer);
	// �R�s�[ �R���X�g���N�^�[
	VertexBuffer(const VertexBuffer&);
};

// ���̃N���X�̃C���X�^���X���쐬���܂��B
template <class T, UINT _Size>
static VertexBuffer* VertexBuffer::Create(
	GraphicsDevice* graphicsDevice, const T(&shaderBytecode)[_Size])
{
	return Create(graphicsDevice, shaderBytecode, sizeof(T) * _Size);
}
