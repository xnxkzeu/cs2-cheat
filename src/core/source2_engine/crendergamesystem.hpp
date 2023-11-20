#ifndef CS2_CHEAT_CRENDERGAMESYSTEM_HPP
#define CS2_CHEAT_CRENDERGAMESYSTEM_HPP

#include "../helpers/memory.hpp"

#include "../types/matrix_t.hpp"

class CRenderGameSystem
{
public:
	CRenderGameSystem( ) = delete;
	CRenderGameSystem( CRenderGameSystem&& ) = delete;
	CRenderGameSystem( const CRenderGameSystem& ) = delete;

	VFUNC( 53, WorldToProjectionMatrix( int iSplitScreenSlot ), ( this, iSplitScreenSlot ), Math::ViewMatrix_t& );
};

#endif // CS2_CHEAT_CRENDERGAMESYSTEM_HPP
