#include "D3DDefine.h"
#include "D3DEngine.h"
#include "DepthStencilView.h"
#include "DepthStencilState.h"
#include "DepthStencilManager.h"

DepthStencilManager::DepthStencilManager()
{
}

DepthStencilManager::~DepthStencilManager()
{

}

void DepthStencilManager::Initialize()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	/// Defalt View 생성..
	CreateView("Defalt", texDesc, descDSV);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// depthStencilDesc 설정..
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀이 정면일 경우 Stencil 설정..
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀이 후면일 경우 Stencil 설정..
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	/// Defalt State 생성..
	CreateState("Defalt", depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// depthStencilDesc 설정..
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀이 정면일 경우 Stencil 설정..
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀이 후면일 경우 Stencil 설정..
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	/// NoDepth State 생성..
	CreateState("NoDepth", depthStencilDesc);
}

void DepthStencilManager::CreateState(string name, D3D11_DEPTH_STENCIL_DESC descDSS)
{
	DepthStencilState* newDSS = new DepthStencilState(name);
	newDSS->Create(descDSS);

	m_DSSList.insert(pair<string, DepthStencilState*>(name, newDSS));
}

void DepthStencilManager::CreateView(string name, D3D11_TEXTURE2D_DESC texDesc, D3D11_DEPTH_STENCIL_VIEW_DESC descDSV)
{
	DepthStencilView* newDSV = new DepthStencilView(name);
	newDSV->Create(texDesc, descDSV);

	m_DSVList.insert(pair<string, DepthStencilView*>(name, newDSV));
}

void DepthStencilManager::OnResize(int width, int height)
{
	for (pair<string, DepthStencilView*> dsv : m_DSVList)
	{
		dsv.second->OnResize(width, height);
	}
}
