#pragma once

#include <ppltasks.h>	// create_task의 경우

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 이 줄에 Win32 API 오류를 catch하기 위한 중단점을 설정합니다.
			throw Platform::Exception::CreateException(hr);
		}
	}

	// 이진 파일에서 비동기적으로 읽는 함수입니다.
	inline Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring& filename)
	{
		using namespace Windows::Storage;
		using namespace Concurrency;

		auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;

		return create_task(folder->GetFileAsync(Platform::StringReference(filename.c_str()))).then([] (StorageFile^ file) 
		{
			return FileIO::ReadBufferAsync(file);
		}).then([] (Streams::IBuffer^ fileBuffer) -> std::vector<byte> 
		{
			std::vector<byte> returnBuffer;
			returnBuffer.resize(fileBuffer->Length);
			Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), fileBuffer->Length));
			return returnBuffer;
		});
	}

	// DIP(디바이스 독립적 픽셀) 길이를 물리적 픽셀 길이로 변환합니다.
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // 가장 근접한 정수로 반올림합니다.
	}

#if defined(_DEBUG)
	// SDK 레이어 지원을 확인하세요.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // 실제 하드웨어 디바이스를 만들 필요가 없습니다.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // SDK 레이어를 확인하세요.
			nullptr,                    // 모든 기능 수준이 적용됩니다.
			0,
			D3D11_SDK_VERSION,          // Microsoft Store 앱의 경우 항상 이 값을 D3D11_SDK_VERSION으로 설정합니다.
			nullptr,                    // D3D 디바이스 참조를 보관할 필요가 없습니다.
			nullptr,                    // 기능 수준을 알 필요가 없습니다.
			nullptr                     // D3D 디바이스 컨텍스트 참조를 보관할 필요가 없습니다.
			);

		return SUCCEEDED(hr);
	}
#endif
}
