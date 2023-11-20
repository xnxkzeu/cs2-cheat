#ifndef CS2_CHEAT_IINPUTSYSTEM_HPP
#define CS2_CHEAT_IINPUTSYSTEM_HPP

#include "../helpers/memory.hpp"

struct IInputSystem
{
	IInputSystem( ) = delete;
	IInputSystem( IInputSystem&& ) = delete;
	IInputSystem( const IInputSystem& ) = delete;

	OFFSET( 0x4F, IsRelativeMouseMode, bool );
	OFFSET( 0x2678, GetSDLWindow, void* );
};

#endif // CS2_CHEAT_IINPUTSYSTEM_HPP
