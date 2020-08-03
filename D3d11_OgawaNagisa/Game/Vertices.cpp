//=============================================================================
// Vertices.cpp
// 
//=============================================================================
#include "Game.h"

namespace 
{
	const D3D11_INPUT_ELEMENT_DESC VertexPosition_InputElementDescs[1] =
	{
		{
			"POSITION",						// この要素のセマンティック名
			0,								// この要素のセマンティックインデックス。同じセマンティックを持つ要素が複数あるときに使用
			DXGI_FORMAT_R32G32B32_FLOAT,	// データの形式
			0,								// 入力アセンブラーを識別する整数値
			0,								// 頂点の始点からのオフセット(バイト単位)
			D3D11_INPUT_PER_VERTEX_DATA,	// 入力要素のデータタイプを指定する構造体
			0								// バッファ内を1要素ずつ進む前に、同じインスタンスごとのデータを使用して描画するインスタンスの数。
											// 頂点データを含む要素の場合、この値は0でなければない
		},
	};
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormal_InputElementDescs[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTexture_InputElementDescs[3] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}

// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
const D3D11_INPUT_ELEMENT_DESC* VertexPosition::GetInputElementDescs()
{
	return VertexPosition_InputElementDescs;
}

// GetInputElementDescs()関数で取得される配列の要素数を取得します。
UINT VertexPosition::GetInputElementDescsLength()
{
	return ARRAYSIZE(VertexPosition_InputElementDescs);
}

// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormal::GetInputElementDescs()
{
	return VertexPositionNormal_InputElementDescs;
}

// GetInputElementDescs()関数で取得される配列の要素数を取得します。
UINT VertexPositionNormal::GetInputElementDescsLength()
{
	return ARRAYSIZE(VertexPositionNormal_InputElementDescs);
}

// この頂点情報をD3D11_INPUT_ELEMENT_DESCで表した配列を取得します。
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormalTexture::GetInputElementDescs()
{
	return VertexPositionNormalTexture_InputElementDescs;
}

// GetInputElementDescs()関数で取得される配列の要素数を取得します。
UINT VertexPositionNormalTexture::GetInputElementDescsLength()
{
	return ARRAYSIZE(VertexPositionNormalTexture_InputElementDescs);
}
