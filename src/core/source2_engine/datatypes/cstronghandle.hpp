#ifndef CS2_CHEAT_CSTRONGHANDLE_HPP
#define CS2_CHEAT_CSTRONGHANDLE_HPP

#include <utility>

#include "resourcebinding_t.hpp"

template< typename tResource >
class CStrongHandle
{
public:
	constexpr CStrongHandle( ) = default;

	constexpr CStrongHandle( CStrongHandle&& hResource ) noexcept
		: m_pBinding( std::exchange( hResource.m_pBinding, nullptr ) )
	{
	}

	constexpr CStrongHandle( const CStrongHandle& hResource ) noexcept
	{
		Init( hResource.m_pBinding );
	}

	constexpr ~CStrongHandle( ) noexcept
	{
		Shutdown( );
	}

	[[nodiscard]] constexpr bool IsCached( ) const noexcept
	{
		return m_pBinding->uFlags & RESOURCE_BINDING_CACHED;
	}

	[[nodiscard]] constexpr operator tResource*( ) noexcept
	{
		return static_cast< tResource* >( m_pBinding->pData );
	}

	[[nodiscard]] constexpr operator const tResource*( ) const noexcept
	{
		return static_cast< const tResource* >( m_pBinding->pData );
	}

	[[nodiscard]] constexpr tResource* operator->( ) noexcept
	{
		return static_cast< tResource* >( m_pBinding->pData );
	}

	[[nodiscard]] constexpr const tResource* operator->( ) const noexcept
	{
		return static_cast< const tResource* >( m_pBinding->pData );
	}

	constexpr CStrongHandle& operator=( CStrongHandle&& hResource ) noexcept
	{
		if ( this != &hResource )
		{
			Shutdown( );
			m_pBinding = std::exchange( hResource.m_pBinding, nullptr );
		}

		return *this;
	}

	constexpr CStrongHandle& operator=( const CStrongHandle& hResource ) noexcept
	{
		if ( this != &hResource )
			Init( hResource.m_pBinding );

		return *this;
	}

	constexpr void Init( ResourceBinding_t< tResource >* pBinding ) noexcept
	{
		Shutdown( );

		m_pBinding = pBinding;
		if ( m_pBinding )
			++m_pBinding->uRefCount;
	}

	constexpr void Shutdown( ) noexcept
	{
		if ( !m_pBinding )
			return;

		--m_pBinding->uRefCount;
		m_pBinding = nullptr;
	}

private:
	ResourceBinding_t< tResource >* m_pBinding = nullptr;
};

#endif // CS2_CHEAT_CSTRONGHANDLE_HPP
