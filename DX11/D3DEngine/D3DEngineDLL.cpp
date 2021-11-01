#include "D3DEngineDLL.h"

D3DEngine* gEngine = D3DEngine::GetInstance();
ResourceManager* gResource = nullptr;
Factory* gFactory = nullptr;
DXInput* gInput = nullptr;
GameTimer* gTimer = nullptr;
KHSound* gSound = nullptr;

namespace KH_ENGINE
{
	ENGINE_DLL bool KH_STDCALL Initialize(INT_PTR hinstance, INT_PTR hwnd, int screenWidth, int screenHeight)
	{
		bool result = gEngine->Initialize(hinstance, hwnd, screenWidth, screenHeight);

		gResource = gEngine->GetResourceManager();
		gFactory = gEngine->GetFactory();
		gInput = gEngine->GetInput();
		gTimer = gEngine->GetTimer();
		gSound = gEngine->GetSound();

		return result;
	}
	extern "C" ENGINE_DLL void KH_CCALL DrawTextColor(int x, int y, float scale, DXVector4 color, TCHAR * text, ...)
	{
		TCHAR _buffer[1024] = L"";
		va_list vl;
		va_start(vl, text);
		_vstprintf(_buffer, 1024, text, vl);
		va_end(vl);

		return gEngine->DrawTextColor(x, y, scale, color, _buffer);
	}
	extern "C" ENGINE_DLL bool KH_STDCALL CheckDevice()
	{
		if (gEngine->GetDevice() == nullptr)
		{
			return false;
		}
		return true;
	}
	extern "C" ENGINE_DLL void KH_STDCALL Picking(int x, int y)
	{
		gEngine->Picking(x, y);
	}
	extern "C" ENGINE_DLL void KH_STDCALL Render()
	{
		gEngine->Render();
	}
	extern "C" ENGINE_DLL void KH_STDCALL EndRender()
	{
		gEngine->EndRender();
	}
	extern "C" ENGINE_DLL void KH_STDCALL Update()
	{
		gEngine->Update();
	}
	extern "C" ENGINE_DLL void KH_STDCALL OnResize(int screenWidth, int screenHeight)
	{
		gEngine->OnResize(screenWidth, screenHeight);
		gEngine->GetScene()->SetCameraLens(screenWidth, screenHeight);
	}
	extern "C" ENGINE_DLL void KH_STDCALL Release()
	{
		gEngine->Release();
	}
	extern "C" ENGINE_DLL POINT* KH_STDCALL GetScreenSize()
	{
		return gEngine->GetClinetSize();
	}
	extern "C" ENGINE_DLL void KH_STDCALL CreateSkyBox()
	{
		gEngine->GetScene()->CreateSkyBox();
	}
	extern "C" ENGINE_DLL void KH_STDCALL SetSkyBox(const char* mapName)
	{
		gEngine->GetScene()->SetSkyBox(mapName);
	}

	extern "C" ENGINE_DLL void KH_STDCALL AddNewScene(const char* name, Scene* scene)
	{
		gEngine->AddScene(name, scene);
	}

	extern "C" ENGINE_DLL Scene* KH_STDCALL FindScene(const char* name)
	{
		return gEngine->FindScene(name);
	}

	extern "C" ENGINE_DLL void KH_STDCALL SelectScene(const char* name)
	{
		gEngine->SelectScene(name);
	}
	extern "C" ENGINE_DLL void KH_STDCALL RemoveScene(const char* name)
	{
		gEngine->RemoveScene(name);
	}
	ENGINE_DLL GameObject* KH_STDCALL FindObject(const char* objName)
	{
		return gEngine->GetScene()->FindObject(objName, eObjectType::Model);
	}
	ENGINE_DLL GameObject * KH_STDCALL FindObject(const char* objName, eObjectType objType)
	{
		return gEngine->GetScene()->FindObject(objName, objType);
	}
}

namespace KH_UTILITY
{
	ENGINE_DLL void KH_STDCALL LoadData(eLoadType loadType, const char* objName, const char* fileName)
	{
		gResource->LoadData(loadType, objName, fileName);
	}

	ENGINE_DLL void KH_STDCALL LoadData(eLoadType loadType, const char* objName, const char* fileName, bool fbxScaling)
	{
		gResource->LoadData(loadType, objName, fileName, fbxScaling);
	}

	ENGINE_DLL void KH_STDCALL ResetFBX()
	{
		gResource->ResetFBX();
	}

	ENGINE_DLL GameObject* KH_STDCALL CreateObject(const char* objName, eModelType modelType, DXVector3 scale, bool isCol, eModelCollider colType)
	{
		return gFactory->CreateObject(objName, modelType, scale, isCol, colType);
	}

	ENGINE_DLL GameObject * KH_STDCALL CreateObject(const char* objName, eModelType modelType, DXVector3 scale, bool isCol)
	{
		return gFactory->CreateObject(objName, modelType, scale, isCol);
	}

	ENGINE_DLL GameObject* KH_STDCALL CreateObject(const char* objName, eModelType modelType, DXVector3 scale)
	{
		return gFactory->CreateObject(objName, modelType, scale);
	}

	ENGINE_DLL GameObject* KH_STDCALL CreateObject(const char* objName, eModelType modelType)
	{
		return gFactory->CreateObject(objName, modelType, DXVector3::One());
	}
	ENGINE_DLL GameObject* KH_STDCALL CreateUI(const char* objName, eUIType uiType, const char* texKey, DXVector2 scale, DXVector2 pos, bool isCol)
	{
		return gFactory->CreateUI(objName, uiType, texKey, scale, pos, isCol);
	}
	ENGINE_DLL GameObject* KH_STDCALL CreateUI(const char* objName, eUIType uiType, const char* texKey, DXVector2 scale, DXVector2 pos)
	{
		return gFactory->CreateUI(objName, uiType, texKey, scale, pos, false);
	}
	ENGINE_DLL Animation* KH_STDCALL CreateAnimation(const char* objName, const char* aniName, GameObject* topNode, bool play)
	{
		return gFactory->CreateAnimation(objName, aniName, topNode, play);
	}
	ENGINE_DLL Animation* KH_STDCALL CreateAnimation(const char* objName, const char* aniName, GameObject* topNode)
	{
		return gFactory->CreateAnimation(objName, aniName, topNode, false);
	}
	ENGINE_DLL Animation * KH_STDCALL CreateAnimation(const char* objName, const char* aniName, const char* nodeName, bool play)
	{
		return gFactory->CreateAnimation(objName, aniName, nodeName, play);
	}
	ENGINE_DLL Animation* KH_STDCALL CreateAnimation(const char* objName, const char* aniName, const char* nodeName)
	{
		return gFactory->CreateAnimation(objName, aniName, nodeName, false);
	}

	ENGINE_DLL void KH_STDCALL CreateBoxCollider(GameObject* obj, DXVector3 size)
	{
		gFactory->CreateBoxCollider(obj, size);
	}

	ENGINE_DLL void KH_STDCALL CreateSphereCollider(GameObject* obj, float radius)
	{
		gFactory->CreateSphereCollider(obj, radius);
	}

	extern "C" ENGINE_DLL ID3D11ShaderResourceView * KH_STDCALL GetTexture(const char* name)
	{
		return gResource->GetTexture(name);
	}
}

namespace KH_KEYINPUT
{
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyDoubleDown(BYTE Key_Input)
	{
		return gInput->IsKeyDoubleDown(Key_Input);
	}
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyUP(BYTE Key_Input)
	{
		return gInput->IsKeyUP(Key_Input);
	}
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyDown(BYTE Key_Input)
	{
		return gInput->IsKeyDown(Key_Input);
	}
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyKeep(BYTE Key_Input)
	{
		return gInput->IsKeyKeep(Key_Input);
	}
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyDownKeep(BYTE Key_Input)
	{
		return gInput->IsKeyDownKeep(Key_Input);
	}
	extern "C" ENGINE_DLL void KH_STDCALL OnMouseMove(int x, int y)
	{
		gInput->OnMouseMove(x, y);
	}
	extern "C" ENGINE_DLL POINT * KH_STDCALL GetMousePos()
	{
		return gInput->GetMousePos();
	}
}

namespace KH_TIME
{
	extern "C" ENGINE_DLL float KH_STDCALL TotalTime()
	{
		return gTimer->TotalTime();
	}
	extern "C" ENGINE_DLL float KH_STDCALL DeltaTime()
	{
		return gTimer->DeltaTime();
	}
	extern "C" ENGINE_DLL void KH_STDCALL Reset()
	{
		return gTimer->Reset();
	}
	extern "C" ENGINE_DLL void KH_STDCALL TimerStart()
	{
		return gTimer->Start();
	}
	extern "C" ENGINE_DLL void KH_STDCALL TimerStop()
	{
		return gTimer->Stop();
	}
	extern "C" ENGINE_DLL void KH_STDCALL Tick()
	{
		return gTimer->Tick();
	}
}
namespace KH_SOUND
{
	ENGINE_DLL void KH_STDCALL LoadSoundBGM(const char* fileName, const char* key)
	{
		gSound->LoadSoundBGM(fileName, key);
	}
	ENGINE_DLL void KH_STDCALL LoadSoundBGM(const char* fileName, const char* key, bool loop)
	{
		gSound->LoadSoundBGM(fileName, key, loop);
	}
	ENGINE_DLL void KH_STDCALL LoadSoundSFX(const char* fileName, const char* key)
	{
		gSound->LoadSoundSFX(fileName, key);
	}
	ENGINE_DLL void KH_STDCALL LoadSoundSFX(const char* fileName, const char* key, bool loop)
	{
		gSound->LoadSoundSFX(fileName, key, loop);
	}
	extern "C" ENGINE_DLL void KH_STDCALL PlaySoundBGM(const char* key)
	{
		gSound->PlaySoundBGM(key);

	}
	extern "C" ENGINE_DLL void KH_STDCALL PlaySoundSFX(const char* key)
	{
		gSound->PlaySoundSFX(key);
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeChangeBGM(float volume)
	{
		gSound->VolumeChangeBGM(volume);
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeUpBGM()
	{
		gSound->VolumeUpBGM();
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeDownBGM()
	{
		gSound->VolumeDownBGM();
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeChangeSFX(float volume)
	{
		gSound->VolumeChangeSFX(volume);
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeUpSFX()
	{
		gSound->VolumeUpSFX();
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeDownSFX()
	{
		gSound->VolumeDownSFX();
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeChangeMaster(float volume)
	{
		gSound->VolumeChangeMaster(volume);
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeUpMaster()
	{
		gSound->VolumeUpMaster();
	}
	extern "C" ENGINE_DLL void KH_STDCALL VolumeDownMaster()
	{
		gSound->VolumeDownMaster();
	}
	extern "C" ENGINE_DLL void KH_STDCALL StopBGM()
	{
		gSound->StopBGM();
	}
	extern "C" ENGINE_DLL void KH_STDCALL PausedBGM(bool play)
	{
		gSound->PausedBGM(play);
	}
	extern "C" ENGINE_DLL void KH_STDCALL StopSFX()
	{
		gSound->StopSFX();
	}
	extern "C" ENGINE_DLL void KH_STDCALL PausedSFX(bool play)
	{
		gSound->PausedSFX(play);
	}
	extern "C" ENGINE_DLL void KH_STDCALL StopAll()
	{
		gSound->StopAll();
	}
}

