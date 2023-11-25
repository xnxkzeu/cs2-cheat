#ifndef CS2_CHEAT_QUATERNION_T_HPP
#define CS2_CHEAT_QUATERNION_T_HPP

#include <array>
#include <cmath>
#include <algorithm>

#include "vector_t.hpp"

namespace Math
{
	struct Quaternion_t
	{
		constexpr Quaternion_t( ) = default;
		constexpr Quaternion_t( const Quaternion_t& quatOther ) noexcept = default;
		constexpr Quaternion_t& operator=( const Quaternion_t& quatOther ) noexcept = default;

		constexpr Quaternion_t( float flW, float flX, float flY, float flZ ) noexcept
			: m_arrValues( { flW, flX, flY, flZ } )
		{
		}

		[[nodiscard]] constexpr float& operator[]( std::size_t uIndex ) noexcept
		{
			return this->m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr float operator[]( std::size_t uIndex ) const noexcept
		{
			return this->m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr float& operator[]( Axis uIndex ) noexcept
		{
			return this->m_arrValues[ std::to_underlying( uIndex ) ];
		}

		[[nodiscard]] constexpr float operator[]( Axis uIndex ) const noexcept
		{
			return this->m_arrValues[ std::to_underlying( uIndex ) ];
		}

		[[nodiscard]] constexpr bool IsValid( ) const noexcept
		{
			return std::ranges::all_of( this->m_arrValues, []( float flValue )
										{ return std::isfinite( flValue ); } );
		}

		constexpr void Invalidate( ) noexcept
		{
			std::ranges::fill( this->m_arrValues, std::numeric_limits< float >::infinity( ) );
		}

	private:
		std::array< float, 4 > m_arrValues = { };
	};
} // namespace Math

#endif // CS2_CHEAT_QUATERNION_T_HPP
