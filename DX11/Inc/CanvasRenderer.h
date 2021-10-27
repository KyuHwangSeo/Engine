#pragma once
///
/// 2021/08/13 11:31
/// SeoKyuHwang
///
/// CanvasRenderer Class
///
/// - UI 전용 랜더러
///

class CanvasRenderer : public Renderer
{
public:
	CanvasRenderer(eRasterizerType renderType = eRasterizerType::Solid, eTopologyType topolType = eTopologyType::Triangle);
	~CanvasRenderer();

public:
	void Render(DXMatrix4X4 view, DXMatrix4X4 proj);

private:
	cbPerUI m_UIObjectData;
};

