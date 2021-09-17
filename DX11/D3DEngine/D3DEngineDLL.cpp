#include "D3DEngineDLL.h"

D3DEngine*			gEngine = D3DEngine::GetInstance();
Factory*			gFactory = nullptr;
ResourceManager*	gResource = nullptr;
DXInput*			gInput = nullptr;
GameTimer*			gTimer = nullptr;
KHSound*			gSound = nullptr;

namespace KH_ENGINE
{
	KH_API bool Initialize(INT_PTR hinstance, INT_PTR hwnd, int screenWidth, int screenHeight)
	{
		bool result = gEngine->Initialize(hinstance, hwnd, screenWidth, screenHeight);

		gFactory = gEngine->GetFactory();
		gResource = ResourceManager::GetInstance();
		gInput = DXInput::GetInstance();
		gTimer = GameTimer::GetInstance();
		gSound = KHSound::GetInstance();

		return result;
	}
	extern "C" KH_API void SetSolid()
	{
		gEngine->SetSolid();
	}
	extern "C" KH_API void SetWireFrame()
	{
		gEngine->SetWireFrame();
	}

	extern "C" KH_API void CameraSetLens(int screenWidth, int screenHeight)
	{
		gEngine->GetScene()->SetCameraLens(screenWidth, screenHeight);
	}

	extern "C" KH_API void DrawTextColor(int x, int y, float scale, XMFLOAT4 color, TCHAR * text, ...)
	{
		TCHAR _buffer[1024] = L"";
		va_list vl;
		va_start(vl, text);
		_vstprintf(_buffer, 1024, text, vl);
		va_end(vl);

		return gEngine->DrawTextColor(x, y, scale, color, _buffer);
	}
	extern "C" KH_API bool CheckDevice()
	{
		if (gEngine->GetDevice() == nullptr)
		{
			return false;
		}
		return true;
	}
	extern "C" KH_API void Picking(int x, int y)
	{
		gEngine->Picking(x, y);
	}
	extern "C" KH_API void Render()
	{
		gEngine->Render();
	}
	extern "C" KH_API void EndRender()
	{
		gEngine->EndRender();
	}
	extern "C" KH_API void Update()
	{
		gEngine->Update();
	}
	extern "C" KH_API void OnResize(int width, int height)
	{
		gEngine->OnResize(width, height);
	}
	extern "C" KH_API void Release()
	{
		gEngine->Release();
	}
	extern "C" KH_API POINT* GetScreenSize()
	{
		return gEngine->GetClinetSize();
	}
	extern "C" KH_API void CreateSkyBox()
	{
		gEngine->GetScene()->CreateSkyBox();
	}
	extern "C" KH_API void SetSkyBox(const char* mapName)
	{
		gEngine->GetScene()->SetSkyBox(mapName);
	}

	extern "C" KH_API void AddScene(const char* name, Scene * scene)
	{
		gEngine->AddScene(name, scene);
	}

	extern "C" KH_API Scene* FindScene(const char* name)
	{
		return gEngine->FindScene(name);
	}

	extern "C" KH_API void SelectScene(const char* name)
	{
		gEngine->SelectScene(name);
	}
	extern "C" KH_API void RemoveScene(const char* name)
	{
		gEngine->RemoveScene(name);
	}
	KH_API GameObject* FindObject(const char* objName)
	{
		return gEngine->GetScene()->FindObject(objName, eObjectType::Model);
	}
	KH_API GameObject * FindObject(const char* objName, eObjectType objType)
	{
		return gEngine->GetScene()->FindObject(objName, objType);
	}
}

namespace KH_UTILITY
{
	KH_API ResourceManager* Get()
	{
		return ResourceManager::GetInstance();
	}

	KH_API void Initialize()
	{
		gResource->Initialize();
	}

	KH_API void LoadData(eLoadType loadType, const char* objName, const char* fileName)
	{
		gResource->LoadData(loadType, objName, fileName);
	}

	KH_API void LoadData(eLoadType loadType, const char* objName, const char* fileName, bool fbxScaling)
	{
		gResource->LoadData(loadType, objName, fileName, fbxScaling);
	}

	KH_API void ResetFBX()
	{
		gResource->ResetFBX();
	}

	KH_API void CreateBoneCollider(const char* objName, eColliderType colType, float range)
	{
		gResource->CreateBoneCollider(objName, colType, range);
	}

	KH_API GameObject* CreateObject(const char* objName, eModelType modelType, DXVector3 scale, bool isCol, eModelCollider colType)
	{
		return gFactory->CreateObject(objName, modelType, scale, isCol, colType);
	}

	KH_API GameObject * CreateObject(const char* objName, eModelType modelType, DXVector3 scale, bool isCol)
	{
		return gFactory->CreateObject(objName, modelType, scale, isCol);
	}

	KH_API GameObject* CreateObject(const char* objName, eModelType modelType, DXVector3 scale)
	{
		return gFactory->CreateObject(objName, modelType, scale);
	}

	KH_API GameObject* CreateObject(const char* objName, eModelType modelType)
	{
		return gFactory->CreateObject(objName, modelType, DXVector3::One());
	}
	KH_API GameObject* CreateUI(const char* objName, eUIType uiType, const char* texKey, DXVector2 scale, DXVector2 pos, bool isCol)
	{
		return gFactory->CreateUI(objName, uiType, texKey, scale, pos, isCol);
	}
	KH_API GameObject* CreateUI(const char* objName, eUIType uiType, const char* texKey, DXVector2 scale, DXVector2 pos)
	{
		return gFactory->CreateUI(objName, uiType, texKey, scale, pos, false);
	}
	KH_API Animation* CreateAnimation(const char* objName, const char* aniName, GameObject* topNode, bool play)
	{
		return gFactory->CreateAnimation(objName, aniName, topNode, play);
	}
	KH_API Animation* CreateAnimation(const char* objName, const char* aniName, GameObject* topNode)
	{
		return gFactory->CreateAnimation(objName, aniName, topNode, false);
	}
	KH_API Animation * CreateAnimation(const char* objName, const char* aniName, const char* nodeName, bool play)
	{
		return gFactory->CreateAnimation(objName, aniName, nodeName, play);
	}
	KH_API Animation* CreateAnimation(const char* objName, const char* aniName, const char* nodeName)
	{
		return gFactory->CreateAnimation(objName, aniName, nodeName, false);
	}

	KH_API void CreateBoxCollider(GameObject* obj, DXVector3 size)
	{
		gFactory->CreateBoxCollider(obj, size);
	}

	KH_API void CreateSphereCollider(GameObject* obj, float radius)
	{
		gFactory->CreateSphereCollider(obj, radius);
	}

	extern "C" KH_API ID3D11ShaderResourceView * GetTexture(const char* name)
	{
		return ResourceManager::GetInstance()->GetTexture(name);
	}
}

namespace KH_KEYINPUT
{
	extern "C" KH_API bool IsKeyDoubleDown(BYTE Key_Input)
	{
		return gInput->IsKeyDoubleDown(Key_Input);
	}
	extern "C" KH_API bool IsKeyUP(BYTE Key_Input)
	{
		return gInput->IsKeyUP(Key_Input);
	}
	extern "C" KH_API bool IsKeyDown(BYTE Key_Input)
	{
		return gInput->IsKeyDown(Key_Input);
	}
	extern "C" KH_API bool IsKeyKeep(BYTE Key_Input)
	{
		return gInput->IsKeyKeep(Key_Input);
	}
	extern "C" KH_API bool IsKeyDownKeep(BYTE Key_Input)
	{
		return gInput->IsKeyDownKeep(Key_Input);
	}
	extern "C" KH_API void OnMouseMove(int x, int y)
	{
		gInput->OnMouseMove(x, y);
	}
	extern "C" KH_API POINT * GetMousePos()
	{
		return gInput->GetMousePos();
	}
}

namespace KH_TIME
{
	extern "C" KH_API float TotalTime()
	{
		return gTimer->TotalTime();
	}
	extern "C" KH_API float DeltaTime()
	{
		return gTimer->DeltaTime();
	}
	extern "C" KH_API void Reset()
	{
		return gTimer->Reset();
	}
	extern "C" KH_API void TimerStart()
	{
		return gTimer->Start();
	}
	extern "C" KH_API void TimerStop()
	{
		return gTimer->Stop();
	}
	extern "C" KH_API void Tick()
	{
		return gTimer->Tick();
	}
}
namespace KH_SOUND
{
	KH_API void LoadSoundBGM(const char* fileName, const char* key)
	{
		gSound->LoadSoundBGM(fileName, key);
	}
	KH_API void LoadSoundBGM(const char* fileName, const char* key, bool loop)
	{
		gSound->LoadSoundBGM(fileName, key, loop);
	}
	KH_API void LoadSoundSFX(const char* fileName, const char* key)
	{
		gSound->LoadSoundSFX(fileName, key);
	}
	KH_API void LoadSoundSFX(const char* fileName, const char* key, bool loop)
	{
		gSound->LoadSoundSFX(fileName, key, loop);
	}
	extern "C" KH_API void PlaySoundBGM(const char* key)
	{
		gSound->PlaySoundBGM(key);

	}
	extern "C" KH_API void PlaySoundSFX(const char* key)
	{
		gSound->PlaySoundSFX(key);
	}
	extern "C" KH_API void VolumeChangeBGM(float volume)
	{
		gSound->VolumeChangeBGM(volume);
	}
	extern "C" KH_API void VolumeUpBGM()
	{
		gSound->VolumeUpBGM();
	}
	extern "C" KH_API void VolumeDownBGM()
	{
		gSound->VolumeDownBGM();
	}
	extern "C" KH_API void VolumeChangeSFX(float volume)
	{
		gSound->VolumeChangeSFX(volume);
	}
	extern "C" KH_API void VolumeUpSFX()
	{
		gSound->VolumeUpSFX();
	}
	extern "C" KH_API void VolumeDownSFX()
	{
		gSound->VolumeDownSFX();
	}
	extern "C" KH_API void VolumeChangeMaster(float volume)
	{
		gSound->VolumeChangeMaster(volume);
	}
	extern "C" KH_API void VolumeUpMaster()
	{
		gSound->VolumeUpMaster();
	}
	extern "C" KH_API void VolumeDownMaster()
	{
		gSound->VolumeDownMaster();
	}
	extern "C" KH_API void StopBGM()
	{
		gSound->StopBGM();
	}
	extern "C" KH_API void PausedBGM(bool play)
	{
		gSound->PausedBGM(play);
	}
	extern "C" KH_API void StopSFX()
	{
		gSound->StopSFX();
	}
	extern "C" KH_API void PausedSFX(bool play)
	{
		gSound->PausedSFX(play);
	}
	extern "C" KH_API void StopAll()
	{
		gSound->StopAll();
	}
}

