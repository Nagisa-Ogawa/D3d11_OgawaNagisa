//=============================================================================
// Scene.h
// 
//=============================================================================
#pragma once

#include <memory>
#include <wrl.h>
#include <DirectXMath.h>

// �O���錾
class Camera;
class GameWindow;
class Graphics;
class GameObject;
class PlayerObject;
class Input;

// �Q�[����ʂ�\���܂��B
class Scene
{
public:
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	Scene::Scene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics,
		std::shared_ptr<Input> input)
	{
		this->window = window;
		this->graphics = graphics;
		this->input = input;
	}


	// �V�[��������������ۂɌĂяo����܂��B
	virtual void Start() = 0;
	// �t���[�����X�V����ۂɌĂяo����܂��B
	virtual void Update(float time, float elapsedTime) = 0;
	// �t���[����`�悷��ۂɌĂяo����܂��B
	virtual void Draw(float time, float elapsedTime) = 0;
	// �V�[�����폜����ۂɌĂяo����܂��B
	virtual void End() = 0;
	int a;

protected:
	// ���̃V�[���Ɗ֘A�t����ꂽ�E�B���h�E
	std::shared_ptr<GameWindow> window;
	// ���̃V�[���Ɗ֘A�t����ꂽ�O���t�B�b�N�X�@�\
	std::shared_ptr<Graphics> graphics;
	// ���[�U�[���̓f�o�C�X
	std::shared_ptr<Input> input;
	// �J���� �p�����[�^�[�̂��߂̒萔�o�b�t�@�[�̒�`
	struct ConstantBufferDescForCamera
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	// �J�����p�̒萔�o�b�t�@�[
	std::shared_ptr<ConstantBuffer> constantBufferForCamera;

	// ���C�e�B���O�̂��߂̒萔�o�b�t�@�[�̒�`
	struct ConstantBufferDescForLighting
	{
		DirectX::XMFLOAT3A lightPosition;
		DirectX::XMFLOAT4 diffuseLightColor;
		DirectX::XMFLOAT4 specularLightColor;
		DirectX::XMFLOAT4 ambientLightColor;

		DirectX::XMFLOAT3 viewPosition;
		//�}�e���A�� �v���p�e�B
		float specularPower;
		float specularIntensity;
	};
	// ���C�e�B���O�p�̒萔�o�b�t�@�[
	std::shared_ptr<ConstantBuffer> constantBufferForLighting;

#pragma region CustomPixelShader�p

	struct LightingDesc
	{
		// ���s�����̏��
		DirectX::XMFLOAT4 direction;
		DirectX::XMFLOAT4 color;
	};

	struct MaterialDesc
	{
		// ���̕\�ʂ̎���
		DirectX::XMFLOAT3 baseColor;
		float metallic;
		DirectX::XMFLOAT3 specular;
		float roughness;
	};

	struct ConstantBufferDescForCustomLighting
	{
		LightingDesc lightDesc;
		MaterialDesc materialDesc;
		DirectX::XMFLOAT4 cameraPosition;
	};

	// ���C�e�B���O�p�̒萔�o�b�t�@�[
	std::shared_ptr<ConstantBuffer> constantBufferForCustomLighting;

#pragma endregion


	// �t���[�����ƂɍX�V�����萔�o�b�t�@�[�̒�`
	struct ConstantBufferDescForPerFrame
	{
		DirectX::XMMATRIX world;
	};
	// �t���[�����ƂɍX�V�����̒萔�o�b�t�@�[
	std::shared_ptr<ConstantBuffer> constantBufferForPerFrame;

	// �J���� �I�u�W�F�N�g
	std::shared_ptr<Camera> camera;

	// �R�s�[�Ƒ�����Z���֎~
	Scene(const Scene&) {}
	Scene& operator=(const Scene&) { return *this; }

private:

};

class TitleScene : public Scene
{
public:
	TitleScene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics,
		std::shared_ptr<Input> input);
	void Start();
	void Update(float time, float elapsedTime);
	void Draw(float time, float elapsedTime);
	void End();

private:
	// �Q�[���I�u�W�F�N�g
	std::shared_ptr<GameObject> titleObj;

};

class GameScene : public Scene
{
public:
	GameScene(
		std::shared_ptr<GameWindow> window,
		std::shared_ptr<Graphics> graphics,
		std::shared_ptr<Input> input);
	void Start();
	void Update(float time, float elapsedTime);
	void Draw(float time, float elapsedTime);
	void End();

private:
	// �Q�[���I�u�W�F�N�g
	std::shared_ptr<PlayerObject> playerObject;
	std::shared_ptr<GameObject> gameObjectB;

};



