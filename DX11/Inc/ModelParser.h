#pragma once
#pragma warning(disable : 4251)

#include "ParserDLL.h"
#include "../ShareData/ParserData.h"

/// Model Parser InterFace Class
/// 
/// - 외부에서 실질적으로 사용할 함수들을 모아논 InterFace Class
/// - Parser Type을 통해 DLL 내부에서 할당 후 포인터 반환 형식
/// - 해당 Parser Class가 아닌 InterFace Class를 통해 함수 호출 방식

class ModelParser
{
public:
	ModelParser() = default;
	virtual ~ModelParser() = default;

public:
	// 사용 할 Model Parser 타입
	enum Type
	{
		ASE,
		FBX
	};

public:
	// Create 함수를 통해 ModelParser 생성..
	static PARSER_DLL ModelParser* Create(Type type);

public:
	// 기본적인 초기화 함수..
	virtual PARSER_DLL void Initialize() abstract;

	// 기본 Texture 경로 설정 함수..
	virtual PARSER_DLL void SetTextureRoute(std::string texRoute) abstract;

	// Model Data 해제 함수..
	virtual PARSER_DLL void Release() abstract;

	// Model Load 함수..
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName) { return nullptr; }
	virtual PARSER_DLL ParserData::Model* LoadModel(std::string fileName, bool scaling, bool onlyAni = false) { return nullptr; }
};

