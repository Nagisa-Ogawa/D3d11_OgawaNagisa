//=============================================================================
// Textures.cpp
// 
//=============================================================================
#include "stdafx.h"
#include <wincodec.h>
#include "Graphics.h"
#include "DirectXHelper.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace DX;

// このクラスの新しいインスタンスを初期化します。
Image::Image(LPCWSTR fileName)
{
	try {
		// 
		ComPtr<IWICImagingFactory> factory;
		ThrowIfFailed(CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(factory.GetAddressOf())));
		// 
		ComPtr<IWICBitmapDecoder> decoder;
		ThrowIfFailed(factory->CreateDecoderFromFilename(
			fileName,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			decoder.GetAddressOf()));
		// 
		UINT frameCount = 0;
		ThrowIfFailed(decoder->GetFrameCount(&frameCount));

		// 
		for (UINT index = 0; index < frameCount; index++) {
			ComPtr<IWICBitmapFrameDecode> bitmapFrame;
			ThrowIfFailed(decoder->GetFrame(index, &bitmapFrame));
			// 
			//UINT width, height;
			ThrowIfFailed(bitmapFrame->GetSize(&width, &height));
			// 
			WICPixelFormatGUID pixelFormat;
			ThrowIfFailed(bitmapFrame->GetPixelFormat(&pixelFormat));

			UINT bufferSize = width * height * 4;
			data.reset(new BYTE[bufferSize], default_delete<BYTE[]>());

			// 
			if (pixelFormat != GUID_WICPixelFormat32bppRGBA) {
				// 変換する
				Microsoft::WRL::ComPtr<IWICFormatConverter> formatConverter;
				ThrowIfFailed(
					factory->CreateFormatConverter(formatConverter.GetAddressOf()));

				ThrowIfFailed(formatConverter->Initialize(
					bitmapFrame.Get(), GUID_WICPixelFormat32bppRGBA,
					WICBitmapDitherTypeErrorDiffusion,
					0, 0, WICBitmapPaletteTypeCustom));

				// copy
				ThrowIfFailed(
					formatConverter->CopyPixels(nullptr, width * 4, bufferSize, data.get()));
			}
			else {
				// copy
				ThrowIfFailed(
					bitmapFrame->CopyPixels(nullptr, width * 4, bufferSize, data.get()));
			}
		}
	}
	catch (...) {
		data.reset();
		throw;
	}
}

UINT Image::GetWidth() const
{
	return width;
}

UINT Image::GetHeight() const
{
	return height;
}

UINT Image::GetStride() const
{
	return width * pixelSize;
}

UINT Image::GetPixelSize() const
{
	return pixelSize;
}

UINT Image::GetSize() const
{
	return width * height * pixelSize;
}

const BYTE* Image::GetPixels() const
{
	return data.get();
}

// このクラスの新しいインスタンスを初期化します。
Texture2D::Texture2D(
	std::shared_ptr<GraphicsDevice> graphicsDevice, LPCWSTR fileName)
	: GraphicsResource(graphicsDevice)
{
	try {
		// テクスチャーについての記述
		D3D11_TEXTURE2D_DESC texture2dDesc = {};
		texture2dDesc.Width = 1024;
		texture2dDesc.Height = 1024;
		texture2dDesc.MipLevels = 1;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2dDesc.SampleDesc = { 1, 0 };
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture2dDesc.CPUAccessFlags = 0;
		texture2dDesc.MiscFlags = 0;
		// サブリソース初期化についての記述
		D3D11_SUBRESOURCE_DATA initialData = {};
		shared_ptr<Image> image(new Image(fileName));
		initialData.pSysMem = image->GetPixels();
		initialData.SysMemPitch = image->GetStride();
		initialData.SysMemSlicePitch = image->GetSize();
		// テクスチャーを作成
		ThrowIfFailed(graphicsDevice->GetDevice()->CreateTexture2D(
			&texture2dDesc, &initialData, texture.GetAddressOf()));

		//D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
		//ThrowIfFailed(graphicsDevice->GetImmediateContext()->Map(
		//	texture.Get(), 0, D3D11_MAP_WRITE, 0, &mappedSubresource));
		//mappedSubresource.pData;

		//graphicsDevice->GetImmediateContext()->Unmap(texture.Get(), 0);


		// シェイダー リソース ビューについての記述
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = texture2dDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipLevels = texture2dDesc.MipLevels;
		// シェイダー リソース ビューを作成
		ThrowIfFailed(graphicsDevice->GetDevice()->CreateShaderResourceView(
			texture.Get(), &viewDesc, view.GetAddressOf()));

		// テクスチャー サンプラーについての記述
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		// テクスチャー サンプラーを作成
		ThrowIfFailed(graphicsDevice->GetDevice()->CreateSamplerState(
			&samplerDesc, samplerState.GetAddressOf()));
	}
	catch (const ComException& ex) {
		OutputDebugStringA(ex.what());
		texture.Reset();
		view.Reset();
		samplerState.Reset();
		throw;
	}
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture2D::GetShaderResourceView() const
{
	return this->view;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Texture2D::GetSamplerState() const
{
	return this->samplerState;
}
