#ifndef CS2_CHEAT_CBASEANIMGRAPH_HPP
#define CS2_CHEAT_CBASEANIMGRAPH_HPP

#include "c_basemodelentity.hpp"

class CBaseAnimGraph
	: public C_BaseModelEntity
{
public:
	CBaseAnimGraph( ) = delete;
	CBaseAnimGraph( CBaseAnimGraph&& ) = delete;
	CBaseAnimGraph( const CBaseAnimGraph& ) = delete;
};

#endif // CS2_CHEAT_CBASEANIMGRAPH_HPP
