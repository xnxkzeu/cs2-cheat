#ifndef CS2_CHEAT_INPUT_HPP
#define CS2_CHEAT_INPUT_HPP

#include <Windows.h>

#include <memory>
#include <mutex>
#include <functional>

namespace Core
{
	using KeyID_t = std::uint8_t;

	class CInput
	{
	public:
		CInput( );
		~CInput( ) noexcept;

		void RegisterCallback( KeyID_t uKeyID, std::function< void( ) >&& fnCallback ) noexcept;
		void RemoveCallback( KeyID_t uKeyID ) noexcept;

		[[nodiscard]] HWND GetGameWindow( ) const noexcept;
	private:
		HWND m_hGameWindow = nullptr;
		WNDPROC m_wndProc = nullptr;

		std::function< void( ) > m_arrCallbacks[ std::numeric_limits< KeyID_t >::max( ) ] = { };

		std::mutex m_wndProcMutex = { };

		static std::intptr_t WndProc( HWND hWindow, std::uint32_t uMsg, std::uintptr_t uParam, std::intptr_t iParam ) noexcept;
	};

	inline std::unique_ptr< CInput > pInput = nullptr;
} // namespace Core

#endif // CS2_CHEAT_INPUT_HPP
