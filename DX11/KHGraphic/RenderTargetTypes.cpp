#include <vector>
#include "EnumDefine.h"
#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
#include "ResourceManagerBase.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
#include "ResourceManager.h"
#include "RenderTargetTypes.h"

OriginalRenderTarget::operator BasicRenderTarget* ()
{
	return pThis->GetBasicRenderTarget(state);
}

OriginalRenderTarget::operator ComputeRenderTarget* ()
{
	return pThis->GetComputeRenderTarget(state);
}
