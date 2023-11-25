#ifndef CS2_CHEAT_CCSPLAYERCONTROLLER_HPP
#define CS2_CHEAT_CCSPLAYERCONTROLLER_HPP

#include "cbaseplayercontroller.hpp"

class CCSPlayerController
	: public CBasePlayerController
{
public:
	CCSPlayerController( ) = delete;
	CCSPlayerController( CCSPlayerController&& ) = delete;
	CCSPlayerController( const CCSPlayerController& ) = delete;

	SCHEMA_VARIABLE( "CCSPlayerController->m_hPlayerPawn", GetPlayerPawn, CEntityHandle );
	SCHEMA_VARIABLE( "CCSPlayerController->m_sSanitizedPlayerName", GetSanitizedPlayerName, CUtlString );
};

#endif // CS2_CHEAT_CCSPLAYERCONTROLLER_HPP
