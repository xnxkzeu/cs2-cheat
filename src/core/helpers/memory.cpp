#include <Windows.h>
#include <winternl.h>

#include <memory>
#include <charconv>

#include <xorstr.hpp>

#include "memory.hpp"

namespace Core::Memory
{
	PEB* GetProcessEnvironmentBlock( ) noexcept
	{
		return reinterpret_cast< PEB* >( __readgsqword( 0x60 ) );
	}

	Address_t GetModuleBaseAddress( std::string_view szModuleName ) noexcept
	{
		std::unique_ptr pWideString = std::make_unique< std::wstring_view::value_type[] >( szModuleName.length( ) );
		std::size_t uLength = std::mbstowcs( pWideString.get( ), szModuleName.data( ), szModuleName.length( ) );
		return GetModuleBaseAddress( std::wstring_view( pWideString.get( ), uLength ) );
	}

	Address_t GetModuleBaseAddress( std::wstring_view szModuleName ) noexcept
	{
		PEB* pProcessEnvironmentBlock = GetProcessEnvironmentBlock( );
		if ( !pProcessEnvironmentBlock )
			return { };

		LIST_ENTRY* pModuleList = &pProcessEnvironmentBlock->Ldr->InMemoryOrderModuleList;

		for ( LIST_ENTRY* pListEntry = pModuleList->Flink; pListEntry != pModuleList; pListEntry = pListEntry->Flink )
		{
			LDR_DATA_TABLE_ENTRY* pEntry = CONTAINING_RECORD( pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks );
			if ( !pEntry )
				continue;

			std::wstring_view szModulePath( pEntry->FullDllName.Buffer );

			std::size_t uLastDelimiter = szModulePath.find_last_of( '\\' );
			if ( uLastDelimiter == std::wstring_view::npos )
				continue;

			if ( szModulePath.substr( uLastDelimiter + 1 ) != szModuleName )
				continue;

			return Address_t( pEntry->DllBase );
		}

		return { };
	}

	Address_t GetExportAddress( Address_t uModuleBaseAddress, std::string_view szProcedureName ) noexcept
	{
		auto* pDOSHeader = uModuleBaseAddress.Cast< const IMAGE_DOS_HEADER* >( );
		auto* pNTHeaders = uModuleBaseAddress.Offset< const IMAGE_NT_HEADERS* >( pDOSHeader->e_lfanew );

		const IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNTHeaders->OptionalHeader;

		const std::uint32_t uExportSize = pOptionalHeader->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
		const std::uint32_t uExportAddress = pOptionalHeader->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

		if ( uExportSize <= 0 )
			return { };

		auto* pExportDirectory = uModuleBaseAddress.Offset< const IMAGE_EXPORT_DIRECTORY* >( uExportAddress );
		auto* pNamesRVA = uModuleBaseAddress.Offset< const std::uint32_t* >( pExportDirectory->AddressOfNames );
		auto* pFunctionsRVA = uModuleBaseAddress.Offset< const std::uint32_t* >( pExportDirectory->AddressOfFunctions );
		auto* pNameOrdinalsRVA = uModuleBaseAddress.Offset< const std::uint16_t* >( pExportDirectory->AddressOfNameOrdinals );

		std::uint32_t uRight = pExportDirectory->NumberOfNames;
		std::uint32_t uLeft = 0;

		while ( uRight != uLeft )
		{
			const std::uint32_t uMiddle = uLeft + ( ( uRight - uLeft ) >> 1 );
			const int iResult = szProcedureName.compare( uModuleBaseAddress.Offset< const char* >( pNamesRVA[ uMiddle ] ) );

			if ( iResult > 0 )
				uLeft = uMiddle;
			else if ( iResult < 0 )
				uRight = uMiddle;
			else
				return uModuleBaseAddress.Offset( pFunctionsRVA[ pNameOrdinalsRVA[ uMiddle ] ] );
		}

		return { };
	}

	bool GetSectionInfo( Address_t uModuleAddress, std::string_view szSectionName, Address_t* pOutSectionAddress, std::size_t* pOutSectionSize ) noexcept
	{
		auto* const pDOSHeader = uModuleAddress.Cast< const IMAGE_DOS_HEADER* >( );
		auto* const pNTHeaders = uModuleAddress.Offset< const IMAGE_NT_HEADERS* >( pDOSHeader->e_lfanew );

		const IMAGE_FILE_HEADER* const pFileHeader = &pNTHeaders->FileHeader;

		for ( std::uint16_t uSectionIndex = 0; uSectionIndex < pFileHeader->NumberOfSections; uSectionIndex++ )
		{
			auto* const pSection = Address_t( &pNTHeaders->OptionalHeader ).Offset< const IMAGE_SECTION_HEADER* >( pNTHeaders->FileHeader.SizeOfOptionalHeader + sizeof( IMAGE_SECTION_HEADER ) * uSectionIndex );

			const std::string_view szName( reinterpret_cast< const char* >( pSection->Name ) );
			if ( szName == szSectionName )
			{
				if ( pOutSectionAddress )
					*pOutSectionAddress = uModuleAddress.Offset( pSection->VirtualAddress );

				if ( pOutSectionSize )
					*pOutSectionSize = pSection->Misc.VirtualSize;

				return true;
			}
		}

		return false;
	}

	Address_t FindPattern( const std::string_view szModuleName, const std::string_view szPattern ) noexcept
	{
		const Address_t uModuleAddress = GetModuleBaseAddress( szModuleName );
		if ( !uModuleAddress )
			return { };

		Address_t uCodeSection = { };
		std::size_t uSectionSize = 0;

		if ( !GetSectionInfo( uModuleAddress, xorstr_( ".text" ), &uCodeSection, &uSectionSize ) )
			return { };

		return FindPattern( uCodeSection, uSectionSize, szPattern );
	}

	Address_t FindPattern( Address_t uRegionAddress, std::size_t uRegionSize, std::string_view szPattern ) noexcept
	{
		const std::byte* const pBytes = uRegionAddress.Cast< std::byte* >( );

		const std::vector< std::optional< std::byte > > vecBytes = PatternToBytes( szPattern );
		if ( vecBytes.empty( ) )
			return { };

		for ( std::uintptr_t uIndex = 0, uSize = uRegionSize - vecBytes.size( ); uIndex < uSize; ++uIndex )
		{
			bool bByteFound = true;

			for ( std::size_t uByteIndex = 0; uByteIndex < vecBytes.size( ); ++uByteIndex )
			{
				if ( !vecBytes[ uByteIndex ].has_value( ) )
					continue;

				if ( pBytes[ uIndex + uByteIndex ] == vecBytes[ uByteIndex ].value( ) )
					continue;

				bByteFound = false;
				break;
			}

			if ( bByteFound )
				return Address_t( reinterpret_cast< const void* >( &pBytes[ uIndex ] ) );
		}

		return { };
	}

	std::vector< std::optional< std::byte > > PatternToBytes( std::string_view szPattern ) noexcept
	{
		std::vector< std::optional< std::byte > > vecBytes = { };

		const char* pStart = szPattern.data( );
		const char* pEnd = pStart + szPattern.size( );

		for ( const char* pCurrent = pStart; pCurrent < pEnd; ++pCurrent )
		{
			if ( *pCurrent == '?' )
			{
				++pCurrent;
				if ( *pCurrent == '?' )
					++pCurrent;

				vecBytes.emplace_back( std::nullopt );
			}
			else
				vecBytes.emplace_back( static_cast< std::byte >( std::strtoul( pCurrent, const_cast< char** >( &pCurrent ), 16 ) ) );
		}

		return vecBytes;
	}
} // namespace Core::Memory