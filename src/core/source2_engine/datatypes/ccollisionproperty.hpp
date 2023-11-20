#ifndef CS2_CHEAT_CCOLLISIONPROPERTY_HPP
#define CS2_CHEAT_CCOLLISIONPROPERTY_HPP

#include "../../helpers/schema.hpp"
#include "../../types/vector_t.hpp"

class CCollisionProperty
{
public:
	CCollisionProperty( ) = delete;
	CCollisionProperty( CCollisionProperty&& ) = delete;
	CCollisionProperty( const CCollisionProperty& ) = delete;

	SCHEMA_VARIABLE( "CCollisionProperty->m_vecMins", GetMins, Math::Vector_t< float, 3 > );
	SCHEMA_VARIABLE( "CCollisionProperty->m_vecMaxs", GetMaxs, Math::Vector_t< float, 3 > );
};

#endif // CS2_CHEAT_CCOLLISIONPROPERTY_HPP
