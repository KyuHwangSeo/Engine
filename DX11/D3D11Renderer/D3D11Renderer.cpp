#include "DirectDefine.h"
#include "D3D11Renderer.h"

D3D11Renderer::D3D11Renderer()
{

}

D3D11Renderer::~D3D11Renderer()
{

}

bool D3D11Renderer::Initialize(INT_PTR hwnd, int screenWidth, int screenHeight)
{
	// ���� ü�� ������ �ʱ�ȭ..
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// swapChainDesc�� ���� �� ���۷� ����..
	swapChainDesc.BufferCount = 1;

	// �� ������ �ʺ�� ���̸� ����..
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// �� ���ۿ� �Ϲ� 32 ��Ʈ ǥ���� ����..
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �� ������ ������� ����..
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// ��ĵ ���� ���� �� ������ �������� �������� ����..
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �� ������ ��� ����..
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.OutputWindow = (HWND)hwnd;

	// ��Ƽ ���ø��� ���ϴ�..
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �÷��� ���� �� ��..
	swapChainDesc.Flags = 0;

	// featureLevel�� DirectX 11�� ����..
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0;

	// ���� ü��, Direct3D ��ġ �� Direct3D ��ġ ���ؽ�Ʈ ����..
	/// MSDN���� SwapChain �� Device �� �ѹ��� �������ִ� �Լ��� ������..
	HR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featurelevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext));
}