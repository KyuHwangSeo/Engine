#pragma once
#pragma warning(disable : 4251)
///--------------------------------------------------
/// Animation Data�� �����ϱ� ���� Ŭ����
///--------------------------------------------------

struct PARSER_DLL AnimationData
{
	float		 m_Time;
	DXVector3	 m_Pos;
	DXQuaternion m_RotQt;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
	DXVector3	 m_Scale;
};

///--------------------------------------------------
/// �� Ŭ������ ������ Animation Ŭ����
///--------------------------------------------------
struct PARSER_DLL OneAnimation
{
	OneAnimation() {}
	OneAnimation(const OneAnimation& ani) : m_AniData(ani.m_AniData), m_TicksPerFrame(ani.m_TicksPerFrame), 
		m_StartFrame(ani.m_StartFrame), m_EndFrame(ani.m_EndFrame), m_TotalFrame(ani.m_TotalFrame), m_FrameTime(ani.m_FrameTime), m_TickFrame(ani.m_TickFrame), 
		m_NowIndex(ani.m_NowIndex), m_NextIndex(ani.m_NextIndex), m_isScaleAnimation(ani.m_isScaleAnimation), m_isPosAnimation(ani.m_isPosAnimation), m_isRotAnimation(ani.m_isRotAnimation) {}

	std::vector<AnimationData*>	m_AniData;
	
	float	m_TicksPerFrame = 0.0f;
	float	m_FrameTime = 0.0f;
	int		m_TickFrame = 0;
	int		m_TotalFrame = 0;
	int		m_StartFrame = 0;
	int		m_EndFrame = 0;
	int		m_NowIndex = 0;
	int		m_NextIndex = 1;
	
	bool	m_isScaleAnimation = false;
	bool	m_isPosAnimation = false;
	bool	m_isRotAnimation = false;
};
