#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "Collider.h"
#include "BoxCollider2D.h"

BoxCollider2D::BoxCollider2D()
{
	m_ColType = eColliderType::Box2D;
}

BoxCollider2D::~BoxCollider2D()
{

}

void BoxCollider2D::LateUpdate(float dTime)
{
	m_ColData.center = m_GameObject->GetPosition();

	m_ColData.center.x += m_ColData.size2D.x / 2.0f;
	m_ColData.center.y += m_ColData.size2D.y / 2.0f;

	m_Box2D.center.x = m_ColData.center.x;
	m_Box2D.center.y = m_ColData.center.y;
}

bool BoxCollider2D::CollisionCheck(Collider* col)
{
	switch (col->GetColliderType())
	{
	case eColliderType::Box2D:
		break;
	case eColliderType::Circle2D:
		break;
	break;
	}

	return false;
}

void BoxCollider2D::SetBoundBox2D(DXVector2 center, DXVector2 size)
{
	m_Box2D.center = center;
	m_Box2D.center = size;
}
