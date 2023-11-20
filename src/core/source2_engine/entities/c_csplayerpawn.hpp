#ifndef CS2_CHEAT_C_CSPLAYERPAWN_HPP
#define CS2_CHEAT_C_CSPLAYERPAWN_HPP

#include "c_baseplayerpawn.hpp"

class C_CSPlayerPawn
	: public C_BasePlayerPawn
{
public:
	C_CSPlayerPawn( ) = delete;
	C_CSPlayerPawn( C_CSPlayerPawn&& ) = delete;
	C_CSPlayerPawn( const C_CSPlayerPawn& ) = delete;

};

#endif // CS2_CHEAT_C_CSPLAYERPAWN_HPP
