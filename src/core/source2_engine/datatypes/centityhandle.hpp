#ifndef CS2_CHEAT_CENTITYHANDLE_HPP
#define CS2_CHEAT_CENTITYHANDLE_HPP

#include <cstdint>

#define INVALID_EHANDLE ( 0xFFFFFFFF )

class CEntityHandle
{
public:
	constexpr CEntityHandle( ) noexcept
		: m_uHandle( INVALID_EHANDLE )
	{
	}

	constexpr CEntityHandle( const CEntityHandle& hOther ) = default;

	[[nodiscard]] constexpr bool IsValid( ) const noexcept
	{
		return m_uHandle != INVALID_EHANDLE;
	}

	[[nodiscard]] constexpr int GetEntryIndex( ) const noexcept
	{
		return static_cast< int >( static_cast< std::uint16_t >( m_uHandle ) & 0x7FFF );
	}

private:
	std::uint32_t m_uHandle;
};

#endif // CS2_CHEAT_CENTITYHANDLE_HPP
