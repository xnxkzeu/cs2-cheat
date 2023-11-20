#ifndef CS2_CHEAT_ISCHEMASYSTEM_HPP
#define CS2_CHEAT_ISCHEMASYSTEM_HPP

#include "datatypes/cutltshash.hpp"

#include "iappsystem.hpp"

#include "../helpers/memory.hpp"

class CSchemaType
{
public:
	OFFSET( 0x8, GetTypeName, const char* );
};

union USchemaNetworkValue
{
	const char* szValue;
	int iValue;
	float flValue;
	void* pValue;
};

struct SchemaMetadataEntryData_t
{
	const char* szName;
	USchemaNetworkValue* pNetworkValue;
};

struct SchemaClassFieldData_t
{
	const char* szFieldName;
	CSchemaType* pSchemaType;
	std::uint32_t uOffset;
	std::uint32_t uMetadataSize;
	SchemaMetadataEntryData_t* pMetadata;
};

struct SchemaStaticFieldData_t
{
	const char* szFieldName;
	CSchemaType* pSchemaType;
	void* pInstance;
private:
	[[maybe_unused]] std::byte m_pad[ 0x10 ];
};


class CSchemaClassBindingBase
{
public:
	CSchemaClassBindingBase( ) = delete;
	CSchemaClassBindingBase( CSchemaClassBindingBase&& ) = delete;
	CSchemaClassBindingBase( const CSchemaClassBindingBase& ) = delete;

private:
	[[maybe_unused]] std::byte m_pad0[ 0x8 ];
public:
	const char* szClassName;
	const char* szModuleName;

	std::uint32_t uSize;
	std::uint16_t uFields;
	std::uint16_t uStaticSize;
	std::uint16_t uMetadataSize;

	SchemaClassFieldData_t* pFields;
	SchemaStaticFieldData_t* pStaticFields;
};

class CSchemaEnumBindingBase
{
public:
	CSchemaEnumBindingBase( ) = delete;
	CSchemaEnumBindingBase( CSchemaEnumBindingBase&& ) = delete;
	CSchemaEnumBindingBase( const CSchemaEnumBindingBase& ) = delete;
};

class CSchemaSystemTypeScope
{
public:
	CSchemaSystemTypeScope( ) = delete;
	CSchemaSystemTypeScope( CSchemaSystemTypeScope&& ) = delete;
	CSchemaSystemTypeScope( const CSchemaSystemTypeScope& ) = delete;

	virtual void InsertNewClassBinding( const char* szModuleName, CSchemaClassBindingBase* pClassBinding ) noexcept = 0;
	virtual void InsertNewEnumBinding( const char* szModuleName, CSchemaEnumBindingBase* pEnumBinding ) noexcept = 0;
	virtual void FindDeclaredClass( CSchemaClassBindingBase*& pOutClassBinding, const char* szClassName ) noexcept = 0;
	virtual void FindDeclaredEnum( CSchemaEnumBindingBase*& pOutEnumBinding, const char* szClassName ) noexcept = 0;

	OFFSET( 0x8, GetName, char[ 256 ] );
	OFFSET( 0x588, GetBindingsTable, CUtlTSHash< CSchemaClassBindingBase*, 256 > );
};

struct ISchemaSystem
	: public IAppSystem
{
	ISchemaSystem( ) = delete;
	ISchemaSystem( ISchemaSystem&& ) = delete;
	ISchemaSystem( const ISchemaSystem& ) = delete;

	[[nodiscard]] virtual CSchemaSystemTypeScope* GlobalTypeScope( ) = 0;
	[[nodiscard]] virtual CSchemaSystemTypeScope* FindOrCreateTypeScopeForModule( const char* szModuleName, void* = nullptr ) = 0;
	[[nodiscard]] virtual CSchemaSystemTypeScope* FindTypeScopeForModule( const char* szModuleName, void* = nullptr ) const = 0;
};

#endif // CS2_CHEAT_ISCHEMASYSTEM_HPP
