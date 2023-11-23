#ifndef CS2_CHEAT_CGAMESCENENODE_HPP
#define CS2_CHEAT_CGAMESCENENODE_HPP

#include "ctransform.hpp"

#include "../../helpers/memory.hpp"
#include "../../helpers/schema.hpp"

#include "../../types/vector_t.hpp"
#include "../../types/qangle_t.hpp"

class CEntityInstance;
class CEntityKeyValues;
class CSkeletonInstance;

struct ScriptClassDesc_t;

class CGameSceneNode
{
public:
	CGameSceneNode( ) = delete;
	CGameSceneNode( CGameSceneNode&& ) = delete;
	CGameSceneNode( const CGameSceneNode& ) = delete;

	[[nodiscard]] virtual ScriptClassDesc_t* GetScriptDesc( ) = 0;
	virtual ~CGameSceneNode( ) = 0;
	virtual void AddedToEntityDatabase( CEntityInstance* pEntityInstance ) = 0;
	virtual void Connect( const char* szUnknown, CEntityInstance* pEntityInstance ) = 0;
	virtual void Spawn( CEntityInstance* pEntityInstance, CEntityKeyValues* pEntityKeyValues ) = 0;
	virtual void Activate( CEntityInstance* pEntityInstance, int iUnknown ) = 0;
	virtual void UpdateOnRemove( CEntityInstance* pEntityInstance ) = 0;
	virtual void OnSetDormant( CEntityInstance* pEntityInstance, bool bUnknown, bool bNewDormant ) = 0;
	[[nodiscard]] virtual CSkeletonInstance* GetSkeletonInstance( ) = 0;
	[[nodiscard]] virtual const CSkeletonInstance* GetSkeletonInstance( ) const = 0;

	SCHEMA_VARIABLE( "CGameSceneNode->m_nodeToWorld", GetTransform, CTransform );
	SCHEMA_VARIABLE( "CGameSceneNode->m_pOwner", GetOwner, CEntityInstance* );

	SCHEMA_VARIABLE( "CGameSceneNode->m_vecAbsOrigin", GetAbsOrigin, Math::Vector_t< float, 3 > );
	SCHEMA_VARIABLE( "CGameSceneNode->m_vecAbsRotation", GetAbsRotation, Math::QAngle_t );
};

#endif // CS2_CHEAT_CGAMESCENENODE_HPP
