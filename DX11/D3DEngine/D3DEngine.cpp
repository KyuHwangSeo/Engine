#include "framework.h"
#include "DLLDefine.h"
#include "D3DDefine.h"
#include "DXTKFont.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "GameTimer.h"
#include "D3DEngine.h" 
#include "ResourceManager.h"
#include "KHSound.h"

/// Component Header
#include "Gizmos.h"
#include "Material.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "SkinMeshRenderer.h"
#include "CanvasRenderer.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Light.h"
#include "Animation.h"
#include "Animator.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SkyBox.h"

/// Manager Header
#include "IManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "HelperManager.h"
#include "ObjectManager.h"
#include "MaterialManager.h"
#include "CollisionManager.h"
#include "UIManager.h"
#include "Factory.h"

/// Renderer Header
#include "IRenderer.h"
#include "RenderTarget.h"
#include "Forward.h"
#include "PostProcessing.h"
#include "Shadow.h"
#include "Deferred.h"
#include "SSAO.h"
#include "DebugViewPort.h"
#include "MotionBlur.h"
#include "DepthStencilState.h"
#include "DepthStencilView.h"
#include "DepthStencilManager.h"
#include "D3DRenderer.h"

/// Scene Header
#include "Scene.h"
#include "SceneManager.h"

D3DEngine::D3DEngine()
{
	m_Font = new DXTKFont();
}

D3DEngine::~D3DEngine()
{
}

bool D3DEngine::Initialize(INT_PTR hinstance, INT_PTR hwnd, int screenWidth, int screenHeight)
{

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 메인 핸들 저장..
	m_Mainhwnd = (HWND)hwnd;

	// 현재 화면 크기 업데이트..
	m_ClientSize = new POINT;
	m_ClientSize->x = screenWidth;
	m_ClientSize->y = screenHeight;

	// Device 생성..
	CreateDevice();

	// Rasterizer 상태 설정 및 생성..
	CreateRenderStates();

	// ResourceManager 초기화..
	ResourceManager::GetInstance()->Initialize();

	// Renderer 생성 및 초기화..
	m_Renderer = new D3DRenderer(m_Device, m_DeviceContext, m_SwapChain.Get());
	m_Renderer->Initialize();

	// 폰트 생성
	m_Font->Create(m_Device.Get());

	// GameTimer 초기화..
	GameTimer::GetInstance()->Reset();

	// Sound 초기화..
	KHSound::GetInstance()->Initialize();

	// Layer List 생성..
	LayerList::Initialize();

	// Factroy 생성 및 초기화..
	m_Factory = new Factory();

	// SceneManager 생성 및 초기화..
	m_SceneManager = new SceneManager();

	return true;
}

void D3DEngine::CreateDevice()
{
	// 스왑 체인 설정을 초기화..
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// swapChainDesc을 단일 백 버퍼로 설정..
	swapChainDesc.BufferCount = 1;

	// 백 버퍼의 너비와 높이를 설정..
	swapChainDesc.BufferDesc.Width = m_ClientSize->x;
	swapChainDesc.BufferDesc.Height = m_ClientSize->y;

	// 백 버퍼에 일반 32 비트 표면을 설정..
	/// - DXGI_FORMAT_R8G8B8A8_UNORM
	///  -> 알파를 포함하여 채널당 8 비트를 지원하는 4 성분, 32 비트 부호없는 정규화 정수 형식
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 백 버퍼의 재생률을 설정..
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 백 버퍼의 사용 설정..
	/// - DXGI_USAGE_RENDER_TARGET_OUTPUT
	///  1. 표면 또는 리소스는 백 버퍼로 사용됩니다. 
	///  2. 스왑 체인을 생성 할 때 DXGI_USAGE_BACK_BUFFER 를 전달할 필요가 없습니다. 
	///  3. 호출 할 때 자원이 스왑 체인에 속해 있는지 여부를 확인할 수 있습니다. 
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.OutputWindow = (HWND)m_Mainhwnd;

	// 멀티 샘플링을 끕니다..
	/// - DXGI_SAMPLE_DESC 
	///  -> 리소스에 대한 다중 샘플링 매개 변수 설정하는 구조체
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;	// 전체 화면 또는 창 모드로 설정

	// 스캔 라인 순서 및 배율을 지정되지 않음으로 설정..
	/// - DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED
	///  -> 래스터가 표면에 이미지를 만드는 데 사용하는 방법을 나타내는 플래그
	/// - DXGI_MODE_SCALING_UNSPECIFIED
	///  -> 주어진 모니터의 해상도에 맞게 이미지가 늘어나는 방식을 나타내는 플래그
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/// DXGI_SWAP_EFFECT_DISCARD -> IDXGISwapChain1 :: Present1 호출 후 디스플레이 표면의 픽셀을 처리하는 옵션
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 플래그 설정 안 함..
	swapChainDesc.Flags = 0;

	// featureLevel을 DirectX 11로 설정..
	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑 체인, Direct3D 장치 및 Direct3D 장치 컨텍스트 생성..
	/// MSDN에선 SwapChain 과 Device 를 한번에 생성해주는 함수를 지향함..
	HR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &m_FeatureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext));
}

void D3DEngine::CreateHelper()
{
	// Camera 생성 및 초기화..
	GameObject* cam1 = m_Factory->CreateObject("Camera1", eModelType::Camera);
	cam1->GetTransform()->SetScale(DXVector3(0.5f, 0.5f, 0.5f));
	cam1->GetComponent<Camera>()->SetType(eCameraType::NormalCam);

	GameObject* cam2 = m_Factory->CreateObject("Camera2", eModelType::Camera);
	cam2->GetTransform()->SetScale(DXVector3(0.5f, 0.5f, 0.5f));
	cam2->GetComponent<Camera>()->SetType(eCameraType::FollowCam_3);

	// Main Camera 설정..
	Camera::g_MainCamera = cam2->GetComponent<Camera>();

	// HerlperObject 생성 및 초기화..
#ifdef _DEBUG
	m_Factory->CreateObject("Grid", eModelType::Grid);
	m_Factory->CreateObject("Axis", eModelType::Axis);
#endif

	// 기본 Directional Light 생성..
	GameObject* light1 = m_Factory->CreateObject("Direction Light", eModelType::DirLight);
	light1->GetTransform()->SetScale(DXVector3(10.0f, 10.0f, 10.0f));
	light1->GetTransform()->MoveLocal(DXVector3(0.0f, 15.0f, 0.0f));

	// Direction Light 설정..
	Light::g_DirLight = light1->GetComponent<Light>();
}

void D3DEngine::EndRender()
{
	m_Renderer->EndRender();

	// 최종 렌더링 된 이미지를 그린다..
	m_SwapChain->Present(0, 0);
}

void D3DEngine::SceneStart()
{
	// 기본 오브젝트 생성..
	CreateHelper();

	// 창 사이즈에 대한 재설정..
	OnResize(m_ClientSize->x, m_ClientSize->y);

	// Scene 내에 존재하는 GameObject 생성..
	m_Scene->Create();

	// Scene 내에 존재하는 GameObject 설정..
	m_Scene->Start();

	GameTimer::GetInstance()->Tick();
}

void D3DEngine::Update()
{
	GameTimer::GetInstance()->Tick();
	dTime = GameTimer::GetInstance()->DeltaTime();

	KHSound::GetInstance()->Update();

	CheckKeyInput();

	// Scene Update..
	m_Scene->SceneUpdate(dTime);

	// 현재 Scene Object Update..
	m_Scene->Update(dTime);

	// 후 순위 Scene Object Update..
	m_Scene->FinalUpdate(dTime);
}

void D3DEngine::Render()
{
	// 현재 Scene 오브젝트 그리기..
	m_Scene->Render();

	// 현재 Scene DebugDraw..
	if (m_DebugMode)
	{
		m_Scene->DebugDraw();

		// 폰트를 이용한 텍스트 출력..
		Draw_Text();
	}
}

void D3DEngine::Draw_Text()
{
	//if (D3DEngine::GetInstance()->GetDebug() == false) return;

	DXVector4 _white(1.0f, 1.0f, 1.0f, 1.0f);	// white
	DXVector4 _orange(0.9f, 0.5f, 0.2f, 1.0f);
	DXVector4 _red(0.9f, 0.3f, 0.3f, 1.0f);
	DXVector4 _yellow(0.9f, 0.9f, 0.3f, 1.0f);

	// 갱신주기
	static float _addedTime = 0;
	static float _FPS = 0;
	static float _deltaTimeMS = 0;

	float deltaTime = GameTimer::GetInstance()->DeltaTime();

	// 갱신주기는 0.2초
	if (0.2f < _addedTime)
	{
		_FPS = (1.0f / deltaTime);
		_deltaTimeMS = deltaTime * 1000.0f;

		_addedTime = 0;
	}

	_addedTime += (deltaTime);

	int _yPos = 20;
	int _xPos = m_ClientSize->x / 2 - 100;

	const wchar_t* cameraType = nullptr;
	const wchar_t* cameraStr = nullptr;

	_yPos = 10;

	DXVector3 pos;
	if (Camera::g_MainCamera->m_Target != nullptr)
	{
		pos = Camera::g_MainCamera->GetTargetPos();
	}

	m_Font->DrawTextColorE(10, _yPos, 1.0f, _white, (TCHAR*)L"FPS : %.2f", _FPS);
	m_Font->DrawTextColorE(10, _yPos += 18, 1.0f, _white, (TCHAR*)L"DTime : %.4f ms", _deltaTimeMS);
	m_Font->DrawTextColorE(10, _yPos += 18, 1.0f, _white, (TCHAR*)L"Pos : %.2f / %.2f / %.2f", pos.x, pos.y, pos.z);
	m_Font->DrawTextColorE(10, _yPos += 18, 1.0f, _white, (TCHAR*)L"Debug : F5");
}

void D3DEngine::CheckKeyInput()
{
	if (DXInput::GetInstance()->IsKeyUP(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if (DXInput::GetInstance()->IsKeyUP(VK_F4))
	{
		m_Renderer->ResetSSAO();
		m_Renderer->Is_SSAO = !m_Renderer->Is_SSAO;
	}
	if (DXInput::GetInstance()->IsKeyUP(VK_F5))
	{
		m_DebugMode = !m_DebugMode;
		m_Renderer->Is_Debug = !m_Renderer->Is_Debug;
	}
}

void D3DEngine::AddScene(const char* name, Scene* scene)
{
	// Scene 추가..
	m_SceneManager->AddScene(name, scene);

	// Scene Renderer 설정..
	scene->SetRenderer(m_Renderer);

	// Scene 최초 설정..
	if (m_Scene == nullptr)
	{
		m_Scene = scene;
		m_Scene->SetFactory(m_Factory);
	}
}

void D3DEngine::SelectScene(const char* name)
{
	Scene* scene = m_SceneManager->SelectScene(name);

	if (scene == nullptr) return;

	m_Scene = scene;
	m_Scene->SetFactory(m_Factory);

	// Scene 적용후 초기 설정..
	SceneStart();
}

void D3DEngine::RemoveScene(const char* name)
{
	m_SceneManager->RemoveScene(name);
}

Scene* D3DEngine::FindScene(const char* name)
{
	Scene* scene = m_SceneManager->SelectScene(name);

	if (scene == nullptr) return nullptr;

	return scene;
}

void D3DEngine::Picking(int x, int y)
{
	m_Scene->Picking(x, y, m_ClientSize->x, m_ClientSize->y);
}

Factory* D3DEngine::GetFactory()
{
	return m_Factory;
}

Scene* D3DEngine::GetScene()
{
	return m_Scene;
}

GameObject* D3DEngine::FindObject(const char* objName, eObjectType objType)
{
	return m_Scene->FindObject(objName, objType);
}

void D3DEngine::SetSolid()
{
	m_DeviceContext->RSSetState(m_SolidRS.Get());
}

void D3DEngine::SetWireFrame()
{
	m_DeviceContext->RSSetState(m_WireframeRS.Get());
}

void D3DEngine::SetNoCull()
{
	m_DeviceContext->RSSetState(m_NoCullRS.Get());
}

void D3DEngine::SetDepth()
{
	m_DeviceContext->RSSetState(m_DepthRS.Get());
}

ComPtr<ID3D11Device> D3DEngine::GetDevice()
{
	return m_Device;
}

ComPtr<ID3D11DeviceContext> D3DEngine::GetDeviceContext()
{
	return m_DeviceContext;
}

void D3DEngine::DrawTextColor(int x, int y, float scale, DXVector4 color, TCHAR* text, ...)
{
	m_Font->DrawTextColorD(x, y, scale, color, text);
}

void D3DEngine::OnResize(int width, int height)
{
	m_ClientSize->x = width;
	m_ClientSize->y = height;

	m_Renderer->OnResize(width, height);
}

void D3DEngine::CreateRenderStates()
{
	/// Polygon 을 그리는 방법 및 방법을 결정하는 Rasterizer 설정..
	// Solid Rasterizer 생성..
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;			// 렌더링 할 때 사용할 채우기 모드를 결정
	solidDesc.CullMode = D3D11_CULL_BACK;			// 특정 방향을 향하는 삼각형이 그려지지 않음을 나타냅니다
	solidDesc.FrontCounterClockwise = false;		// 삼각형이 앞면인지 뒷면인지 확인 여부 설정
	solidDesc.DepthClipEnable = true;				// 거리에 따라 클리핑을 활성화여부 설정

	solidDesc.DepthBias = 0;						// 주어진 픽셀에 추가 된 깊이 값
	solidDesc.DepthBiasClamp = 0.0f;				// 픽셀의 최대 깊이 바이어스
	solidDesc.SlopeScaledDepthBias = 0.0f;			// 주어진 픽셀의 기울기에 대한 스칼라 설정값
	solidDesc.AntialiasedLineEnable = false;		// 라인 앤티 앨리어싱 활성화 여부를 지정
	solidDesc.MultisampleEnable = true;				// TRUE : 사변형 라인 앤티 앨리어싱 알고리즘을 사용, FALSE : 알파 라인 앤티 앨리어싱 알고리즘을 사용
	solidDesc.ScissorEnable = false;				// 가위 직사각형 컬링 활성화 여부 설정

	// 위에서 설정한 값을 기반으로 RasterizerState 생성..
	HR(m_Device->CreateRasterizerState(&solidDesc, m_SolidRS.GetAddressOf()));

	// WireFrame Rasterizer 생성..
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;		// 렌더링 할 때 사용할 채우기 모드를 결정
	wireframeDesc.CullMode = D3D11_CULL_BACK;			// 특정 방향을 향하는 삼각형이 그려지지 않음을 나타냅니다
	wireframeDesc.FrontCounterClockwise = false;		// 삼각형이 앞면인지 뒷면인지 확인 여부 설정
	wireframeDesc.DepthClipEnable = true;				// 거리에 따라 클리핑을 활성화여부 설정

	wireframeDesc.DepthBias = 0;						// 주어진 픽셀에 추가 된 깊이 값
	wireframeDesc.DepthBiasClamp = 0.0f;				// 픽셀의 최대 깊이 바이어스
	wireframeDesc.SlopeScaledDepthBias = 0.0f;			// 주어진 픽셀의 기울기에 대한 스칼라 설정값
	wireframeDesc.AntialiasedLineEnable = true;		// 라인 앤티 앨리어싱 활성화 여부를 지정
	wireframeDesc.MultisampleEnable = false;			// TRUE : 사변형 라인 앤티 앨리어싱 알고리즘을 사용, FALSE : 알파 라인 앤티 앨리어싱 알고리즘을 사용
	wireframeDesc.ScissorEnable = false;				// 가위 직사각형 컬링 활성화 여부 설정

	// 위에서 설정한 값을 기반으로 RasterizerState 생성..
	HR(m_Device->CreateRasterizerState(&wireframeDesc, m_WireframeRS.GetAddressOf()));

	D3D11_RASTERIZER_DESC skycubeDesc;
	ZeroMemory(&skycubeDesc, sizeof(skycubeDesc));
	skycubeDesc.FillMode = D3D11_FILL_SOLID;
	skycubeDesc.CullMode = D3D11_CULL_NONE;
	skycubeDesc.FrontCounterClockwise = false;
	skycubeDesc.DepthClipEnable = true;
	HR(m_Device->CreateRasterizerState(&skycubeDesc, m_NoCullRS.GetAddressOf()));

	D3D11_RASTERIZER_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.FillMode = D3D11_FILL_SOLID;
	depthDesc.CullMode = D3D11_CULL_BACK;
	depthDesc.FrontCounterClockwise = false;
	depthDesc.DepthClipEnable = true;
	depthDesc.DepthBias = 100000;
	depthDesc.DepthBiasClamp = 0.0f;
	depthDesc.SlopeScaledDepthBias = 0.005f;
	HR(m_Device->CreateRasterizerState(&depthDesc, m_DepthRS.GetAddressOf()));
}

void D3DEngine::Release()
{
	SAFE_DELETE(m_ClientSize);

	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_SwapChain)
	{
		m_SwapChain->SetFullscreenState(false, NULL);
	}

	SAFE_DELETE(m_Font);

	// 오브젝트들을 전부 해제한 후에 Utility 를 해제해주자..
	ResourceManager::GetInstance()->Release();

	RESET_COM(m_DeviceContext);
	RESET_COM(m_SwapChain);
	RESET_COM(m_Device);
}
