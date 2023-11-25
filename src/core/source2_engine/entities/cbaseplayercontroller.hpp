#ifndef CS2_CHEAT_CBASEPLAYERCONTROLLER_HPP
#define CS2_CHEAT_CBASEPLAYERCONTROLLER_HPP

#include "c_baseentity.hpp"

#include "../datatypes/cutlstring.hpp"

class CBasePlayerController
	: public C_BaseEntity
{
public:
	CBasePlayerController( ) = delete;
	CBasePlayerController( CBasePlayerController&& ) = delete;
	CBasePlayerController( const CBasePlayerController& ) = delete;

	SCHEMA_VARIABLE( "CBasePlayerController->m_hPawn", GetPawn, CEntityHandle );
	SCHEMA_VARIABLE( "CBasePlayerController->m_iszPlayerName", GetPlayerName, char[ 128 ] );
	SCHEMA_VARIABLE( "CBasePlayerController->m_bIsLocalPlayerController", IsLocalPlayerController, bool );
};

#endif // CS2_CHEAT_CBASEPLAYERCONTROLLER_HPP
