#ifndef CS2_CHEAT_CTRANSFORM_HPP
#define CS2_CHEAT_CTRANSFORM_HPP

#include "../../types/quaternion_t.hpp"
#include "../../types/vector_t.hpp"
#include "../../types/matrix_t.hpp"

class alignas( 16 ) CTransform
{
public:
	[[nodiscard]] constexpr Math::Matrix_t< float, 3, 4 > ToMatrix( ) const noexcept
	{
		Math::Matrix_t< float, 3, 4 > matOutput = { };

		matOutput[ 0, 0 ] = 1.0f - 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Y ] - 2.0f * quatOrientation[ Axis::Z ] * quatOrientation[ Axis::Z ];
		matOutput[ 1, 0 ] = 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Y ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Z ];
		matOutput[ 2, 0 ] = 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Z ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Y ];

		matOutput[ 0, 1 ] = 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Y ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Z ];
		matOutput[ 1, 1 ] = 1.0f - 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::X ] - 2.0f * quatOrientation[ Axis::Z ] * quatOrientation[ Axis::Z ];
		matOutput[ 2, 1 ] = 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Z ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::X ];

		matOutput[ 0, 2 ] = 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Z ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Y ];
		matOutput[ 1, 2 ] = 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Z ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::X ];
		matOutput[ 2, 2 ] = 1.0f - 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::X ] - 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Y ];

		matOutput[ 0, 3 ] = vecPosition[ Axis::X ];
		matOutput[ 1, 3 ] = vecPosition[ Axis::Y ];
		matOutput[ 2, 3 ] = vecPosition[ Axis::Z ];

		return matOutput;
	}

	[[nodiscard]] constexpr Math::Vector_t< float, 3 > RotateVector( const Math::Vector_t< float, 3 >& vecRotate ) const noexcept
	{
		Math::Vector_t< float, 3 > vecOutput = { };

		vecOutput[ Axis::X ] = ( 1.0f - 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Y ] - 2.0f * quatOrientation[ Axis::Z ] * quatOrientation[ Axis::Z ] ) * vecRotate[ Axis::X ] + ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Y ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Z ] ) * vecRotate[ Axis::Y ] + ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Z ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Y ] ) * vecRotate[ Axis::Z ];
		vecOutput[ Axis::Y ] = ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Y ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Z ] ) * vecRotate[ Axis::X ] + ( 1.0f - 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::X ] - 2.0f * quatOrientation[ Axis::Z ] * quatOrientation[ Axis::Z ] ) * vecRotate[ Axis::Y ] + ( 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Z ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::X ] ) * vecRotate[ Axis::Z ];
		vecOutput[ Axis::Z ] = ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Z ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Y ] ) * vecRotate[ Axis::X ] + ( 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Z ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::X ] ) * vecRotate[ Axis::Y ] + ( 1.0f - 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::X ] - 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Y ] ) * vecRotate[ Axis::Z ];

		return vecOutput;
	}

	[[nodiscard]] constexpr Math::Vector_t< float, 3 > TransformVector( const Math::Vector_t< float, 3 >& vecTransform ) const noexcept
	{
		Math::Vector_t< float, 3 > vecOutput = { };

		vecOutput[ Axis::X ] = vecPosition[ Axis::X ] + ( 1.0f - 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Y ] - 2.0f * quatOrientation[ Axis::Z ] * quatOrientation[ Axis::Z ] ) * vecTransform[ Axis::X ] + ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Y ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Z ] ) * vecTransform[ Axis::Y ] + ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Z ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Y ] ) * vecTransform[ Axis::Z ];
		vecOutput[ Axis::Y ] = vecPosition[ Axis::Y ] + ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Y ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Z ] ) * vecTransform[ Axis::X ] + ( 1.0f - 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::X ] - 2.0f * quatOrientation[ Axis::Z ] * quatOrientation[ Axis::Z ] ) * vecTransform[ Axis::Y ] + ( 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Z ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::X ] ) * vecTransform[ Axis::Z ];
		vecOutput[ Axis::Z ] = vecPosition[ Axis::Z ] + ( 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::Z ] - 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::Y ] ) * vecTransform[ Axis::X ] + ( 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Z ] + 2.0f * quatOrientation[ Axis::W ] * quatOrientation[ Axis::X ] ) * vecTransform[ Axis::Y ] + ( 1.0f - 2.0f * quatOrientation[ Axis::X ] * quatOrientation[ Axis::X ] - 2.0f * quatOrientation[ Axis::Y ] * quatOrientation[ Axis::Y ] ) * vecTransform[ Axis::Z ];

		return vecOutput;
	}

	alignas( 16 ) Math::Vector_t< float, 3 > vecPosition;
	alignas( 16 ) Math::Quaternion_t quatOrientation;
};

#endif // CS2_CHEAT_CTRANSFORM_HPP
