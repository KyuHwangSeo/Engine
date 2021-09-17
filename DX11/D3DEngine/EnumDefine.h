#pragma once

// Tag Type Enum Class
enum class eTag
{
	Defalt,
	Player,
	Enemy,
	Field,

	Sword,
	Note,
	NoteCheck,
	Portal,
	ExitPortal,
};

// Mesh Type Enum Class
enum class eMeshType
{
	None = 0,
	NormalMesh,
	Helper,
	SkinMesh,
	Canvas,
	Bone
};

// Rasterizer Type Enum Class
enum class eRasterizerType
{
	Wire,
	Solid
};

// Topology Type Enum Class
enum class eTopologyType
{
	Triangle,
	Line
};

// Camera Type Enum Class
enum class eCameraType
{
	NormalCam,
	FollowCam_3,
	FollowCam_1
};

// Light Type Enum Class
enum class eLightType
{
	Direction,
	Point,
	Spot
};

// Data Load Type Enum Class
enum class eLoadType
{
	Texture,
	Sky,
	Text,
	ASE,
	ASE_Ani,
	FBX,
	FBX_Ani
};

// Model Data Load Type Enum Class
enum class eModelType
{
	Empty,
	Camera,
	DirLight,
	PointLight,
	SpotLight,
	Grid,
	Axis,
	Sky,
	Box,
	Sphere,
	Cylinder,
	Text,
	Gizmo,
	ASE,
	FBX,
};

// ModelCollider Type Enum Class
enum class eModelCollider
{
	ALL,
	BoneList,
	MeshList
};

// Object Create Type Enum Class
enum class eObjectType
{
	Model,
	Camera,
	Light
};

// UI Type Enum Class
enum class eUIType
{
	Image,
	Button

};

// Shader Tech Type Enum Class
enum class eShaderType
{
	None,
	Color,
	Basic,
	Texture,
	Skinning,
	NormalMap,
	ReflectBasic,
	ReflectNormalMap,
	Reflection,
	CubeMapSky,
};

// Gizmo Type Enum Class
enum class eGizmoType
{
	Axis,
	Camera,
	Light
};

// Texture Map Enum Class
enum class eTextureType
{
	Diffuse,
	Bump,
	Specular,
	Shine
};

// Collider Type Enum Class
enum class eColliderType
{
	None,
	Box2D,
	Circle2D,
	Box,
	Sphere,
	Mesh,
};

// Collision State Enum Class
enum class eCollisionState
{
	None,
	Start,
	Stay,
	Exit
};