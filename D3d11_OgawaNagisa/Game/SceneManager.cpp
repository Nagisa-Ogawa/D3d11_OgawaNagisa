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
std::shared_ptr<Scene> SceneManager::Update(float time, float elapsedTime) {
	std::shared_ptr<Scene> nowScene;
	nowScene = this->scene->Update(time, elapsedTime);
	return nowScene;
}

// ���݂̃V�[����Draw()���Ăяo���܂��B
void SceneManager::Draw(float time, float elapsedTime) {
	this->scene->Draw(time, elapsedTime);
}

// ���݂̃V�[����؂�ւ��܂��B
void SceneManager::ChangeScene(std::shared_ptr<Scene> nowScene)
{
	if (nowScene == NULL) {
		return;
	}
	// ���݂̃V�[���̏I�����������܂��B
	if (this->scene != NULL) {
		this->scene->End();
	}
	
	// �V�[����؂�ւ��܂��B
	this->scene = nowScene;
	// �V�����V�[���̏������������Ăяo���܂��B
	this->scene->Start();
}

// ���݂̃V�[�����擾���܂��B
std::shared_ptr<Scene> SceneManager::GetScene()
{
	if (this->scene == nullptr) {
		return nullptr;
	}
	return this->scene;
}