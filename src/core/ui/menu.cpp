#include <span>
#include <string_view>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <xorstr.hpp>

#include "menu.hpp"

#include "../helpers/input.hpp"

#include "../source2_engine/iinputsystem.hpp"

#include "../../context/cs2.hpp"
#include "../../context/memory.hpp"
#include "../../context/config.hpp"

namespace Core::UI
{
	CMenu::CMenu( )
	{
		m_pImGuiIO = &ImGui::GetIO( );
		m_pImGuiIO->ConfigFlags = IsVisible( ) ? ( m_pImGuiIO->ConfigFlags & ~ImGuiConfigFlags_NoMouseCursorChange ) : ( m_pImGuiIO->ConfigFlags | ImGuiConfigFlags_NoMouseCursorChange );

		ImGuiStyle* pImGuiStyle = &ImGui::GetStyle( );
		pImGuiStyle->ItemSpacing = ImVec2( 8.f, 8.f );
		pImGuiStyle->WindowPadding = ImVec2( 8.f, 8.f );

		Core::pInput->RegisterCallback( VK_INSERT, std::bind( &CMenu::Toggle, this ) );

		ImGui::SetColorEditOptions( ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_AlphaBar );
	}

	CMenu::~CMenu( ) noexcept
	{
		SetVisible( false );
	}

	void RenderTabs( std::span< const std::string_view > spanTabNames, std::size_t& uActiveTab ) noexcept
	{
		ImVec2 vecButtonSize = ImVec2( ( ImGui::GetWindowContentRegionMax( ).x - ImGui::GetWindowContentRegionMin( ).x ) / spanTabNames.size( ) - ( ImGui::GetStyle( ).ItemSpacing.x / spanTabNames.size( ) * ( spanTabNames.size( ) - 1 ) ), 0.f );

		for ( std::size_t uTabIndex = 0; uTabIndex < spanTabNames.size( ); uTabIndex++ )
		{
			if ( ImGui::Button( spanTabNames[ uTabIndex ].data( ), vecButtonSize ) )
				uActiveTab = uTabIndex;

			if ( uTabIndex < spanTabNames.size( ) - 1 )
				ImGui::SameLine( );
		}
	}

	void CMenu::Render( ) noexcept
	{
		if ( !m_bIsVisible )
			return;

		CTX::pMemory->SDL.SetRelativeMouseMode( false );
		CTX::pMemory->SDL.SetWindowGrab( CTX::pCS2->pInputSystem->GetSDLWindow( ), false );

		ImGui::SetNextWindowSize( ImVec2( 736.f, 400.f ) );
		if ( ImGui::Begin( xorstr_( PROJECT_NAME ), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar ) )
		{
			{
				static constexpr std::string_view arrTabs[] = {
					"ESP",
					"Misc",
				};

				RenderTabs( arrTabs, m_uActiveTab );

				ImGui::SeparatorText( arrTabs[ m_uActiveTab ].data( ) );
			}

			switch ( m_uActiveTab )
			{
				case 0:
					ImGui::Checkbox( "ESP Master switch", &CTX::pConfig->ESP.bEnabled );

					ImGui::Checkbox( "Player box", &CTX::pConfig->ESP.bPlayerBox );
					ImGui::SameLine( 250.f );

					{
						ImVec4 vecColor = ImGui::ColorConvertU32ToFloat4( static_cast< ImU32 >( CTX::pConfig->ESP.colPlayerBoxEnemy ) );
						if ( ImGui::ColorEdit4( "Enemy##Box", &vecColor.x, ImGuiColorEditFlags_NoInputs ) )
							CTX::pConfig->ESP.colPlayerBoxEnemy = ImGui::ColorConvertFloat4ToU32( vecColor );
					}
					ImGui::SameLine( 450.f );
					{
						ImVec4 vecColor = ImGui::ColorConvertU32ToFloat4( static_cast< ImU32 >( CTX::pConfig->ESP.colPlayerBoxTeammate ) );
						if ( ImGui::ColorEdit4( "Teammate##Box", &vecColor.x, ImGuiColorEditFlags_NoInputs ) )
							CTX::pConfig->ESP.colPlayerBoxTeammate = ImGui::ColorConvertFloat4ToU32( vecColor );
					}
					ImGui::Checkbox( "Player name", &CTX::pConfig->ESP.bPlayerName );
					ImGui::SameLine( 250.f );

					{
						ImVec4 vecColor = ImGui::ColorConvertU32ToFloat4( static_cast< ImU32 >( CTX::pConfig->ESP.colPlayerNameEnemy ) );
						if ( ImGui::ColorEdit4( "Enemy##Name", &vecColor.x, ImGuiColorEditFlags_NoInputs ) )
							CTX::pConfig->ESP.colPlayerNameEnemy = ImGui::ColorConvertFloat4ToU32( vecColor );
					}
					ImGui::SameLine( 450.f );
					{
						ImVec4 vecColor = ImGui::ColorConvertU32ToFloat4( static_cast< ImU32 >( CTX::pConfig->ESP.colPlayerNameTeammate ) );
						if ( ImGui::ColorEdit4( "Teammate##Name", &vecColor.x, ImGuiColorEditFlags_NoInputs ) )
							CTX::pConfig->ESP.colPlayerNameTeammate = ImGui::ColorConvertFloat4ToU32( vecColor );
					}
					break;
				case 1:
					break;
				default:
					break;
			}
		}
		ImGui::End( );
	}

	void CMenu::Toggle( ) noexcept
	{
		SetVisible( !m_bIsVisible );
	}

	void CMenu::SetVisible( bool bState ) noexcept
	{
		m_bIsVisible = bState;

		m_pImGuiIO->ConfigFlags = bState ? ( m_pImGuiIO->ConfigFlags & ~ImGuiConfigFlags_NoMouseCursorChange ) : ( m_pImGuiIO->ConfigFlags | ImGuiConfigFlags_NoMouseCursorChange );

		if ( CTX::pCS2->pInputSystem->IsRelativeMouseMode( ) )
		{
			ImVec2 vecScreenCenter = ImVec2( m_pImGuiIO->DisplaySize.x * 0.5f, m_pImGuiIO->DisplaySize.y * 0.5f );

			CTX::pMemory->SDL.SetRelativeMouseMode( !m_bIsVisible );
			CTX::pMemory->SDL.SetWindowGrab( CTX::pCS2->pInputSystem->GetSDLWindow( ), !m_bIsVisible );
			CTX::pMemory->SDL.WarpMouseInWindow( nullptr, vecScreenCenter.x, vecScreenCenter.y );
		}
	}

	bool CMenu::IsVisible( ) const noexcept
	{
		return m_bIsVisible;
	}
} // namespace Core::UI