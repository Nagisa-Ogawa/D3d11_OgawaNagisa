#pragma once

#include <exception>
#include <string>

namespace DX
{
	class win32_exception : public std::exception
	{
	public:
		// �Ō�̃G���[���X�V���郁�\�b�h
		win32_exception(DWORD lastError)
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

	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr)
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

	// D3D11�̃G���[���O�ɕϊ�����w���p�[�֐�
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr)) {
			throw com_exception(hr);
		}
	}
}

#define SAFE_DELETE(p) { delete (p); (p) = nullptr; }
#define SAFE_DELETE_ARRAY(p) { delete[] (p); (p) = nullptr; }
#define SAFE_RELEASE(p) if ((p) != nullptr) { (p)->Release(); (p) = nullptr; }
