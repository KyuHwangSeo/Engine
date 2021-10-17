//------------------------------------------------------------
//
// ASE Parser
// Ver 2.1.20080124
// 
// 자체적으로 데이터 구조가 있으며 
// 이것을 D3D용 특정 오브젝트로 변환하는 곳까지 커버한다
//
//------------------------------------------------------------

// 파서 내부에서 특정 모드를 가지고 작동한다. (파서의 재귀구조 때문)
enum eParsingmode
{
	eNone, eGeomobject, eAnimation, eShape, eCamera, eLight, eHelperObject,
	eSkin_InitTM, eBone_List,
	eEnd,
};

/// ASE를 파싱하기 위한 클래스. 기본적으로 주어지는 Lexer클래스를 이용한다.

/// <summary>
/// 학생 때의 ASE Parser를 예시용으로 약간 개조했다.
///
/// 2021.04.12 LeHideKGIT
/// </summary>
class CASEParser
{
public:
	PARSER_DLL CASEParser();
	PARSER_DLL ~CASEParser();


	/// 초기화, 로드
	PARSER_DLL bool Load(LPSTR p_File);		// ASE파일을 읽어서, 자체 리스트에 넣는다.

	void OptimizeVertex(ParserData::ASEMesh* pMesh);		// 버텍스(노말), 텍스쳐 값으로 최적화를 해 준다.
	void RecombinationTM(ParserData::ASEMesh* pMesh);
	void OptimizeData();
	PARSER_DLL void SetBoneTM(ParserData::ASEMesh* pMesh);

private:
	// 토큰을 구별해내기 위한 스트링
	char m_TokenString[256];
	int m_Token;
	int m_parsingmode;				// 지금 모드. 재귀 내의 동일 변수때문. nodename이라던지.. row123, mapname등등..


public:
	/// 기본적인 읽어들인 내용들
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - 맨 처음 나오는 헤더인데 무슨 의미인지는 모르겠고, 일단 예의상 읽어서 보관해준다.
	ParserData::Scenedata m_scenedata;			// 현재 Scene의 데이터가 들어갈 구조체

	/// 매트리얼 관련
	int	m_materialcount;					// Material이 총 몇개인가?
	ParserData::CMaterial* m_MaterialData;			// Material의 데이터가 들어갈 구조체
	ParserData::MaterialMap* m_materialmap;
	std::vector<ParserData::CMaterial*> m_list_materialdata;	// Material들의 리스트

	/// 한 씬에 N개의 메시가 존재할 수 있다.
	ParserData::ASEMesh* m_OneMesh;						// 메시 하나의 데이터가 들어갈 구조체
	std::vector<ParserData::ASEMesh*> m_MeshList;	 			// 메시들

	ParserData::Bone* m_Bone;

	/// 애니메이션
	bool m_IsAnimation;
	OneAnimation* m_Animation;					// 한 오브젝트의 애니메이션 데이터(파싱용)

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
	// Parsing_DivergeRecursiveALL() 내부에서 쓰기 위한 함수들.
	// (parsing을 위한 단위별 함수들)
	//--------------------------------------------------
private:
	/// 파싱을 맡게 되는 비재귀함수. 여기서 모든 토큰의 처리를 한다.
	void DataParsing();

	int Parsing_NumberLong();			// long을 읽어서 리턴해준다.
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int
	Vector3 Parsing_ChangeNumberVector3();	// 3개의 Float를 벡터 하나로 읽어서 리턴해준다.
	Vector3 Parsing_NormalNumberVector3();	// 3개의 Float를 벡터 하나로 읽어서 리턴해준다.

	void Create_OneMesh_to_list();				// 메시를 하나 동적으로 생성하고, 리스트에 넣는다.
	void Create_MaterialData_to_list();			// 메트리얼 하나를
	void Create_DiffuseMap_to_list();			// 메트리얼 DiffuseMap Data
	void Create_BumpMap_to_list();				// 메트리얼 BumpMap Data
	void Create_SpecularMap_to_list();			// 메트리얼 SpecularMap Data
	void Create_ShineMap_to_list();				// 메트리얼 ShineMap Data
	void Create_AnimationData_to_mesh(ParserData::Mesh* nowMesh);
	void Create_OneVertex_to_list();	
	void Create_BoneData_to_list();	
};




























