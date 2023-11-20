#include <algorithm>

#include "../source2_engine/entities/c_baseentity.hpp"
#include "../source2_engine/entities/c_csplayerpawn.hpp"
#include "../source2_engine/entities/ccsplayercontroller.hpp"

#include "entities.hpp"

#include "../../context/cs2.hpp"

namespace Core
{
	CEntityList::CEntityList( )
	{
		m_vecEntities.reserve( 0x7FFF );
		m_vecPlayerPawns.reserve( 64 );
		m_vecPlayerControllers.reserve( 64 );

		CTX::pCS2->pEntitySystem->GetEntityListeners( ).AddToHead( &m_entityListener );

		for ( std::int32_t iIndex = 0; iIndex < CTX::pCS2->pEntitySystem->GetHighestEntityIndex( ); iIndex++ )
		{
			CEntityInstance* pEntityInstance = CTX::pCS2->pEntitySystem->GetEntityInstance( iIndex );
			if ( !pEntityInstance )
				continue;

			AddEntity( pEntityInstance );
		}

		Sort( );
	};

	CEntityList::~CEntityList( ) noexcept
	{
		CTX::pCS2->pEntitySystem->GetEntityListeners( ).FindAndRemove( &m_entityListener );
	}

	[[nodiscard]] std::span< CEntityInstance* const > CEntityList::GetEntities( ) const noexcept
	{
		return m_vecEntities;
	}

	std::span< C_CSPlayerPawn* const > CEntityList::GetPlayerPawns( ) const noexcept
	{
		return m_vecPlayerPawns;
	}

	std::span< CCSPlayerController* const > CEntityList::GetPlayerControllers( ) const noexcept
	{
		return m_vecPlayerControllers;
	}

	void CEntityList::AddEntity( CEntityInstance* pEntityInstance ) noexcept
	{
		if ( !pEntityInstance )
			return;

		if ( std::ranges::find( m_vecEntities, pEntityInstance ) != m_vecEntities.end( ) )
			return;

		m_vecEntities.emplace_back( pEntityInstance );

		auto* pBaseEntity = static_cast< C_BaseEntity* >( pEntityInstance );

		if ( pBaseEntity->IsBasePlayerPawn( ) )
			m_vecPlayerPawns.emplace_back( static_cast< C_CSPlayerPawn* >( pBaseEntity ) );
		else if ( pBaseEntity->IsBasePlayerController( ) )
			m_vecPlayerControllers.emplace_back( static_cast< CCSPlayerController* >( pBaseEntity ) );
	}

	void CEntityList::RemoveEntity( CEntityInstance* pEntityInstance ) noexcept
	{
		if ( !pEntityInstance )
			return;

		const auto itEntity = std::ranges::find( m_vecEntities, pEntityInstance );
		if ( itEntity == m_vecEntities.end( ) )
			return;

		m_vecEntities.erase( itEntity );

		auto* pBaseEntity = static_cast< C_BaseEntity* >( pEntityInstance );

		if ( pBaseEntity->IsBasePlayerPawn( ) )
		{
			if ( const auto itPlayerPawn = std::ranges::find( m_vecPlayerPawns, pEntityInstance ); itPlayerPawn != m_vecPlayerPawns.end( ) )
				m_vecPlayerPawns.erase( itPlayerPawn );
		}
		else if ( pBaseEntity->IsBasePlayerController( ) )
		{
			if ( const auto itPlayerController = std::ranges::find( m_vecPlayerControllers, pEntityInstance ); itPlayerController != m_vecPlayerControllers.end( ) )
				m_vecPlayerControllers.erase( itPlayerController );
		}
	}

	void CEntityList::Sort( ) noexcept
	{
		auto fnEntitySort = []( CEntityInstance* pEntityA, CEntityInstance* pEntityB )
		{
			return pEntityA->GetEntityIdentity( )->hEntity.GetEntryIndex( ) < pEntityB->GetEntityIdentity( )->hEntity.GetEntryIndex( );
		};

		std::ranges::sort( m_vecEntities, fnEntitySort );
		std::ranges::sort( m_vecPlayerPawns, fnEntitySort );
		std::ranges::sort( m_vecPlayerControllers, fnEntitySort );
	}

	void CEntityList::CEntityListener::OnEntityCreated( CEntityInstance* pEntity ) noexcept
	{
		pEntityList->AddEntity( pEntity );
		pEntityList->Sort( );
	}

	void CEntityList::CEntityListener::OnEntityDeleted( CEntityInstance* pEntity ) noexcept
	{
		pEntityList->RemoveEntity( pEntity );
		pEntityList->Sort( );
	}
} // namespace Core