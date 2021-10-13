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

	// ���� �ڵ� ����..
	m_Mainhwnd = (HWND)hwnd;

	// ���� ȭ�� ũ�� ������Ʈ..
	m_ClientSize = new POINT;
	m_ClientSize->x = screenWidth;
	m_ClientSize->y = screenHeight;

	// Device ����..
	CreateDevice();

	// Rasterizer ���� ���� �� ����..
	CreateRenderStates();

	// ResourceManager �ʱ�ȭ..
	ResourceManager::GetInstance()->Initialize();

	// Renderer ���� �� �ʱ�ȭ..
	m_Renderer = new D3DRenderer(m_Device, m_DeviceContext, m_SwapChain.Get());
	m_Renderer->Initialize();

	// ��Ʈ ����
	m_Font->Create(m_Device.Get());

	// GameTimer �ʱ�ȭ..
	GameTimer::GetInstance()->Reset();

	// Sound �ʱ�ȭ..
	KHSound::GetInstance()->Initialize();

	// Layer List ����..
	LayerList::Initialize();

	// Factroy ���� �� �ʱ�ȭ..
	m_Factory = new Factory();

	// SceneManager ���� �� �ʱ�ȭ..
	m_SceneManager = new SceneManager();

	return true;
}

void D3DEngine::CreateDevice()
{
	// ���� ü�� ������ �ʱ�ȭ..
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// swapChainDesc�� ���� �� ���۷� ����..
	swapChainDesc.BufferCount = 1;

	// �� ������ �ʺ�� ���̸� ����..
	swapChainDesc.BufferDesc.Width = m_ClientSize->x;
	swapChainDesc.BufferDesc.Height = m_ClientSize->y;

	// �� ���ۿ� �Ϲ� 32 ��Ʈ ǥ���� ����..
	/// - DXGI_FORMAT_R8G8B8A8_UNORM
	///  -> ���ĸ� �����Ͽ� ä�δ� 8 ��Ʈ�� �����ϴ� 4 ����, 32 ��Ʈ ��ȣ���� ����ȭ ���� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �� ������ ������� ����..
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// �� ������ ��� ����..
	/// - DXGI_USAGE_RENDER_TARGET_OUTPUT
	///  1. ǥ�� �Ǵ� ���ҽ��� �� ���۷� ���˴ϴ�. 
	///  2. ���� ü���� ���� �� �� DXGI_USAGE_BACK_BUFFER �� ������ �ʿ䰡 �����ϴ�. 
	///  3. ȣ�� �� �� �ڿ��� ���� ü�ο� ���� �ִ��� ���θ� Ȯ���� �� �ֽ��ϴ�. 
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.OutputWindow = (HWND)m_Mainhwnd;

	// ��Ƽ ���ø��� ���ϴ�..
	/// - DXGI_SAMPLE_DESC 
	///  -> ���ҽ��� ���� ���� ���ø� �Ű� ���� �����ϴ� ����ü
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;	// ��ü ȭ�� �Ǵ� â ���� ����

	// ��ĵ ���� ���� �� ������ �������� �������� ����..
	/// - DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED
	///  -> �����Ͱ� ǥ�鿡 �̹����� ����� �� ����ϴ� ����� ��Ÿ���� �÷���
	/// - DXGI_MODE_SCALING_UNSPECIFIED
	///  -> �־��� ������� �ػ󵵿� �°� �̹����� �þ�� ����� ��Ÿ���� �÷���
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/// DXGI_SWAP_EFFECT_DISCARD -> IDXGISwapChain1 :: Present1 ȣ�� �� ���÷��� ǥ���� �ȼ��� ó���ϴ� �ɼ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �÷��� ���� �� ��..
	swapChainDesc.Flags = 0;

	// featureLevel�� DirectX 11�� ����..
	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// ���� ü��, Direct3D ��ġ �� Direct3D ��ġ ���ؽ�Ʈ ����..
	/// MSDN���� SwapChain �� Device �� �ѹ��� �������ִ� �Լ��� ������..
	HR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &m_FeatureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext));
}

void D3DEngine::CreateHelper()
{
	// Camera ���� �� �ʱ�ȭ..
	GameObject* cam1 = m_Factory->CreateObject("Camera1", eModelType::Camera);
	cam1->GetTransform()->SetScale(DXVector3(0.5f, 0.5f, 0.5f));
	cam1->GetComponent<Camera>()->SetType(eCameraType::NormalCam);

	GameObject* cam2 = m_Factory->CreateObject("Camera2", eModelType::Camera);
	cam2->GetTransform()->SetScale(DXVector3(0.5f, 0.5f, 0.5f));
	cam2->GetComponent<Camera>()->SetType(eCameraType::FollowCam_3);

	// Main Camera ����..
	Camera::g_MainCamera = cam2->GetComponent<Camera>();

	// HerlperObject ���� �� �ʱ�ȭ..
#ifdef _DEBUG
	m_Factory->CreateObject("Grid", eModelType::Grid);
	m_Factory->CreateObject("Axis", eModelType::Axis);
#endif

	// �⺻ Directional Light ����..
	GameObject* light1 = m_Factory->CreateObject("Direction Light", eModelType::DirLight);
	light1->GetTransform()->SetScale(DXVector3(10.0f, 10.0f, 10.0f));
	light1->GetTransform()->MoveLocal(DXVector3(0.0f, 15.0f, 0.0f));

	// Direction Light ����..
	Light::g_DirLight = light1->GetComponent<Light>();
}

void D3DEngine::EndRender()
{
	m_Renderer->EndRender();

	// ���� ������ �� �̹����� �׸���..
	m_SwapChain->Present(0, 0);
}

void D3DEngine::SceneStart()
{
	// �⺻ ������Ʈ ����..
	CreateHelper();

	// â ����� ���� �缳��..
	OnResize(m_ClientSize->x, m_ClientSize->y);

	// Scene ���� �����ϴ� GameObject ����..
	m_Scene->Create();

	// Scene ���� �����ϴ� GameObject ����..
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

	// ���� Scene Object Update..
	m_Scene->Update(dTime);

	// �� ���� Scene Object Update..
	m_Scene->FinalUpdate(dTime);
}

void D3DEngine::Render()
{
	// ���� Scene ������Ʈ �׸���..
	m_Scene->Render();

	// ���� Scene DebugDraw..
	if (m_DebugMode)
	{
		m_Scene->DebugDraw();

		// ��Ʈ�� �̿��� �ؽ�Ʈ ���..
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

	// �����ֱ�
	static float _addedTime = 0;
	static float _FPS = 0;
	static float _deltaTimeMS = 0;

	float deltaTime = GameTimer::GetInstance()->DeltaTime();

	// �����ֱ�� 0.2��
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
	// Scene �߰�..
	m_SceneManager->AddScene(name, scene);

	// Scene Renderer ����..
	scene->SetRenderer(m_Renderer);

	// Scene ���� ����..
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

	// Scene ������ �ʱ� ����..
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
	/// Polygon �� �׸��� ��� �� ����� �����ϴ� Rasterizer ����..
	// Solid Rasterizer ����..
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;			// ������ �� �� ����� ä��� ��带 ����
	solidDesc.CullMode = D3D11_CULL_BACK;			// Ư�� ������ ���ϴ� �ﰢ���� �׷����� ������ ��Ÿ���ϴ�
	solidDesc.FrontCounterClockwise = false;		// �ﰢ���� �ո����� �޸����� Ȯ�� ���� ����
	solidDesc.DepthClipEnable = true;				// �Ÿ��� ���� Ŭ������ Ȱ��ȭ���� ����

	solidDesc.DepthBias = 0;						// �־��� �ȼ��� �߰� �� ���� ��
	solidDesc.DepthBiasClamp = 0.0f;				// �ȼ��� �ִ� ���� ���̾
	solidDesc.SlopeScaledDepthBias = 0.0f;			// �־��� �ȼ��� ���⿡ ���� ��Į�� ������
	solidDesc.AntialiasedLineEnable = false;		// ���� ��Ƽ �ٸ���� Ȱ��ȭ ���θ� ����
	solidDesc.MultisampleEnable = true;				// TRUE : �纯�� ���� ��Ƽ �ٸ���� �˰����� ���, FALSE : ���� ���� ��Ƽ �ٸ���� �˰����� ���
	solidDesc.ScissorEnable = false;				// ���� ���簢�� �ø� Ȱ��ȭ ���� ����

	// ������ ������ ���� ������� RasterizerState ����..
	HR(m_Device->CreateRasterizerState(&solidDesc, m_SolidRS.GetAddressOf()));

	// WireFrame Rasterizer ����..
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;		// ������ �� �� ����� ä��� ��带 ����
	wireframeDesc.CullMode = D3D11_CULL_BACK;			// Ư�� ������ ���ϴ� �ﰢ���� �׷����� ������ ��Ÿ���ϴ�
	wireframeDesc.FrontCounterClockwise = false;		// �ﰢ���� �ո����� �޸����� Ȯ�� ���� ����
	wireframeDesc.DepthClipEnable = true;				// �Ÿ��� ���� Ŭ������ Ȱ��ȭ���� ����

	wireframeDesc.DepthBias = 0;						// �־��� �ȼ��� �߰� �� ���� ��
	wireframeDesc.DepthBiasClamp = 0.0f;				// �ȼ��� �ִ� ���� ���̾
	wireframeDesc.SlopeScaledDepthBias = 0.0f;			// �־��� �ȼ��� ���⿡ ���� ��Į�� ������
	wireframeDesc.AntialiasedLineEnable = true;		// ���� ��Ƽ �ٸ���� Ȱ��ȭ ���θ� ����
	wireframeDesc.MultisampleEnable = false;			// TRUE : �纯�� ���� ��Ƽ �ٸ���� �˰����� ���, FALSE : ���� ���� ��Ƽ �ٸ���� �˰����� ���
	wireframeDesc.ScissorEnable = false;				// ���� ���簢�� �ø� Ȱ��ȭ ���� ����

	// ������ ������ ���� ������� RasterizerState ����..
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

	// ������Ʈ���� ���� ������ �Ŀ� Utility �� ����������..
	ResourceManager::GetInstance()->Release();

	RESET_COM(m_DeviceContext);
	RESET_COM(m_SwapChain);
	RESET_COM(m_Device);
}
