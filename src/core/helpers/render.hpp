#ifndef CS2_CHEAT_RENDER_HPP
#define CS2_CHEAT_RENDER_HPP

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

#include <variant>
#include <memory>
#include <mutex>
#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_freetype.h>

#include "../types/vector_t.hpp"
#include "../types/color_t.hpp"

#include "../hash/fnv1a.hpp"

enum ERectRenderFlags : std::uint32_t
{
	RECT_RENDER_OUTLINE = ( 1 << 0 ),
	RECT_RENDER_FILLED = ( 1 << 1 )
};

enum ETextRenderFlags : std::uint32_t
{
	TEXT_RENDER_CENTERED_X = ( 1 << 0 ),
	TEXT_RENDER_CENTERED_Y = ( 1 << 1 ),
};

namespace Core
{
	class CRender
	{
	public:
		CRender( );
		~CRender( ) noexcept;

		void PrePresent( IDXGISwapChain* pSwapChain ) noexcept;
		void PostPresent( ) noexcept;

		void BeginFrame( ) noexcept;
		void FinishFrame( ) noexcept;

		bool WorldToScreen( const Math::Vector_t< float, 3 >& vecWorld, Math::Vector_t< float, 2 >& vecOutScreen, int iSplitScreenSlot = 0 ) noexcept;

		void Line( const Math::Vector_t< float, 2 >& vecStart, const Math::Vector_t< float, 2 >& vecEnd, Color_t colLine, float flThickness = 1.f ) noexcept;
		void Rect( const Math::Vector_t< float, 2 >& vecMin, const Math::Vector_t< float, 2 >& vecMax, Color_t colRect, std::uint32_t uFlags = { }, Color_t colOutline = Color_t( 0, 0, 0, 255 ), float flRounding = 0.f, ImDrawFlags uRoundingCorners = ImDrawFlags_RoundCornersAll, float flThickness = 1.f ) noexcept;
		void Text( const Math::Vector_t< float, 2 >& vecPos, std::string_view szText, Color_t colText, std::uint32_t uFlags = { } ) noexcept;
		void Text( ImFont* pFont, float flFontSize, const Math::Vector_t< float, 2 >& vecPos, std::string_view szText, Color_t colText, std::uint32_t uFlags = { } ) noexcept;
	private:
		void RenderObjects( ) noexcept;

		struct LineObject_t
		{
			ImVec2 vecStartPos = { };
			ImVec2 vecEndPos = { };
			ImU32 uColor = 0x00000000;
			float flThickness = 0.f;
		};

		struct RectObject_t
		{
			ImVec2 vecMin = { };
			ImVec2 vecMax = { };
			ImU32 uColor = 0x00000000;
			ERectRenderFlags uFlags = { };
			ImU32 uOutlineColor = 0x00000000;
			float flRounding = 0.f;
			ImDrawFlags uRoundingCorners = ImDrawFlags_None;
			float flThickness = 0.f;
		};

		struct TextObject_t
		{
			ImFont* pFont = nullptr;
			float flFontSize = 0.f;
			ImVec2 vecPos = { };
			ImU32 uColor = 0x00000000;
			ETextRenderFlags uFlags = { };
			std::string szText = { };
		};

		using Object_t = std::variant< LineObject_t, RectObject_t, TextObject_t >;

		std::mutex m_mutexRender = { };

		std::vector< Object_t > m_vecDrawData = { };
		std::vector< Object_t > m_vecSafeDrawData = { };
		std::vector< Object_t > m_vecRenderDrawData = { };

		Microsoft::WRL::ComPtr< ID3D11Device > m_pD3D11Device = nullptr;
		Microsoft::WRL::ComPtr< ID3D11DeviceContext > m_pD3D11DeviceContext = nullptr;

		Microsoft::WRL::ComPtr< ID3D11RenderTargetView > m_pRenderTargetView = nullptr;
		Microsoft::WRL::ComPtr< ID3D11RenderTargetView > m_pBackupRenderTargetView = nullptr;

		ImGuiIO* m_pImGuiIO = nullptr;
	};

	inline std::unique_ptr< CRender > pRender = nullptr;
} // namespace Core

#endif // CS2_CHEAT_RENDER_HPP
