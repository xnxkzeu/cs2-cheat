#ifndef CS2_CHEAT_CENTITYINSTANCE_HPP
#define CS2_CHEAT_CENTITYINSTANCE_HPP

#include "../../helpers/memory.hpp"
#include "../../helpers/schema.hpp"

struct ScriptClassDesc_t;

class CEntityPrecacheContext;
class CEntityKeyValues;
class CEntityIdentity;

class CEntityInstance
{
public:
	CEntityInstance( ) = delete;
	CEntityInstance( CEntityInstance&& ) = delete;
	CEntityInstance( const CEntityInstance& ) = delete;

	[[nodiscard]] virtual ScriptClassDesc_t* GetScriptDesc( ) = 0;
	virtual ~CEntityInstance( ) = 0;
	virtual void Connect( ) = 0;
	virtual void Precache( CEntityPrecacheContext* pEntityPrecacheContext ) = 0;
	virtual void AddedToEntityDatabase( ) = 0;
	virtual void Spawn( CEntityKeyValues* pEntityKeyValues ) = 0;
	virtual void PostDataUpdate( int iUpdateType ) = 0;
	virtual void Activate( ) = 0;
	virtual void UpdateOnRemove( ) = 0;

	SCHEMA_VARIABLE( "CEntityInstance->m_pEntity", GetEntityIdentity, CEntityIdentity* );
};

#endif // CS2_CHEAT_CENTITYINSTANCE_HPP
