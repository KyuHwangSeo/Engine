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
//KH_TEMPLATE template class DLL_DECLSPEC std::vector<int>;

namespace KH_ENGINE
{
	/// D3DEngine export �Լ�..
	extern "C" DLL_DECLSPEC bool KH_STDCALL Initialize(INT_PTR, INT_PTR, int, int);
	extern "C" DLL_DECLSPEC void KH_CCALL DrawTextColor(int, int, float, DXVector4, TCHAR*, ...);
	extern "C" DLL_DECLSPEC bool KH_STDCALL CheckDevice();
	extern "C" DLL_DECLSPEC void KH_STDCALL Picking(int, int);
	extern "C" DLL_DECLSPEC void KH_STDCALL Render();
	extern "C" DLL_DECLSPEC void KH_STDCALL EndRender();
	extern "C" DLL_DECLSPEC void KH_STDCALL Update();
	extern "C" DLL_DECLSPEC void KH_STDCALL OnResize(int, int);
	extern "C" DLL_DECLSPEC void KH_STDCALL Release();
	
	extern "C" DLL_DECLSPEC POINT* KH_STDCALL GetScreenSize();
	extern "C" DLL_DECLSPEC void KH_STDCALL CreateSkyBox();
	extern "C" DLL_DECLSPEC void KH_STDCALL SetSkyBox(const char*);

	extern "C" DLL_DECLSPEC void KH_STDCALL AddScene(const char*, Scene*);
	extern "C" DLL_DECLSPEC Scene* KH_STDCALL FindScene(const char*);
	extern "C" DLL_DECLSPEC void KH_STDCALL SelectScene(const char*);
	extern "C" DLL_DECLSPEC void KH_STDCALL RemoveScene(const char*);

	DLL_DECLSPEC GameObject* KH_STDCALL FindObject(const char*);
	DLL_DECLSPEC GameObject* KH_STDCALL FindObject(const char*, eObjectType);
}

namespace KH_UTILITY
{
	/// Utility export �Լ�..
	DLL_DECLSPEC void KH_STDCALL LoadData(eLoadType, const char*, const char*);
	DLL_DECLSPEC void KH_STDCALL LoadData(eLoadType, const char*, const char*, bool);
	DLL_DECLSPEC void KH_STDCALL ResetFBX();

	DLL_DECLSPEC GameObject * KH_STDCALL CreateObject(const char*, eModelType, DXVector3, bool, eModelCollider);
	DLL_DECLSPEC GameObject * KH_STDCALL CreateObject(const char*, eModelType, DXVector3, bool);
	DLL_DECLSPEC GameObject * KH_STDCALL CreateObject(const char*, eModelType, DXVector3);
	DLL_DECLSPEC GameObject * KH_STDCALL CreateObject(const char*, eModelType);
	DLL_DECLSPEC GameObject * KH_STDCALL CreateUI(const char*, eUIType, const char*, DXVector2, DXVector2, bool);
	DLL_DECLSPEC GameObject * KH_STDCALL CreateUI(const char*, eUIType, const char*, DXVector2, DXVector2);
	DLL_DECLSPEC Animation * KH_STDCALL CreateAnimation(const char*, const char*, GameObject*, bool);
	DLL_DECLSPEC Animation * KH_STDCALL CreateAnimation(const char*, const char*, GameObject*);
	DLL_DECLSPEC Animation * KH_STDCALL CreateAnimation(const char*, const char*, const char*, bool);
	DLL_DECLSPEC Animation * KH_STDCALL CreateAnimation(const char*, const char*, const char*);

	DLL_DECLSPEC void KH_STDCALL CreateBoxCollider(GameObject*, DXVector3);
	DLL_DECLSPEC void KH_STDCALL CreateSphereCollider(GameObject*, float);

	extern "C" DLL_DECLSPEC ID3D11ShaderResourceView* KH_STDCALL GetTexture(const char*);
}

namespace KH_SOUND
{
	DLL_DECLSPEC void KH_STDCALL LoadSoundBGM(const char*, const char*);
	DLL_DECLSPEC void KH_STDCALL LoadSoundBGM(const char*, const char*, bool);
	DLL_DECLSPEC void KH_STDCALL LoadSoundSFX(const char*, const char*);
	DLL_DECLSPEC void KH_STDCALL LoadSoundSFX(const char*, const char*, bool);
	extern "C" DLL_DECLSPEC void KH_STDCALL PlaySoundBGM(const char*);
	extern "C" DLL_DECLSPEC void KH_STDCALL PlaySoundSFX(const char*);
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeChangeBGM(float);
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeUpBGM();
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeDownBGM();
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeChangeSFX(float);
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeUpSFX();
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeDownSFX();
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeChangeMaster(float);
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeUpMaster();
	extern "C" DLL_DECLSPEC void KH_STDCALL VolumeDownMaster();
	extern "C" DLL_DECLSPEC void KH_STDCALL StopBGM();
	extern "C" DLL_DECLSPEC void KH_STDCALL PausedBGM(bool);
	extern "C" DLL_DECLSPEC void KH_STDCALL StopSFX();
	extern "C" DLL_DECLSPEC void KH_STDCALL PausedSFX(bool);
	extern "C" DLL_DECLSPEC void KH_STDCALL StopAll();
}

namespace KH_KEYINPUT
{
	/// KeyInput export �Լ�..
	extern "C" DLL_DECLSPEC bool KH_STDCALL IsKeyDoubleDown(BYTE);
	extern "C" DLL_DECLSPEC bool KH_STDCALL IsKeyUP(BYTE);
	extern "C" DLL_DECLSPEC bool KH_STDCALL IsKeyDown(BYTE);
	extern "C" DLL_DECLSPEC bool KH_STDCALL IsKeyKeep(BYTE);
	extern "C" DLL_DECLSPEC bool KH_STDCALL IsKeyDownKeep(BYTE);
	extern "C" DLL_DECLSPEC bool KH_STDCALL IsKeyInput(BYTE);
	extern "C" DLL_DECLSPEC void KH_STDCALL OnMouseMove(int, int);
	extern "C" DLL_DECLSPEC POINT * KH_STDCALL GetMousePos();
}

namespace KH_TIME
{
	/// GameTimer export �Լ�..
	extern "C" DLL_DECLSPEC float KH_STDCALL TotalTime();
	extern "C" DLL_DECLSPEC float KH_STDCALL DeltaTime();
	extern "C" DLL_DECLSPEC void KH_STDCALL Reset();
	extern "C" DLL_DECLSPEC void KH_STDCALL TimerStart();
	extern "C" DLL_DECLSPEC void KH_STDCALL TimerStop();
	extern "C" DLL_DECLSPEC void KH_STDCALL Tick();
}