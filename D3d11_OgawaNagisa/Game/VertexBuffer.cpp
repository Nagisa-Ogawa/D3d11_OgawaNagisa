//=============================================================================
// VertexBuffer.cpp
// 
//=============================================================================
#include "VertexBuffer.h"
#include "Game.h"
#include "DirectXHelper.h"

// ���̃N���X�̃C���X�^���X���쐬���܂��B
VertexBuffer* VertexBuffer::Create(const void* initialData, UINT byteWidth)
{
	VertexBuffer* retVal = nullptr;
	ID3D11Buffer* nativePointer = nullptr;
	try {
		// ���_�o�b�t�@�[�ɂ��Ă̋L�q
	// �쐬���钸�_�o�b�t�@�ɂ��Ă̋L�q
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = byteWidth;	// �쐬����o�b�t�@�̃T�C�Y(bytes)
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;	// �o�b�t�@�̎g�p���@(�Ƃ肠����DEFAULT)
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// Vertex Buffer�Ƃ��ė��p����
		vertexBufferDesc.CPUAccessFlags = 0;			// CPU����̓ǂݏ����Ɏg��Ȃ��ꍇ��0
		vertexBufferDesc.MiscFlags = 0;				// �I�v�V�����̃t���O
		vertexBufferDesc.StructureByteStride = 0;		// ���_�o�b�t�@�Ƃ��Ďg���Ȃ�0
			// �������f�[�^�ɂ��Ă̋L�q
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = initialData;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		// ���_�o�b�t�@�[���쐬
		DX::ThrowIfFailed(
			Graphics::GetGraphicsDevice()->CreateBuffer(
				&vertexBufferDesc,
				&subresourceData,	// �o�b�t�@�[�̏����f�[�^
				&nativePointer));
		// ���̃N���X�̃C���X�^���X��������
		retVal = new VertexBuffer(nativePointer);
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
VertexBuffer::VertexBuffer(ID3D11Buffer* nativePointer)
{
	nativePointer->AddRef();
	this->nativePointer = nativePointer;
}

// �f�X�g���N�^�[
VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(nativePointer);
}

// �l�C�e�B�u�����̃|�C���^�[���擾���܂��B
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return nativePointer;
}
