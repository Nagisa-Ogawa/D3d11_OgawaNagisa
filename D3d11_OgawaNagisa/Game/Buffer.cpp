//=============================================================================
// Buffers.cpp
// 
//=============================================================================
#include "Game.h"

// ���̃N���X�̐V�����C���X�^���X���쐬���܂�
VertexBuffer* VertexBuffer::Create(
	ID3D11Device* graphicsDevice, UINT byteWidth)
{
	// ���̃N���X�̃������[���m��
	auto result = new VertexBuffer();
	if (result == nullptr)
	{
		OutputDebugString(L"�������[���m�ۂł��܂���ł����B");
		return nullptr;
	}

	// �쐬���钸�_�o�b�t�@�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;	// �쐬����o�b�t�@�̃T�C�Y(bytes)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// �o�b�t�@�̎g�p���@(�Ƃ肠����DEFAULT)
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// Vertex Buffer�Ƃ��ė��p����
	bufferDesc.CPUAccessFlags = 0;			// CPU����̓ǂݏ����Ɏg��Ȃ��ꍇ��0
	bufferDesc.MiscFlags = 0;				// �I�v�V�����̃t���O
	bufferDesc.StructureByteStride = 0;		// ���_�o�b�t�@�Ƃ��Ďg���Ȃ�0
	// ���_�o�b�t�@���쐬
	graphicsDevice->CreateBuffer(
		&bufferDesc,
		NULL,	// �o�b�t�@�̏����f�[�^
		&result->buffer);

	return result;

}

// ���\�[�X���J�����܂��B
void VertexBuffer::Release()
{
	SAFE_RELEASE(buffer);
	delete this;
}

// ID3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer;
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
void VertexBuffer::SetData(void* data)
{
	// ID3D11Device���擾
	ID3D11Device* graphicsDevice = nullptr;
	buffer->GetDevice(&graphicsDevice);
	// ID3D11DeviceContext���擾
	ID3D11DeviceContext* immediateContext = nullptr;
	graphicsDevice->GetImmediateContext(&immediateContext);

	// �o�b�t�@�Ƀf�[�^��]��(�R�s�[)����
	immediateContext->UpdateSubresource(
		buffer, // �����̃o�b�t�@�̃|�C���^�[
		0,	// �����̃T�u���\�[�X�𔻕ʂ���index
		NULL,
		data,	// ����f�[�^�ւ̃|�C���^ 
		0,	// ����f�[�^�̈�s�̃T�C�Y
		0	// ����f�[�^�̈�̐[�x�X���C�X�̃T�C�Y
	);

	// �擾�����C���^�[�t�F�[�X���J��
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
IndexBuffer* IndexBuffer::Create(
	ID3D11Device* graphicsDevice, UINT indexCount)
{
	// ���̃N���X�̃������[���m��
	auto result = new IndexBuffer();
	if (result == nullptr) {
		OutputDebugString(L"�������[���m�ۂł��܂���ł����B");
		return nullptr;
	}

	// �쐬����o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = indexCount * sizeof(UINT32);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �C���f�b�N�X�o�b�t�@�[���쐬
	graphicsDevice->CreateBuffer(&bufferDesc, nullptr, &result->buffer);

	return result;
}

// ���\�[�X��������܂��B
void IndexBuffer::Release()
{
	SAFE_RELEASE(buffer);
	delete this;
}

// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer;
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
void IndexBuffer::SetData(UINT32* data)
{
	// ID3D11Device���擾
	ID3D11Device* graphicsDevice = nullptr;
	buffer->GetDevice(&graphicsDevice);
	// ID3D11DeviceContext���擾
	ID3D11DeviceContext* immediateContext = nullptr;
	graphicsDevice->GetImmediateContext(&immediateContext);

	// �o�b�t�@�[��data��]��(�R�s�[)����
	immediateContext->UpdateSubresource(
		buffer, 0, NULL, data, 0, 0);

	// �擾�����C���^�[�t�F�C�X�����
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
ConstantBuffer* ConstantBuffer::Create(
	ID3D11Device* graphicsDevice, UINT byteWidth)
{
	// ���̃N���X�̃������[���m��
	auto result = new ConstantBuffer();
	if (result == nullptr) {
		OutputDebugString(L"�������[���m�ۂł��܂���ł����B");
		return nullptr;
	}

	// �쐬����o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �萔�o�b�t�@�[���쐬
	graphicsDevice->CreateBuffer(&bufferDesc, nullptr, &result->buffer);

	return result;
}

// ���\�[�X��������܂��B
void ConstantBuffer::Release()
{
	SAFE_RELEASE(buffer);
	delete this;
}

// D3D11�̃l�C�e�B�u�|�C���^�[���擾���܂��B
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer;
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肵�܂��B
void ConstantBuffer::SetData(void* data)
{
	// ID3D11Device���擾
	ID3D11Device* graphicsDevice = nullptr;
	buffer->GetDevice(&graphicsDevice);
	// ID3D11DeviceContext���擾
	ID3D11DeviceContext* immediateContext = nullptr;
	graphicsDevice->GetImmediateContext(&immediateContext);

	// �o�b�t�@�[��data��]��(�R�s�[)����
	immediateContext->UpdateSubresource(
		buffer, 0, NULL, data, 0, 0);

	// �擾�����C���^�[�t�F�C�X�����
	SAFE_RELEASE(immediateContext);
	SAFE_RELEASE(graphicsDevice);
}
