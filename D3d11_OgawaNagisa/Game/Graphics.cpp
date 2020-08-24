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
/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
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
/// ���j�^�[�o�͂̐����ݒ���������܂��B
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
/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
/// </summary>
Adapter::Adapter(Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter)
{
	nativePointer = dxgiAdapter;

	// ���ׂẴA�_�v�^�[�o�͂�����
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
/// ���̃N���X�̐V�����C���X�^���X�����������܂��B
/// </summary>
Graphics::Graphics(
	const GraphicsSettings& settings, std::shared_ptr<GameWindow> window)
{
	try {
		// IDXGIFactory1�C���^�[�t�F�C�X���쐬
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

		// �쐬�����E�B���h�E��\�����Ă��郂�j�^�[���擾
		const auto monitor = window->GetHMonitor();

		// �A�_�v�^�[���
		HRESULT hr = S_OK;
		for (UINT adapterIndex = 0; hr != DXGI_ERROR_NOT_FOUND; adapterIndex++) {
			ComPtr<IDXGIAdapter1> dxgiAdapter;
			hr = dxgiFactory->EnumAdapters1(adapterIndex, dxgiAdapter.GetAddressOf());
			if (FAILED(hr)) {
				continue;
			}
			// �A�_�v�^�[�ɂ��Ă̋L�q���擾
			DXGI_ADAPTER_DESC1 adapterDesc = {};
			ThrowIfFailed(dxgiAdapter->GetDesc1(&adapterDesc));
			// �n�[�h�E�F�A������I��
			if (adapterDesc.Flags != DXGI_ADAPTER_FLAG_NONE) {
				continue;
			}

			// �A�_�v�^�[���쐬
			shared_ptr<Adapter> adapter(new Adapter(dxgiAdapter));

			// ���j�^�[�̃n���h������`��Ɏg�p����f�o�C�X��I��
			for (auto output : adapter->GetOutputs()) {
				if (monitor == output->GetHMonitor()) {
					this->adapter = adapter;
					this->output = output;
				}
			}

			adapters.push_back(adapter);
		}

		// �O���t�B�b�N�X�`��Ɏg�p����f�o�C�X���쐬
		graphicsDevice.reset(new GraphicsDevice(adapter, settings.creationFlags));

		// �X���b�v�`�F�[�����쐬
		swapChain.reset(new SwapChain(graphicsDevice, window, output));
		// �����_�[�^�[�Q�b�g���쐬
		renderTarget.reset(new RenderTarget(swapChain));
		// �[�x�X�e���V�����쐬
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

// �O���t�B�b�N�X �f�o�C�X���擾���܂��B
std::shared_ptr<GraphicsDevice> Graphics::GetGraphicsDevice()
{
	return graphicsDevice;
}

// �O���t�B�b�N�X �f�o�C�X���擾���܂��B
std::shared_ptr<const GraphicsDevice> Graphics::GetGraphicsDevice() const
{
	return graphicsDevice;
}

// �X���b�v�`�F�[�����擾���܂��B
std::shared_ptr<SwapChain> Graphics::GetSwapChain()
{
	return swapChain;
}

// �X���b�v�`�F�[�����擾���܂��B
std::shared_ptr<const SwapChain> Graphics::GetSwapChain() const
{
	return swapChain;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
std::shared_ptr<RenderTarget> Graphics::GetRenderTarget()
{
	return renderTarget;
}

// �����_�[�^�[�Q�b�g���擾���܂��B
std::shared_ptr<const RenderTarget> Graphics::GetRenderTarget() const
{
	return renderTarget;
}

// �[�x�X�e���V�����擾���܂��B
std::shared_ptr<DepthStencil> Graphics::GetDepthStencil()
{
	return depthStencil;
}

// �[�x�X�e���V�����擾���܂��B
std::shared_ptr<const DepthStencil> Graphics::GetDepthStencil() const
{
	return depthStencil;
}
