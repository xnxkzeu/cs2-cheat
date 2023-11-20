#ifndef CS2_CHEAT_ISOURCE2CLIENT_HPP
#define CS2_CHEAT_ISOURCE2CLIENT_HPP

#include "iappsystem.hpp"

#include "../helpers/memory.hpp"

enum EClientFrameStage
{
	FRAME_RENDER_START = 9,
};

struct ISource2Client
	: public IAppSystem
{
	ISource2Client( ) = delete;
	ISource2Client( ISource2Client&& ) = delete;
	ISource2Client( const ISource2Client& ) = delete;
};

#endif // CS2_CHEAT_ISOURCE2CLIENT_HPP
