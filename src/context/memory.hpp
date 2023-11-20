#ifndef CS2_CHEAT_CTX_MEMORY_HPP
#define CS2_CHEAT_CTX_MEMORY_HPP

#include <memory>

#include <xorstr.hpp>

#include "../core/helpers/memory.hpp"

#define ADD_ADDRESS( uVariable, szModuleName, szPattern ) Core::Memory::Address_t uVariable = Core::Memory::FindPattern( xorstr_( szModuleName ), xorstr_( szPattern ) )
#define ADD_EXPORT_ADDRESS( uVariable, szModuleName, szProcedureName ) Core::Memory::Address_t uVariable = Core::Memory::GetExportAddress( Core::Memory::GetModuleBaseAddress( xorstr_( szModuleName ) ), xorstr_( szProcedureName ) )
#define ADD_EXPORT_ADDRESS_TYPED( uVariable, szModuleName, szProcedureName, tVariable, ... ) std::decay_t< tVariable __VA_OPT__(, ) __VA_ARGS__ > uVariable = Core::Memory::GetExportAddress( Core::Memory::GetModuleBaseAddress( xorstr_( szModuleName ) ), xorstr_( szProcedureName ) ).Cast< decltype( uVariable ) >( )

namespace CTX
{
	struct Memory_t
	{
		struct
		{
			Core::Memory::Address_t Present = Core::Memory::FindPattern( xorstr_( GAMEOVERLAYRENDERER64_MODULE ), xorstr_( "48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 20 41 8B E8" ) );
			Core::Memory::Address_t ResizeBuffers = Core::Memory::FindPattern( xorstr_( GAMEOVERLAYRENDERER64_MODULE ), xorstr_( "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 8B FA" ) );
		} IDXGISwapChain;

		struct
		{
			ADD_EXPORT_ADDRESS_TYPED( SetRelativeMouseMode, SDL3_MODULE, "SDL_SetRelativeMouseMode", int ( * )( int iEnabled ) );
			ADD_EXPORT_ADDRESS_TYPED( SetWindowGrab, SDL3_MODULE, "SDL_SetWindowGrab", void( * )( void* pWindow, int iGrabbed ) );
			ADD_EXPORT_ADDRESS_TYPED( WarpMouseInWindow, SDL3_MODULE, "SDL_WarpMouseInWindow", void( * )( void* pWindow, float flTargetPosX, float flTargetPosY ) );
		} SDL;

		struct
		{
			ADD_EXPORT_ADDRESS_TYPED( AllocFunc, TIER0_MODULE, "MemAlloc_AllocFunc", void* ( * )( std::size_t uSize ) );
			ADD_EXPORT_ADDRESS_TYPED( ReallocFunc, TIER0_MODULE, "MemAlloc_ReallocFunc", void* ( * )( void* pMemory, std::size_t uNewSize ) );
			ADD_EXPORT_ADDRESS_TYPED( GetSizeFunc, TIER0_MODULE, "MemAlloc_GetSizeFunc", std::size_t ( * )( void* pMemory ) );
			ADD_EXPORT_ADDRESS_TYPED( FreeFunc, TIER0_MODULE, "MemAlloc_FreeFunc", void ( * )( void* pMemory ) );
			ADD_EXPORT_ADDRESS_TYPED( StrDupFunc, TIER0_MODULE, "MemAlloc_StrDupFunc", void ( * )( const char* szString ) );
		} MemAlloc;
	};

	inline std::unique_ptr< Memory_t > pMemory = nullptr;
} // namespace CTX

#undef ADD_EXPORT_ADDRESS
#undef ADD_ADDRESS

#endif // CS2_CHEAT_CTX_MEMORY_HPP
