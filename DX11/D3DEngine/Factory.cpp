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
#include "Terrain.h"
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
#include "SkyBox.h"

#include "IManager.h"
#include "MaterialManager.h"
#include "ObjectManager.h"
#include "HelperManager.h"
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

GameObject* Factory::CreateObject(std::string objName, eModelType modelType, DXVector3 scale, bool isCol, eModelCollider modelColType)
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
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("ColorShader"));

		newObj->AddComponent(mRenderer);
		m_HelpMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Axis:
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Wire, eTopologyType::Line);
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("Axis"));
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("ColorShader"));

		newObj->AddComponent(mRenderer);
		m_HelpMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Sky:
	{
		MeshRenderer* mRenderer = new MeshRenderer();
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer("Sky"));
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("SkyCubeShader"));

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
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicDeferredShader"));

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
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicDeferredShader"));

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
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicDeferredShader"));

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
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicDeferredShader"));

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
	case eModelType::Gizmo:
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Wire);
		mRenderer->SetVertexBuffer(m_RsMG->GetVertexBuffer(objName));
		mRenderer->m_Material->SetShader(m_RsMG->GetShader("ColorShader"));

		newObj->AddComponent(mRenderer);
		m_ObjMG->AddObject(newObj);
		m_MatMG->AddMaterial(mRenderer->m_Material);
	}
	break;
	case eModelType::Model:
	{
		ParserData::Model* parser = m_RsMG->GetModel(objName);
		Animation* animation = nullptr;
		Animator* animator = nullptr;

		// 계층 구조를 설정하기위한 리스트..
		vector<GameObject*> objList;
		objList.emplace_back(newObj);

		m_ObjMG->AddTopObject(newObj);

		// 애니메이션이 있는경우..
		if (parser->m_isAnimation)
		{
			animator = new Animator();
			newObj->AddComponent(animator);
		}

		// 로딩한 데이터를 기반으로 모든 Mesh를 생성한다..
		size_t meshSize = parser->m_MeshList.size();
		for (size_t i = 0; i < meshSize; i++)
		{
			std::string meshKey = m_RsMG->GetMeshKey(objName, i);

			ParserData::Mesh* newMesh = m_RsMG->GetMesh(meshKey);
			VertexBuffer* newBuffer = m_RsMG->GetVertexBuffer(meshKey);

			GameObject* newNode = nullptr;

			// Mesh가 1개 이상인 경우 최상위 오브젝트로 묶어주기 위해..
			if (meshSize == 1)
				newNode = newObj;
			else
			{
				newNode = new GameObject();

				// 오브젝트(노드) 리스트에 삽입..
				objList.emplace_back(newNode);

				// 파싱 데이터중 최상위 노드들을 한 오브젝트로 묶어준다..
				if (newMesh->m_TopNode)
				{
					newNode->SetParent(newObj->GetTransform());
					newObj->AddChild(newNode->GetTransform());
				}
			}

			newNode->SetName(newMesh->m_NodeName);
			newNode->GetTransform()->SetNodeTM(newMesh->m_LocalTM);
			newNode->GetTransform()->SetLocalTM(newMesh->m_WorldTM);

			// Mesh 정보에 기반하여 Renderer 설정..
			SetRenderer(newNode, meshKey, newMesh, newBuffer);

			// 현 Object가 Bone일 경우..
			if (newMesh->m_IsBone)
			{
				newNode->SetMeshType(eMeshType::Bone);

				if (isCol && modelColType != eModelCollider::MeshList)
					SetCollider(newNode, newBuffer);
			}
			// 현 Obejct가 Mesh일 경우..
			else
			{
				if (isCol && modelColType != eModelCollider::BoneList)
					SetCollider(newNode, newBuffer);
			}

			// 현 Object가 애니메이션이 있다면..
			if (parser->m_isAnimation)
				animator->AddMeshObject(newNode);
		}

		// 현재 오브젝트 Collider 저장..
		if (isCol)
			m_ColMG->AddModel(objList);

		// 현재 오브젝트 계층구조 설정..
		m_ObjMG->AddModel(objList);
		m_ObjMG->SetHierarchy(objList);

		// 리스트 초기화..
		objList.clear();
	}
	break;
	case eModelType::TerrainMesh:
	{
		ParserData::Model* parser = m_RsMG->GetModel(objName);
		m_ObjMG->AddTopObject(newObj);
		m_ObjMG->AddObject(newObj);

		std::string meshKey = m_RsMG->GetMeshKey(objName);

		ParserData::Mesh* newMesh = m_RsMG->GetMesh(meshKey);
		VertexBuffer* newBuffer = m_RsMG->GetVertexBuffer(meshKey);

		newObj->GetTransform()->SetNodeTM(newMesh->m_LocalTM);
		newObj->GetTransform()->SetLocalTM(newMesh->m_WorldTM);

		Terrain* terrainCom = new Terrain();
		terrainCom->SetVertexBuffer(newBuffer);
		terrainCom->SetShadowShader(m_RsMG->GetShader("NormalShadowShader"));
		terrainCom->SetShader(m_RsMG->GetShader("TerrainShader"));

		/// 임시 Layer 추가 코드..
		//MaterialLayer matLayer1 = MaterialLayer(m_RsMG->GetTexture("Grass_Ivy_Diffuse"), m_RsMG->GetTexture("Grass_Ivy_Normal"));
		//MaterialLayer matLayer2 = MaterialLayer(m_RsMG->GetTexture("Dry_Ground_Diffuse"), m_RsMG->GetTexture("Dry_Ground_Normal"));
		//MaterialLayer matLayer3 = MaterialLayer(m_RsMG->GetTexture("Dead_Leaves_Diffuse"), m_RsMG->GetTexture("Dead_Leaves_Normal"));
		//MaterialLayer matLayer4 = MaterialLayer(m_RsMG->GetTexture("Rock_Diffuse"), m_RsMG->GetTexture("Rock_Normal"));
		//terrainCom->AddLayer(m_RsMG->GetTexture("mask"), matLayer1, matLayer2, matLayer3);
		//terrainCom->AddLayer(m_RsMG->GetTexture("Rock_Mask"), matLayer4);

		MaterialLayer matLayer1 = MaterialLayer(m_RsMG->GetTexture("ground01_Albedo"), m_RsMG->GetTexture("ground01__Normal"));
		MaterialLayer matLayer2 = MaterialLayer(m_RsMG->GetTexture("ground02_Albedo"), m_RsMG->GetTexture("ground02__Normal"));
		terrainCom->AddLayer(m_RsMG->GetTexture("Terrain_RGB"), matLayer1, matLayer2);

		newObj->AddComponent(terrainCom);
	}
		break;
	default:
		break;
	}

	return newObj;
}

void Factory::SetRenderer(GameObject* obj, std::string meshKey, ParserData::Mesh* mesh, VertexBuffer* vBuffer)
{
	// 스키닝 오브젝트 구분..
	if (mesh->m_IsSkinningObject)
	{
		SkinMeshRenderer* sRenderer = new SkinMeshRenderer(eRasterizerType::Solid);
		sRenderer->SetMesh(mesh);
		sRenderer->SetVertexBuffer(vBuffer);

		// 적용되있는 Material 추가
		/// Mesh의 m_materialdata는 ref Material이므로 바로 적용시켜주자..
		if (mesh->m_MaterialData != nullptr)
		{
			sRenderer->m_Material->SetMaterialData(m_RsMG->GetMaterial(meshKey));

			if (mesh->m_MaterialData->m_IsDiffuseMap)
			{
				sRenderer->m_Material->SetDiffuseMap(m_RsMG->GetTexture(meshKey, eTextureType::Diffuse));
				sRenderer->m_Material->SetShader(m_RsMG->GetShader("SkinDeferredShader"));
			}
			if (mesh->m_MaterialData->m_IsBumpMap)
			{
				sRenderer->m_Material->SetNormalMap(m_RsMG->GetTexture(meshKey, eTextureType::Bump));
				sRenderer->m_Material->SetShader(m_RsMG->GetShader("NormalSkinDeferredShader"));
			}
		}
		else
		{
			sRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicSkinDeferredShader"));
		}

		m_MatMG->AddMaterial(sRenderer->m_Material);
		obj->AddComponent(sRenderer);
	}
	else
	{
		MeshRenderer* mRenderer = new MeshRenderer(eRasterizerType::Solid);
		mRenderer->SetMesh(mesh);
		mRenderer->SetVertexBuffer(vBuffer);

		// 적용되있는 Material 추가
		/// Mesh의 m_materialdata는 ref Material이므로 바로 적용시켜주자..
		if (mesh->m_MaterialData != nullptr)
		{
			mRenderer->m_Material->SetMaterialData(m_RsMG->GetMaterial(meshKey));
			mRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicDeferredShader"));
			if (mesh->m_MaterialData->m_IsDiffuseMap)
			{
				mRenderer->m_Material->SetDiffuseMap(m_RsMG->GetTexture(meshKey, eTextureType::Diffuse));
				mRenderer->m_Material->SetShader(m_RsMG->GetShader("TextureDeferredShader"));
			}
			if (mesh->m_MaterialData->m_IsBumpMap)
			{
				mRenderer->m_Material->SetNormalMap(m_RsMG->GetTexture(meshKey, eTextureType::Bump));
				mRenderer->m_Material->SetShader(m_RsMG->GetShader("NormalDeferredShader"));
			}
		}
		else
		{
			mRenderer->m_Material->SetShader(m_RsMG->GetShader("BasicDeferredShader"));
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
		cRenderer->m_Material->SetShader(m_RsMG->GetShader("UIBasicShader"));
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
		cRenderer->m_Material->SetShader(m_RsMG->GetShader("UIBasicShader"));
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

Animation* Factory::CreateAnimation(std::string objName, std::string aniName, GameObject* topNode, bool play)
{
	// Animator가 있는 최상위 노드의 Animator Component 찾기..
	GameObject* topObject = topNode;
	Animator* animator = topObject->GetComponent<Animator>();

	// 새로운 애니메이션 생성..
	Animation* animation = new Animation();

	// 현 Object의 Animation Data 삽입..
	ParserData::Model* model = m_RsMG->GetModel(objName);
	for (unsigned int i = 0; i < model->m_MeshList.size(); i++)
	{
		std::string meshKey = m_RsMG->GetMeshKey(objName, i);

		// Object 생성시 Animator에 들어있는 Obejct에 대한 애니메이션 정보를 넣는다..
		ParserData::OneAnimation* animationData = m_RsMG->GetAnimation(meshKey);
		animation->AddAnimationData(animator->GetMeshObject(i), animationData);
	}

	animator->AddAnimation(aniName, animation);

	if (play)
		animator->PlayAnimation(aniName);

	return animation;
}

Animation* Factory::CreateAnimation(string objName, string aniName, string nodeName, bool play)
{
	// Animator가 있는 최상위 노드의 Animator Component 찾기..
	GameObject* topNode = m_ObjMG->FindObject(nodeName);
	Animator* animator = topNode->GetComponent<Animator>();

	// 새로운 애니메이션 생성..
	Animation* animation = new Animation();

	// 현 Object의 Animation Data 삽입..
	ParserData::Model* model = m_RsMG->GetModel(objName);
	for (unsigned int i = 0; i < model->m_MeshList.size(); i++)
	{
		string meshKey = m_RsMG->GetMeshKey(objName, i);

		// Object 생성시 Animator에 들어있는 Obejct에 대한 애니메이션 정보를 넣는다..
		ParserData::OneAnimation* animationData = m_RsMG->GetAnimation(meshKey);
		animation->AddAnimationData(animator->GetMeshObject(i), animationData);
	}

	animator->AddAnimation(aniName, animation);

	if (play)
		animator->PlayAnimation(aniName);

	return animation;
}
