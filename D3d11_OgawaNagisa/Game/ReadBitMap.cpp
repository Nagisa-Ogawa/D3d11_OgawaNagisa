//=============================================================================
// ReadBitMap.cpp
// 
//=============================================================================
#include "Game.h"
#include <new>
#include <stdio.h>


uint32_t* ReadBitMap::ReadFromBitMap(char* path)
{
	
	unsigned long offset = 0;

	FILE* fp;
	uint16_t isbmp;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	fopen_s(&fp, path, "rb");
	if (fp == NULL)
	{
		printf("失敗");
		return nullptr;
	}

	// ファイルヘッダーを読み込み
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	//fread(&isbmp, sizeof(uint16_t), 1, fp);
	// BitMapかどうかチェック
	if (fileHeader.bfType != 0x4d42)
	{
		fclose(fp);
		return nullptr;
	}
	fseek(fp, 14, SEEK_SET);
	// 情報ヘッダ読み込み
	fread(&infoHeader, sizeof(infoHeader), 1, fp);
	if (infoHeader.biHeight <= 0 ||
		infoHeader.biWidth <= 0)
	{
		fclose(fp);
		return nullptr;
	}
	//画像の幅と高さを取得
	this->height = infoHeader.biHeight;
	this->width = infoHeader.biWidth;
	UINT size = this->width * this->height;

	uint8_t** writeSource = new uint8_t * [size];

	// 画像データまで移動し、取得
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);
	for (int i = 0; i < size; i++)
	{
		writeSource[i] = new uint8_t[4];
		fread(writeSource[i], sizeof(uint8_t), 4, fp);
	}

	fclose(fp);

	uint32_t** reverseSource = new uint32_t * [this->height];
	this->source = new uint32_t[size];

	for (int h = 0; h < this->height; h++)
	{
		reverseSource[this->height - 1 - h] = new uint32_t[this->width];
		for (int w = 0; w < this->width; w++)
		{
			// BGRAをRGBAに変換
			uint32_t array = writeSource[h * this->width + w][2] | (writeSource[h * this->width + w][1] << 8) | 
								(writeSource[h * this->width + w][0] << 16) | (writeSource[h * this->width + w][3] << 24);
			reverseSource[this->height - 1 - h][w] = array;
		}
	}
	

	for (int h = 0; h < this->height; h++)
	{
		for (int w = 0; w < this->width; w++)
		{
			this->source[h * this->width + w] = reverseSource[h][w];
		}
	}

	// 開放処理
	for (int i = 0; i < size; i++)
	{
		delete[] writeSource[i];
	}
	delete[] writeSource;
	for (int i = 0; i < this->height; i++)
	{
		delete[] reverseSource[i];
	}
	delete[] reverseSource;

	return this->source;
}

void ReadBitMap::Release()
{
	delete[] this->source;
}