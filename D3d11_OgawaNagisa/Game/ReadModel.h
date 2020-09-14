#pragma once
#include <Windows.h>
#include <DirectXMath.h>

class ReadModel
{

public:
	// 位置座標のみをもつ頂点データの型
	struct VertexPosition
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT3 normal;	// 法線ベクトル
		DirectX::XMFLOAT2 texCoord;	// UV座標
	};

	// .objファイルの読み込みをするメソッド
	void ReadModelData(char* filename);
	// verticesを取得するメソッド
	VertexPosition* GetVertexData();
	// indicesを取得するメソッド
	UINT32* GetIndexData();
	// verticesのサイズを取得するメソッド
	SIZE_T GetVerticesSize();
	// indicesのサイズを取得するメソッド
	SIZE_T GetIndicesSize();
	// デストラクタ
	~ReadModel();

private:
	// 配列のサイズ
	UINT32 max;
	// 頂点データ
	VertexPosition* vertices;
	// インデックスデータ
	UINT32* indices;

	// 指定した文字で文字列を切り分けるメソッド
	void SplitBuffer(
		const char splitKeyword,
		const char* readBuffer,
		std::vector<std::string>& splitBuffer);
};
