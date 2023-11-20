#include <Windows.h>

#include <array>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/wincolor_sink.h>

#include <xorstr.hpp>

#include "logger.hpp"

namespace Core
{
	CLogger::CLogger( )
	{
#ifdef _DEBUG
		if ( !AllocConsole( ) )
			throw std::runtime_error( xorstr_( "failed to allocate console." ) );

		AttachConsole( ATTACH_PARENT_PROCESS );

		if ( ( m_pOutputStream.reset( std::freopen( "CONOUT$", "w", stdout ) ) ); !m_pOutputStream )
			throw std::runtime_error( xorstr_( "failed to redirect console output." ) );

		SetConsoleTitle( xorstr_( TEXT( PROJECT_NAME " debug console" ) ) );

		std::shared_ptr pWinColorSink = std::make_shared< spdlog::sinks::wincolor_stdout_sink_mt >( );
		pWinColorSink->set_color( spdlog::level::debug, FOREGROUND_RED | FOREGROUND_GREEN );
		pWinColorSink->set_color( spdlog::level::trace, FOREGROUND_INTENSITY );
		pWinColorSink->set_color( spdlog::level::info, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
		pWinColorSink->set_color( spdlog::level::warn, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		pWinColorSink->set_color( spdlog::level::err, FOREGROUND_RED | FOREGROUND_INTENSITY );
		pWinColorSink->set_color( spdlog::level::critical, FOREGROUND_RED );
#endif

		std::array arrSinks = std::to_array< spdlog::sink_ptr >( {
			std::make_shared< spdlog::sinks::basic_file_sink_mt >( xorstr_( PROJECT_NAME ".log" ), true ),
#ifdef _DEBUG
			pWinColorSink,
#endif
		} );

		m_pLogger = std::make_unique< spdlog::logger >( xorstr_( PROJECT_NAME " logger" ), arrSinks.begin( ), arrSinks.end( ) );

		m_pLogger->set_pattern( xorstr_( "%^[%H:%M:%S][%t][%l] %v%$" ) );

#ifdef _DEBUG
		m_pLogger->set_level( spdlog::level::trace );
#endif
	}

	CLogger::~CLogger( ) noexcept
	{
		m_pLogger.reset( );

		spdlog::shutdown( );

		m_pOutputStream.reset( );

		FreeConsole( );
	}
} // namespace Core