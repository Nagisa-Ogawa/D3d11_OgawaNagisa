//=============================================================================
// Graphics.cpp
// 
//=============================================================================
#include "stdafx.h"
#include "Graphics.h"
#include "DirectXHelper.h"
#include <vector>

using namespace std;
using namespace Microsoft::WRL;
using namespace DX;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
Output::Output(Microsoft::WRL::ComPtr<IDXGIOutput> dxgiOutput)
{
	nativePointer = dxgiOutput;
}

Microsoft::WRL::ComPtr<IDXGIOutput> Output::GetNativePointer()
{
	return nativePointer;
}

DXGI_OUTPUT_DESC Output::GetDesc() const
{
	DXGI_OUTPUT_DESC desc = {};
	ThrowIfFailed(nativePointer->GetDesc(&desc));
	return desc;
}

HMONITOR Output::GetHMonitor() const
{
	return GetDesc().Monitor;
}

/// <summary>
/// モニター出力の推奨設定を検索します。
/// </summary>
DXGI_MODE_DESC Output::FindClosestMatchingMode(
	UINT width, UINT height, std::shared_ptr<GraphicsDevice> graphicsDevice) const
{
	DXGI_MODE_DESC modeToMatch = {};
	modeToMatch.Width = width;
	modeToMatch.Height = height;
	DXGI_MODE_DESC closestMatch = {};
	ThrowIfFailed(nativePointer->FindClosestMatchingMode(
		&modeToMatch, &closestMatch, graphicsDevice->GetDevice().Get()));
	return closestMatch;
}

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
Adapter::Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter)
{
	nativePointer = dxgiAdapter;

	// すべてのアダプター出力を検索
	HRESULT hr = S_OK;
	for (UINT outputIndex = 0; hr != DXGI_ERROR_NOT_FOUND; outputIndex++) {
		ComPtr<IDXGIOutput> dxgiOutput;
		hr = dxgiAdapter->EnumOutputs(outputIndex, &dxgiOutput);
		if (FAILED(hr)) {
			continue;
		}
		outputs.push_back(shared_ptr<Output>(new Output(dxgiOutput)));
	}
}

Microsoft::WRL::ComPtr<IDXGIAdapter1> Adapter::GetNativePointer()
{
	return nativePointer;
}

DXGI_ADAPTER_DESC1 Adapter::GetDesc() const
{
	DXGI_ADAPTER_DESC1 desc = {};
	ThrowIfFailed(nativePointer->GetDesc1(&desc));
	return desc;
}

std::shared_ptr<Output> Adapter::GetOutput(int output)
{
	return outputs[output];
}

std::vector<std::shared_ptr<Output>>& Adapter::GetOutputs()
{
	return outputs;
}

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
Graphics::Graphics(
	const GraphicsSettings& settings, std::shared_ptr<GameWindow> window)
{
	try {
		// IDXGIFactory1インターフェイスを作成
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

		// 作成したウィンドウを表示しているモニターを取得
		const auto monitor = window->GetHMonitor();

		// アダプターを列挙
		HRESULT hr = S_OK;
		for (UINT adapterIndex = 0; hr != DXGI_ERROR_NOT_FOUND; adapterIndex++) {
			ComPtr<IDXGIAdapter1> dxgiAdapter;
			hr = dxgiFactory->EnumAdapters1(adapterIndex, dxgiAdapter.GetAddressOf());
			if (FAILED(hr)) {
				continue;
			}
			// アダプターについての記述を取得
			DXGI_ADAPTER_DESC1 adapterDesc = {};
			ThrowIfFailed(dxgiAdapter->GetDesc1(&adapterDesc));
			// ハードウェアだけを選択
			if (adapterDesc.Flags != DXGI_ADAPTER_FLAG_NONE) {
				continue;
			}

			// アダプターを作成
			shared_ptr<Adapter> adapter(new Adapter(dxgiAdapter));

			// モニターのハンドルから描画に使用するデバイスを選択
			for (auto output : adapter->GetOutputs()) {
				if (monitor == output->GetHMonitor()) {
					this->adapter = adapter;
					this->output = output;
				}
			}

			adapters.push_back(adapter);
		}

		// グラフィックス描画に使用するデバイスを作成
		graphicsDevice.reset(new GraphicsDevice(adapter, settings.creationFlags));

		// スワップチェーンを作成
		swapChain.reset(new SwapChain(graphicsDevice, window, output));
		// レンダーターゲットを作成
		renderTarget.reset(new RenderTarget(swapChain));
		// 深度ステンシルを作成
		depthStencil.reset(new DepthStencil(renderTarget, settings.depthStencilFormat));
	}
	catch (...) {
		dxgiFactory.Reset();
		for (auto& adapter : adapters) {
			adapter.reset();
		}
		adapters.clear();
		graphicsDevice.reset();
		swapChain.reset();
		renderTarget.reset();
		depthStencil.reset();
		throw;
	}
}

// グラフィックス デバイスを取得します。
std::shared_ptr<GraphicsDevice> Graphics::GetGraphicsDevice()
{
	return graphicsDevice;
}

// グラフィックス デバイスを取得します。
std::shared_ptr<const GraphicsDevice> Graphics::GetGraphicsDevice() const
{
	return graphicsDevice;
}

// スワップチェーンを取得します。
std::shared_ptr<SwapChain> Graphics::GetSwapChain()
{
	return swapChain;
}

// スワップチェーンを取得します。
std::shared_ptr<const SwapChain> Graphics::GetSwapChain() const
{
	return swapChain;
}

// レンダーターゲットを取得します。
std::shared_ptr<RenderTarget> Graphics::GetRenderTarget()
{
	return renderTarget;
}

// レンダーターゲットを取得します。
std::shared_ptr<const RenderTarget> Graphics::GetRenderTarget() const
{
	return renderTarget;
}

// 深度ステンシルを取得します。
std::shared_ptr<DepthStencil> Graphics::GetDepthStencil()
{
	return depthStencil;
}

// 深度ステンシルを取得します。
std::shared_ptr<const DepthStencil> Graphics::GetDepthStencil() const
{
	return depthStencil;
}
