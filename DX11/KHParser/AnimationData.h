#pragma once

///--------------------------------------------------
/// Animation Data�� �����ϱ� ���� Ŭ����
///--------------------------------------------------

class CAnimation_pos
{
public:
	float		 m_time;
	DXVector3	 m_pos;
};

class CAnimation_rot
{
public:
	float			m_time;
	DXVector3		m_rot;
	float			m_angle;
	Quaternion		m_rotQT_accumulation;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
};

class CAnimation_scl
{
public:
	float			m_time;
	DXVector3		m_scale;
	DXVector3		m_scaleaxis;
	float			m_scaleaxisang;
	Quaternion		m_scalerotQT_accumulation;
};

///--------------------------------------------------
/// �� Ŭ������ ������ Animation Ŭ����
///--------------------------------------------------
class AnimationData
{
public:
	AnimationData() {}
	AnimationData(const AnimationData& ani) : m_nodename(ani.m_nodename), m_position(ani.m_position), m_rotation(ani.m_rotation), m_scale(ani.m_scale), m_ticksperFrame(ani.m_ticksperFrame),
		m_totalFrame(ani.m_totalFrame), m_frameTime(ani.m_frameTime), m_tickFrame(ani.m_tickFrame), m_lastScaleFrame(ani.m_lastScaleFrame), m_lastPosFrame(ani.m_lastPosFrame), m_lastRotFrame(ani.m_lastRotFrame),
		m_nowScaleIndex(ani.m_nowScaleIndex), m_nowPosIndex(ani.m_nowPosIndex), m_nowRotIndex(ani.m_nowRotIndex), m_nextScaleIndex(ani.m_nextScaleIndex), m_nextPosIndex(ani.m_nextPosIndex), m_nextRotIndex(ani.m_nextRotIndex),
		m_isScaleAnimation(ani.m_isScaleAnimation), m_isPosAnimation(ani.m_isPosAnimation), m_isRotAnimation(ani.m_isRotAnimation) {}

public:
	// �ִϸ��̼� �������� �̸�. ������ ���������̴�
	string							m_nodename;
	vector<CAnimation_pos*>			m_position;
	vector<CAnimation_rot*>			m_rotation;
	vector<CAnimation_scl*>			m_scale;
	
	float	m_ticksperFrame = 0.0f;	// �� �������� ƽ ũ��. �̰��� AnimationTable�� ������ ������ ������Ʈ���� �ٸ� �� �ִ�.
	int		m_startFrame = 0;
	int		m_endFrame = 0;
	int		m_totalFrame = 0;
	float	m_frameTime = 0.0f;
	int		m_tickFrame = 0;
	int		m_lastScaleFrame = 0;
	int		m_lastPosFrame = 0;
	int		m_lastRotFrame = 0;
	int		m_nowScaleIndex = 0;
	int		m_nowPosIndex = 0;
	int		m_nowRotIndex = 0;
	int		m_nextScaleIndex = 1;
	int		m_nextPosIndex = 1;
	int		m_nextRotIndex = 1;
	bool	m_isScaleAnimation = false;
	bool	m_isPosAnimation = false;
	bool	m_isRotAnimation = false;
};

