#ifndef CS2_CHEAT_C_BASEENTITY_HPP
#define CS2_CHEAT_C_BASEENTITY_HPP

#include "centityinstance.hpp"

#include "../datatypes/centityhandle.hpp"

#include "../../types/vector_t.hpp"

class CGameSceneNode;
class CRenderComponent;
class CCollisionProperty;

enum EEntityFlags : std::uint32_t
{
	FL_ONGROUND = 0x1,
	FL_DUCKING = 0x2,
	FL_ANIMDUCKING = 0x4,
	FL_WATERJUMP = 0x8,
};

enum EMoveType : std::uint8_t
{
	MOVETYPE_NONE,
	MOVETYPE_OBSOLETE,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_OBSERVER,
	MOVETYPE_LADDER,
	MOVETYPE_CUSTOM,
};

enum ELifeState : std::uint8_t
{
	LIFE_ALIVE = 0,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_RESPAWNING,
};

class C_BaseEntity
	: public CEntityInstance
{
public:
	C_BaseEntity( ) = delete;
	C_BaseEntity( C_BaseEntity&& ) = delete;
	C_BaseEntity( const C_BaseEntity& ) = delete;

	VFUNC( 143, IsBasePlayerPawn( ), ( this ), bool );
	VFUNC( 144, IsBasePlayerController( ), ( this ), bool );
	VFUNC( 150, IsBasePlayerWeapon( ), ( this ), bool );
	VFUNC( 242, IsViewModel( ), ( this ), bool );

	SCHEMA_VARIABLE( "C_BaseEntity->m_pGameSceneNode", GetGameSceneNode, CGameSceneNode* );
	SCHEMA_VARIABLE( "C_BaseEntity->m_pRenderComponent", GetRenderComponent, CRenderComponent* );
	SCHEMA_VARIABLE( "C_BaseEntity->m_pCollision", GetCollisionProperty, CCollisionProperty* );
	SCHEMA_VARIABLE( "C_BaseEntity->m_iHealth", GetHealth, int );
	SCHEMA_VARIABLE( "C_BaseEntity->m_iMaxHealth", GetMaxHealth, int );
	SCHEMA_VARIABLE( "C_BaseEntity->m_lifeState", GetLifeState, ELifeState );
	SCHEMA_VARIABLE( "C_BaseEntity->m_iTeamNum", GetTeam, std::uint8_t );
	SCHEMA_VARIABLE( "C_BaseEntity->m_fFlags", GetFlags, std::uint32_t );
	SCHEMA_VARIABLE( "C_BaseEntity->m_flWaterLevel", GetWaterLevel, float );
	SCHEMA_VARIABLE( "C_BaseEntity->m_MoveType", GetMoveType, EMoveType );
	SCHEMA_VARIABLE( "C_BaseEntity->m_hOwnerEntity", GetOwnerEntity, CEntityHandle );
	SCHEMA_VARIABLE( "C_BaseEntity->m_vecVelocity", GetVelocity, Math::Vector_t< float, 3 > );

	[[nodiscard]] bool IsAlive( ) const noexcept
	{
		return GetHealth( ) > 0 && GetLifeState( ) == LIFE_ALIVE;
	}
};

#endif // CS2_CHEAT_C_BASEENTITY_HPP
