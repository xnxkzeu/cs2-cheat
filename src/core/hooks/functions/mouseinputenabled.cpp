#include "../hooks.hpp"

#include "../../ui/menu.hpp"

bool Core::CHooks::MouseInputEnabled( CCSGOInput* pCSGOInput ) noexcept
{
	return !UI::pMenu->IsVisible( ) && pfnMouseInputEnabled( pCSGOInput );
}