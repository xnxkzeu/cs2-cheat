#include <unordered_map>
#include <array>
#include <memory>
#include <stdexcept>
#include <format>

#include <xorstr.hpp>

#include "schema.hpp"
#include "logger.hpp"

#include "../source2_engine/datatypes/cutltshash.hpp"

#include "../source2_engine/ischemasystem.hpp"

#include "../../context/cs2.hpp"

namespace Core
{
	CSchema::CSchema( )
	{
		auto fnHandleTypeScope = [ this ]( const char* szModuleName )
		{
			CSchemaSystemTypeScope* pTypeScope = CTX::pCS2->pSchemaSystem->FindTypeScopeForModule( szModuleName );
			if ( !pTypeScope )
				throw std::runtime_error( std::vformat( xorstr_( R"(failed to find "{}" module type scope.)" ), std::make_format_args( szModuleName ) ) );

			CUtlTSHash< CSchemaClassBindingBase*, 256 >& utlBindingTable = pTypeScope->GetBindingsTable( );
			const auto pElements = std::make_unique_for_overwrite< CSchemaClassBindingBase*[] >( utlBindingTable.Count( ) );
			const auto iElements = utlBindingTable.GetElements( 0, utlBindingTable.Count( ), pElements.get( ) );

			for ( int iElementIndex = 0; iElementIndex < iElements; iElementIndex++ )
			{
				CSchemaClassBindingBase* const pClassBinding = pElements[ iElementIndex ];
				if ( !pClassBinding )
					continue;

				const Hash::FNV1A_t fnvClassHashed = Hash::FNV1A( pClassBinding->szClassName );
				const Hash::FNV1A_t fnvDelimiterHashed = Hash::FNV1A( xorstr_( "->" ), fnvClassHashed );

				for ( int iFieldIndex = 0; iFieldIndex < pClassBinding->uFields; iFieldIndex++ )
				{
					SchemaClassFieldData_t& schemaField = pClassBinding->pFields[ iFieldIndex ];
					const Hash::FNV1A_t fnvFieldHashed = Hash::FNV1A( schemaField.szFieldName, fnvDelimiterHashed );

					m_mapOffsets.try_emplace( fnvFieldHashed, schemaField.uOffset );
				}
			}
		};

		fnHandleTypeScope( xorstr_( CLIENT_MODULE ) );
		fnHandleTypeScope( xorstr_( ENGINE2_MODULE ) );

		m_mapOffsets.rehash( 0 );
	}

	std::uint32_t CSchema::GetOffset( Hash::FNV1A_t fnvFieldHashed ) const noexcept
	{
		const auto& itOffset = m_mapOffsets.find( fnvFieldHashed );
		if ( itOffset == m_mapOffsets.end( ) )
			return 0;

		return itOffset->second;
	}
} // namespace Core