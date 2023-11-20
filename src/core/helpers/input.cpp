#include <stdexcept>

#include <imgui.h>

#include <xorstr.hpp>

#include "input.hpp"

#include "../ui/menu.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam );

namespace Core
{
	CInput::CInput( )
	{
		m_hGameWindow = FindWindow( xorstr_( TEXT( "SDL_app" ) ), xorstr_( TEXT( "Counter-Strike 2" ) ) );
		if ( !m_hGameWindow )
			throw std::runtime_error( xorstr_( "failed to find Counter-Strike 2 window." ) );

		m_wndProc = reinterpret_cast< WNDPROC >( SetWindowLongPtr( m_hGameWindow, GWLP_WNDPROC, reinterpret_cast< std::intptr_t >( WndProc ) ) );
		if ( !m_wndProc )
			throw std::runtime_error( xorstr_( "failed to set window procedure." ) );
	}

	CInput::~CInput( ) noexcept
	{
		SetWindowLongPtr( m_hGameWindow, GWLP_WNDPROC, reinterpret_cast< std::intptr_t >( m_wndProc ) );

		{
			const std::unique_lock< std::mutex > lockWndProc( m_wndProcMutex );

			m_wndProc = nullptr;
		}

		m_hGameWindow = nullptr;
	}

	std::intptr_t CInput::WndProc( HWND hWindow, std::uint32_t uMsg, std::uintptr_t uParam, std::intptr_t iParam ) noexcept
	{
		WNDPROC wndProc = pInput->m_wndProc;
		const std::unique_lock< std::mutex > lockWndProc( pInput->m_wndProcMutex, std::try_to_lock_t( ) ); // @note / xnxkzeu: locks wndproc, so it won't be reset until we finish processing our input.

		KeyID_t uKeyID = 0;
		bool bKeyPressed = false;

		switch ( uMsg )
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				uKeyID = static_cast< KeyID_t >( uParam );
				bKeyPressed = true;

				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				uKeyID = static_cast< KeyID_t >( uParam );
				bKeyPressed = false;

				break;
			}
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			{
				uKeyID = VK_LBUTTON;
				bKeyPressed = true;

				break;
			}
			case WM_LBUTTONUP:
			{
				uKeyID = VK_LBUTTON;
				bKeyPressed = false;

				break;
			}
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			{
				uKeyID = VK_RBUTTON;
				bKeyPressed = true;

				break;
			}
			case WM_RBUTTONUP:
			{
				uKeyID = VK_RBUTTON;
				bKeyPressed = false;

				break;
			}
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
			{
				uKeyID = VK_MBUTTON;
				bKeyPressed = true;

				break;
			}
			case WM_MBUTTONUP:
			{
				uKeyID = VK_MBUTTON;
				bKeyPressed = false;

				break;
			}
			case WM_XBUTTONDOWN:
			case WM_XBUTTONDBLCLK:
			{
				uKeyID = static_cast< KeyID_t >( GET_XBUTTON_WPARAM( uParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
				bKeyPressed = true;

				break;
			}
			case WM_XBUTTONUP:
			{
				uKeyID = static_cast< KeyID_t >( GET_XBUTTON_WPARAM( uParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
				bKeyPressed = false;

				break;
			}
			default:
				break;
		}

		if ( uKeyID )
		{
			// @note / xnxkzeu: callback array is [0, 254], and keycodes are [1-255], so we need to compensate it.
			if ( auto& fnCallback = pInput->m_arrCallbacks[ uKeyID - 1 ]; bKeyPressed && fnCallback )
				std::invoke( fnCallback );
		}

		if ( UI::pMenu )
		{
			ImGui_ImplWin32_WndProcHandler( hWindow, uMsg, uParam, iParam );
			if ( UI::pMenu->IsVisible( ) )
			{
				switch ( uMsg )
				{
					case WM_MOUSEMOVE:
					case WM_NCMOUSEMOVE:
					case WM_MOUSELEAVE:
					case WM_NCMOUSELEAVE:
					case WM_LBUTTONDOWN:
					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONDBLCLK:
					case WM_MBUTTONDOWN:
					case WM_MBUTTONDBLCLK:
					case WM_XBUTTONDOWN:
					case WM_XBUTTONDBLCLK:
					case WM_LBUTTONUP:
					case WM_RBUTTONUP:
					case WM_MBUTTONUP:
					case WM_XBUTTONUP:
					case WM_MOUSEWHEEL:
					case WM_MOUSEHWHEEL:
					case WM_KEYDOWN:
					case WM_KEYUP:
					case WM_SYSKEYDOWN:
					case WM_SYSKEYUP:
					case WM_SETFOCUS:
					case WM_KILLFOCUS:
					case WM_CHAR:
					case WM_SETCURSOR:
					case WM_DEVICECHANGE:
						return 1;
					default:
						break;
				}
			}
		}

		return CallWindowProc( wndProc, hWindow, uMsg, uParam, iParam );
	}

	void CInput::RegisterCallback( KeyID_t uKeyID, std::function< void( ) >&& fnCallback ) noexcept
	{
		m_arrCallbacks[ uKeyID - 1 ] = std::move( fnCallback );
	}

	void CInput::RemoveCallback( KeyID_t uKeyID ) noexcept
	{
		m_arrCallbacks[ uKeyID - 1 ] = nullptr;
	}

	HWND CInput::GetGameWindow( ) const noexcept
	{
		return m_hGameWindow;
	}
} // namespace Core