#ifndef CS2_CHEAT_LOGGER_HPP
#define CS2_CHEAT_LOGGER_HPP

#include <cstdlib>
#include <cstdio>
#include <memory>

#include <spdlog/spdlog.h>

enum class LoggingLevel : std::uint8_t
{
	Trace,
	Debug,
	Info,
	Warning,
	Error,
	Critical,
};

namespace Core
{
	class CLogger
	{
	public:
		CLogger( );
		~CLogger( ) noexcept;

		template< typename... tArgs >
		constexpr void Trace( std::format_string< tArgs... > szFormat, tArgs&&... argList ) noexcept
		{
			m_pLogger->trace( szFormat, std::forward< tArgs >( argList )... );
		}

		template< typename tString >
		constexpr void Trace( const tString& szText ) noexcept
		{
			m_pLogger->trace( szText );
		}

		template< typename... tArgs >
		constexpr void Debug( std::format_string< tArgs... > szFormat, tArgs&&... argList ) noexcept
		{
			m_pLogger->debug( szFormat, std::forward< tArgs >( argList )... );
		}

		template< typename tString >
		constexpr void Debug( const tString& szText ) noexcept
		{
			m_pLogger->debug( szText );
		}

		template< typename... tArgs >
		constexpr void Info( std::format_string< tArgs... > szFormat, tArgs&&... argList ) noexcept
		{
			m_pLogger->info( szFormat, std::forward< tArgs >( argList )... );
		}

		template< typename tString >
		constexpr void Info( const tString& szText ) noexcept
		{
			m_pLogger->info( szText );
		}

		template< typename... tArgs >
		constexpr void Warning( std::format_string< tArgs... > szFormat, tArgs&&... argList ) noexcept
		{
			m_pLogger->warn( szFormat, std::forward< tArgs >( argList )... );
		}

		template< typename tString >
		constexpr void Warning( const tString& szText ) noexcept
		{
			m_pLogger->warn( szText );
		}

		template< typename... tArgs >
		constexpr void Error( std::format_string< tArgs... > szFormat, tArgs&&... argList ) noexcept
		{
			m_pLogger->error( szFormat, std::forward< tArgs >( argList )... );
		}

		template< typename tString >
		constexpr void Error( const tString& szText ) noexcept
		{
			m_pLogger->error( szText );
		}

		template< typename... tArgs >
		constexpr void Critical( std::format_string< tArgs... > szFormat, tArgs&&... argList ) noexcept
		{
			m_pLogger->critical( szFormat, std::forward< tArgs >( argList )... );
		}

		template< typename tString >
		constexpr void Critical( const tString& szText ) noexcept
		{
			m_pLogger->critical( szText );
		}
	private:
		class CFileDestructor
		{
		public:
			static void operator( )( std::FILE* pFile ) noexcept
			{
				std::fclose( pFile );
			}
		};

		std::unique_ptr< std::FILE, CFileDestructor > m_pOutputStream = nullptr;
		std::unique_ptr< spdlog::logger > m_pLogger = nullptr;
	};

	inline std::unique_ptr< CLogger > pLogger = nullptr;
} // namespace Core

#endif // CS2_CHEAT_LOGGER_HPP
