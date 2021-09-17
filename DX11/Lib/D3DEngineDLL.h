#pragma once
#pragma warning(disable : 4251)

#include "DLLDefine.h"
#include "D3DDefine.h"
#include "D3DEngine.h"
#include "GameTimer.h"
#include "KHSound.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Patch_Chapters.h"
#include "DXTKFont.h"
#include "Shader.h"
#include "ResourceManager.h"

/// Component Header
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "Gizmos.h"
#include "Material.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "SkinMeshRenderer.h"
#include "CanvasRenderer.h"
#include "Animation.h"
#include "Animator.h"
#include "Camera.h"
#include "Light.h"
#include "SkyBox.h"
#include "UI_Image.h"
#include "UI_Button.h"

/// Manager Header
#include "IManager.h"
#include "ObjectManager.h"
#include "HelperManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "MaterialManager.h"
#include "CollisionManager.h"
#include "UIManager.h"
#include "Factory.h"

/// Scene Header
#include "Scene.h"
#include "SceneManager.h"

/// <summary>
/// DLL Export
/// </summary>
/// 
/// ���������� �Լ�/����/Ŭ���� �տ� export define �� �־ ������ �ѹ� �� ���α� ���� ��Ƽ� ���..
/// ������ �Լ��� Export �Ϸ��� extern "C" �� ������Ѵ�..
/// ���� static �̳� Singleton Ŭ������ �ƴѰ�� �Լ����� Ŭ���� ������ �޾Ƽ� Ŭ���� ������ �Լ��� ������ �־�� �ϴ°� ����.. 
/// 
/// - STL Container Export ������ -
///  1) Visual C++���� STL�� Export �Ϸ��� DLL ���ο��� ����Ŀ� ��ȯ�� �ϰų�
///     Export �� ���ø��� ����� ��� ���ø��� Export ������ ���־�� �ϴ� �������ִ�..
///	 2) ����� ��� ���ø��� Export �����ϱ�� ��ٷο�� DLL ������ 
///     Container �� ������ ��ȯ���־�� �� �� ����..
///  3) ���������� vector �� ���� ���ǰ� �־� �� ������ ��ġ�� �ʰ� �ٷ� Export �� �����ϴٰ� �Ѵ�..
/// 
/// - DLL �Ҵ� & ���� ������ -
///  1) DLL ���� �Ҵ� �� EXE ���� ���� -> X
///  2) EXE ���� �Ҵ� �� DLL ���� ���� -> X
///  3) DLL ���� �Ҵ� �� DLL ���� ���� -> O
///  4) EXE ���� �Ҵ� �� EXE ���� ���� -> O
///  -> DLL Heap ������ EXE Heap ������ �ٸ��Ƿ� �Ҵ� & ������ ���� ������ ���־���Ѵٰ� �Ѵ�..
///  
/// - ��Ÿ�� ���̺귯�� ���� -
///  1) ������Ʈ�� ����(Debug/Release)�� ���� ��Ÿ�� ���̺귯���� �ٸ���..
///  2) ��Ÿ�� ���̺귯���� ������ ���� Heap ������ �����Ƿ� �����ؼ� ����ؾ��Ѵ�..
///  3) DLL ���� DLL�� EXE�� ������ ���� ��Ÿ�� ���̺귯�� ������ ���ƾ� �Ѵ�..
///   -> ��Ÿ�� ���̺귯�� ������ �ٸ� ��� �����ϴ� Heap ������ �ٸ��Ƿ� �ſ� �����ϴ�
///
/// - warning C4251 (Ŭ���̾�Ʈ���� DLL �������̽��� ����ϵ��� �����ؾ� �մϴ�) -
///  1) �� warning�� �ߴ� ���� : Export/Import�� ���������� �ϴ� ������ ���������ε� Export/Import�� �Ǿ�� �ϱ� ����
///   -> ���� DirectXMath.h �� ���ԵǾ� �ִ� ����ü�� �����ϴ� Ŭ������ Export/Import �ϸ鼭 ������ �����..
///   -> �� warning�� �����Ϸ��� Export/Import�ϴ� Ŭ���� ���� ����ü�� Export/Import�� �������־���ϴµ� ���� ���� ����ü�� �ƴ��̻�
///		 Export/Import ������ �� ���� ����..
///  2) �ذ�å
///	  -> ���� ������� ����ü�� ���� ����� Export/Import ������ ���־�� �Ѵ�..
///   -> ��ȯ�ϴ� �������� ���������� ���� Export/Import�� ���ָ� �Ǳ� ������ ���� Object Class�� ��� ����� �ϱ� ���� �������� ������
///		 Class ��ü�� Export/Import�� �Ͽ��� �ϹǷ� ����ü�� �������ϰų� �ٽ� ����� ����ۿ� ���� �� ����..
///
/// - DLL include ������ -
///  1) DLL �Ӹ� �ƴ϶� ��� ���� ����� �����ϴ� ������ ���ϵ�������.. ���߿� include ������ ������ �ʵ��� �ϱ� ���ؼ���...
///  2) ������Ʈ �Ӽ� �������� ���� ���丮�� ���� include�� ������ ���� DLL�� ���� Export �Ǵ� ���� �ƴϴ� �����ϵ��� ����..
///  3) ���� ���� �ִ� ����� ���̺귯���� ����θ� ���� include �ϵ�������..

/// - Template Export -
/// ���������� ���ø��� ������ ���� ����.. (���������� vector�� ����)
/// ��¿�� ���� ���ø��� ����ϴ� ��Ȳ�̶�� ���ø��� �籸�� �ؾ��Ѵ�.. (���ø� �Լ��� DLL ���ο��� ������ �� ��ȯ�ϴ� ����..)
//KH_TEMPLATE template class KH_API std::vector<int>;

namespace KH_ENGINE
{
	/// D3DEngine export �Լ�..
	KH_API bool Initialize(INT_PTR, INT_PTR, int, int);
	extern "C" KH_API void SetSolid();
	extern "C" KH_API void SetWireFrame();
	extern "C" KH_API void CameraSetLens(int, int);
	extern "C" KH_API void DrawTextColor(int, int, float, XMFLOAT4, TCHAR*, ...);
	extern "C" KH_API bool CheckDevice();
	extern "C" KH_API void Picking(int, int);
	extern "C" KH_API void Render();
	extern "C" KH_API void EndRender();
	extern "C" KH_API void Update();
	extern "C" KH_API void OnResize(int, int);
	extern "C" KH_API void Release();
	
	extern "C" KH_API POINT* GetScreenSize();
	extern "C" KH_API void CreateSkyBox();
	extern "C" KH_API void SetSkyBox(const char*);

	extern "C" KH_API void AddScene(const char*, Scene*);
	extern "C" KH_API Scene* FindScene(const char*);
	extern "C" KH_API void SelectScene(const char*);
	extern "C" KH_API void RemoveScene(const char*);

	KH_API GameObject* FindObject(const char*);
	KH_API GameObject* FindObject(const char*, eObjectType);
}

namespace KH_UTILITY
{
	/// Utility export �Լ�..
	KH_API ResourceManager* Get();
	KH_API void Initialize();
	KH_API void LoadData(eLoadType, const char*, const char*);
	KH_API void LoadData(eLoadType, const char*, const char*, bool);
	KH_API void ResetFBX();
	KH_API void CreateBoneCollider(const char*, eColliderType, float);
	KH_API GameObject * CreateObject(const char*, eModelType, DXVector3, bool, eModelCollider);
	KH_API GameObject * CreateObject(const char*, eModelType, DXVector3, bool);
	KH_API GameObject * CreateObject(const char*, eModelType, DXVector3);
	KH_API GameObject * CreateObject(const char*, eModelType);
	KH_API GameObject* CreateUI(const char*, eUIType, const char*, DXVector2, DXVector2, bool);
	KH_API GameObject* CreateUI(const char*, eUIType, const char*, DXVector2, DXVector2);
	KH_API Animation * CreateAnimation(const char*, const char*, GameObject*, bool);
	KH_API Animation * CreateAnimation(const char*, const char*, GameObject*);
	KH_API Animation * CreateAnimation(const char*, const char*, const char*, bool);
	KH_API Animation * CreateAnimation(const char*, const char*, const char*);
	KH_API void CreateBoxCollider(GameObject*, DXVector3);
	KH_API void CreateSphereCollider(GameObject*, float);
	extern "C" KH_API ID3D11ShaderResourceView* GetTexture(const char*);
}

namespace KH_SOUND
{
	KH_API void LoadSoundBGM(const char*, const char*);
	KH_API void LoadSoundBGM(const char*, const char*, bool);
	KH_API void LoadSoundSFX(const char*, const char*);
	KH_API void LoadSoundSFX(const char*, const char*, bool);
	extern "C" KH_API void PlaySoundBGM(const char*);
	extern "C" KH_API void PlaySoundSFX(const char*);
	extern "C" KH_API void VolumeChangeBGM(float);
	extern "C" KH_API void VolumeUpBGM();
	extern "C" KH_API void VolumeDownBGM();
	extern "C" KH_API void VolumeChangeSFX(float);
	extern "C" KH_API void VolumeUpSFX();
	extern "C" KH_API void VolumeDownSFX();
	extern "C" KH_API void VolumeChangeMaster(float);
	extern "C" KH_API void VolumeUpMaster();
	extern "C" KH_API void VolumeDownMaster();
	extern "C" KH_API void StopBGM();
	extern "C" KH_API void PausedBGM(bool);
	extern "C" KH_API void StopSFX();
	extern "C" KH_API void PausedSFX(bool);
	extern "C" KH_API void StopAll();
}

namespace KH_KEYINPUT
{
	/// KeyInput export �Լ�..
	extern "C" KH_API bool IsKeyDoubleDown(BYTE);
	extern "C" KH_API bool IsKeyUP(BYTE);
	extern "C" KH_API bool IsKeyDown(BYTE);
	extern "C" KH_API bool IsKeyKeep(BYTE);
	extern "C" KH_API bool IsKeyDownKeep(BYTE);
	extern "C" KH_API bool IsKeyInput(BYTE);
	extern "C" KH_API void OnMouseMove(int, int);
	extern "C" KH_API POINT * GetMousePos();
}

namespace KH_TIME
{
	/// GameTimer export �Լ�..
	extern "C" KH_API float TotalTime();
	extern "C" KH_API float DeltaTime();
	extern "C" KH_API void Reset();
	extern "C" KH_API void TimerStart();
	extern "C" KH_API void TimerStop();
	extern "C" KH_API void Tick();
}