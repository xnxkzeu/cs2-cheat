#ifndef CS2_CHEAT_ISOURCE2ENGINE_HPP
#define CS2_CHEAT_ISOURCE2ENGINE_HPP

#include "iappsystem.hpp"

#include "../helpers/memory.hpp"

struct ISource2Engine
	: public IAppSystem
{
	ISource2Engine( ) = delete;
	ISource2Engine( ISource2Engine&& ) = delete;
	ISource2Engine( const ISource2Engine& ) = delete;

	[[nodiscard]] virtual bool IsPaused( ) const noexcept = 0;
	[[nodiscard]] virtual float GetTimescale( ) const noexcept = 0;

	VFUNC( 27, GetLocalPlayer( int& iOutPlayerID, int iSplitScreenSlot = 0 ), ( this, std::ref( iOutPlayerID ), iSplitScreenSlot ), void );
	[[nodiscard]] VFUNC( 31, GetMaxClients( ), ( this ), int );
	[[nodiscard]] VFUNC( 32, IsInGame( ), ( this ), bool );
	[[nodiscard]] VFUNC( 33, IsConnected( ), ( this ), bool );
	VFUNC( 50, GetScreenSize( int& iWidth, int& iHeight ), ( this, std::ref( iWidth ), std::ref( iHeight ) ), void );
};

#endif // CS2_CHEAT_ISOURCE2ENGINE_HPP
