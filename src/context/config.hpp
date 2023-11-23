#ifndef CS2_CHEAT_CONFIG_HPP
#define CS2_CHEAT_CONFIG_HPP

#include <memory>

#include "../core/types/color_t.hpp"

namespace CTX
{
	struct Config_t
	{
		struct
		{
			bool bEnabled = false;
			bool bPlayerBox = false;
			bool bPlayerName = false;
			bool bPlayerSkeleton = false;

			Color_t colPlayerBoxTeammate = Color_t( 0, 255, 0, 255 );
			Color_t colPlayerBoxEnemy = Color_t( 255, 0, 0, 255 );
			Color_t colPlayerNameTeammate = Color_t( 255, 255, 255, 255 );
			Color_t colPlayerNameEnemy = Color_t( 255, 255, 255, 255 );
			Color_t colPlayerSkeletonTeammate = Color_t( 255, 255, 255, 255 );
			Color_t colPlayerSkeletonEnemy = Color_t( 255, 255, 255, 255 );
		} ESP;
	};

	inline std::unique_ptr< Config_t > pConfig = nullptr;
} // namespace CTX

#endif // CS2_CHEAT_CONFIG_HPP
