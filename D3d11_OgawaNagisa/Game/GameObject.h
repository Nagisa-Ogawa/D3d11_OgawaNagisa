//=============================================================================
// GameObject.h
// 
//=============================================================================
#pragma once
#include <memory>

// �O���錾
class Transform;
class Mesh;
class MeshRenderer;
class Input;

// �Q�[���I�u�W�F�N�g��\���܂��B
class GameObject
{
public:
	// �g�����X�t�H�[�����擾���܂��B
	std::shared_ptr<Transform> GetTransform();
	// �g�����X�t�H�[�����擾���܂��B
	std::shared_ptr<const Transform> GetTransform() const;
	// ���b�V��
	std::shared_ptr<Mesh> mesh;
	// �����_�����O
	std::shared_ptr<MeshRenderer> renderer;

	// ���̃N���X�̐V�����C���X�^���X�����������܂��B
	GameObject(std::shared_ptr<Input> input);

	// �V�[��������������ۂɌĂяo����܂��B
	virtual void Start();
	// �t���[�����X�V����ۂɌĂяo����܂��B
	virtual void Update(float time, float deltaTime);
	// �t���[����`�悷��ۂɌĂяo����܂��B
	virtual void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);

protected:
	// �g�����X�t�H�[��
	std::shared_ptr<Transform> transform;
	// ���[�U�[���̓f�o�C�X
	std::shared_ptr<Input> input;

	// �R�s�[�Ƒ�����Z���֎~
	GameObject(const GameObject&) {}
	GameObject& operator=(const GameObject&) { return *this; }
private:
};


// �v���C���[�̃Q�[���I�u�W�F�N�g��\���܂��B
class PlayerObject : public GameObject
{
public:
	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	PlayerObject(std::shared_ptr<Input> input);
	// �V�[��������������ۂɌĂяo����܂��B
	void Start();
	// �t���[�����X�V����ۂɌĂяo����܂��B
	void Update(float time, float deltaTime);
	// �t���[����`�悷��ۂɌĂяo����܂��B
	void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);

private:
};

