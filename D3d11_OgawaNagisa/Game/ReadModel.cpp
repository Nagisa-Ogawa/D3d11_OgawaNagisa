#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "ReadModel.h"

using namespace std;
using namespace DirectX;

void ReadModel::ReadModelData(char* filename)
{
	// ファイルを開く
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		fclose(fp);
	}
	// 配列の要素数
	const UINT32 readMax = 128;
	const UINT32 max = 1024 * 1024;
	this->max = max;
	// 一行ごとに読み込んだバッファー
	char* readBuffer = new char[readMax];
	// 頂点情報の配列
	vector<XMFLOAT3> vertexPositions;
	vector<XMFLOAT3> vertexNormals;
	vector<XMFLOAT2> texCoord;
	// 同のデータを何回とったかのカウント
	int vertexCount = 0;
	int indexCount = 0;
	// 初期化
	vertices = new VertexPosition[max];
	indices = new UINT32[max];

	while (fgets(readBuffer, readMax, fp) != nullptr) {
		// スペースで区切った文字列を格納するリスト
		vector<string> splitBuffer;
		// 読み取った文字列をスペースごとに切り分ける
		SplitBuffer(' ',readBuffer, splitBuffer);
		// 先頭の文字から何の情報か判断
		string dataType = splitBuffer[0];
		splitBuffer.erase(splitBuffer.begin());

		// 頂点座標データの場合
		if (dataType == "v") {
			// float3型にデータを格納
			vertexPositions.push_back({
				stof(splitBuffer[0]),
				stof(splitBuffer[1]),
				stof(splitBuffer[2])
			});
		}
		// 法線データの場合
		else if (dataType == "vn"){
			vertexNormals.push_back({
				stof(splitBuffer[0]),
				stof(splitBuffer[1]),
				stof(splitBuffer[2])
			});
		}
		// テクスチャ座標の場合
		else if (dataType == "vt") {
			texCoord.push_back({
				stof(splitBuffer[0]),
				stof(splitBuffer[1]),
			});
		}
		// 面データの場合
		else if (dataType == "f") {
			for (int i = 0; i < splitBuffer.size(); i++) {
				// 頂点情報のリスト
				vector<string> fInfo;
				// 切り取れるように面データの最後に改行を追加
				splitBuffer[i] += '\n';
				// "/"ごとに切り取る
				SplitBuffer('/', splitBuffer[i].c_str(), fInfo);
				// 頂点座標を取得
				int index = stoi(fInfo[0]) - 1;
				vertices[vertexCount].position = vertexPositions.at(index);
				// テクスチャ座標を取得
				index = stoi(fInfo[1]) - 1;
				vertices[vertexCount].texCoord = texCoord.at(index);
				// 法線を取得
				index = stoi(fInfo[2]) - 1;
				vertices[vertexCount].normal = vertexNormals.at(index);
				// 頂点座標のインデックスをindicesにセット
				indices[indexCount] = indexCount;

				// カウントアップ
				vertexCount++;
				indexCount++;
			}
		}		
	}	
	fclose(fp);

	delete[] readBuffer;

}

// 指定した文字で文字列を切り分けるメソッド
void ReadModel::SplitBuffer(const char splitKeyword, const char* readBuffer, vector<string> &splitBuffer)
{
	UINT32 counter = 0;
	string buffer = "";
	while (readBuffer[counter]!='\n'){

		if (readBuffer[counter] == splitKeyword) {
			splitBuffer.push_back(buffer);
			buffer = "";
		}
		else {
			buffer += readBuffer[counter];
		}
		counter++;
		if (readBuffer[counter] == '\n') {
			splitBuffer.push_back(buffer);
		}
	}
	return;
}

ReadModel::VertexPosition* ReadModel::GetVertexData() {
	return vertices;
}

UINT32* ReadModel::GetIndexData() {
	return indices;
}

// verticesのサイズを取得するメソッド
SIZE_T ReadModel::GetVerticesSize() {
	return sizeof(VertexPosition) * max;
}

// indicesのサイズを取得するメソッド
SIZE_T ReadModel::GetIndicesSize() {
	return sizeof(UINT32) * max;
}

ReadModel::~ReadModel() {
	delete[] vertices;
	delete[] indices;
}