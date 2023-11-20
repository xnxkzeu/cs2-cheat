#include <Windows.h>

#include <thread>

#include <xorstr.hpp>

#include "core/helpers/logger.hpp"
#include "core/helpers/memory.hpp"
#include "core/helpers/schema.hpp"
#include "core/helpers/input.hpp"
#include "core/helpers/render.hpp"
#include "core/helpers/entities.hpp"

#include "core/ui/menu.hpp"

#include "core/hooks/hooks.hpp"

#include "features/esp.hpp"

#include "context/cs2.hpp"
#include "context/memory.hpp"
#include "context/config.hpp"

void OnDllDetach( HMODULE hModule ) noexcept;
void OnDllAttach( HMODULE hModule ) noexcept
{
	try
	{
		Core::pLogger = std::make_unique< Core::CLogger >( );
		Core::pLogger->Info( xorstr_( "logger initialized." ) );

		Core::pLogger->Info( xorstr_( "looking for \"" MATCHMAKING_MODULE "\"" ) );

		while ( !Core::Memory::GetModuleBaseAddress( xorstr_( MATCHMAKING_MODULE ) ) )
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );

		Core::pLogger->Info( xorstr_( "found \"" MATCHMAKING_MODULE "\"" ) );

		CTX::pCS2 = std::make_unique< CTX::CS2_t >( );
		CTX::pMemory = std::make_unique< CTX::Memory_t >( );
		CTX::pConfig = std::make_unique< CTX::Config_t >( );
		Core::pLogger->Info( xorstr_( "context initialized." ) );

		Core::pSchema = std::make_unique< Core::CSchema >( );
		Core::pLogger->Info( xorstr_( "schema initialized." ) );

		Core::pInput = std::make_unique< Core::CInput >( );
		Core::pLogger->Info( xorstr_( "input handler initialized." ) );

		Core::pRender = std::make_unique< Core::CRender >( );
		Core::pLogger->Info( xorstr_( "render initialized." ) );

		Core::UI::pMenu = std::make_unique< Core::UI::CMenu >( );
		Core::pLogger->Info( xorstr_( "menu initialized." ) );

		Core::pEntityList = std::make_unique< Core::CEntityList >( );
		Core::pLogger->Info( xorstr_( "entity cacher initialized." ) );

		Features::pESP = std::make_unique< Features::CESP >( );
		Core::pLogger->Info( xorstr_( "features initialized." ) );

		Core::pHooks = std::make_unique< Core::CHooks >( );
		Core::pLogger->Info( xorstr_( "hook handler initialized." ) );

		// clang-format off
		Core::pInput->RegisterCallback( VK_END, [ hModule ]( )
										{
											Core::pInput->RemoveCallback( VK_END ); // @note / xnxkzeu: we need to remove callback as soon as possible.
											std::thread( OnDllDetach, hModule ).detach( );
										} );
		// clang-format on
	}
	catch ( const std::exception& exInfo )
	{
#ifdef _DEBUG
		MessageBoxA( nullptr, std::vformat( xorstr_( "something went wrong during initialization: {}" ), std::make_format_args( exInfo.what( ) ) ).c_str( ), PROJECT_NAME, MB_ICONERROR );
#endif
		std::exit( EXIT_FAILURE );
	}
}

void OnDllDetach( HMODULE hModule ) noexcept
{
	Core::pLogger->Info( xorstr_( "unloading..." ) );

	Core::pHooks.reset( );

	Features::pESP.reset( );

	Core::pEntityList.reset( );
	Core::UI::pMenu.reset( );
	Core::pRender.reset( );
	Core::pInput.reset( );
	Core::pSchema.reset( );

	CTX::pConfig.reset( );
	CTX::pMemory.reset( );
	CTX::pCS2.reset( );

	Core::pLogger.reset( );

	FreeLibrary( hModule );
}

extern "C" BOOL _CRT_INIT( HMODULE hModule, DWORD dwReason, LPVOID lpReserved ) noexcept;

int DllEntryPoint( HMODULE hModule, DWORD dwReason, LPVOID lpReserved ) noexcept
{
	if ( dwReason == DLL_PROCESS_ATTACH )
	{
		if ( !_CRT_INIT( hModule, dwReason, lpReserved ) )
			return FALSE;

		std::thread( OnDllAttach, hModule ).detach( );
		return TRUE;
	}

	return _CRT_INIT( hModule, dwReason, lpReserved );
}