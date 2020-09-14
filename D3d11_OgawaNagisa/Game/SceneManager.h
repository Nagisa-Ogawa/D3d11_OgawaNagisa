//=============================================================================
// SceneManager.h
// 
//=============================================================================
#pragma once
#include <memory>

// �O���錾
class Scene;
class GameWindow;
class Graphics;
class Input;

class SceneManager
{
private:
	// ���݂̃V�[��
	std::shared_ptr<Scene> nowScene;
public:
	// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
	static SceneManager& GetInstance();
	SceneManager();
	// ���݂̃V�[����Update()���Ăяo���܂��B
	void Update(float time, float elapsedTime);
	// ���݂̃V�[����Draw()���Ăяo���܂��B
	void Draw(float time, float elapsedTime);

	// ���݂̃V�[����؂�ւ��܂��B
	void ChangeScene(const std::shared_ptr<Scene> nowScene);
	// ���݂̃V�[�����擾���܂��B
	std::shared_ptr<Scene> GetScene();

};

