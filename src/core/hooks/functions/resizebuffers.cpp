#include "../hooks.hpp"

std::int32_t Core::CHooks::ResizeBuffers( IDXGISwapChain* pSwapChain, std::uint32_t uBufferCount, std::uint32_t uWidth, std::uint32_t uHeight, std::int32_t iNewFormat, std::uint32_t uSwapChainFlags ) noexcept
{
	return pfnResizeBuffers( pSwapChain, uBufferCount, uWidth, uHeight, iNewFormat, uSwapChainFlags );
}