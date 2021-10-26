#pragma once

class CameraFrustom : public Object
{
public:
	CameraFrustom();
	~CameraFrustom();

private:
	XNA::Frustum m_Frustom;

public:
	void ComputeFrustum(XMMATRIX proj);
	void BuildGeometryBuffers(XMMATRIX viewproj);
	XNA::Frustum GetFrustum() { return m_Frustom; };
};

