//=============================================================================
// Renderer.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace DX;

// インスタンス生成を禁止
MeshRenderer::MeshRenderer(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material,
	std::shared_ptr<Texture2D> texture2D)
{
	OnInitialize(graphicsDevice, mesh, material,texture2D);
}

// 初期化の際に呼び出されます。
void MeshRenderer::OnInitialize(
	std::shared_ptr<GraphicsDevice> graphicsDevice,
	std::shared_ptr<Mesh> mesh,
	std::shared_ptr<Material> material,
	std::shared_ptr<Texture2D> texture2D)
{
	this->mesh = mesh;
	this->material = material;
	this->texture2D = texture2D;
	// 入力レイアウトを作成
	inputLayout.reset(
		new InputLayout(graphicsDevice,
			mesh->GetInputElementDescs(), mesh->GetNumElements(), material->GetVertexShader()->GetShaderBytecode()));
}

// このレンダラーによる描画を実行します。
void MeshRenderer::Draw(
	ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, texture2D->GetShaderResourceView().GetAddressOf());
	deviceContext->PSSetSamplers(0, 1, texture2D->GetSamplerState().GetAddressOf());
	// 入力アセンブラーに頂点バッファーを設定
	deviceContext->IASetVertexBuffers(
		0, mesh->GetNumBuffers(),
		mesh->GetNativePointers(), mesh->GetStrides(), mesh->GetOffsets());
	// 入力アセンブラーにインデックス バッファーを設定
	auto indexBuffer = mesh->GetIndexBuffer();
	deviceContext->IASetIndexBuffer(
		indexBuffer->GetNativePointer().Get(),
		indexBuffer->GetFormat(), indexBuffer->GetOffset());
	// プリミティブの種類を設定
	deviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());
	// 入力レイアウトを設定
	deviceContext->IASetInputLayout(inputLayout->GetNativePointer().Get());

	// シェーダーを設定
	deviceContext->VSSetShader(material->GetVertexShader()->GetNativePointer().Get(), NULL, 0);
	deviceContext->GSSetShader(material->GetGeometryShader()->GetNativePointer().Get(), NULL, 0);
	deviceContext->PSSetShader(material->GetPixelShader()->GetNativePointer().Get(), NULL, 0);

	// 描画
	UINT indexCount = indexBuffer->GetCount();
	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;
	deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

// レンダリングに使用するメッシュを取得します。
std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
	return mesh;
}

// レンダリングに使用するメッシュを取得します。
std::shared_ptr<const Mesh> MeshRenderer::GetMesh() const
{
	return mesh;
}

// レンダリングに使用するマテリアルを取得します。
std::shared_ptr<Material> MeshRenderer::GetMaterial()
{
	return material;
}

// レンダリングに使用するマテリアルを取得します。
std::shared_ptr<const Material> MeshRenderer::GetMaterial() const
{
	return material;
}
