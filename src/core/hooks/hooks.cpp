#include <stdexcept>
#include <format>

#include <MinHook.h>

#include <xorstr.hpp>

#include "hooks.hpp"

#include "../../context/memory.hpp"
#include "../../context/cs2.hpp"

#define CREATE_HOOK( pFunc, pDetour ) pfn##pDetour = reinterpret_cast< decltype( &pDetour ) >( HookFunction( pFunc, reinterpret_cast< void* >( pDetour ) ) )
#define CREATE_VIRTUAL_HOOK( pInstance, uIndex, pDetour ) pfn##pDetour = reinterpret_cast< decltype( &pDetour ) >( HookVirtualFunction( pInstance, uIndex, reinterpret_cast< void* >( pDetour ) ) )

namespace Core
{
	CHooks::CHooks( )
	{
		MH_Initialize( );

		CREATE_VIRTUAL_HOOK( CTX::pCS2->pSource2Client, 33, FrameStageNotify );
		CREATE_VIRTUAL_HOOK( CTX::pCS2->pCSGOInput, 12, MouseInputEnabled );

		CREATE_HOOK( CTX::pMemory->IDXGISwapChain.Present, Present );
		CREATE_HOOK( CTX::pMemory->IDXGISwapChain.ResizeBuffers, ResizeBuffers );

		MH_EnableHook( MH_ALL_HOOKS );
	}

	CHooks::~CHooks( ) noexcept
	{
		MH_DisableHook( MH_ALL_HOOKS );

		MH_Uninitialize( );
	}

	void* CHooks::HookFunction( void* pfnTarget, void* pfnDetour )
	{
		void* pfnOriginal = nullptr;
		if ( MH_STATUS mhStatus = MH_CreateHook( pfnTarget, pfnDetour, &pfnOriginal ); mhStatus != MH_OK )
			throw std::runtime_error( std::vformat( xorstr_( "failed to hook function: {}" ), std::make_format_args( MH_StatusToString( mhStatus ) ) ) );

		return pfnOriginal;
	}

	void* CHooks::HookVirtualFunction( void* pInstance, std::uint32_t uIndex, void* pfnDetour )
	{
		void* pfnOriginal = nullptr;
		if ( MH_STATUS mhStatus = MH_CreateHook( ( *reinterpret_cast< void*** >( pInstance ) )[ uIndex ], pfnDetour, &pfnOriginal ); mhStatus != MH_OK )
			throw std::runtime_error( std::vformat( xorstr_( "Failed to hook virtual function: {}" ), std::make_format_args( MH_StatusToString( mhStatus ) ) ) );

		return pfnOriginal;
	}
} // namespace Core

#undef CREATE_VIRTUAL_HOOK
#undef CREATE_HOOK