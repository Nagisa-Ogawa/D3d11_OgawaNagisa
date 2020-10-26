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
class FloorObject;

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

// �����蔻��p�̎O�p�`�̍\����
struct TriangleCollider {
	DirectX::XMVECTOR t1;
	DirectX::XMVECTOR t2;
	DirectX::XMVECTOR t3;
};

// �v���C���[�̃Q�[���I�u�W�F�N�g��\���܂��B
class PlayerObject : public GameObject
{
public:

	// �O��̃|�W�V����
	DirectX::XMVECTOR beforPosition = { 0.0f,0.0f,0.0f,1.0f };
	// ����̃t���[���̍��v�x�N�g��
	DirectX::XMVECTOR Vector = { 0.0f,0.0f,0.0f,0.0f };

	// ���Ƃ̓����蔻����s�����W
	DirectX::XMVECTOR hitPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	// ���̃I�u�W�F�N�g�ƍs�������蔻��p�̋��̔��a
	float colliderRadius = 2.0f;

	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	PlayerObject(std::shared_ptr<Input> input);
	// �V�[��������������ۂɌĂяo����܂��B
	void Start();
	// �t���[�����X�V����ۂɌĂяo����܂��B
	void Update(float time, float deltaTime,float rad, const std::unique_ptr<FloorObject>& floor);
	// �t���[����`�悷��ۂɌĂяo����܂��B
	void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);
	// ���Ƃ̓����蔻������郁�\�b�h
	bool CollisionWithFloor(DirectX::XMVECTOR hitPos, DirectX::XMVECTOR ofterHitPos ,TriangleCollider tri);

private:
};

class FloorObject : public GameObject
{
public:

	TriangleCollider tri1;
	TriangleCollider tri2;

	// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
	FloorObject(std::shared_ptr<Input> input);
	// �V�[��������������ۂɌĂяo����܂��B
	void Start();
	// �t���[�����X�V����ۂɌĂяo����܂��B
	void Update(float time, float deltaTime);
	// �t���[����`�悷��ۂɌĂяo����܂��B
	void Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime);
private:
};



