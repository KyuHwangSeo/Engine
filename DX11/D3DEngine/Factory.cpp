#include "D3DDefine.h"
#include "D3DEngine.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "Material.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "SkinMeshRenderer.h"
#include "CanvasRenderer.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Light.h"
#include "Animation.h"
#include "Animator.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "UI_Image.h"
#include "UI_Button.h"

#include "IManager.h"
#include "MaterialManager.h"
#include "ObjectManager.h"
#include "HelperManager.h"
#include "SkyBox.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "CollisionManager.h"
#include "UIManager.h"
#include "Factory.h"

Factory::Factory(CameraManager* camM, LightManager* lightM, HelperManager* helperM, ObjectManager* objM, MaterialManager* matM, CollisionManager* colM, UIManager* uiM)
	:m_CamMG(camM), m_LightMG(lightM), m_HelpMG(helperM), m_ObjMG(objM), m_MatMG(matM), m_ColMG(colM), m_UiMG(uiM)
{
	m_RsMG = D3DEngine::GetInstance()->GetResourceManager();
}

Factory::~Factory()
{

}

GameObject* Factory::CreateObject(string objName, eModelType modelType, DXVector3 scale, bool isCol, eModelCollider modelColType)
{
	GameObject* newObj = new GameObject();
	newObj->SetName(objName);
	newObj->GetTransform()->SetScale(scale);

	switch (modelType)
	{
	case eModelType::Empty:
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);
		break;
	case eModelType::Camera:
	{
		Camera* cam = new Camera();

		newObj->AddComponent(cam);
		m_CamMG->AddObject(newObj);
	}
	break;
	case eModelType::DirLight:
	{
		Light* light = new Light();
		light->SetLightType(eLightType::Direction);

		newObj->AddComponent(light);
		m_LightMG->AddObject(newObj);
	}
	break;
	case eModelType::PointLight:
	{
		Light* light = new Light();
		light->SetLightType(eLightType::Point);

		newObj->AddComponent(light);
		m_LightMG->AddObject(newObj);
	}
	break;
	case eModelType::SpotLight:
	{
		Light* light = new Light();
		light->SetLightType(eLightType::Spot);

		newObj->AddComponent(light);
		m_LightMG->AddObject(newObj);
	}
	break;
	case eModelType::Grid:
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Wire, eTopologyType::Line);
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("Grid"));
		mRenderer->m_Material->SetShader("ColorShader");

		newObj->AddComponent(mRenderer);
		m_HelpMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Axis:
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Wire, eTopologyType::Line);
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("Axis"));
		mRenderer->m_Material->SetShader("ColorShader");

		newObj->AddComponent(mRenderer);
		m_HelpMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Sky:
	{
		MeshRenderer* mRenderer = new MeshRenderer();
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("Sky"));
		mRenderer->m_Material->SetShader("SkyCubeShader");

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Box:
	{
		MeshRenderer* mRenderer = new MeshRenderer();
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("GeoBox"));
		mRenderer->m_Material->SetShader("BasicDeferredShader");

		if (isCol)
		{
			BoxCollider* col = new BoxCollider();
			col->SetBoundBox(m_RsMG->GetVertexBuffer("GeoBox"));
			newObj->AddComponent(col);
			m_ColMG->AddObject3D(newObj);
		}

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Sphere:
	{
		MeshRenderer* mRenderer = new MeshRenderer();
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("GeoSphere"));
		mRenderer->m_Material->SetShader("BasicDeferredShader");

		if (isCol)
		{
			BoxCollider* col = new BoxCollider();
			col->SetBoundBox(m_RsMG->GetVertexBuffer("GeoSphere"));
			newObj->AddComponent(col);
			m_ColMG->AddObject3D(newObj);
		}

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Cylinder:
	{
		MeshRenderer* mRenderer = new MeshRenderer();
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("GeoCylinder"));
		mRenderer->m_Material->SetShader("BasicDeferredShader");

		if (isCol)
		{
			BoxCollider* col = new BoxCollider();
			col->SetBoundBox(m_RsMG->GetVertexBuffer("GeoCylinder"));
			newObj->AddComponent(col);
			m_ColMG->AddObject3D(newObj);
		}

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Text:
	{
		MeshRenderer* mRenderer = new MeshRenderer();
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer(objName));
		mRenderer->m_Material->SetShader("BasicDeferredShader");

		if (isCol)
		{
			BoxCollider* col = new BoxCollider();
			col->SetBoundBox(m_RsMG->GetVertexBuffer(objName));
			newObj->AddComponent(col);
		}

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::ASE:
	{
		CASEParser* parser = m_RsMG->GetASEParser(objName);
		Animation* animation = nullptr;
		Animator* animator = nullptr;

		// ���� ������ �����ϱ����� ����Ʈ..
		vector<GameObject*> objList;
		objList.emplace_back(newObj);

		m_ObjMG->AddTopObject(newObj);

		// �ִϸ��̼��� �ִ°��..
		if (parser->m_isAnimation)
		{
			animator = new Animator();
			newObj->AddComponent(animator);
		}

		// �ε��� �����͸� ������� ��� Mesh�� �����Ѵ�..
		int meshSize = (int)m_RsMG->GetMeshListSize(objName);
		for (int i = 0; i < meshSize; i++)
		{
			string meshKey = m_RsMG->GetMeshKey(objName, i);

			ParserData::Mesh* newMesh = m_RsMG->GetMesh(meshKey);
			VertexBuffer* newBuffer = m_RsMG->GetVertexBuffer(meshKey);

			GameObject* newNode = nullptr;

			// Mesh�� 1�� �̻��� ��� �ֻ��� ������Ʈ�� �����ֱ� ����..
			if (meshSize == 1)
				newNode = newObj;
			else
			{
				newNode = new GameObject();

				// ������Ʈ(���) ����Ʈ�� ����..
				objList.emplace_back(newNode);

				// �Ľ� �������� �ֻ��� ������ �� ������Ʈ�� �����ش�..
				if (newMesh->m_parent == nullptr)
				{
					newNode->SetParent(newObj->GetTransform());
					newObj->AddChild(newNode->GetTransform());
				}
			}

			newNode->SetName(newMesh->m_nodename);
			newNode->GetTransform()->SetNodeTM(newMesh->m_localTM);
			newNode->GetTransform()->SetLocalTM(newMesh->m_worldTM);

			// Mesh ������ ����Ͽ� Renderer ����..
			SetRenderer(newNode, meshKey, newMesh, newBuffer);

			// �� Object�� Bone�� ���..
			if (newMesh->m_IsBone)
			{
				newNode->SetMeshType(eMeshType::Bone);
			}
			// �� Object�� Helper�� ���..
			else if (newMesh->m_IsHelper)
			{
				newNode->SetMeshType(eMeshType::Helper);
			}
			// �� Obejct�� Mesh�� ���..
			else
			{
				if (isCol && modelColType != eModelCollider::BoneList)
					SetCollider(newNode, newBuffer);
			}

			// �� Object�� �ִϸ��̼��� �ִٸ�..
			if (parser->m_isAnimation)
				animator->AddMeshObject(newNode);
		}

		// ���� ������Ʈ Collider ����..
		if (isCol)
			m_ColMG->AddModel(objList);

		// ���� ������Ʈ �������� ���� �� ����Ʈ �ʱ�ȭ..
		m_ObjMG->AddModel(objList);
		m_ObjMG->SetHierarchy(objList);
		objList.clear();
	}
	break;
	case eModelType::Gizmo:
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Wire);
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer(objName));
		mRenderer->m_Material->SetShader("ColorShader");

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::FBX:
	{
		FBXModel* parser = m_RsMG->GetFBXParser(objName);
		Animation* animation = nullptr;
		Animator* animator = nullptr;

		// ���� ������ �����ϱ����� ����Ʈ..
		vector<GameObject*> objList;
		objList.emplace_back(newObj);

		m_ObjMG->AddTopObject(newObj);

		// �ִϸ��̼��� �ִ°��..
		if (parser->m_isAnimation)
		{
			animator = new Animator();
			newObj->AddComponent(animator);
		}

		// �ε��� �����͸� ������� ��� Mesh�� �����Ѵ�..
		size_t meshSize = m_RsMG->GetMeshListSize(objName);
		for (size_t i = 0; i < meshSize; i++)
		{
			string meshKey = m_RsMG->GetMeshKey(objName, i);

			ParserData::Mesh* newMesh = m_RsMG->GetMesh(meshKey);
			VertexBuffer* newBuffer = m_RsMG->GetVertexBuffer(meshKey);

			GameObject* newNode = nullptr;

			// Mesh�� 1�� �̻��� ��� �ֻ��� ������Ʈ�� �����ֱ� ����..
			if (meshSize == 1)
				newNode = newObj;
			else
			{
				newNode = new GameObject();

				// ������Ʈ(���) ����Ʈ�� ����..
				objList.emplace_back(newNode);

				// �Ľ� �������� �ֻ��� ������ �� ������Ʈ�� �����ش�..
				if (newMesh->m_parent == nullptr)
				{
					newNode->SetParent(newObj->GetTransform());
					newObj->AddChild(newNode->GetTransform());
				}
			}

			newNode->SetName(newMesh->m_nodename);
			newNode->GetTransform()->SetNodeTM(newMesh->m_localTM);
			newNode->GetTransform()->SetLocalTM(newMesh->m_worldTM);

			// Mesh ������ ����Ͽ� Renderer ����..
			SetRenderer(newNode, meshKey, newMesh, newBuffer);

			// �� Object�� Bone�� ���..
			if (newMesh->m_IsBone)
			{
				newNode->SetMeshType(eMeshType::Bone);

				if (isCol && modelColType != eModelCollider::MeshList)
					SetCollider(newNode, newBuffer);
			}
			// �� Object�� Helper�� ���..
			else if (newMesh->m_IsHelper)
			{
				newNode->SetMeshType(eMeshType::Helper);
			}
			// �� Obejct�� Mesh�� ���..
			else
			{
				if (isCol && modelColType != eModelCollider::BoneList)
					SetCollider(newNode, newBuffer);
			}

			// �� Object�� �ִϸ��̼��� �ִٸ�..
			if (parser->m_isAnimation)
				animator->AddMeshObject(newNode);
		}

		// ���� ������Ʈ Collider ����..
		if (isCol)
			m_ColMG->AddModel(objList);

		// ���� ������Ʈ �������� ����..
		m_ObjMG->AddModel(objList);
		m_ObjMG->SetHierarchy(objList);

		// ����Ʈ �ʱ�ȭ..
		objList.clear();
	}
	break;
	default:
		break;
	}

	return newObj;
}

void Factory::SetRenderer(GameObject* obj, std::string meshKey, ParserData::Mesh* mesh, VertexBuffer* vBuffer)
{
	// ��Ű�� ������Ʈ ����..
	if (mesh->m_IsSkinningObject)
	{
		SkinMeshRenderer* sRenderer = new SkinMeshRenderer(eRasterizerType::Solid);
		sRenderer->SetMesh(mesh);
		sRenderer->SetVertexBuffer(vBuffer);

		// ������ִ� Material �߰�
		/// Mesh�� m_materialdata�� ref Material�̹Ƿ� �ٷ� �����������..
		if (mesh->m_materialdata != nullptr)
		{
			sRenderer->m_Material->SetMaterialData(m_RsMG->GetMaterial(meshKey));

			if (mesh->m_materialdata->m_isDiffuseMap)
			{
				sRenderer->m_Material->SetDiffuseMap(m_RsMG->GetTexture(meshKey, eTextureType::Diffuse));
				sRenderer->m_Material->SetShader("SkinDeferredShader");
			}
			if (mesh->m_materialdata->m_isBumpMap)
			{
				sRenderer->m_Material->SetNormalMap(m_RsMG->GetTexture(meshKey, eTextureType::Bump));
				sRenderer->m_Material->SetShader("NormalSkinDeferredShader");
			}
		}

		m_MatMG->AddMaterial(sRenderer->m_Material);
		obj->AddComponent(sRenderer);
	}
	else
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Solid);
		mRenderer->SetMesh(mesh);
		mRenderer->SetVertexBuffer(vBuffer);

		// ������ִ� Material �߰�
		/// Mesh�� m_materialdata�� ref Material�̹Ƿ� �ٷ� �����������..
		if (mesh->m_materialdata != nullptr)
		{
			mRenderer->m_Material->SetMaterialData(m_RsMG->GetMaterial(meshKey));
			mRenderer->m_Material->SetShader("BasicDeferredShader");
			if (mesh->m_materialdata->m_isDiffuseMap)
			{
				mRenderer->m_Material->SetDiffuseMap(m_RsMG->GetTexture(meshKey, eTextureType::Diffuse));
				mRenderer->m_Material->SetShader("TextureDeferredShader");
			}
			if (mesh->m_materialdata->m_isBumpMap)
			{
				mRenderer->m_Material->SetNormalMap(m_RsMG->GetTexture(meshKey, eTextureType::Bump));
				mRenderer->m_Material->SetShader("NormalDeferredShader");
			}
		}
		else
		{
			mRenderer->m_Material->SetShader("BasicDeferredShader");
		}

		m_MatMG->AddMaterial(mRenderer->m_Material);
		obj->AddComponent(mRenderer);
	}
}

void Factory::SetCollider(GameObject* obj, VertexBuffer* vBuffer)
{
	if (vBuffer == nullptr) return;

	switch (vBuffer->m_ColType)
	{
	case eColliderType::Box:
	{
		BoxCollider* col = new BoxCollider();
		col->SetBoundBox(vBuffer);
		obj->AddComponent(col);
	}
	break;
	case eColliderType::Sphere:
	{
		SphereCollider* col = new SphereCollider();
		col->SetBoundSphere(vBuffer);
		obj->AddComponent(col);
	}
	break;
	default:
		break;
	}
}

void Factory::CreateBoxCollider(GameObject* obj, DXVector3 size)
{
	BoxCollider* col = new BoxCollider();
	col->SetBoundBox(DXVector3::Zero(), size);
	obj->AddComponent(col);
	m_ColMG->AddObject3D(obj);
}

void Factory::CreateSphereCollider(GameObject* obj, float radius)
{
	SphereCollider* col = new SphereCollider();
	col->SetBoundSphere(DXVector3::Zero(), radius);
	obj->AddComponent(col);
	m_ColMG->AddObject3D(obj);
}

GameObject* Factory::CreateUI(string objName, eUIType uiType, string texKey, DXVector2 scale, DXVector2 pos, bool isCol)
{
	GameObject* newObj = new GameObject();
	newObj->SetName(objName);
	newObj->GetTransform()->SetPosition(DXVector3(pos.x, pos.y, 0.0f));
	newObj->GetTransform()->SetScale(DXVector3(scale.x, scale.y, 1.0f));

	switch (uiType)
	{
	case eUIType::Image:
	{
		CanvasRenderer* cRenderer = new CanvasRenderer(eRasterizerType::Solid);
		cRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("UIBasic"));
		cRenderer->m_Material->SetDiffuseMap(m_RsMG->GetTexture(texKey));
		cRenderer->m_Material->SetShader("UIBasicShader");
		newObj->AddComponent(cRenderer);

		UI_Image* uiImage = new UI_Image();
		newObj->AddComponent(uiImage);

		if (isCol)
		{
			CircleCollider2D* circleCol = new CircleCollider2D();
			circleCol->SetBoundCircle2D(DXVector2::Zero(), min(scale.x / 2, scale.y / 2));
			newObj->AddComponent(circleCol);
			m_ColMG->AddObject2D(newObj);
		}

		m_UiMG->AddObject(newObj);
	}
	break;
	case eUIType::Button:
	{
		CanvasRenderer* cRenderer = new CanvasRenderer(eRasterizerType::Solid);
		cRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("UIBasic"));
		cRenderer->m_Material->SetDiffuseMap(m_RsMG->GetTexture(texKey));
		cRenderer->m_Material->SetShader("UIBasicShader");
		newObj->AddComponent(cRenderer);

		UI_Button* uiButton = new UI_Button();
		newObj->AddComponent(uiButton);

		m_UiMG->AddObject(newObj);
	}
	break;
	default:
		break;
	}
	return newObj;
}

Animation* Factory::CreateAnimation(string objName, string aniName, GameObject* topNode, bool play)
{
	int meshSize = (int)m_RsMG->GetMeshListSize(objName);

	// Animator�� �ִ� �ֻ��� ����� Animator Component ã��..
	GameObject* topObject = topNode;
	Animator* animator = topObject->GetComponent<Animator>();

	// ���ο� �ִϸ��̼� ����..
	Animation* animation = new Animation();

	// �� Object�� Animation Data ����..
	for (int i = 0; i < meshSize; i++)
	{
		string meshKey = m_RsMG->GetMeshKey(objName, i);

		// Object ������ Animator�� ����ִ� Obejct�� ���� �ִϸ��̼� ������ �ִ´�..
		AnimationData* animationData = m_RsMG->GetAnimation(meshKey);
		animation->AddAnimationData(animator->GetMeshObject(i), animationData);
	}

	animator->AddAnimation(aniName, animation);

	if (play)
		animator->PlayAnimation(aniName);

	return animation;
}

Animation* Factory::CreateAnimation(string objName, string aniName, string nodeName, bool play)
{
	// Animator�� �ִ� �ֻ��� ����� Animator Component ã��..
	GameObject* topNode = m_ObjMG->FindObject(nodeName);
	Animator* animator = topNode->GetComponent<Animator>();

	// ���ο� �ִϸ��̼� ����..
	Animation* animation = new Animation();

	// �� Object�� Animation Data ����..
	int meshSize = (int)m_RsMG->GetMeshListSize(objName);
	for (int i = 0; i < meshSize; i++)
	{
		string meshKey = m_RsMG->GetMeshKey(objName, i);

		// Object ������ Animator�� ����ִ� Obejct�� ���� �ִϸ��̼� ������ �ִ´�..
		AnimationData* animationData = m_RsMG->GetAnimation(meshKey);
		animation->AddAnimationData(animator->GetMeshObject(i), animationData);
	}

	animator->AddAnimation(aniName, animation);

	if (play)
		animator->PlayAnimation(aniName);

	return animation;
}
