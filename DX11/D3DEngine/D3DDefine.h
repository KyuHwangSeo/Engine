#pragma once

// DX Lib
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Sound Lib
#pragma comment(lib, "dsound.lib") 
#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "winmm.lib")

#ifdef _WIN64
	#ifdef _DEBUG
		#pragma comment( lib, "DirectXTK_x64d.lib" )
	#else
		#pragma comment( lib, "DirectXTK_x64.lib" )
	#endif
#else
	#ifdef _DEBUG
		#pragma comment( lib, "DirectXTK_x86d.lib" )
	#else
		#pragma comment( lib, "DirectXTK_x86.lib" )
	#endif
#endif

#include <d3dcompiler.h>
#include <D3D11Shader.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <tchar.h>
#include <DirectXPackedVector.h>
#include "d3dx11Effect.h"
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <atlstr.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>

// Sound
#include <mmsystem.h> 
#include <dsound.h> 
#include <stdio.h>

#include "MathHelper.h"
#include <d3d11.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "SingleTon.h"

#include "Patch_Chapters.h"
#include "DXInput.h"
#include "KHMath.h"
#include "LightHelper.h"
#include "EnumDefine.h"
#include "DeleteDefine.h"
#include "ConstantBufferDefine.h"

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <concrt.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;

struct VertexPos
{
	DXVector3 Pos;
};

struct Vertex
{
	DXVector3 Pos;
	DXVector4 Color;
};

struct PosTex
{
	DXVector3 Pos;
	DXVector2 Tex;
};

struct PosNormal
{
	DXVector3 Pos;
	DXVector3 Normal;
};

struct TexVertex
{
	DXVector3 Pos;
	DXVector3 Normal;
	DXVector2 Tex;
};

struct SkinVertex
{
	DXVector3 Pos;
	DXVector3 Normal;
	DXVector2 Tex;
	UINT BoneIndex1[4];
	UINT BoneIndex2[4];
	UINT BoneIndex3[2];
	FLOAT BoneWeight1[4];
	FLOAT BoneWeight2[4];
	FLOAT BoneWeight3[2];
	DXVector3 Tangent;
};

struct NormalMapVertex
{
	DXVector3 Pos;
	DXVector3 Normal;
	DXVector2 Tex;
	DXVector3 Tangent;
};

struct VertexBuffer
{
	ComPtr<ID3D11Buffer> VB;		// 버텍스버퍼
	ComPtr<ID3D11Buffer> IB;		// 인덱스버퍼

	UINT IndexCount;			// 버텍스 개수
	UINT Stride;				// 구조체 사이즈
	UINT Offset = 0;			// Offset

	eColliderType m_ColType;
	DirectX::BoundingBox m_MeshBox;
	DirectX::BoundingSphere m_MeshSphere;

	std::vector<DXVector3> m_VertexPos;
	std::vector<UINT> m_MeshIndices;
};

namespace DXColors
{
	XMGLOBALCONST XMVECTORF32 DeepDarkGray = { { { 0.1f, 0.1f, 0.1f, 1.0f } } };
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

