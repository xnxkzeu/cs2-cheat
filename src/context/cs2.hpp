#ifndef CS2_CHEAT_CTX_CS2_HPP
#define CS2_CHEAT_CTX_CS2_HPP

#include <memory>
#include <format>

#include <xorstr.hpp>

#include "../core/helpers/memory.hpp"

#define ADD_INTERFACE( tInterface, pVariable, szModuleName, szInterfaceName ) tInterface* pVariable = GetInterface( xorstr_( szModuleName ), xorstr_( szInterfaceName ) ).Cast< tInterface* >( )
#define ADD_INTERFACE_CUSTOM( tInterface, pVariable, uAddress ) tInterface* pVariable = uAddress.Cast< tInterface* >( )
#define ADD_GAMESYSTEM( tGameSystem, ppVariable, szGameSystemName ) tGameSystem* ppVariable = GetGameSystem( xorstr_( szGameSystemName ) ).Cast< tGameSystem* >( )

struct ISource2Engine;
struct ISchemaSystem;
struct ISource2Client;
struct IInputSystem;
struct ICvar;

class CCSGOInput;
class CRenderDeviceDx11;
class CGameEntitySystem;

class CRenderGameSystem;

namespace CTX
{
	struct CS2_t
	{
		ADD_INTERFACE( ISource2Engine, pSource2Engine, ENGINE2_MODULE, "Source2EngineToClient" );
		ADD_INTERFACE( ISource2Client, pSource2Client, CLIENT_MODULE, "Source2Client" );
		ADD_INTERFACE( ISchemaSystem, pSchemaSystem, SCHEMASYSTEM_MODULE, "SchemaSystem_" );
		ADD_INTERFACE( IInputSystem, pInputSystem, INPUTSYSTEM_MODULE, "InputSystemVersion" );
		ADD_INTERFACE( ICvar, pCvar, TIER0_MODULE, "VEngineCvar" );

		ADD_INTERFACE_CUSTOM( CCSGOInput, pCSGOInput, Core::Memory::FindPattern( xorstr_( CLIENT_MODULE ), xorstr_( "48 8B 0D ? ? ? ? 41 8B D5 4C 8B 09" ) ).Jump( 0x3 ).Get( ) );
		ADD_INTERFACE_CUSTOM( CRenderDeviceDx11, pRenderDeviceDx11, Core::Memory::FindPattern( xorstr_( CLIENT_MODULE ), xorstr_( "48 8B 0D ? ? ? ? 66 44 89 6C 24" ) ).Jump( 0x3 ).Get( ) );
		ADD_INTERFACE_CUSTOM( CGameEntitySystem, pEntitySystem, Core::Memory::FindPattern( xorstr_( CLIENT_MODULE ), xorstr_( "48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 48 8B F8" ) ).Jump( 0x3 ).Get( ) );

		ADD_GAMESYSTEM( CRenderGameSystem*, ppRenderGameSystem, "RenderGameSystem" );
	private:
		[[nodiscard]] static Core::Memory::Address_t GetInterface( std::string_view szModuleName, std::string_view szInterfaceName )
		{
			class CInterfaceRegister
			{
			public:
				CInterfaceRegister( ) = delete;
				CInterfaceRegister( CInterfaceRegister&& ) = delete;
				CInterfaceRegister( const CInterfaceRegister& ) = delete;

				void* ( *pfnCreate )( );
				const char* szName;
				CInterfaceRegister* pNext;
			};

			Core::Memory::Address_t uModuleBaseAddress = Core::Memory::GetModuleBaseAddress( szModuleName );
			if ( !uModuleBaseAddress )
				throw std::runtime_error( std::vformat( xorstr_( R"(failed to find "{}" module.)" ), std::make_format_args( szModuleName ) ) );

			Core::Memory::Address_t uCreateInterfaceAddress = Core::Memory::GetExportAddress( uModuleBaseAddress, xorstr_( "CreateInterface" ) );
			if ( !uCreateInterfaceAddress )
				throw std::runtime_error( std::vformat( xorstr_( R"(failed to find "CreateInterface" in "{}" module.)" ), std::make_format_args( szModuleName ) ) );

			const CInterfaceRegister* pRegister = uCreateInterfaceAddress.Jump( 0x3 ).Get< CInterfaceRegister* >( );

			for ( ; pRegister; pRegister = pRegister->pNext )
			{
				if ( ( std::string_view( pRegister->szName ).compare( 0, szInterfaceName.length( ), szInterfaceName ) == 0 && std::atoi( pRegister->szName + szInterfaceName.length( ) ) > 0 ) ||
					 szInterfaceName.compare( pRegister->szName ) == 0 )
					return Core::Memory::Address_t( pRegister->pfnCreate( ) );
			}

			throw std::runtime_error( std::vformat( xorstr_( R"(failed to find "{}" interface in "{}" module.)" ), std::make_format_args( szInterfaceName, szModuleName ) ) );
		}

		struct IGameSystemFactory
		{
			IGameSystemFactory( ) = delete;
			IGameSystemFactory( IGameSystemFactory&& ) = delete;
			IGameSystemFactory( const IGameSystemFactory& ) = delete;

			void* pVTable;
			IGameSystemFactory* pNextFactory;
			const char* szName;
			void* pGameSystem;
		};

		static inline IGameSystemFactory* m_pGameSystemFactory = nullptr;

		[[nodiscard]] static Core::Memory::Address_t GetGameSystem( std::string_view szGameSystemName )
		{
			if ( !m_pGameSystemFactory )
				m_pGameSystemFactory = Core::Memory::FindPattern( xorstr_( CLIENT_MODULE ), xorstr_( "48 8B 1D ? ? ? ? 48 85 DB 0F 84 ? ? ? ? BE" ) ).Jump( 0x3 ).Get( ).Cast< IGameSystemFactory* >( );

			IGameSystemFactory* pFactory = m_pGameSystemFactory;
			if ( !pFactory )
				throw std::runtime_error( xorstr_( "failed to find game system factory list." ) );

			for ( ; pFactory; pFactory = pFactory->pNextFactory )
			{
				if ( szGameSystemName.compare( pFactory->szName ) != 0 )
					continue;

				return Core::Memory::Address_t( pFactory->pGameSystem );
			}

			throw std::runtime_error( std::vformat( xorstr_( R"(failed to find "{}" game system.)" ), std::make_format_args( szGameSystemName ) ) );
		}
	};

	inline std::unique_ptr< CS2_t > pCS2 = nullptr;
} // namespace CTX

#undef ADD_GAMESYSTEM
#undef ADD_INTERFACE_CUSTOM
#undef ADD_INTERFACE

#endif // CS2_CHEAT_CTX_CS2_HPP
