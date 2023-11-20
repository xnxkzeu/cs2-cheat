#ifndef CS2_CHEAT_C_BASECOMBATCHARACTER_HPP
#define CS2_CHEAT_C_BASECOMBATCHARACTER_HPP

#include "c_baseflex.hpp"

class C_BaseCombatCharacter
	: public C_BaseFlex
{
public:
	C_BaseCombatCharacter( ) = delete;
	C_BaseCombatCharacter( C_BaseCombatCharacter&& ) = delete;
	C_BaseCombatCharacter( const C_BaseCombatCharacter& ) = delete;
};

#endif // CS2_CHEAT_C_BASECOMBATCHARACTER_HPP
