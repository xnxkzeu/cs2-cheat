#ifndef CS2_CHEAT_C_BASEPLAYERPAWN_HPP
#define CS2_CHEAT_C_BASEPLAYERPAWN_HPP

#include "c_basecombatcharacter.hpp"

enum EObserverMode
{
	OBS_MODE_NONE = 0,
	OBS_MODE_IN_EYE = 2,
	OBS_MODE_CHASE = 3,
	OBS_MODE_ROAMING = 4,
};

class CPlayerPawnComponent
{
public:
};

class CPlayer_ObserverServices
	: public CPlayerPawnComponent
{
public:
	SCHEMA_VARIABLE( "CPlayer_ObserverServices->m_iObserverMode", GetObserverMode, std::uint8_t );
	SCHEMA_VARIABLE( "CPlayer_ObserverServices->m_hObserverTarget", GetObserverTarget, CEntityHandle );
};

class C_BasePlayerPawn
	: public C_BaseCombatCharacter
{
public:
	C_BasePlayerPawn( ) = delete;
	C_BasePlayerPawn( C_BasePlayerPawn&& ) = delete;
	C_BasePlayerPawn( const C_BasePlayerPawn& ) = delete;

	SCHEMA_VARIABLE( "C_BasePlayerPawn->m_pObserverServices", GetObserverServices, CPlayer_ObserverServices* );

	SCHEMA_VARIABLE( "C_BasePlayerPawn->m_hController", GetController, CEntityHandle );
};

#endif // CS2_CHEAT_C_BASEPLAYERPAWN_HPP
