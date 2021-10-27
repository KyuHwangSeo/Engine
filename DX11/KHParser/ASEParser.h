//------------------------------------------------------------
// ASE Parser
//------------------------------------------------------------

enum eParsingmode
{
	eNone, eGeomobject, eAnimation, eShape, eCamera, eLight, eHelperObject,
	eSkin_InitTM, eBone_List,
	eEnd,
};

class ASEParser : public ModelParser
{
public:
	ASEParser();
	~ASEParser();

public:
	void Initialize();
	void SetTextureRoute(std::string texRoute);
	void Release();
	ParserData::Model* LoadModel(std::string fileName);

	void OptimizeVertex(ParserData::ASEMesh* pMesh);		// ���ؽ�(�븻), �ؽ��� ������ ����ȭ�� �� �ش�.
	void RecombinationTM(ParserData::ASEMesh* pMesh);
	void OptimizeData();
	void SetBoneTM(ParserData::ASEMesh* pMesh);

	void CreateModel();
	void ResetData();
public:
	ParserData::Model* m_Model;

private:
	// ��ū�� �����س��� ���� ��Ʈ��
	char m_TokenString[256];
	int m_Token;
	int m_parsingmode;				// ���� ���. ��� ���� ���� ��������. nodename�̶����.. row123, mapname���..

private:
	/// �⺻���� �о���� �����
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - �� ó�� ������ ����ε� ���� �ǹ������� �𸣰ڰ�, �ϴ� ���ǻ� �о �������ش�.
	ParserData::Scenedata m_scenedata;			// ���� Scene�� �����Ͱ� �� ����ü

	/// ��Ʈ���� ����
	int	m_materialcount;					// Material�� �� ��ΰ�?
	ParserData::CMaterial* m_MaterialData;			// Material�� �����Ͱ� �� ����ü
	ParserData::MaterialMap* m_materialmap;
	std::vector<ParserData::CMaterial*> m_MaterialList;	// Material���� ����Ʈ

	/// �� ���� N���� �޽ð� ������ �� �ִ�.
	ParserData::ASEMesh* m_OneMesh;						// �޽� �ϳ��� �����Ͱ� �� ����ü
	std::vector<ParserData::ASEMesh*> m_MeshList;	 			// �޽õ�

	ParserData::Bone* m_Bone;

	/// �ִϸ��̼�
	bool m_IsAnimation;
	ParserData::OneAnimation* m_Animation;					// �� ������Ʈ�� �ִϸ��̼� ������(�Ľ̿�)

	std::string m_TexRoute;

private:
	ASE::CASELexer* m_lexer;


	//--------------------------------------------------
	// Parsing_DivergeRecursiveALL() ���ο��� ���� ���� �Լ���.
	// (parsing�� ���� ������ �Լ���)
	//--------------------------------------------------
private:
	/// �Ľ��� �ð� �Ǵ� ������Լ�. ���⼭ ��� ��ū�� ó���� �Ѵ�.
	void DataParsing();

	int Parsing_NumberLong();			// long�� �о �������ش�.
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int
	DirectX::SimpleMath::Vector3 Parsing_ChangeNumberVector3();	// 3���� Float�� ���� �ϳ��� �о �������ش�.
	DirectX::SimpleMath::Vector3 Parsing_NormalNumberVector3();	// 3���� Float�� ���� �ϳ��� �о �������ش�.

	void Create_OneMesh_to_list();				// �޽ø� �ϳ� �������� �����ϰ�, ����Ʈ�� �ִ´�.
	void Create_MaterialData_to_list();			// ��Ʈ���� �ϳ���
	void Create_DiffuseMap_to_list();			// ��Ʈ���� DiffuseMap Data
	void Create_BumpMap_to_list();				// ��Ʈ���� BumpMap Data
	void Create_SpecularMap_to_list();			// ��Ʈ���� SpecularMap Data
	void Create_ShineMap_to_list();				// ��Ʈ���� ShineMap Data
	void Create_AnimationData_to_mesh(ParserData::Mesh* nowMesh);
	void Create_OneVertex_to_list();
	void Create_BoneData_to_list();
};




























