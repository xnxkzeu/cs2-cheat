#include <ShlObj.h>
#include <atlbase.h>

#include <filesystem>
#include <array>

#include <xorstr.hpp>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "render.hpp"
#include "input.hpp"

#include "../types/matrix_t.hpp"

#include "../source2_engine/crenderdevicedx11.hpp"
#include "../source2_engine/crendergamesystem.hpp"

#include "../../context/cs2.hpp"

namespace Core
{
	CRender::CRender( )
	{
		if ( m_pD3D11Device = CTX::pCS2->pRenderDeviceDx11->GetD3DDevice( ); !m_pD3D11Device )
			throw std::runtime_error( xorstr_( "failed to get d3d11 device." ) );

		if ( m_pD3D11Device->GetImmediateContext( &m_pD3D11DeviceContext ); !m_pD3D11DeviceContext )
			throw std::runtime_error( xorstr_( "Failed to get d3d11 device context." ) );

		std::filesystem::path pathFonts = { };

		{
			CComHeapPtr< wchar_t > pFontsPath = { };
			if ( FAILED( SHGetKnownFolderPath( FOLDERID_Fonts, 0, nullptr, &pFontsPath ) ) )
				throw std::runtime_error( xorstr_( "Failed to get fonts directory path." ) );

			pathFonts = static_cast< wchar_t* >( pFontsPath );
		}

		ImGui::CreateContext( );

		m_pImGuiIO = &ImGui::GetIO( );
		m_pImGuiIO->IniFilename = nullptr;
		m_pImGuiIO->LogFilename = nullptr;

		ImVector< ImWchar > vecRanges = { };
		ImFontGlyphRangesBuilder rangesBuilder = { };

		static constexpr auto arrBaseRanges = std::to_array< ImWchar >(
			{ 0x0100, 0x024F,
			  0x0300, 0x03FF,
			  0x0600, 0x06FF,
			  0x0E00, 0x0E7F,
			  0 } );

		rangesBuilder.AddRanges( arrBaseRanges.data( ) );
		rangesBuilder.AddRanges( m_pImGuiIO->Fonts->GetGlyphRangesCyrillic( ) );
		rangesBuilder.BuildRanges( &vecRanges );

		ImFontConfig verdanaConfig = { };
#ifdef IMGUI_ENABLE_FREETYPE
		verdanaConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
#endif
		m_pImGuiIO->Fonts->AddFontFromFileTTF( ( pathFonts / xorstr_( "Verdana.ttf" ) ).string( ).c_str( ), 14.f, &verdanaConfig, vecRanges.Data );
		m_pImGuiIO->Fonts->Build( );

		ImGui_ImplWin32_Init( pInput->GetGameWindow( ) );
		ImGui_ImplDX11_Init( m_pD3D11Device.Get( ), m_pD3D11DeviceContext.Get( ) );
	}

	CRender::~CRender( ) noexcept
	{
		if ( ImGui::GetCurrentContext( ) )
		{
			ImGui_ImplDX11_Shutdown( );
			ImGui_ImplWin32_Shutdown( );

			ImGui::DestroyContext( );
		}
	}

	void CRender::PrePresent( IDXGISwapChain* pSwapChain ) noexcept
	{
#pragma clang diagnostic ignored "-Wlanguage-extension-token"
		Microsoft::WRL::ComPtr< ID3D11Texture2D > pBackBuffer = nullptr;
		if ( FAILED( pSwapChain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) ) ) )
			return;
#pragma clang diagnostic warning "-Wlanguage-extension-token"

		D3D11_RENDER_TARGET_VIEW_DESC d3d11RenderTargetViewDesc;
		std::memset( &d3d11RenderTargetViewDesc, 0, sizeof( d3d11RenderTargetViewDesc ) );
		d3d11RenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3d11RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

		m_pD3D11Device->CreateRenderTargetView( pBackBuffer.Get( ), &d3d11RenderTargetViewDesc, &m_pRenderTargetView );

		m_pD3D11DeviceContext->OMGetRenderTargets( 1, m_pBackupRenderTargetView.GetAddressOf( ), nullptr );
		m_pD3D11DeviceContext->OMSetRenderTargets( 1, m_pRenderTargetView.GetAddressOf( ), nullptr );

		RenderObjects( );
	}

	void CRender::PostPresent( ) noexcept
	{
		if ( !m_pRenderTargetView )
			return;

		m_pD3D11DeviceContext->OMSetRenderTargets( 1, m_pBackupRenderTargetView.GetAddressOf( ), nullptr );
		m_pBackupRenderTargetView.Reset( );
		m_pRenderTargetView.Reset( );
	}

	void CRender::BeginFrame( ) noexcept
	{
		if ( !m_vecDrawData.empty( ) )
			m_vecDrawData.clear( );
	}

	void CRender::FinishFrame( ) noexcept
	{
		std::scoped_lock< std::mutex > lockRender( m_mutexRender );

		m_vecSafeDrawData = m_vecDrawData;
	}

	bool CRender::WorldToScreen( const Math::Vector_t< float, 3 >& vecWorld, Math::Vector_t< float, 2 >& vecOutScreen, int iSplitScreenSlot ) noexcept
	{
		CRenderGameSystem* pRenderGameSystem = *CTX::pCS2->ppRenderGameSystem;
		if ( !pRenderGameSystem )
			return false;

		const Math::ViewMatrix_t& matWorldToScreen = pRenderGameSystem->WorldToProjectionMatrix( iSplitScreenSlot );
		const float flWidth = matWorldToScreen[ 3, 0 ] * vecWorld[ 0 ] + matWorldToScreen[ 3, 1 ] * vecWorld[ 1 ] + matWorldToScreen[ 3, 2 ] * vecWorld[ 2 ] + matWorldToScreen[ 3, 3 ];

		if ( flWidth < 0.001f )
			return false;

		ImVec2& vecDisplaySize = m_pImGuiIO->DisplaySize;

		const float flInverse = 1.0f / flWidth;
		vecOutScreen[ 0 ] = ( matWorldToScreen[ 0, 0 ] * vecWorld[ 0 ] + matWorldToScreen[ 0, 1 ] * vecWorld[ 1 ] + matWorldToScreen[ 0, 2 ] * vecWorld[ 2 ] + matWorldToScreen[ 0, 3 ] ) * flInverse;
		vecOutScreen[ 1 ] = ( matWorldToScreen[ 1, 0 ] * vecWorld[ 0 ] + matWorldToScreen[ 1, 1 ] * vecWorld[ 1 ] + matWorldToScreen[ 1, 2 ] * vecWorld[ 2 ] + matWorldToScreen[ 1, 3 ] ) * flInverse;

		vecOutScreen[ 0 ] = std::floor( ( vecDisplaySize[ 0 ] * 0.5f ) + ( vecOutScreen[ 0 ] * vecDisplaySize[ 0 ] ) * 0.5f );
		vecOutScreen[ 1 ] = std::floor( ( vecDisplaySize[ 1 ] * 0.5f ) - ( vecOutScreen[ 1 ] * vecDisplaySize[ 1 ] ) * 0.5f );
		return true;
	}

	void CRender::Line( const Math::Vector_t< float, 2 >& vecStart, const Math::Vector_t< float, 2 >& vecEnd, const Color_t colLine, float flThickness ) noexcept
	{
		m_vecDrawData.emplace_back(
			LineObject_t {
				*reinterpret_cast< const ImVec2* >( &vecStart ),
				*reinterpret_cast< const ImVec2* >( &vecEnd ),
				static_cast< ImU32 >( colLine ),
				flThickness,
			} );
	}

	void CRender::Rect( const Math::Vector_t< float, 2 >& vecMin, const Math::Vector_t< float, 2 >& vecMax, const Color_t colRect, std::uint32_t uFlags, const Color_t colOutline, float flRounding, ImDrawFlags uRoundingCorners, float flThickness ) noexcept
	{
		m_vecDrawData.emplace_back(
			RectObject_t {
				*reinterpret_cast< const ImVec2* >( &vecMin ),
				*reinterpret_cast< const ImVec2* >( &vecMax ),
				static_cast< ImU32 >( colRect ),
				static_cast< ERectRenderFlags >( uFlags ),
				static_cast< ImU32 >( colOutline ),
				flRounding,
				uRoundingCorners,
				flThickness,
			} );
	}

	void CRender::Text( const Math::Vector_t< float, 2 >& vecPos, const char* szText, const Color_t colText, std::uint32_t uFlags ) noexcept
	{
		m_vecDrawData.emplace_back(
			TextObject_t {
				nullptr,
				0.f,
				*reinterpret_cast< const ImVec2* >( &vecPos ),
				static_cast< ImU32 >( colText ),
				static_cast< ETextRenderFlags >( uFlags ),
				szText,
			} );
	}

	void CRender::Text( ImFont* pFont, float flFontSize, const Math::Vector_t< float, 2 >& vecPos, const char* szText, Color_t colText, std::uint32_t uFlags ) noexcept
	{
		if ( pFont && !pFont->ContainerAtlas )
			pFont = nullptr;

		m_vecDrawData.emplace_back(
			TextObject_t {
				pFont,
				flFontSize,
				*reinterpret_cast< const ImVec2* >( &vecPos ),
				static_cast< ImU32 >( colText ),
				static_cast< ETextRenderFlags >( uFlags ),
				szText,
			} );
	}

	void CRender::RenderObjects( ) noexcept
	{
		if ( m_mutexRender.try_lock( ) )
		{
			m_vecRenderDrawData = m_vecSafeDrawData;
			m_mutexRender.unlock( );
		}

		if ( m_vecRenderDrawData.empty( ) )
			return;

		struct ObjectVisitor_t
		{
			explicit ObjectVisitor_t( ImDrawList* pDrawList = nullptr ) noexcept
				: m_pDrawList( pDrawList )
			{
			}

			ImDrawList* m_pDrawList = nullptr;

			void operator( )( const LineObject_t& lineObject ) const noexcept
			{
				m_pDrawList->AddLine( lineObject.vecStartPos, lineObject.vecEndPos, lineObject.uColor, lineObject.flThickness );
			}

			void operator( )( const RectObject_t& rectObject ) const noexcept
			{
				if ( rectObject.uFlags & RECT_RENDER_FILLED )
				{
					if ( rectObject.uFlags & RECT_RENDER_OUTLINE )
						m_pDrawList->AddRectFilled( rectObject.vecMin, rectObject.vecMax, rectObject.uOutlineColor, rectObject.flRounding, rectObject.uRoundingCorners );

					m_pDrawList->AddRectFilled( rectObject.vecMin, rectObject.vecMax, rectObject.uColor, rectObject.flRounding, rectObject.uRoundingCorners );
				}
				else
				{
					if ( rectObject.uFlags & RECT_RENDER_OUTLINE )
						m_pDrawList->AddRect( rectObject.vecMin, rectObject.vecMax, rectObject.uOutlineColor, rectObject.flRounding, rectObject.uRoundingCorners, rectObject.flThickness + 2.f );

					m_pDrawList->AddRect( rectObject.vecMin, rectObject.vecMax, rectObject.uColor, rectObject.flRounding, rectObject.uRoundingCorners, rectObject.flThickness );
				}
			}

			void operator( )( const TextObject_t& textObject ) const noexcept
			{
				ImFont* pFont = textObject.pFont ? textObject.pFont : ImGui::GetFont( );
				float flFontSize = textObject.flFontSize == 0.f ? pFont->FontSize : textObject.flFontSize;

				ImVec2 vecRenderPos = textObject.vecPos;

				if ( textObject.uFlags & TEXT_RENDER_CENTERED_X || textObject.uFlags & TEXT_RENDER_CENTERED_Y )
				{
					ImVec2 vecTextSize = pFont->CalcTextSizeA( flFontSize, std::numeric_limits< float >::max( ), 0.f, textObject.szText.c_str( ) );
					if ( textObject.uFlags & TEXT_RENDER_CENTERED_X )
						vecRenderPos.x -= vecTextSize.x * 0.5f;
					if ( textObject.uFlags & TEXT_RENDER_CENTERED_Y )
						vecRenderPos.y -= vecTextSize.y * 0.5f;
				}

				m_pDrawList->AddText( pFont, flFontSize, vecRenderPos, textObject.uColor, textObject.szText.c_str( ) );
			}
		};

		ObjectVisitor_t objectVisitor( ImGui::GetBackgroundDrawList( ) );

		for ( const auto& objectInfo : m_vecRenderDrawData )
			std::visit( objectVisitor, objectInfo );
	}
} // namespace Core