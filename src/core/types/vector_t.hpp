#ifndef CS2_CHEAT_VECTOR_T_HPP
#define CS2_CHEAT_VECTOR_T_HPP

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <array>

enum class Axis
{
	X = 0,
	Y = 1,
	Z = 2,
};

namespace Math
{
	template< typename tValue, std::size_t uSize >
	requires( uSize > 0 )
	struct Vector_t
	{
		constexpr Vector_t( ) = default;

		constexpr Vector_t( ) noexcept
		requires( std::floating_point< tValue > )
		{
			Invalidate( );
		}

		constexpr Vector_t( const Vector_t& vecOther ) noexcept
		{
			m_arrValues = vecOther.m_arrValues;
		}

		constexpr Vector_t& operator=( const Vector_t& vecOther ) noexcept
		{
			m_arrValues = vecOther.m_arrValues;
			return *this;
		}

		constexpr Vector_t( Vector_t&& vecOther ) noexcept
		{
			m_arrValues = std::move( vecOther.m_arrValues );
		}

		constexpr Vector_t( const tValue ( &arrValues )[ uSize ] ) noexcept
		{
			std::memcpy( m_arrValues.data( ), arrValues, sizeof( m_arrValues ) );
		}

		constexpr Vector_t( std::same_as< tValue > auto... listValues ) noexcept
		requires( sizeof...( listValues ) == uSize )
		{
			m_arrValues = { listValues... };
		}

		[[nodiscard]] constexpr bool IsValid( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; uIndex++ )
				if ( !std::isfinite( m_arrValues[ uIndex ] ) )
					return false;

			return true;
		}

		constexpr void Invalidate( ) noexcept
		requires( std::floating_point< tValue > )
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; uIndex++ )
				m_arrValues[ uIndex ] = std::numeric_limits< tValue >::infinity( );
		}

		constexpr tValue NormalizeInPlace( ) noexcept
		requires( std::floating_point< tValue > )
		{
			const float flLength = GetLength( );
			const float flRadius = 1.0f / ( flLength + std::numeric_limits< tValue >::epsilon( ) );

			for ( std::size_t uIndex = 0; uIndex < uSize; uIndex++ )
				m_arrValues[ uIndex ] *= flRadius;

			return flLength;
		}

		[[nodiscard]] constexpr Vector_t Normalized( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			Vector_t vecOut = *this;
			vecOut.NormalizeInPlace( );
			return vecOut;
		}

		template< std::size_t uVectorLength = uSize >
		constexpr tValue GetDot( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			tValue valResult = { };

			for ( std::size_t uIndex = 0; uIndex < uVectorLength; ++uIndex )
				valResult += this->m_arrValues[ uIndex ] * this->m_arrValues[ uIndex ];

			return valResult;
		}

		template< std::size_t uVectorLength = uSize >
		constexpr tValue GetLength( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			return std::sqrt( GetDot< uVectorLength >( ) );
		}

		template< std::size_t uVectorLength = uSize >
		constexpr tValue Dot( const Vector_t& vecOther ) const noexcept
		requires( std::floating_point< tValue > )
		{
			tValue valResult = { };

			for ( std::size_t uIndex = 0; uIndex < uVectorLength; ++uIndex )
				valResult += this->m_arrValues[ uIndex ] * vecOther.m_arrValues[ uIndex ];

			return valResult;
		}

		template< std::size_t uVectorLength = uSize >
		constexpr tValue Length( const Vector_t& vecOther ) const noexcept
		requires( std::floating_point< tValue > )
		{
			return std::sqrt( Dot< uVectorLength >( vecOther ) );
		}

		[[nodiscard]] constexpr tValue& operator[]( std::size_t uIndex ) noexcept
		{
			return m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr const tValue& operator[]( std::size_t uIndex ) const noexcept
		{
			return m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr tValue& operator[]( Axis uIndex ) noexcept
		{
			return m_arrValues[ std::to_underlying( uIndex ) ];
		}

		[[nodiscard]] constexpr const tValue& operator[]( Axis uIndex ) const noexcept
		{
			return m_arrValues[ std::to_underlying( uIndex ) ];
		}

		Vector_t& operator+=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] += vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		Vector_t& operator-=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] -= vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		Vector_t& operator*=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] *= vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		Vector_t& operator/=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] /= vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		friend Vector_t operator+( Vector_t vecLeft, const Vector_t& vecRight ) noexcept
		{
			vecLeft += vecRight;
			return vecLeft;
		}

		friend Vector_t operator-( Vector_t vecLeft, const Vector_t& vecRight ) noexcept
		{
			vecLeft -= vecRight;
			return vecLeft;
		}

		friend Vector_t operator*( Vector_t vecLeft, const Vector_t& vecRight ) noexcept
		{
			vecLeft *= vecRight;
			return vecLeft;
		}

		friend Vector_t operator/( Vector_t vecLeft, const Vector_t& vecRight ) noexcept
		{
			vecLeft /= vecRight;
			return vecLeft;
		}

	protected:
		std::array< tValue, uSize > m_arrValues = { };
	};
} // namespace Math

#endif // CS2_CHEAT_VECTOR_T_HPP
