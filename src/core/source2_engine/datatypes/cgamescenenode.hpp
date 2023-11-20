#ifndef CS2_CHEAT_CGAMESCENENODE_HPP
#define CS2_CHEAT_CGAMESCENENODE_HPP

#include "../../helpers/memory.hpp"
#include "../../helpers/schema.hpp"

#include "../../types/vector_t.hpp"
#include "../../types/qangle_t.hpp"

class CEntityInstance;

class CGameSceneNode
{
public:
	CGameSceneNode( ) = delete;
	CGameSceneNode( CGameSceneNode&& ) = delete;
	CGameSceneNode( const CGameSceneNode& ) = delete;

	SCHEMA_VARIABLE( "CGameSceneNode->m_pOwner", GetOwner, CEntityInstance* );

	SCHEMA_VARIABLE( "CGameSceneNode->m_vecAbsOrigin", GetAbsOrigin, Math::Vector_t< float, 3 > );
	SCHEMA_VARIABLE( "CGameSceneNode->m_vecAbsRotation", GetAbsRotation, Math::QAngle_t );
};

#endif // CS2_CHEAT_CGAMESCENENODE_HPP
