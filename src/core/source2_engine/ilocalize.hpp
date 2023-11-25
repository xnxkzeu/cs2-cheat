#ifndef CS2_CHEAT_ILOCALIZE_HPP
#define CS2_CHEAT_ILOCALIZE_HPP

#include "iappsystem.hpp"

#include "../helpers/memory.hpp"

struct ILocalize
	: public IAppSystem
{
	ILocalize( ) = delete;
	ILocalize( ILocalize&& ) = delete;
	ILocalize( const ILocalize& ) = delete;

	VFUNC( 16, Find( const char* szLocalizationToken ), ( this, szLocalizationToken ), const char* );
	VFUNC( 17, FindSafe( const char* szLocalizationToken ), ( this, szLocalizationToken ), const char* );
};

#endif // CS2_CHEAT_ILOCALIZE_HPP
