#include "DirectDefine.h"
#include "ShaderBase.h"
#include "ShaderResourceBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderManagerBase.h"

ShaderType IShaderManager::GetShader(std::string shaderName)
{
	return ShaderType{ this,shaderName };
}
