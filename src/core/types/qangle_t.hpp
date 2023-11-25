#ifndef CS2_CHEAT_QANGLE_T_HPP
#define CS2_CHEAT_QANGLE_T_HPP

#include <cmath>
#include <array>
#include <algorithm>

enum class Rotation
{
	Pitch = 0,
	Yaw = 1,
	Roll = 2,
};

namespace Math
{
	struct QAngle_t
	{
		constexpr QAngle_t( ) = default;

		constexpr QAngle_t( float flPitch, float flYaw, float flRoll ) noexcept
		{
			operator[]( Rotation::Pitch ) = flPitch;
			operator[]( Rotation::Yaw ) = flYaw;
			operator[]( Rotation::Roll ) = flRoll;

			Normalize( );
			Clamp( );
		}

		constexpr QAngle_t( const QAngle_t& vecOther ) noexcept
		{
			this->m_arrAngles = vecOther.m_arrAngles;
		}

		constexpr QAngle_t& operator=( const QAngle_t& vecOther ) noexcept
		{
			this->m_arrAngles = vecOther.m_arrAngles;
			return *this;
		}

		constexpr QAngle_t( QAngle_t&& vecOther ) noexcept
		{
			this->m_arrAngles = vecOther.m_arrAngles;
		}

		constexpr explicit QAngle_t( const float* arrAngles )
		{
			for ( std::size_t uIndex = 0; uIndex < this->m_arrAngles.size( ); uIndex++ )
				this->m_arrAngles[ uIndex ] = arrAngles[ uIndex ];
		}

		void Normalize( ) noexcept
		{
			operator[]( Rotation::Pitch ) = std::isfinite( operator[]( Rotation::Pitch ) ) ? std::remainder( operator[]( Rotation::Pitch ), 360.f ) : 0.f;
			operator[]( Rotation::Yaw ) = std::isfinite( operator[]( Rotation::Yaw ) ) ? std::remainder( operator[]( Rotation::Yaw ), 360.f ) : 0.f;
			operator[]( Rotation::Roll ) = std::clamp( operator[]( Rotation::Roll ), -50.f, 50.f );
		}

		void Clamp( ) noexcept
		{
			operator[]( Rotation::Pitch ) = std::clamp( operator[]( Rotation::Pitch ), -89.f, 89.f );
			operator[]( Rotation::Yaw ) = std::clamp( operator[]( Rotation::Yaw ), -180.f, 180.f );
			operator[]( Rotation::Roll ) = std::clamp( operator[]( Rotation::Roll ), -50.f, 50.f );
		}

		[[nodiscard]] constexpr float& GetPitch( ) noexcept
		{
			return operator[]( Rotation::Pitch );
		}

		[[nodiscard]] constexpr float GetPitch( ) const noexcept
		{
			return operator[]( Rotation::Pitch );
		}

		[[nodiscard]] constexpr float& GetYaw( ) noexcept
		{
			return operator[]( Rotation::Yaw );
		}

		[[nodiscard]] constexpr float GetYaw( ) const noexcept
		{
			return operator[]( Rotation::Yaw );
		}

		[[nodiscard]] constexpr float& GetRoll( ) noexcept
		{
			return operator[]( Rotation::Roll );
		}

		[[nodiscard]] constexpr float GetRoll( ) const noexcept
		{
			return operator[]( Rotation::Roll );
		}

		[[nodiscard]] constexpr float& operator[]( std::size_t uIndex )
		{
			return this->m_arrAngles[ uIndex ];
		}

		[[nodiscard]] constexpr float operator[]( std::size_t uIndex ) const
		{
			return this->m_arrAngles[ uIndex ];
		}

		[[nodiscard]] constexpr float& operator[]( Rotation uIndex )
		{
			return this->m_arrAngles[ std::to_underlying( uIndex ) ];
		}

		[[nodiscard]] constexpr float operator[]( Rotation uIndex ) const
		{
			return this->m_arrAngles[ std::to_underlying( uIndex ) ];
		}

		QAngle_t& operator+=( const QAngle_t& angOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < this->m_arrAngles.size( ); ++uIndex )
				this->m_arrAngles[ uIndex ] += angOther.m_arrAngles[ uIndex ];

			return *this;
		}

		QAngle_t& operator-=( const QAngle_t& angOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < this->m_arrAngles.size( ); ++uIndex )
				this->m_arrAngles[ uIndex ] -= angOther.m_arrAngles[ uIndex ];

			return *this;
		}

		QAngle_t& operator*=( const QAngle_t& angOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < this->m_arrAngles.size( ); ++uIndex )
				this->m_arrAngles[ uIndex ] *= angOther.m_arrAngles[ uIndex ];

			return *this;
		}

		QAngle_t& operator/=( const QAngle_t& angOther ) noexcept
		{
			for ( std::size_t uIndex = 0; uIndex < this->m_arrAngles.size( ); ++uIndex )
				this->m_arrAngles[ uIndex ] /= angOther.m_arrAngles[ uIndex ];

			return *this;
		}

		[[nodiscard]] QAngle_t operator+( const QAngle_t& angOther ) const noexcept
		{
			QAngle_t angOutput = *this;
			angOutput += angOther;
			return angOutput;
		}

		[[nodiscard]] QAngle_t operator-( const QAngle_t& angOther ) const noexcept
		{
			QAngle_t angOutput = *this;
			angOutput -= angOther;
			return angOutput;
		}

		[[nodiscard]] QAngle_t operator*( const QAngle_t& angOther ) const noexcept
		{
			QAngle_t angOutput = *this;
			angOutput *= angOther;
			return angOutput;
		}

		[[nodiscard]] QAngle_t operator/( const QAngle_t& angOther ) const noexcept
		{
			QAngle_t angOutput = *this;
			angOutput *= angOther;
			return angOutput;
		}

	private:
		std::array< float, 3 > m_arrAngles = { };
	};
} // namespace Math

#endif // CS2_CHEAT_QANGLE_T_HPP
