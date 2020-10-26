//=============================================================================
// PlayerObject.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"

using namespace DirectX;

// ���̃N���X�̐V�����C���X�^���X���쐬���܂��B
PlayerObject::PlayerObject(
	std::shared_ptr<Input> input) : GameObject(input)
{
	transform->position = { 0.0f,3.0f,0.0f,1.0f };
	beforPosition = transform->position;
}

void PlayerObject::Start() {
	GameObject::Start();
}

void PlayerObject::Update(float time, float deltaTime, float rad, const std::unique_ptr<FloorObject>& floor) {
	GameObject::Update(time, deltaTime);
	if (input->GetKey(VK_SPACE)) {
		transform->SetRotation(0, 0, 0);
	}
	const float moveSpeed = 0.1f;
	if (input->GetKey('W')) {
		Vector += XMVectorSet(sin(rad) * moveSpeed, 0.0f, cos(rad) * moveSpeed, 0.0f);
	}
	if (input->GetKey('S')) {
		Vector += XMVectorSet(sin(rad) * -moveSpeed, 0.0f, cos(rad) * -moveSpeed, 0.0f);
	}
	// �d�͂�������
	Vector += XMVectorSet(0.0f, -(deltaTime * 0.96f), 0.0f, 0.0f);
	transform->SetRotation(0, rad, 0);
	hitPos = transform->position + Vector;
	hitPos = XMVectorSetY(hitPos, XMVectorGetY(hitPos) - 1.0f);
	if (CollisionWithFloor(hitPos, beforPosition, floor->tri1)||CollisionWithFloor(hitPos,beforPosition,floor->tri2)) {
		Vector = { XMVectorGetX(Vector),0.0f,XMVectorGetZ(Vector),0.0f };
	}
	transform->position += Vector;
	Vector = { 0.0f,0.0f,0.0f,0.0f };
	beforPosition = transform->position;
	beforPosition = XMVectorSetY(beforPosition, XMVectorGetY(beforPosition) - 1.0f);
}

/// <summary>
/// ���ƎO�p�`�̓����蔻������郁�\�b�h
/// </summary>
/// <param name="hitPos">�v���C���[�̏��Ƃ̓����蔻�������_</param>
/// <param name="beforeHitPos">�O��̃t���[����hitPos</param>
/// <param name="tri">�����蔻�������O�p�`</param>
/// <returns></returns>
bool PlayerObject::CollisionWithFloor(XMVECTOR hitPos, XMVECTOR beforeHitPos,TriangleCollider tri) {
	// ���̊J�n�_
	XMVECTOR a = beforeHitPos;
	// ���̏I���_
	XMVECTOR b = hitPos - beforeHitPos;
	// �O�p�`�̂��钸�_
	XMVECTOR c = tri.t1;
	// �O�p�`�̂��钸�_����̓�̃x�N�g��
	XMVECTOR vec1 = tri.t2 - tri.t1;
	XMVECTOR vec2 = tri.t3 - tri.t1;
	// ��̃x�N�g���̊O�ς����߂�
	XMVECTOR nVec = XMVector3Cross(vec1, vec2);
	XMVECTOR d = c - a;
	XMVECTOR nd = XMVector3Dot(nVec, d);
	XMVECTOR nb = XMVector3Dot(nVec, b);
	// nb��0�Ȃ瓖����Ȃ�
	if (XMVectorGetX(nb) == 0.0f) {
		return false;
	}
	// ����p=a+bt�Ƃ����Ƃ��̖������ʂƂ�����Ƃ���t
	float t = XMVectorGetX(nd) / XMVectorGetX(nb);
	// 0����1�ɂȂ�Ȃ���Γ�����Ȃ�
	if (t < 0.0f || 1.0f < t) {
		return false;
	}
	// ����̖������ʂƓ�����_
	XMVECTOR p = a+b*t;
	XMVECTOR g = p - c;
	float gd = XMVectorGetX(XMVector3Dot(g, vec1));
	float ge = XMVectorGetX(XMVector3Dot(g, vec2));
	float dd = XMVectorGetX(XMVector3Dot(vec1, vec1));
	float ee = XMVectorGetX(XMVector3Dot(vec2, vec2));
	float de = XMVectorGetX(XMVector3Dot(vec1, vec2));
	// u��v��0�ȏ��u+v��1�ȉ��Ȃ�O�p�`���œ������Ă���
	float v = (gd * de - ge * dd) / (de * de - ee * dd);
	if (v < 0.0f || 1.0f < v) {
		return false;
	}
	float u = (ge * de - gd * ee) / (de * de - ee * dd);
	if (u < 0.0f || 1.0f < u + v) {
		return false;
	}
	return true;
}

// �t���[����`�悷��ۂɌĂяo����܂��B
void PlayerObject::Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime) {
	GameObject::Draw(deviceContext, time, deltaTime);
}
