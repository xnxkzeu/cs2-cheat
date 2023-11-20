#ifndef CS2_CHEAT_ENTITIES_HPP
#define CS2_CHEAT_ENTITIES_HPP

#include <memory>
#include <vector>
#include <span>

#include "../source2_engine/cgameentitysystem.hpp"

class CEntityInstance;
class C_CSPlayerPawn;
class CCSPlayerController;

namespace Core
{
	class CEntityList
	{
	public:
		class CEntityListener
			: public IEntityListener
		{
		public:
			void OnEntityCreated( CEntityInstance* pEntity ) noexcept final;
			void OnEntitySpawned( [[maybe_unused]] CEntityInstance* pEntity ) noexcept final { };
			void OnEntityDeleted( CEntityInstance* pEntity ) noexcept final;
			void OnEntityParentChanged( [[maybe_unused]] CEntityInstance* pEntity, [[maybe_unused]] CEntityInstance* pParentEntity ) noexcept final { };
		};

		CEntityList( );
		~CEntityList( ) noexcept;

		[[nodiscard]] std::span< CEntityInstance* const > GetEntities( ) const noexcept;
		[[nodiscard]] std::span< C_CSPlayerPawn* const > GetPlayerPawns( ) const noexcept;
		[[nodiscard]] std::span< CCSPlayerController* const > GetPlayerControllers( ) const noexcept;

	private:
		void AddEntity( CEntityInstance* pEntityInstance ) noexcept;
		void RemoveEntity( CEntityInstance* pEntityInstance ) noexcept;
		void Sort( ) noexcept;

		CEntityListener m_entityListener = { };

		std::vector< CEntityInstance* > m_vecEntities = { };
		std::vector< C_CSPlayerPawn* > m_vecPlayerPawns = { };
		std::vector< CCSPlayerController* > m_vecPlayerControllers = { };
	};

	inline std::unique_ptr< CEntityList > pEntityList = nullptr;
} // namespace Core

#endif // CS2_CHEAT_ENTITIES_HPP
