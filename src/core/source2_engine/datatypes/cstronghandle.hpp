#ifndef CS2_CHEAT_CSTRONGHANDLE_HPP
#define CS2_CHEAT_CSTRONGHANDLE_HPP

#include "resourcebinding_t.hpp"

template< typename tResource >
class CStrongHandle
{
public:
	using ResourceHandleTyped_t = const ResourceBinding_t< tResource >*;

	constexpr CStrongHandle( ) noexcept = default;

	CStrongHandle( const CStrongHandle< tResource >& hResource ) noexcept
	{
		Shutdown( );
		Init( hResource );
	}

	CStrongHandle( CStrongHandle< tResource >&& hResource ) noexcept
	{
		m_pBinding = std::move( hResource.m_pBinding );
	}

	constexpr ~CStrongHandle( ) noexcept
	{
		Shutdown( );
	}

	constexpr void Init( ResourceHandleTyped_t hResource )
	{
		Shutdown( );

		m_pBinding = hResource;
		if ( m_pBinding )
			++m_pBinding->uRefCount;
	}

	constexpr void Init( const CStrongHandle< tResource >& hResource )
	{
		Shutdown( );

		m_pBinding = hResource.m_pBinding;
		if ( m_pBinding )
			++m_pBinding->uRefCount;
	}

	constexpr void Shutdown( ) noexcept
	{
		if ( m_pBinding )
		{
			--m_pBinding->uRefCount;
			m_pBinding = nullptr;
		}
	}

	[[nodiscard]] constexpr bool IsCached( ) const noexcept
	{
		return m_pBinding->uFlags & RESOURCE_BINDING_CACHED;
	}

	explicit constexpr operator bool( ) const noexcept
	{
		return m_pBinding;
	}

	constexpr operator const tResource*( ) const noexcept
	{
		return reinterpret_cast< const tResource* >( m_pBinding->pData );
	}

	constexpr const tResource* operator->( ) const noexcept
	{
		return reinterpret_cast< const tResource* >( m_pBinding->pData );
	}

	constexpr operator ResourceHandleTyped_t( ) const noexcept
	{
		return m_pBinding;
	}

	constexpr operator ResourceHandle_t( ) const noexcept
	{
		return static_cast< ResourceHandle_t >( m_pBinding );
	}

	CStrongHandle< tResource >& operator=( ResourceHandleTyped_t hResource ) noexcept
	{
		Init( hResource );
		return *this;
	}
private:
	const ResourceBinding_t< tResource >* m_pBinding = nullptr;
};

#endif // CS2_CHEAT_CSTRONGHANDLE_HPP
