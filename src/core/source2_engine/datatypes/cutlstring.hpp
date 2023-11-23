#ifndef CS2_CHEAT_CUTLSTRING_HPP
#define CS2_CHEAT_CUTLSTRING_HPP

#include "../../../context/memory.hpp"

class CUtlString
{
public:
	CUtlString( ) noexcept = default;

	CUtlString( const char* szValue ) noexcept
	{
		m_szValue = CTX::pMemory->MemAlloc.StrDupFunc( szValue );
	}

	CUtlString& operator=( const char* szValue )
	{
		m_szValue = CTX::pMemory->MemAlloc.StrDupFunc( szValue );

		return *this;
	}

	~CUtlString( ) noexcept
	{
		Purge( );
	}

	[[nodiscard]] char* Get( )
	{
		return m_szValue;
	}

	[[nodiscard]] const char* Get( ) const
	{
		return m_szValue;
	}

	void Purge( )
	{
		if ( !m_szValue )
			return;

		CTX::pMemory->MemAlloc.FreeFunc( m_szValue );
	}
private:
	char* m_szValue = nullptr;
};

#endif // CS2_CHEAT_CUTLSTRING_HPP
