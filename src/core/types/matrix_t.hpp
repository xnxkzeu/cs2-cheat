#ifndef CS2_CHEAT_MATRIX_T_HPP
#define CS2_CHEAT_MATRIX_T_HPP

#include <array>
#include <cstddef>
#include <algorithm>

namespace Math
{
	template< typename tValue, std::size_t uRows, std::size_t uColumns >
	struct Matrix_t
	{
		constexpr Matrix_t( ) = default;

		constexpr Matrix_t( const tValue ( &arrValues )[ uRows ][ uColumns ] ) noexcept
		{
			std::ranges::copy( arrValues, m_arrValues );
		}

		constexpr Matrix_t& operator=( const tValue ( &arrValues )[ uRows ][ uColumns ] ) noexcept
		{
			std::ranges::copy( arrValues, m_arrValues );

			return *this;
		}

		[[nodiscard]] constexpr std::array< tValue, uColumns >& operator[]( std::size_t uIndex ) noexcept
		{
			return this->m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr const std::array< tValue, uColumns >& operator[]( std::size_t uIndex ) const noexcept
		{
			return this->m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr tValue& operator[]( std::size_t uRow, std::size_t uColumn ) noexcept
		{
			return this->m_arrValues[ uRow ][ uColumn ];
		}

		[[nodiscard]] constexpr const tValue& operator[]( std::size_t uRow, std::size_t uColumn ) const noexcept
		{
			return this->m_arrValues[ uRow ][ uColumn ];
		}
	private:
		std::array< std::array< tValue, uColumns >, uRows > m_arrValues = { };
	};

	using ViewMatrix_t = Matrix_t< float, 4, 4 >;
} // namespace Math

#endif // CS2_CHEAT_MATRIX_T_HPP
