#ifndef CS2_CHEAT_COLOR_T_HPP
#define CS2_CHEAT_COLOR_T_HPP

#include <bit>
#include <array>
#include <cstdint>

enum class Color
{
	Red = 0,
	Green,
	Blue,
	Alpha
};

struct Color_t
{
	Color_t( ) = default;

	constexpr Color_t( std::uint8_t uRed, std::uint8_t uGreen, std::uint8_t uBlue, std::uint8_t uAlpha ) noexcept
	{
		operator[]( Color::Red ) = uRed;
		operator[]( Color::Green ) = uGreen;
		operator[]( Color::Blue ) = uBlue;
		operator[]( Color::Alpha ) = uAlpha;
	}

	constexpr Color_t( std::uint32_t uColor ) noexcept
	{
		m_arrColors = std::bit_cast< decltype( m_arrColors ) >( uColor );
	}

	[[nodiscard]] constexpr operator std::uint32_t( ) const noexcept
	{
		return std::bit_cast< std::uint32_t >( m_arrColors );
	}

	[[nodiscard]] constexpr std::uint8_t& operator[]( std::size_t uIndex ) noexcept
	{
		return m_arrColors[ uIndex ];
	}

	[[nodiscard]] constexpr const std::uint8_t& operator[]( std::size_t uIndex ) const noexcept
	{
		return m_arrColors[ uIndex ];
	}

	[[nodiscard]] constexpr std::uint8_t& operator[]( Color uColor ) noexcept
	{
		return m_arrColors[ std::to_underlying( uColor ) ];
	}

	[[nodiscard]] constexpr const std::uint8_t& operator[]( Color uColor ) const noexcept
	{
		return m_arrColors[ std::to_underlying( uColor ) ];
	}
private:
	std::array< std::uint8_t, 4 > m_arrColors = { };
};

#endif // CS2_CHEAT_COLOR_T_HPP
