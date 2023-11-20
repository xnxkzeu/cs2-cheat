#ifndef CS2_CHEAT_C_BASEFLEX_HPP
#define CS2_CHEAT_C_BASEFLEX_HPP

#include "cbaseanimgraph.hpp"

class C_BaseFlex
	: public CBaseAnimGraph
{
public:
	C_BaseFlex( ) = delete;
	C_BaseFlex( C_BaseFlex&& ) = delete;
	C_BaseFlex( const C_BaseFlex& ) = delete;
};

#endif // CS2_CHEAT_C_BASEFLEX_HPP
