//=============================================================================
// Scene.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"
#include "DirectXHelper.h"
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"
#include "PhongPixelShader.h"
#include "CustomPixelShader.h"
#include "ReadModel.h"

using namespace std;
using namespace DirectX;
using namespace DX;


GameScene::GameScene(
	std::shared_ptr<GameWindow> window,
	std::shared_ptr<Graphics> graphics,
	std::shared_ptr<Input> input) : Scene(window,graphics,input)
{
	
}

// �V�[��������������ۂɌĂяo����܂��B
void GameScene::Start()
{
	auto graphicsDevice = graphics->GetGraphicsDevice();

	// �萔�o�b�t�@�[���쐬
	constantBufferForCamera.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForCamera)));
	//constantBufferForLighting.reset(
	//	new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForLighting)));
	constantBufferForCustomLighting.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForCustomLighting)));
	constantBufferForPerFrame.reset(
		new ConstantBuffer(graphicsDevice, sizeof(ConstantBufferDescForPerFrame)));

	// �J���� �I�u�W�F�N�g���쐬
	camera.reset(new Camera(window));
	camera->Start();


	playerObject.reset(new PlayerObject(input));
	enemy.reset(new PlayerObject(input));
	floor.reset(new FloorObject(input));
	
	// ���_�f�[�^�ɂ��ē��͗v�f���`
	ReadModel playerModel;
	playerModel.ReadModelData("human.obj");

	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// ���_�f�[�^
	ReadModel::VertexPosition vertices[] =
	{
		{ {-3.0f, -1.0f,3.0f},{0.0f,0.0f,-1.0f},{0.0f,0.0f} },
		{ { 3.0f, -1.0f,3.0f},{0.0f,0.0f,-1.0f},{1.0f,0.0f} },
		{ {-3.0f, -1.0f,-3.0f},{0.0f,0.0f,-1.0f},{0.0f,1.0f} },
		{ { 3.0f, -1.0f,-3.0f},{0.0f,0.0f,-1.0f},{1.0f,1.0f} },
	};
	floor->tri1.t1 = XMLoadFloat3(&vertices[0].position);
	floor->tri1.t2 = XMLoadFloat3(&vertices[1].position);
	floor->tri1.t3 = XMLoadFloat3(&vertices[2].position);
	floor->tri2.t1 = XMLoadFloat3(&vertices[3].position);
	floor->tri2.t2 = XMLoadFloat3(&vertices[2].position);
	floor->tri2.t3 = XMLoadFloat3(&vertices[1].position);

	floor->tri1.t1 = XMVectorSetW(floor->tri1.t1, 1.0f);
	floor->tri1.t2 = XMVectorSetW(floor->tri1.t2, 1.0f);
	floor->tri1.t3 = XMVectorSetW(floor->tri1.t3, 1.0f);
	floor->tri2.t1 = XMVectorSetW(floor->tri2.t1, 1.0f);
	floor->tri2.t2 = XMVectorSetW(floor->tri2.t2, 1.0f);
	floor->tri2.t3 = XMVectorSetW(floor->tri2.t3, 1.0f);


	{
		//// ���_�f�[�^�̔z��
		//// ���̕�����
		//const int u_max = 64;
		//// �c�̕�����
		//const int v_max = 64;
		//// ���a
		//const float radius = 1.0f;
		//const int vertex_num = (u_max + 1) * (v_max + 1);
		//// UV���W�̃I�t�Z�b�g
		//const float uOffset = 1.0f / u_max;
		//const float vOffset = 1.0f / v_max;
		//// ���݂�UV���W
		//float nowUPos = 0.0f;
		//float nowVPos = 0.0f;

		//// ���_�f�[�^
		//VertexPosition* vertices = new VertexPosition[vertex_num];

		//for (int v = 0; v <= v_max; v++) {
		//	for (int u = 0; u <= u_max; u++) {
		//		// 0<=theta<=��
		//		float theta = XM_PI * v / v_max;
		//		// 0<=phi<2��
		//		float phi = XM_2PI * u / u_max;
		//		if (u == u_max)
		//		{

		//			float phi = XM_2PI * 0 / u_max;
		//			vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
		//			// �@��
		//			vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
		//			// UV���W
		//			vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };
		//		}
		//		else
		//		{
		//			// �O�����ɍ��W����O�����������W�ւ̕ϊ��������(y��z�����ւ�)
		//			// x = r * sin�� * cos��
		//			// y = r * cos��
		//			// z = r * sin�� * sin��
		//			vertices[(u_max + 1) * v + u].position.x = radius * XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].position.y = radius * XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].position.z = radius * XMScalarSin(theta) * XMScalarSin(phi);
		//			// �@��
		//			vertices[(u_max + 1) * v + u].normal.x = XMScalarSin(theta) * XMScalarCos(phi);
		//			vertices[(u_max + 1) * v + u].normal.y = XMScalarCos(theta);
		//			vertices[(u_max + 1) * v + u].normal.z = XMScalarSin(theta) * XMScalarSin(phi);
		//			// UV���W
		//			vertices[(u_max + 1) * v + u].texCoord = { nowUPos,nowVPos };

		//		}
		//		nowUPos += uOffset;
		//	}
		//	nowUPos = 0.0f;
		//	nowVPos += vOffset;
		//}
	}
	// ���_�o�b�t�@�[���쐬
	shared_ptr<VertexBuffer> vertexBuffers[] = {
		shared_ptr<VertexBuffer>(new VertexBuffer(
			graphicsDevice, playerModel.GetVertexData(),playerModel.GetVerticesSize())
		)
	};
	// ���_�o�b�t�@�[���쐬
	shared_ptr<VertexBuffer> floorVertexBuffers[] = {
		shared_ptr<VertexBuffer>(new VertexBuffer(
			graphicsDevice, vertices)
		)
	};


	UINT strides[] = {
		sizeof(ReadModel::VertexPosition),
	};

	//// �C���f�b�N�X�f�[�^�̗v�f��
	//const int index_num = (v_max * u_max) * 6;
	////�C���f�b�N�X�f�[�^�̔z��
	//UINT32* indices = new UINT32[index_num];
	//for (int v = 0; v < v_max; v++) {
	//	// �c�����̃I�t�Z�b�g
	//	int v_offset = v * u_max;
	//	int offset = v * (u_max + 1);
	//	for (int u = 0; u < u_max; u++) 
	//	{
	//		indices[(v_offset + u) * 6 + 0] = offset + u + 0;
	//		indices[(v_offset + u) * 6 + 1] = offset + u + 1;
	//		indices[(v_offset + u) * 6 + 2] = offset + u + u_max + 1;
	//		indices[(v_offset + u) * 6 + 3] = offset + u_max + 1 + (u + 1);
	//		indices[(v_offset + u) * 6 + 4] = offset + u_max + 1 + u;
	//		indices[(v_offset + u) * 6 + 5] = offset + u + 1;
	//	}
	//}

		//�C���f�b�N�X�f�[�^�̔z��
	UINT32 indices[] =
	{
		0,1,2,
		3,2,1
	};

	shared_ptr<IndexBuffer> indexBuffer(
		new IndexBuffer(
			graphicsDevice, 
			playerModel.GetIndexData(),playerModel.GetIndicesSize(), 
			DXGI_FORMAT_R32_UINT
		)
	);

	shared_ptr<IndexBuffer> floorIndexBuffer(
		new IndexBuffer(
			graphicsDevice,
			indices
		)
	);

	playerObject->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));
	enemy->mesh.reset(new Mesh(vertexBuffers, strides, indexBuffer, inputElementDescs));
	floor->mesh.reset(new Mesh(floorVertexBuffers, strides, floorIndexBuffer, inputElementDescs));


	// �}�e���A�����쐬
	shared_ptr<Material> material(
		new Material(
			shared_ptr<VertexShader>(
				new VertexShader(graphicsDevice, g_BasicVertexShader)),
			shared_ptr<GeometryShader>(
				new GeometryShader(graphicsDevice, g_BasicGeometryShader)),
			shared_ptr<PixelShader>(
				new PixelShader(graphicsDevice, g_CustomPixelShader))));


	std::shared_ptr<Texture2D> texture;
	std::shared_ptr<Texture2D> floorTexture;
	// �e�N�X�`���[��ǂݍ���
	texture.reset(new Texture2D(graphicsDevice, L"../resource/image/test.png"));
	floorTexture.reset(new Texture2D(graphicsDevice, L"../resource/image/grass.png"));


	// �����_���[���쐬
	playerObject->renderer.reset(new MeshRenderer(graphicsDevice, playerObject->mesh, material,texture));
	enemy->renderer.reset(new MeshRenderer(graphicsDevice, enemy->mesh, material, texture));
	floor->renderer.reset(new MeshRenderer(graphicsDevice, floor->mesh, material, floorTexture));
}

// �t���[�����X�V����ۂɌĂяo����܂��B
std::shared_ptr<Scene> GameScene::Update(float time, float elapsedTime)
{
	XMFLOAT3 foucusPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 eyePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 playerPos = { 0.0f,0.0f,0.0f };
	XMStoreFloat3(&foucusPos, camera->focusPosition);
	XMStoreFloat3(&eyePos, camera->eyePosition);
	XMStoreFloat3(&playerPos, playerObject->GetTransform()->position);
	// �J�����̈ʒu���v���C���[�̓��̋߂��ɏC��
	eyePos = { playerPos.x,playerPos.y + 1.0f,playerPos.z-3.0f};
	// �}�E�X�̃|�W�V����
	XMFLOAT2 mousePos = { input->GetMousePosition().x ,input->GetMousePosition().y };
	// ��ʂ̒���
	XMFLOAT2 centerPos = { camera->viewport.Width / 2 ,camera->viewport.Height / 2 };
	// �}�E�X�̈ʒu����c�Ɖ��̈ړ��ʂ��o���p�x�ɂ���
	XMFLOAT2 degree = { (mousePos.x - centerPos.x) / 3 ,-(mousePos.y - centerPos.y) / 3 };
	// radian�ɕϊ�
	XMFLOAT2 rad = { degree.x * (XM_PI / 180.0f) ,degree.y * (XM_PI / 180.0f) };
	float range = 1.0f;
	foucusPos.x = eyePos.x + range * (float)sin(rad.x);
	foucusPos.y = eyePos.y +							 range * (float)sin(rad.y);
	foucusPos.z = eyePos.z + range * (float)cos(rad.x) * range * (float)cos(rad.y);
	// �J�����ɃZ�b�g
	camera->focusPosition = XMVectorSet(foucusPos.x, foucusPos.y, foucusPos.z, 1.0f);
	camera->eyePosition = XMVectorSet(eyePos.x, eyePos.y, eyePos.z, 1.0f);
	// �e�I�u�W�F�N�g��Update
	enemy->GetTransform()->SetRotation(0.0f, 170.0f, 0.0f);
	enemy->GetTransform()->position = XMVectorSet(0.0f, 0.0f, 2.0f, 1.0f);
	playerObject->Update(time, elapsedTime,rad.x,floor);
	floor->Update(time, elapsedTime);

	return SceneManager::GetInstance().GetScene();
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void GameScene::Draw(float time, float elapsedTime)
{
	auto deviceContext = graphics->GetGraphicsDevice()->GetImmediateContext();

	// �����_�[�^�[�Q�b�g��ݒ�
	ID3D11RenderTargetView* renderTargetViews[] = {
		graphics->GetRenderTarget()->GetView().Get(),
	};
	deviceContext->OMSetRenderTargets(
		ARRAYSIZE(renderTargetViews), renderTargetViews,
		graphics->GetDepthStencil()->GetView().Get());

	// ��ʂ��N���A�[
	deviceContext->ClearRenderTargetView(
		graphics->GetRenderTarget()->GetView().Get(), camera->clearColor);
	deviceContext->ClearDepthStencilView(
		graphics->GetDepthStencil()->GetView().Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// �r���[�ƃv���W�F�N�V�����̕ϊ��s����擾
	auto viewMatrix = camera->GetViewMatrix();
	auto projectionMatrix = camera->GetProjectionMatrix();
	{
		// �萔�o�b�t�@�[��ݒ�
		const ConstantBufferDescForCamera srcData = {
			viewMatrix,
			projectionMatrix,
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForCamera->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCamera->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// ���C�e�B���O�p�̒萔�o�b�t�@�[���X�V
	{
		
		const LightingDesc lightDesc =
		{
			{ -0.5f, 0.5f, -1.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f, 0.0f },
		};
		const MaterialDesc materialDesc =
		{
			{ 1.0f, 1.0f, 1.0,  },
			0.0f,
			{1.0f,1.0f,1.0f},
			0.5,
		};
		XMFLOAT4 cameraPosition;
		XMStoreFloat4(&cameraPosition, camera->eyePosition);
		// �萔�o�b�t�@�[��ݒ�
		const ConstantBufferDescForCustomLighting srcData =
		{
			lightDesc,
			materialDesc,
			cameraPosition,
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForCustomLighting->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForCustomLighting->GetNativePointer().Get(),
		};
		deviceContext->PSSetConstantBuffers(
			0, ARRAYSIZE(constantBuffers), constantBuffers);
	}

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT viewports[] = {
		camera->viewport,
	};
	deviceContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

	// �Q�[���I�u�W�F�N�g��`��
	{
		// ���[���h�ϊ��s����X�V
		const ConstantBufferDescForPerFrame srcData = {
			playerObject->GetTransform()->GetWorldMatrix(),
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);

		// �萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
		playerObject->Draw(deviceContext.Get(), time, elapsedTime);
	}
	{
		// ���[���h�ϊ��s����X�V
		const ConstantBufferDescForPerFrame srcData = {
			enemy->GetTransform()->GetWorldMatrix(),
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);

		// �萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
		enemy->Draw(deviceContext.Get(), time, elapsedTime);
	}
	{
		// ���[���h�ϊ��s����X�V
		const ConstantBufferDescForPerFrame srcData = {
			floor->GetTransform()->GetWorldMatrix(),
		};
		// �T�u���\�[�X���X�V
		deviceContext->UpdateSubresource(
			constantBufferForPerFrame->GetNativePointer().Get(), 0, NULL, &srcData, 0, 0);

		// �萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferForPerFrame->GetNativePointer().Get(),
		};
		deviceContext->VSSetConstantBuffers(
			1, ARRAYSIZE(constantBuffers), constantBuffers);
		floor->Draw(deviceContext.Get(), time, elapsedTime);
	}
}

void GameScene::End() {

}