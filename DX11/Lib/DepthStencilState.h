#pragma once
class DepthStencilState
{
public:
	DepthStencilState(std::string dssName);
	~DepthStencilState();

public:
	void Create(D3D11_DEPTH_STENCIL_DESC descDSS);

	ID3D11DepthStencilState* GetDSS() { return m_DSS.Get(); }

private:
	std::string m_Name;
	ComPtr<ID3D11Device> m_Device;

	ComPtr<ID3D11DepthStencilState> m_DSS;
};

