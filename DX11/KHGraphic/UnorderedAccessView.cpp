#include "DirectDefine.h"
#include "Texture2D.h"
#include "UnorderedAccessView.h"
#include "MacroDefine.h"

UnorderedAccessView::UnorderedAccessView(Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav)
	:Texture2D(ResourceType::UAV, tex2D), m_UAV(uav)
{

}

UnorderedAccessView::~UnorderedAccessView()
{
	RESET_COM(m_UAV);
}

D3D11_UNORDERED_ACCESS_VIEW_DESC UnorderedAccessView::GetDesc()
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;

	m_UAV->GetDesc(&uavDesc);

	return uavDesc;
}
