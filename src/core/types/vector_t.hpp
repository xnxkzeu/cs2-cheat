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
	W = 3,
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
			this->m_arrValues = vecOther.m_arrValues;
		}

		constexpr Vector_t& operator=( const Vector_t& vecOther ) noexcept
		{
			this->m_arrValues = vecOther.m_arrValues;
			return *this;
		}

		constexpr Vector_t( Vector_t&& vecOther ) noexcept
		{
			this->m_arrValues = std::move( vecOther.m_arrValues );
		}

		constexpr Vector_t( const tValue ( &arrValues )[ uSize ] ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; uIndex++ )
				this->m_arrValues[ uIndex ] = arrValues[ uIndex ];
		}

		constexpr Vector_t( std::convertible_to< tValue > auto... listValues ) noexcept
		requires( sizeof...( listValues ) == uSize )
		{
			this->m_arrValues = { static_cast< tValue >( listValues )... };
		}

		[[nodiscard]] constexpr bool IsValid( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			return std::ranges::all_of( this->m_arrValues, []( float flValue )
										{ return std::isfinite( flValue ); } );
		}

		constexpr void Invalidate( ) noexcept
		requires( std::floating_point< tValue > )
		{
			std::ranges::fill( this->m_arrValues, std::numeric_limits< float >::infinity( ) );
		}

		constexpr tValue NormalizeInPlace( ) noexcept
		requires( std::floating_point< tValue > )
		{
			const float flLength = GetLength( );
			const float flRadius = 1.0f / ( flLength + std::numeric_limits< tValue >::epsilon( ) );

			for ( std::size_t uIndex = 0; uIndex < uSize; uIndex++ )
				this->m_arrValues[ uIndex ] *= flRadius;

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
		[[nodiscard]] constexpr tValue GetDot( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			tValue valResult = { };

			for ( std::size_t uIndex = 0; uIndex < uVectorLength; ++uIndex )
				valResult += this->m_arrValues[ uIndex ] * this->m_arrValues[ uIndex ];

			return valResult;
		}

		template< std::size_t uVectorLength = uSize >
		[[nodiscard]] constexpr tValue GetLength( ) const noexcept
		requires( std::floating_point< tValue > )
		{
			return std::sqrt( GetDot< uVectorLength >( ) );
		}

		template< std::size_t uVectorLength = uSize >
		[[nodiscard]] constexpr tValue Dot( const Vector_t& vecOther ) const noexcept
		requires( std::floating_point< tValue > )
		{
			tValue valResult = { };

			for ( std::size_t uIndex = 0; uIndex < uVectorLength; ++uIndex )
				valResult += this->m_arrValues[ uIndex ] * vecOther.m_arrValues[ uIndex ];

			return valResult;
		}

		template< std::size_t uVectorLength = uSize >
		[[nodiscard]] constexpr tValue Length( const Vector_t& vecOther ) const noexcept
		requires( std::floating_point< tValue > )
		{
			return std::sqrt( Dot< uVectorLength >( vecOther ) );
		}

		[[nodiscard]] constexpr tValue& operator[]( std::size_t uIndex ) noexcept
		{
			return this->m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr const tValue& operator[]( std::size_t uIndex ) const noexcept
		{
			return this->m_arrValues[ uIndex ];
		}

		[[nodiscard]] constexpr tValue& operator[]( Axis uIndex ) noexcept
		{
			return this->m_arrValues[ std::to_underlying( uIndex ) ];
		}

		[[nodiscard]] constexpr const tValue& operator[]( Axis uIndex ) const noexcept
		{
			return this->m_arrValues[ std::to_underlying( uIndex ) ];
		}

		constexpr Vector_t& operator+=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] += vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		constexpr Vector_t& operator-=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] -= vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		constexpr Vector_t& operator*=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] *= vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		constexpr Vector_t& operator/=( const Vector_t& vecOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] /= vecOther.m_arrValues[ uIndex ];

			return *this;
		}

		constexpr Vector_t& operator+=( float flValue ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] += flValue;

			return *this;
		}

		constexpr Vector_t& operator-=( float flValue ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] -= flValue;

			return *this;
		}

		constexpr Vector_t& operator*=( float flValue ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] *= flValue;

			return *this;
		}

		constexpr Vector_t& operator/=( float flValue ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < uSize; ++uIndex )
				this->m_arrValues[ uIndex ] /= flValue;

			return *this;
		}

		[[nodiscard]] constexpr Vector_t operator+( const Vector_t& vecOther ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput += vecOther;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator-( const Vector_t& vecOther ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput -= vecOther;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator*( const Vector_t& vecOther ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput *= vecOther;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator/( const Vector_t& vecOther ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput *= vecOther;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator+( float flValue ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput += flValue;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator-( float flValue ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput -= flValue;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator*( float flValue ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput *= flValue;
			return vecOutput;
		}

		[[nodiscard]] constexpr Vector_t operator/( float flValue ) const noexcept
		{
			Vector_t vecOutput = *this;
			vecOutput *= flValue;
			return vecOutput;
		}

	protected:
		std::array< tValue, uSize > m_arrValues = { };
	};
} // namespace Math

#endif // CS2_CHEAT_VECTOR_T_HPP
