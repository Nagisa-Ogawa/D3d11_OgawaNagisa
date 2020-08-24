#pragma once

#include <exception>
#include <string>

namespace DX
{
	/// <summary>
	/// 
	/// </summary>
	class Win32Exception : public std::exception
	{
	public:
		Win32Exception(DWORD lastError)
		{
			this->lastError = lastError;
		}

		virtual const char* what() const override
		{
			static char message[1024];
			FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				lastError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g ���[�U�[����
				message, ARRAYSIZE(message),
				NULL);
			return message;
		}

	private:
		DWORD lastError;
	};

	/// <summary>
	/// Helper class for COM exceptions
	/// </summary>
	class ComException : public std::exception
	{
	public:
		ComException(HRESULT hr)
		{
			// �p�~���ꂽ�G���[�R�[�h��L���ȃG���[�R�[�h�ɒu��������
			// D3DERR_INVALIDCALL
			if (hr == 0x8876086c) {
				hResult = DXGI_ERROR_INVALID_CALL;
			}
			// D3DERR_WASSTILLDRAWING
			else if (hr == 0x8876021c) {
				hResult = DXGI_ERROR_WAS_STILL_DRAWING;
			}
			else {
				hResult = hr;
			}
		}

		virtual const char* what() const override
		{
			static char message[1024] = {};

			DWORD lastError;

			// ���s�t�@�C�������擾
			CHAR fileName[MAX_PATH] = {};
			GetModuleFileNameA(NULL, fileName, MAX_PATH);
			lastError = GetLastError();
			if (lastError != 0) {
				FormatMessageA(
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					lastError,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					message, ARRAYSIZE(message),
					NULL);
				OutputDebugStringA(message);
			}

			// HRESULT�̃G���[���b�Z�[�W���擾
			FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hResult,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				message, ARRAYSIZE(message),
				NULL);
			lastError = GetLastError();
			if (lastError != 0) {
				// HRESULT�𐔒l���當����ɕϊ�
				char hrString[sizeof(HRESULT) * 2 + 1] = {};
				sprintf_s(hrString, "%0x", hResult);

				switch (lastError) {
				case ERROR_MR_MID_NOT_FOUND: {
					// HRESULT�̃G���[���b�Z�[�W���擾�ł��Ȃ������ꍇ
					// The system cannot find message text for message number 0x%1 in the message file for %2.
					DWORD_PTR arguments[] = {
						(DWORD_PTR)hrString,	// 0x%1
						(DWORD_PTR)fileName,	// %2
					};
					FormatMessageA(
						FORMAT_MESSAGE_ARGUMENT_ARRAY |
						FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						lastError,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						message, ARRAYSIZE(message),
						(va_list*)&arguments);
					break;
				}

				default:
					FormatMessageA(
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						lastError,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						message, ARRAYSIZE(message),
						NULL);
					break;
				}
			}

			return message;
		}

	private:
		HRESULT hResult;
	};

	/// <summary>
	/// BOOL�^��FALSE���w�肳�ꂽ�ۂɗ�O�ɕϊ�����w���p�[�֐��ł��B
	/// </summary>
	inline void ThrowIfFailed(BOOL result)
	{
		if (result == FALSE) {
			auto lastError = GetLastError();
			if (lastError != 0) {
				throw Win32Exception(lastError);
			}
		}
	}

	/// <summary>
	/// bool�^��false���w�肳�ꂽ�ۂɗ�O�ɕϊ�����w���p�[�֐��ł��B
	/// </summary>
	inline void ThrowIfFailed(bool result)
	{
		if (!result) {
			auto lastError = GetLastError();
			if (lastError != 0) {
				throw Win32Exception(lastError);
			}
		}
	}

	/// <summary>
	/// D3D11�̃G���[���O�ɕϊ�����w���p�[�֐��ł��B
	/// </summary>
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr)) {
			throw ComException(hr);
		}
	}

	template <typename T>
	inline void SafeDelete(T*& p)
	{
		delete p;
		p = nullptr;
	}

	template <typename T>
	inline void SafeDeleteArray(T*& p)
	{
		delete[] p;
		p = nullptr;
	}

	template <typename T>
	inline void SafeRelease(T*& p)
	{
		if (p != nullptr) {
			p->Release();
			p = nullptr;
		}
	}
}
