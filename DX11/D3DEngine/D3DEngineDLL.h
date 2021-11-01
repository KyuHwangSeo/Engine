#pragma once
#pragma warning(disable : 4251)

#include "EngineDLL.h"
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
/// 내보내려는 함수/변수/클래스 앞에 export define 을 넣어도 되지만 한번 더 감싸기 위해 모아서 사용..
/// 재정의 함수를 Export 하려면 extern "C" 를 빼줘야한다..
/// 만약 static 이나 Singleton 클래스가 아닌경우 함수마다 클래스 변수를 받아서 클래스 변수의 함수를 내보내 주어야 하는것 같다.. 
/// 
/// - STL Container Export 주의점 -
///  1) Visual C++에서 STL을 Export 하려면 DLL 내부에서 사용후에 반환을 하거나
///     Export 할 템플릿과 관계된 모든 템플릿에 Export 지정을 해주어야 하는 문제가있다..
///	 2) 관계된 모든 템플릿에 Export 지정하기는 까다로우니 DLL 내에서 
///     Container 를 돌려서 반환해주어야 할 것 같다..
///  3) 예외적으로 vector 는 자주 사용되고 있어 위 과정을 거치지 않고 바로 Export 가 가능하다고 한다..
/// 
/// - DLL 할당 & 해제 주의점 -
///  1) DLL 에서 할당 후 EXE 에서 해제 -> X
///  2) EXE 에서 할당 후 DLL 에서 해제 -> X
///  3) DLL 에서 할당 후 DLL 에서 해제 -> O
///  4) EXE 에서 할당 후 EXE 에서 해제 -> O
///  -> DLL Heap 영역과 EXE Heap 영역이 다르므로 할당 & 해제는 같은 곳에서 해주어야한다고 한다..
///  
/// - 런타임 라이브러리 설정 -
///  1) 프로젝트의 구성(Debug/Release)에 따라 런타임 라이브러리가 다르다..
///  2) 런타임 라이브러리의 설정에 따라 Heap 영역이 잡히므로 주의해서 사용해야한다..
///  3) DLL 사용시 DLL과 EXE의 구성에 따른 런타임 라이브러리 설정이 같아야 한다..
///   -> 런타임 라이브러리 설정이 다를 경우 참조하는 Heap 영역이 다르므로 매우 위험하다
///
/// - warning C4251 (클라이언트에서 DLL 인터페이스를 사용하도록 지정해야 합니다) -
///  1) 위 warning이 뜨는 이유 : Export/Import를 직접적으로 하는 변수는 내부적으로도 Export/Import가 되어야 하기 때문
///   -> 현재 DirectXMath.h 에 포함되어 있는 구조체를 포함하는 클래스를 Export/Import 하면서 문제가 생긴다..
///   -> 위 warning을 제거하려면 Export/Import하는 클래스 내의 구조체도 Export/Import를 선언해주어야하는데 직접 만든 구조체가 아닌이상
///		 Export/Import 선언을 할 수가 없다..
///  2) 해결책
///	  -> 현재 사용중인 구조체를 직접 만들고 Export/Import 선언을 해주어야 한다..
///   -> 반환하는 형식으로 공정과정을 거쳐 Export/Import를 해주면 되긴 하지만 현재 Object Class의 경우 상속을 하기 위해 내보내기 때문에
///		 Class 전체를 Export/Import를 하여야 하므로 구조체를 재정의하거나 다시 만드는 방법밖에 없는 것 같다..
///
/// - DLL include 주의점 -
///  1) DLL 뿐만 아니라 헤더 내에 헤더를 포함하는 구조는 피하도록하자.. 나중에 include 지옥에 빠지지 않도록 하기 위해서라도...
///  2) 프로젝트 속성 설정으로 포함 디렉토리를 통한 include는 설정도 같이 DLL을 통해 Export 되는 것이 아니니 유의하도록 하자..
///  3) 폴더 내에 있는 헤더와 라이브러리는 상대경로를 통해 include 하도록하자..

/// - Template Export -
/// 직접적으로 템플릿을 내보낼 수는 없다.. (예외적으로 vector는 가능)
/// 어쩔수 없이 템플릿을 써야하는 상황이라면 템플릿을 재구축 해야한다.. (템플릿 함수를 DLL 내부에서 실행한 후 반환하는 형식..)
//KH_TEMPLATE template class ENGINE_DLL std::vector<int>;

namespace KH_ENGINE
{
	/// D3DEngine export 함수..
	extern "C" ENGINE_DLL bool KH_STDCALL Initialize(INT_PTR, INT_PTR, int, int);
	extern "C" ENGINE_DLL void KH_CCALL DrawTextColor(int, int, float, DXVector4, TCHAR*, ...);
	extern "C" ENGINE_DLL bool KH_STDCALL CheckDevice();
	extern "C" ENGINE_DLL void KH_STDCALL Picking(int, int);
	extern "C" ENGINE_DLL void KH_STDCALL Render();
	extern "C" ENGINE_DLL void KH_STDCALL EndRender();
	extern "C" ENGINE_DLL void KH_STDCALL Update();
	extern "C" ENGINE_DLL void KH_STDCALL OnResize(int, int);
	extern "C" ENGINE_DLL void KH_STDCALL Release();
	
	extern "C" ENGINE_DLL POINT* KH_STDCALL GetScreenSize();
	extern "C" ENGINE_DLL void KH_STDCALL CreateSkyBox();
	extern "C" ENGINE_DLL void KH_STDCALL SetSkyBox(const char*);

	template <typename T>
	T* KH_STDCALL AddScene(const char* name) { T* scene = new T; AddNewScene(name, scene); return scene; }
	extern "C" ENGINE_DLL void KH_STDCALL AddNewScene(const char*, Scene*);
	extern "C" ENGINE_DLL Scene* KH_STDCALL FindScene(const char*);
	extern "C" ENGINE_DLL void KH_STDCALL SelectScene(const char*);
	extern "C" ENGINE_DLL void KH_STDCALL RemoveScene(const char*);

	ENGINE_DLL GameObject* KH_STDCALL FindObject(const char*);
	ENGINE_DLL GameObject* KH_STDCALL FindObject(const char*, eObjectType);
}

namespace KH_UTILITY
{
	/// Utility export 함수..
	ENGINE_DLL void KH_STDCALL LoadData(eLoadType, const char*, const char*);
	ENGINE_DLL void KH_STDCALL LoadData(eLoadType, const char*, const char*, bool);
	ENGINE_DLL void KH_STDCALL ResetFBX();

	ENGINE_DLL GameObject * KH_STDCALL CreateObject(const char*, eModelType, DXVector3, bool, eModelCollider);
	ENGINE_DLL GameObject * KH_STDCALL CreateObject(const char*, eModelType, DXVector3, bool);
	ENGINE_DLL GameObject * KH_STDCALL CreateObject(const char*, eModelType, DXVector3);
	ENGINE_DLL GameObject * KH_STDCALL CreateObject(const char*, eModelType);
	ENGINE_DLL GameObject * KH_STDCALL CreateUI(const char*, eUIType, const char*, DXVector2, DXVector2, bool);
	ENGINE_DLL GameObject * KH_STDCALL CreateUI(const char*, eUIType, const char*, DXVector2, DXVector2);
	ENGINE_DLL Animation * KH_STDCALL CreateAnimation(const char*, const char*, GameObject*, bool);
	ENGINE_DLL Animation * KH_STDCALL CreateAnimation(const char*, const char*, GameObject*);
	ENGINE_DLL Animation * KH_STDCALL CreateAnimation(const char*, const char*, const char*, bool);
	ENGINE_DLL Animation * KH_STDCALL CreateAnimation(const char*, const char*, const char*);

	ENGINE_DLL void KH_STDCALL CreateBoxCollider(GameObject*, DXVector3);
	ENGINE_DLL void KH_STDCALL CreateSphereCollider(GameObject*, float);

	extern "C" ENGINE_DLL ID3D11ShaderResourceView* KH_STDCALL GetTexture(const char*);
}

namespace KH_SOUND
{
	ENGINE_DLL void KH_STDCALL LoadSoundBGM(const char*, const char*);
	ENGINE_DLL void KH_STDCALL LoadSoundBGM(const char*, const char*, bool);
	ENGINE_DLL void KH_STDCALL LoadSoundSFX(const char*, const char*);
	ENGINE_DLL void KH_STDCALL LoadSoundSFX(const char*, const char*, bool);
	extern "C" ENGINE_DLL void KH_STDCALL PlaySoundBGM(const char*);
	extern "C" ENGINE_DLL void KH_STDCALL PlaySoundSFX(const char*);
	extern "C" ENGINE_DLL void KH_STDCALL VolumeChangeBGM(float);
	extern "C" ENGINE_DLL void KH_STDCALL VolumeUpBGM();
	extern "C" ENGINE_DLL void KH_STDCALL VolumeDownBGM();
	extern "C" ENGINE_DLL void KH_STDCALL VolumeChangeSFX(float);
	extern "C" ENGINE_DLL void KH_STDCALL VolumeUpSFX();
	extern "C" ENGINE_DLL void KH_STDCALL VolumeDownSFX();
	extern "C" ENGINE_DLL void KH_STDCALL VolumeChangeMaster(float);
	extern "C" ENGINE_DLL void KH_STDCALL VolumeUpMaster();
	extern "C" ENGINE_DLL void KH_STDCALL VolumeDownMaster();
	extern "C" ENGINE_DLL void KH_STDCALL StopBGM();
	extern "C" ENGINE_DLL void KH_STDCALL PausedBGM(bool);
	extern "C" ENGINE_DLL void KH_STDCALL StopSFX();
	extern "C" ENGINE_DLL void KH_STDCALL PausedSFX(bool);
	extern "C" ENGINE_DLL void KH_STDCALL StopAll();
}

namespace KH_KEYINPUT
{
	/// KeyInput export 함수..
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyDoubleDown(BYTE);
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyUP(BYTE);
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyDown(BYTE);
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyKeep(BYTE);
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyDownKeep(BYTE);
	extern "C" ENGINE_DLL bool KH_STDCALL IsKeyInput(BYTE);
	extern "C" ENGINE_DLL void KH_STDCALL OnMouseMove(int, int);
	extern "C" ENGINE_DLL POINT * KH_STDCALL GetMousePos();
}

namespace KH_TIME
{
	/// GameTimer export 함수..
	extern "C" ENGINE_DLL float KH_STDCALL TotalTime();
	extern "C" ENGINE_DLL float KH_STDCALL DeltaTime();
	extern "C" ENGINE_DLL void KH_STDCALL Reset();
	extern "C" ENGINE_DLL void KH_STDCALL TimerStart();
	extern "C" ENGINE_DLL void KH_STDCALL TimerStop();
	extern "C" ENGINE_DLL void KH_STDCALL Tick();
}