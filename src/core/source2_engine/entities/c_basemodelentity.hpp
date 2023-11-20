#ifndef CS2_CHEAT_C_BASEMODELENTITY_HPP
#define CS2_CHEAT_C_BASEMODELENTITY_HPP

#include "c_baseentity.hpp"

class C_BaseModelEntity
	: public C_BaseEntity
{
public:
	C_BaseModelEntity( ) = delete;
	C_BaseModelEntity( C_BaseModelEntity&& ) = delete;
	C_BaseModelEntity( const C_BaseModelEntity& ) = delete;

	SCHEMA_VARIABLE( "C_BaseModelEntity->m_vecViewOffset", GetViewOffset, Math::Vector_t< float, 3 > );
};

#endif // CS2_CHEAT_C_BASEMODELENTITY_HPP
