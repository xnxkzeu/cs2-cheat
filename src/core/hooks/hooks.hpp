#ifndef CS2_CHEAT_HOOKS_HPP
#define CS2_CHEAT_HOOKS_HPP

#include <cstdint>
#include <memory>
#include <mutex>

#include "../source2_engine/datatypes/centityhandle.hpp"

class CCSGOInput;
class CGameEntitySystem;
class CEntityInstance;

struct IDXGISwapChain;
struct ISource2Client;

namespace Core
{
	class CHooks
	{
	public:
		CHooks( );
		~CHooks( ) noexcept;

	private:
		[[nodiscard]] static void* HookFunction( void* pfnTarget, void* pfnDetour );
		[[nodiscard]] static void* HookVirtualFunction( void* pInstance, std::uint32_t uIndex, void* pfnDetour );

	public:
		static void FrameStageNotify( ISource2Client* pSource2Client, int iFrameStage ) noexcept;
		static inline decltype( &FrameStageNotify ) pfnFrameStageNotify = nullptr;

		static bool MouseInputEnabled( CCSGOInput* pCSGOInput ) noexcept;
		static inline decltype( &MouseInputEnabled ) pfnMouseInputEnabled = nullptr;

		static std::int32_t Present( IDXGISwapChain* pSwapChain, std::uint32_t uSyncInterval, std::uint32_t uFlags ) noexcept;
		static inline decltype( &Present ) pfnPresent = nullptr;

		static std::int32_t ResizeBuffers( IDXGISwapChain* pSwapChain, std::uint32_t uBufferCount, std::uint32_t uWidth, std::uint32_t uHeight, std::int32_t iNewFormat, std::uint32_t uSwapChainFlags ) noexcept;
		static inline decltype( &ResizeBuffers ) pfnResizeBuffers = nullptr;
	};

	inline std::unique_ptr< CHooks > pHooks = nullptr;
} // namespace Core

#endif // CS2_CHEAT_HOOKS_HPP
