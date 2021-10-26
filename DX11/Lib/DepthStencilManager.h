#pragma once

class DepthStencilManager
{
public:
	DepthStencilManager();
	~DepthStencilManager();

public:
	void Initialize();
	void CreateState(string name, D3D11_DEPTH_STENCIL_DESC descDSS);
	void CreateView(string name, D3D11_TEXTURE2D_DESC texDesc, D3D11_DEPTH_STENCIL_VIEW_DESC descDSV);
	void OnResize(int width, int height);

	ID3D11DepthStencilState* GetDSS(std::string name)	{ return m_DSSList[name]->GetDSS(); }
	ID3D11DepthStencilView* GetDSV(std::string name)	{ return m_DSVList[name]->GetDSV(); }

private:
	std::unordered_map<std::string, DepthStencilState*> m_DSSList;
	std::unordered_map<std::string, DepthStencilView*> m_DSVList;
};

