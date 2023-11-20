#ifndef CS2_CHEAT_MENU_HPP
#define CS2_CHEAT_MENU_HPP

#include <memory>

struct ImGuiIO;

namespace Core::UI
{
	class CMenu
	{
	public:
		CMenu( );
		~CMenu( ) noexcept;

		void Render( ) noexcept;
		void Toggle( ) noexcept;

		void SetVisible( bool bState ) noexcept;
		[[nodiscard]] bool IsVisible( ) const noexcept;

	private:
		bool m_bIsVisible = false;
		ImGuiIO* m_pImGuiIO = nullptr;

		std::size_t m_uActiveTab = 0;
	};

	inline std::unique_ptr< CMenu > pMenu = nullptr;
} // namespace Core::UI

#endif // CS2_CHEAT_MENU_HPP
