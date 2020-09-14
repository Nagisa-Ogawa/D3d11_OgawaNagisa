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
	// �t�@�C�����J��
	FILE* fp;
	if (fopen_s(&fp, filename, "r") != 0) {
		fclose(fp);
	}
	// �z��̗v�f��
	const UINT32 readMax = 128;
	const UINT32 max = 1024 * 1024;
	this->max = max;
	// ��s���Ƃɓǂݍ��񂾃o�b�t�@�[
	char* readBuffer = new char[readMax];
	// ���_���̔z��
	vector<XMFLOAT3> vertexPositions;
	vector<XMFLOAT3> vertexNormals;
	vector<XMFLOAT2> texCoord;
	// ���̃f�[�^������Ƃ������̃J�E���g
	int vertexCount = 0;
	int indexCount = 0;
	// ������
	vertices = new VertexPosition[max];
	indices = new UINT32[max];

	while (fgets(readBuffer, readMax, fp) != nullptr) {
		// �X�y�[�X�ŋ�؂�����������i�[���郊�X�g
		vector<string> splitBuffer;
		// �ǂݎ������������X�y�[�X���Ƃɐ؂蕪����
		SplitBuffer(' ',readBuffer, splitBuffer);
		// �擪�̕������牽�̏�񂩔��f
		string dataType = splitBuffer[0];
		splitBuffer.erase(splitBuffer.begin());

		// ���_���W�f�[�^�̏ꍇ
		if (dataType == "v") {
			// float3�^�Ƀf�[�^���i�[
			vertexPositions.push_back({
				stof(splitBuffer[0]),
				stof(splitBuffer[1]),
				stof(splitBuffer[2])
			});
		}
		// �@���f�[�^�̏ꍇ
		else if (dataType == "vn"){
			vertexNormals.push_back({
				stof(splitBuffer[0]),
				stof(splitBuffer[1]),
				stof(splitBuffer[2])
			});
		}
		// �e�N�X�`�����W�̏ꍇ
		else if (dataType == "vt") {
			texCoord.push_back({
				stof(splitBuffer[0]),
				stof(splitBuffer[1]),
			});
		}
		// �ʃf�[�^�̏ꍇ
		else if (dataType == "f") {
			for (int i = 0; i < splitBuffer.size(); i++) {
				// ���_���̃��X�g
				vector<string> fInfo;
				// �؂����悤�ɖʃf�[�^�̍Ō�ɉ��s��ǉ�
				splitBuffer[i] += '\n';
				// "/"���Ƃɐ؂���
				SplitBuffer('/', splitBuffer[i].c_str(), fInfo);
				// ���_���W���擾
				int index = stoi(fInfo[0]) - 1;
				vertices[vertexCount].position = vertexPositions.at(index);
				// �e�N�X�`�����W���擾
				index = stoi(fInfo[1]) - 1;
				vertices[vertexCount].texCoord = texCoord.at(index);
				// �@�����擾
				index = stoi(fInfo[2]) - 1;
				vertices[vertexCount].normal = vertexNormals.at(index);
				// ���_���W�̃C���f�b�N�X��indices�ɃZ�b�g
				indices[indexCount] = indexCount;

				// �J�E���g�A�b�v
				vertexCount++;
				indexCount++;
			}
		}		
	}	
	fclose(fp);

	delete[] readBuffer;

}

// �w�肵�������ŕ������؂蕪���郁�\�b�h
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

// vertices�̃T�C�Y���擾���郁�\�b�h
SIZE_T ReadModel::GetVerticesSize() {
	return sizeof(VertexPosition) * max;
}

// indices�̃T�C�Y���擾���郁�\�b�h
SIZE_T ReadModel::GetIndicesSize() {
	return sizeof(UINT32) * max;
}

ReadModel::~ReadModel() {
	delete[] vertices;
	delete[] indices;
}