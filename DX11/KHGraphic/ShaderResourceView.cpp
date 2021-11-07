#include "DirectDefine.h"
#include "Texture2D.h"
#include "ShaderResourceView.h"
#include "MacroDefine.h"

ShaderResourceView::ShaderResourceView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
	:Texture2D(ResourceType::SRV, tex2D), m_SRV(srv)
{

}

ShaderResourceView::~ShaderResourceView()
{
	RESET_COM(m_SRV);
}

D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView::GetDesc()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	m_SRV->GetDesc(&srvDesc);

	return srvDesc;
}
