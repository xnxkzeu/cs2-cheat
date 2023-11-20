#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "../hooks.hpp"

#include "../../helpers/render.hpp"

#include "../../ui/menu.hpp"

std::int32_t Core::CHooks::Present( IDXGISwapChain* pSwapChain, std::uint32_t uSyncInterval, std::uint32_t uFlags ) noexcept
{
	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	Core::UI::pMenu->Render( );

	Core::pRender->PrePresent( pSwapChain );

	ImGui::Render( );

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

	const std::int32_t iResult = pfnPresent( pSwapChain, uSyncInterval, uFlags );

	Core::pRender->PostPresent( );

	return iResult;
}