#ifndef CS2_CHEAT_CTRANSFORM_HPP
#define CS2_CHEAT_CTRANSFORM_HPP

#include "../../types/quaternion_t.hpp"
#include "../../types/vector_t.hpp"
#include "../../types/matrix_t.hpp"

class alignas( 16 ) CTransform
{
public:
	[[nodiscard]] Math::Matrix_t< float, 3, 4 > ToMatrix( ) const
	{
		Math::Matrix_t< float, 3, 4 > matOutput = { };

		matOutput[ 0, 0 ] = 1.0f - 2.0f * quatOrientation[ QuatAxis::Y ] * quatOrientation[ QuatAxis::Y ] - 2.0 * quatOrientation[ QuatAxis::Z ] * quatOrientation[ QuatAxis::Z ];
		matOutput[ 1, 0 ] = 2.0f * quatOrientation[ QuatAxis::X ] * quatOrientation[ QuatAxis::Y ] + 2.0 * quatOrientation[ QuatAxis::W ] * quatOrientation[ QuatAxis::Z ];
		matOutput[ 2, 0 ] = 2.0f * quatOrientation[ QuatAxis::X ] * quatOrientation[ QuatAxis::Z ] - 2.0 * quatOrientation[ QuatAxis::W ] * quatOrientation[ QuatAxis::Y ];

		matOutput[ 0, 1 ] = 2.0f * quatOrientation[ QuatAxis::X ] * quatOrientation[ QuatAxis::Y ] - 2.0f * quatOrientation[ QuatAxis::W ] * quatOrientation[ QuatAxis::Z ];
		matOutput[ 1, 1 ] = 1.0f - 2.0f * quatOrientation[ QuatAxis::X ] * quatOrientation[ QuatAxis::X ] - 2.0f * quatOrientation[ QuatAxis::Z ] * quatOrientation[ QuatAxis::Z ];
		matOutput[ 2, 1 ] = 2.0f * quatOrientation[ QuatAxis::Y ] * quatOrientation[ QuatAxis::Z ] + 2.0f * quatOrientation[ QuatAxis::W ] * quatOrientation[ QuatAxis::X ];

		matOutput[ 0, 2 ] = 2.0f * quatOrientation[ QuatAxis::X ] * quatOrientation[ QuatAxis::Z ] + 2.0f * quatOrientation[ QuatAxis::W ] * quatOrientation[ QuatAxis::Y ];
		matOutput[ 1, 2 ] = 2.0f * quatOrientation[ QuatAxis::Y ] * quatOrientation[ QuatAxis::Z ] - 2.0f * quatOrientation[ QuatAxis::W ] * quatOrientation[ QuatAxis::X ];
		matOutput[ 2, 2 ] = 1.0f - 2.0f * quatOrientation[ QuatAxis::X ] * quatOrientation[ QuatAxis::X ] - 2.0f * quatOrientation[ QuatAxis::Y ] * quatOrientation[ QuatAxis::Y ];

		matOutput[ 0, 3 ] = vecPosition[ Axis::X ];
		matOutput[ 1, 3 ] = vecPosition[ Axis::Y ];
		matOutput[ 2, 3 ] = vecPosition[ Axis::Z ];

		return matOutput;
	}

	alignas( 16 ) Math::Vector_t< float, 3 > vecPosition;
	alignas( 16 ) Math::Quaternion_t quatOrientation;
};

#endif // CS2_CHEAT_CTRANSFORM_HPP
