#include "EnginePCH.h"
#include "Renderer/RendererAPI.h"

namespace NanSu
{
#ifdef NS_PLATFORM_WINDOWS
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX11;
#else
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
#endif

}
