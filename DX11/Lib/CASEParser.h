//------------------------------------------------------------
//
// ASE Parser
// Ver 2.1.20080124
// 
// ��ü������ ������ ������ ������ 
// �̰��� D3D�� Ư�� ������Ʈ�� ��ȯ�ϴ� ������ Ŀ���Ѵ�
//
//------------------------------------------------------------

// �ļ� ���ο��� Ư�� ��带 ������ �۵��Ѵ�. (�ļ��� ��ͱ��� ����)
enum eParsingmode
{
	eNone, eGeomobject, eAnimation, eShape, eCamera, eLight, eHelperObject,
	eSkin_InitTM, eBone_List,
	eEnd,
};

/// ASE�� �Ľ��ϱ� ���� Ŭ����. �⺻������ �־����� LexerŬ������ �̿��Ѵ�.

/// <summary>
/// �л� ���� ASE Parser�� ���ÿ����� �ణ �����ߴ�.
///
/// 2021.04.12 LeHideKGIT
/// </summary>
class CASEParser
{
public:
	PARSER_DLL CASEParser();
	PARSER_DLL ~CASEParser();


	/// �ʱ�ȭ, �ε�
	PARSER_DLL bool Load(LPSTR p_File);		// ASE������ �о, ��ü ����Ʈ�� �ִ´�.

	void OptimizeVertex(ParserData::ASEMesh* pMesh);		// ���ؽ�(�븻), �ؽ��� ������ ����ȭ�� �� �ش�.
	void RecombinationTM(ParserData::ASEMesh* pMesh);
	void OptimizeData();
	PARSER_DLL void SetBoneTM(ParserData::ASEMesh* pMesh);

private:
	// ��ū�� �����س��� ���� ��Ʈ��
	char m_TokenString[256];
	int m_Token;
	int m_parsingmode;				// ���� ���. ��� ���� ���� ��������. nodename�̶����.. row123, mapname���..


public:
	/// �⺻���� �о���� �����
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - �� ó�� ������ ����ε� ���� �ǹ������� �𸣰ڰ�, �ϴ� ���ǻ� �о �������ش�.
	ParserData::Scenedata m_scenedata;			// ���� Scene�� �����Ͱ� �� ����ü

	/// ��Ʈ���� ����
	int	m_materialcount;					// Material�� �� ��ΰ�?
	ParserData::CMaterial* m_MaterialData;			// Material�� �����Ͱ� �� ����ü
	ParserData::MaterialMap* m_materialmap;
	std::vector<ParserData::CMaterial*> m_list_materialdata;	// Material���� ����Ʈ

	/// �� ���� N���� �޽ð� ������ �� �ִ�.
	ParserData::ASEMesh* m_OneMesh;						// �޽� �ϳ��� �����Ͱ� �� ����ü
	std::vector<ParserData::ASEMesh*> m_MeshList;	 			// �޽õ�

	ParserData::Bone* m_Bone;

	/// �ִϸ��̼�
	bool m_IsAnimation;
	ParserData::OneAnimation* m_Animation;					// �� ������Ʈ�� �ִϸ��̼� ������(�Ľ̿�)

public:
	//--------------------------------------------------
	// Export
	//--------------------------------------------------
	ParserData::Mesh* GetMesh(int m_Index);


public:
	//--------------------------------------------------
	//	Lexcer
	//--------------------------------------------------
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
	Vector3 Parsing_ChangeNumberVector3();	// 3���� Float�� ���� �ϳ��� �о �������ش�.
	Vector3 Parsing_NormalNumberVector3();	// 3���� Float�� ���� �ϳ��� �о �������ش�.

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




























