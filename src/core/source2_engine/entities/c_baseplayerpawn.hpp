#ifndef CS2_CHEAT_C_BASEPLAYERPAWN_HPP
#define CS2_CHEAT_C_BASEPLAYERPAWN_HPP

#include "c_basecombatcharacter.hpp"

class C_BasePlayerPawn
	: public C_BaseCombatCharacter
{
public:
	C_BasePlayerPawn( ) = delete;
	C_BasePlayerPawn( C_BasePlayerPawn&& ) = delete;
	C_BasePlayerPawn( const C_BasePlayerPawn& ) = delete;

	SCHEMA_VARIABLE( "C_BasePlayerPawn->m_hController", GetController, CEntityHandle );
};

#endif // CS2_CHEAT_C_BASEPLAYERPAWN_HPP
