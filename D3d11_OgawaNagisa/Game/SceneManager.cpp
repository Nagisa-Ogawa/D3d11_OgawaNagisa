#include "stdafx.h"
#include "Game.h"

// ���̃N���X�̃V���O���g���C���X�^���X���擾���܂��B
SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}


SceneManager::SceneManager(){
}

// ���݂̃V�[����Update()���Ăяo���܂��B
void SceneManager::Update(float time, float elapsedTime) {
	this->nowScene->Update(time, elapsedTime);
}

// ���݂̃V�[����Draw()���Ăяo���܂��B
void SceneManager::Draw(float time, float elapsedTime) {
	this->nowScene->Draw(time, elapsedTime);
}

// ���݂̃V�[����؂�ւ��܂��B
void SceneManager::ChangeScene(std::shared_ptr<Scene> nowScene)
{
	if (nowScene == NULL) {
		return;
	}
	// ���݂̃V�[���̏I�����������܂��B
	if (this->nowScene != NULL) {
		this->nowScene->End();
	}
	// �V�[����؂�ւ��܂��B
	this->nowScene = nowScene;
	// �V�����V�[���̏������������Ăяo���܂��B
	this->nowScene->Start();
}

// ���݂̃V�[�����擾���܂��B
std::shared_ptr<Scene> SceneManager::GetScene()
{
	if (this->nowScene == NULL) {
		return NULL;
	}
	return this->nowScene;
}