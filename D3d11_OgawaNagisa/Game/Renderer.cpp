//=============================================================================
// Renderer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace DX;
using namespace DirectX;

// このクラスの新しいインスタンスを作成します。
std::shared_ptr<MeshRenderer> MeshRenderer::Create(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material)
{
	// D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT

	// 頂点データについて入力要素を定義
	const D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// 入力レイアウトを作成
	auto inputLayout = InputLayout::Create(graphicsDevice,
		inputElementDescs, material->GetVertexShader()->GetShaderBytecode());

	return shared_ptr<MeshRenderer>(
		new MeshRenderer(mesh, material, inputLayout));
}

// インスタンス生成を禁止
MeshRenderer::MeshRenderer(
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material,
	std::shared_ptr<InputLayout> inputLayout)
{
	this->mesh = mesh;
	this->material = material;
	this->inputLayout = inputLayout;
}

// このレンダラーによる描画を実行します。
void MeshRenderer::Draw(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext)
{
	// 入力アセンブラーに頂点バッファーを設定
	immediateContext->IASetVertexBuffers(
		0, mesh->GetNumBuffers(),
		mesh->GetNativePointers(), mesh->GetStrides(), mesh->GetOffsets());
	// 入力アセンブラーにインデックス バッファーを設定
	auto indexBuffer = mesh->GetIndexBuffer();
	immediateContext->IASetIndexBuffer(
		indexBuffer->GetNativePointer().Get(),
		indexBuffer->GetFormat(), indexBuffer->GetOffset());
	// プリミティブの種類を設定
	immediateContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());
	// 入力レイアウトを設定
	immediateContext->IASetInputLayout(inputLayout->GetNativePointer().Get());

	// シェーダーを設定
	immediateContext->VSSetShader(material->GetVertexShader()->GetNativePointer().Get(), NULL, 0);
	immediateContext->GSSetShader(material->GetGeometryShader()->GetNativePointer().Get(), NULL, 0);
	immediateContext->PSSetShader(material->GetPixelShader()->GetNativePointer().Get(), NULL, 0);

	// 描画
	UINT indexCount = indexBuffer->GetCount();
	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;
	immediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

// レンダリングに使用するメッシュを取得します。
std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
	return mesh;
}

// レンダリングに使用するマテリアルを取得します。
std::shared_ptr<Material> MeshRenderer::GetMaterial()
{
	return material;
}
