#ifndef CS2_CHEAT_CGAMEENTITYSYSTEM_HPP
#define CS2_CHEAT_CGAMEENTITYSYSTEM_HPP

#include <cstddef>
#include <array>

#include "datatypes/centityhandle.hpp"
#include "datatypes/cutlvector.hpp"

#include "../helpers/memory.hpp"

#define MAX_ENTITIES_IN_LIST ( 512 )
#define MAX_ENTITY_LISTS ( 64 )
#define MAX_TOTAL_ENTITIES ( MAX_ENTITIES_IN_LIST * MAX_ENTITY_LISTS )

class CEntityInstance;
class CEntityClass;

class CEntityIdentity
{
public:
	CEntityInstance* pEntity = nullptr;
	CEntityClass* pEntityClass = nullptr;
	CEntityHandle hEntity = { };
	int iNameStringableIndex = { };
	const char* szName = nullptr;
	const char* szDesignerName = nullptr;
private:
	[[maybe_unused]] std::byte m_pad0[ 0x8 ];
public:
	std::uint32_t uFlags = { };
private:
	[[maybe_unused]] std::byte m_pad1[ 0x4 ];
public:
	int iWorldGroupID;
	std::uint32_t uDataObjectTypes;
	std::uint16_t uPathIndex;
private:
	[[maybe_unused]] std::byte m_pad2[ 0x16 ];
public:
	CEntityIdentity* pPrev;
	CEntityIdentity* pNext;
	CEntityIdentity* pPrevByClass;
	CEntityIdentity* pNextByClass;
};

class CEntityIdentities
{
public:
	CEntityIdentity arrIdentities[ MAX_ENTITIES_IN_LIST ];
};

struct IEntityListener
{
	virtual void OnEntityCreated( CEntityInstance* pEntity ) = 0;
	virtual void OnEntitySpawned( CEntityInstance* pEntity ) = 0;
	virtual void OnEntityDeleted( CEntityInstance* pEntity ) = 0;
	virtual void OnEntityParentChanged( CEntityInstance* pEntity, CEntityInstance* pParentEntity ) = 0;
};

class CGameEntitySystem
{
public:
	CGameEntitySystem( ) = delete;
	CGameEntitySystem( CGameEntitySystem&& ) = delete;
	CGameEntitySystem( const CGameEntitySystem& ) = delete;

	[[nodiscard]] CEntityInstance* GetEntityInstance( int iIndex ) noexcept
	{
		if ( iIndex <= -1 || iIndex >= ( MAX_TOTAL_ENTITIES - 1 ) )
			return nullptr;

		CEntityIdentities* pIdentities = GetIdentityChunks( )[ iIndex / MAX_ENTITIES_IN_LIST ];
		if ( !pIdentities )
			return nullptr;

		const CEntityIdentity& entityIdentity = pIdentities->arrIdentities[ iIndex % MAX_ENTITIES_IN_LIST ];
		return entityIdentity.pEntity;
	}

	[[nodiscard]] CEntityInstance* GetEntityInstanceFromHandle( CEntityHandle hEntity ) noexcept
	{
		return GetEntityInstance( hEntity.GetEntryIndex( ) );
	}

	OFFSET( 0x10, GetIdentityChunks, std::array< CEntityIdentities*, MAX_ENTITY_LISTS > );
	OFFSET( 0x1510, GetHighestEntityIndex, std::int32_t );
	OFFSET( 0x1548, GetEntityListeners, CUtlVector< IEntityListener* > );
};

#endif // CS2_CHEAT_CGAMEENTITYSYSTEM_HPP
