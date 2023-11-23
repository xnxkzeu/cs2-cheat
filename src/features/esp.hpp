#ifndef CS2_CHEAT_ESP_HPP
#define CS2_CHEAT_ESP_HPP

#include <memory>

#include "../core/types/vector_t.hpp"

class CCSPlayerController;
class C_CSPlayerPawn;
class C_BaseEntity;
class CGameSceneNode;
class CSkeletonInstance;

namespace Features
{
	class CESP
	{
	public:
		CESP( ) = default;
		~CESP( ) = default;

		void Render( );

	private:
		void PlayerBox( C_CSPlayerPawn* pPlayerPawn, const Math::Vector_t< float, 2 >& vecMins, const Math::Vector_t< float, 2 >& vecMaxs ) noexcept;
		void PlayerName( C_CSPlayerPawn* pPlayerPawn, CCSPlayerController* pPlayerController, const Math::Vector_t< float, 2 >& vecMins, const Math::Vector_t< float, 2 >& vecMaxs ) noexcept;
		void PlayerSkeleton( C_CSPlayerPawn* pPlayerPawn, CSkeletonInstance* pSkeletonInstance ) noexcept;

		static bool CalculateBoundingBox( C_BaseEntity* pBaseEntity, CGameSceneNode* pGameSceneNode, Math::Vector_t< float, 2 >& vecScreenMins, Math::Vector_t< float, 2 >& vecScreenMaxs ) noexcept;

		C_CSPlayerPawn* m_pLocalPlayerPawn = nullptr;
	};

	inline std::unique_ptr< CESP > pESP = nullptr;
} // namespace Features

#endif // CS2_CHEAT_ESP_HPP
