//=============================================================================
// PlayerObject.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Game.h"

using namespace DirectX;

// このクラスの新しいインスタンスを作成します。
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
	// 重力をかける
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
/// 線と三角形の当たり判定をするメソッド
/// </summary>
/// <param name="hitPos">プレイヤーの床との当たり判定をする点</param>
/// <param name="beforeHitPos">前回のフレームのhitPos</param>
/// <param name="tri">当たり判定をする三角形</param>
/// <returns></returns>
bool PlayerObject::CollisionWithFloor(XMVECTOR hitPos, XMVECTOR beforeHitPos,TriangleCollider tri) {
	// 線の開始点
	XMVECTOR a = beforeHitPos;
	// 線の終了点
	XMVECTOR b = hitPos - beforeHitPos;
	// 三角形のある頂点
	XMVECTOR c = tri.t1;
	// 三角形のある頂点からの二つのベクトル
	XMVECTOR vec1 = tri.t2 - tri.t1;
	XMVECTOR vec2 = tri.t3 - tri.t1;
	// 二つのベクトルの外積を求める
	XMVECTOR nVec = XMVector3Cross(vec1, vec2);
	XMVECTOR d = c - a;
	XMVECTOR nd = XMVector3Dot(nVec, d);
	XMVECTOR nb = XMVector3Dot(nVec, b);
	// nbが0なら当たらない
	if (XMVectorGetX(nb) == 0.0f) {
		return false;
	}
	// 線をp=a+btとしたときの無限平面とあたるときのt
	float t = XMVectorGetX(nd) / XMVectorGetX(nb);
	// 0から1にならなければ当たらない
	if (t < 0.0f || 1.0f < t) {
		return false;
	}
	// 線上の無限平面と当たる点
	XMVECTOR p = a+b*t;
	XMVECTOR g = p - c;
	float gd = XMVectorGetX(XMVector3Dot(g, vec1));
	float ge = XMVectorGetX(XMVector3Dot(g, vec2));
	float dd = XMVectorGetX(XMVector3Dot(vec1, vec1));
	float ee = XMVectorGetX(XMVector3Dot(vec2, vec2));
	float de = XMVectorGetX(XMVector3Dot(vec1, vec2));
	// uとvが0以上でu+vが1以下なら三角形内で当たっている
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

// フレームを描画する際に呼び出されます。
void PlayerObject::Draw(ID3D11DeviceContext* deviceContext, float time, float deltaTime) {
	GameObject::Draw(deviceContext, time, deltaTime);
}
