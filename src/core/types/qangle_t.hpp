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
		QAngle_t( float flPitch = 0.f, float flYaw = 0.f, float flRoll = 0.f ) noexcept
		{
			operator[]( Rotation::Pitch ) = flPitch;
			operator[]( Rotation::Yaw ) = flYaw;
			operator[]( Rotation::Roll ) = flRoll;

			Normalize( );
			Clamp( );
		}

		explicit QAngle_t( const float* arrAngles )
		{
			std::memcpy( m_arrAngles.data( ), arrAngles, sizeof( m_arrAngles ) );
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

		[[nodiscard]] float& GetPitch( ) noexcept
		{
			return operator[]( Rotation::Pitch );
		}

		[[nodiscard]] const float& GetPitch( ) const noexcept
		{
			return operator[]( Rotation::Pitch );
		}

		[[nodiscard]] float& GetYaw( ) noexcept
		{
			return operator[]( Rotation::Yaw );
		}

		[[nodiscard]] const float& GetYaw( ) const noexcept
		{
			return operator[]( Rotation::Yaw );
		}

		[[nodiscard]] float& GetRoll( ) noexcept
		{
			return operator[]( Rotation::Roll );
		}

		[[nodiscard]] const float& GetRoll( ) const noexcept
		{
			return operator[]( Rotation::Roll );
		}

		[[nodiscard]] float& operator[]( std::size_t uIndex )
		{
			return m_arrAngles[ uIndex ];
		}

		[[nodiscard]] const float& operator[]( std::size_t uIndex ) const
		{
			return m_arrAngles[ uIndex ];
		}

		[[nodiscard]] float& operator[]( Rotation uIndex )
		{
			return m_arrAngles[ std::to_underlying( uIndex ) ];
		}

		[[nodiscard]] const float& operator[]( Rotation uIndex ) const
		{
			return m_arrAngles[ std::to_underlying( uIndex ) ];
		}

	private:
		std::array< float, 3 > m_arrAngles = { };
	};
} // namespace Math

#endif // CS2_CHEAT_QANGLE_T_HPP
