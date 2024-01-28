#include "esp.hpp"

#include "../core/helpers/entities.hpp"
#include "../core/helpers/render.hpp"

#include "../core/source2_engine/datatypes/cgamescenenode.hpp"
#include "../core/source2_engine/datatypes/cskeletoninstance.hpp"
#include "../core/source2_engine/datatypes/ccollisionproperty.hpp"
#include "../core/source2_engine/datatypes/cmodel.hpp"

#include "../core/source2_engine/entities/c_csplayerpawn.hpp"
#include "../core/source2_engine/entities/ccsplayercontroller.hpp"

#include "../core/source2_engine/isource2engine.hpp"
#include "../core/source2_engine/ilocalize.hpp"
#include "../core/source2_engine/cgameentitysystem.hpp"

#include "../context/config.hpp"
#include "../context/cs2.hpp"

namespace Features
{
	CESP::CESP( )
	{
		m_szBotLabel = CTX::pCS2->pLocalize->Find( xorstr_( "SFUI_scoreboard_lbl_bot" ) );
	}

	void CESP::Render( )
	{
		if ( !CTX::pConfig->ESP.bEnabled )
			return;

		if ( !CTX::pCS2->pSource2Engine->IsInGame( ) )
			return;

		int iLocalPlayerID = -1;
		CTX::pCS2->pSource2Engine->GetLocalPlayer( iLocalPlayerID );
		if ( iLocalPlayerID == -1 )
			return;

		auto* pLocalPlayerController = static_cast< CCSPlayerController* >( CTX::pCS2->pEntitySystem->GetEntityInstance( iLocalPlayerID + 1 ) );
		if ( !pLocalPlayerController )
			return;

		m_pLocalPlayerPawn = static_cast< C_CSPlayerPawn* >( CTX::pCS2->pEntitySystem->GetEntityInstanceFromHandle( pLocalPlayerController->GetPawn( ) ) );
		if ( !m_pLocalPlayerPawn )
			return;

		CEntityInstance* pObserverTarget = nullptr;
		if ( CPlayer_ObserverServices* pObserverServices = m_pLocalPlayerPawn->GetObserverServices( ); pObserverServices && pObserverServices->GetObserverMode( ) == OBS_MODE_IN_EYE ) // @note / xnxkzeu: only valid when local player is dead
			pObserverTarget = CTX::pCS2->pEntitySystem->GetEntityInstanceFromHandle( pObserverServices->GetObserverTarget( ) );

		for ( C_CSPlayerPawn* pPlayerPawn : Core::pEntityList->GetPlayerPawns( ) )
		{
			if ( pPlayerPawn == m_pLocalPlayerPawn || pPlayerPawn == pObserverTarget || !pPlayerPawn->IsAlive( ) )
				continue;

			const auto pPlayerController = static_cast< CCSPlayerController* >( CTX::pCS2->pEntitySystem->GetEntityInstanceFromHandle( pPlayerPawn->GetController( ) ) );
			if ( !pPlayerController )
				continue;

			CGameSceneNode* pGameSceneNode = pPlayerPawn->GetGameSceneNode( );
			if ( !pGameSceneNode )
				continue;

			CSkeletonInstance* pSkeletonInstance = pGameSceneNode->GetSkeletonInstance( );
			if ( !pSkeletonInstance )
				continue;

			Math::Vector_t< float, 2 > vecMins, vecMaxs;
			if ( !CalculateBoundingBox( pPlayerPawn, pSkeletonInstance, vecMins, vecMaxs ) )
				continue;

			PlayerSkeleton( pPlayerPawn, pSkeletonInstance );
			PlayerBox( pPlayerPawn, vecMins, vecMaxs );
			PlayerName( pPlayerPawn, pPlayerController, vecMins, vecMaxs );
		}
	}

	void CESP::PlayerBox( C_CSPlayerPawn* pPlayerPawn, const Math::Vector_t< float, 2 >& vecMins, const Math::Vector_t< float, 2 >& vecMaxs ) noexcept
	{
		if ( !CTX::pConfig->ESP.bPlayerBox )
			return;

		Color_t colRender = CTX::pConfig->ESP.colPlayerBoxEnemy;
		if ( pPlayerPawn->GetTeam( ) == m_pLocalPlayerPawn->GetTeam( ) )
			colRender = CTX::pConfig->ESP.colPlayerBoxTeammate;

		Core::pRender->Rect( vecMins, vecMaxs, colRender, RECT_RENDER_OUTLINE, Color_t( 0, 0, 0, colRender[ Color::Alpha ] ) );
	}

	void CESP::PlayerName( C_CSPlayerPawn* pPlayerPawn, CCSPlayerController* pPlayerController, const Math::Vector_t< float, 2 >& vecMins, const Math::Vector_t< float, 2 >& vecMaxs ) noexcept
	{
		if ( !CTX::pConfig->ESP.bPlayerName )
			return;

		Color_t colRender = CTX::pConfig->ESP.colPlayerNameEnemy;
		if ( pPlayerPawn->GetTeam( ) == m_pLocalPlayerPawn->GetTeam( ) )
			colRender = CTX::pConfig->ESP.colPlayerNameTeammate;

		const Math::Vector_t< float, 2 > vecTextPosition( vecMins[ Axis::X ] + ( vecMaxs[ Axis::X ] - vecMins[ Axis::X ] ) * 0.5f, vecMaxs[ Axis::Y ] + 1.f );

		const char* szPlayerName = pPlayerController->GetSanitizedPlayerName( ).Get( );

		if ( !( pPlayerController->GetFlags( ) & FL_FAKECLIENT ) || !m_szBotLabel )
			Core::pRender->Text( vecTextPosition, szPlayerName, colRender, TEXT_RENDER_CENTERED_X );
		else
			Core::pRender->Text( vecTextPosition, std::format( "{} {}", m_szBotLabel, szPlayerName ), colRender, TEXT_RENDER_CENTERED_X );
	}

	void CESP::PlayerSkeleton( C_CSPlayerPawn* pPlayerPawn, CSkeletonInstance* pSkeletonInstance ) noexcept
	{
		if ( !CTX::pConfig->ESP.bPlayerSkeleton )
			return;

		Color_t colRender = CTX::pConfig->ESP.colPlayerSkeletonEnemy;
		if ( pPlayerPawn->GetTeam( ) == m_pLocalPlayerPawn->GetTeam( ) )
			colRender = CTX::pConfig->ESP.colPlayerSkeletonTeammate;

		const CModelState& modelState = pSkeletonInstance->GetModelState( );

		CStrongHandle< CModel > hModel = modelState.GetModel( );
		if ( !hModel )
			return;

		const CTransform* pTransform = modelState.GetBoneTransformation( );

		const CModelSkeleton& modelSkeleton = hModel->modelSkeleton;
		for ( std::uint16_t uBoneIndex = 0; uBoneIndex < modelSkeleton.vecBoneParent.Count( ); uBoneIndex++ )
		{
			const std::uint16_t uParentBoneIndex = modelSkeleton.vecBoneParent[ uBoneIndex ];
			if ( uParentBoneIndex == static_cast< std::uint16_t >( -1 ) )
				continue;

			if ( !( modelSkeleton.vecBoneFlags[ uBoneIndex ] & FLAG_HITBOX ) )
				continue;

			Math::Vector_t< float, 2 > vecChild, vecParent;
			if ( !Core::pRender->WorldToScreen( pTransform[ uBoneIndex ].vecPosition, vecChild ) ||
				 !Core::pRender->WorldToScreen( pTransform[ uParentBoneIndex ].vecPosition, vecParent ) )
				continue;

			Core::pRender->Line( vecChild, vecParent, colRender );
		}
	}

	bool CESP::CalculateBoundingBox( C_BaseEntity* pBaseEntity, CGameSceneNode* pGameSceneNode, Math::Vector_t< float, 2 >& vecScreenMins, Math::Vector_t< float, 2 >& vecScreenMaxs ) noexcept
	{
		CCollisionProperty* pCollisionProperty = pBaseEntity->GetCollisionProperty( );
		if ( !pCollisionProperty )
			return false;

		const CTransform& tmNodeToWorld = pGameSceneNode->GetTransform( );

		const Math::Vector_t< float, 3 >
			vecWorldMins = tmNodeToWorld.vecPosition + pCollisionProperty->GetMins( ),
			vecWorldMaxs = tmNodeToWorld.vecPosition + pCollisionProperty->GetMaxs( );

		vecScreenMins = { std::numeric_limits< float >::max( ), std::numeric_limits< float >::max( ) };
		vecScreenMaxs = { std::numeric_limits< float >::lowest( ), std::numeric_limits< float >::lowest( ) };

		for ( int iIndex = 0; iIndex < 8; ++iIndex )
		{
			const Math::Vector_t< float, 3 > vecPoint =
				{ iIndex & 1 ? vecWorldMaxs[ Axis::X ] : vecWorldMins[ Axis::X ],
				  iIndex & 2 ? vecWorldMaxs[ Axis::Y ] : vecWorldMins[ Axis::Y ],
				  iIndex & 4 ? vecWorldMaxs[ Axis::Z ] : vecWorldMins[ Axis::Z ] };

			Math::Vector_t< float, 2 > vecScreenPoint;
			if ( !Core::pRender->WorldToScreen( vecPoint, vecScreenPoint ) )
				return false;

			vecScreenMins[ Axis::X ] = std::min( vecScreenMins[ Axis::X ], vecScreenPoint[ Axis::X ] );
			vecScreenMins[ Axis::Y ] = std::min( vecScreenMins[ Axis::Y ], vecScreenPoint[ Axis::Y ] );
			vecScreenMaxs[ Axis::X ] = std::max( vecScreenMaxs[ Axis::X ], vecScreenPoint[ Axis::X ] );
			vecScreenMaxs[ Axis::Y ] = std::max( vecScreenMaxs[ Axis::Y ], vecScreenPoint[ Axis::Y ] );
		}

		return true;
	}
} // namespace Features