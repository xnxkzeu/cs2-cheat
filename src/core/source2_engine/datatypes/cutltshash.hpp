#ifndef CS2_CHEAT_CUTLTSHASH_HPP
#define CS2_CHEAT_CUTLTSHASH_HPP

#include <cstdint>
#include <limits>

#include "cutlmemorypool.hpp"

using UtlTSHashHandle_t = std::uintptr_t;

inline constexpr std::uint32_t HashIntConventional( const int iValue ) noexcept
{
	std::uint32_t uHash = 0xAAAAAAAA + ( iValue & 0xFF );
	uHash = ( uHash << 5 ) + uHash + ( ( iValue >> 8 ) & 0xFF );
	uHash = ( uHash << 5 ) + uHash + ( ( iValue >> 16 ) & 0xFF );
	uHash = ( uHash << 5 ) + uHash + ( ( iValue >> 24 ) & 0xFF );

	return uHash;
}

template< int iBucketCount, class tKey = std::uintptr_t >
class CUtlTSHashGenericHash
{
public:
	static constexpr int Hash( const tKey& Key, int iBucketMask ) noexcept
	{
		int iHash = HashIntConventional( std::uintptr_t( Key ) );
		if ( iBucketCount <= std::numeric_limits< std::uint16_t >::max( ) )
		{
			iHash ^= ( iHash >> 16 );
		}

		if ( iBucketCount <= std::numeric_limits< std::uint8_t >::max( ) )
		{
			iHash ^= ( iHash >> 8 );
		}

		return ( iHash & iBucketMask );
	}

	static bool Compare( const tKey& lhs, const tKey& rhs ) noexcept
	{
		return lhs == rhs;
	}
};

template< class tElement, int iBucketCount, class tKey = std::uintptr_t, class tHashFuncs = CUtlTSHashGenericHash< iBucketCount, tKey >, int iAlignment = 0 >
class CUtlTSHash
{
	static constexpr int iBucketMask = iBucketCount - 1;
public:
	static consteval UtlTSHashHandle_t InvalidHandle( ) noexcept
	{
		return static_cast< UtlTSHashHandle_t >( 0 );
	}

	[[nodiscard]] UtlTSHashHandle_t Find( tKey Key ) noexcept
	{
		int iBucket = tHashFuncs::Hash( Key, iBucketCount );
		const HashBucket_t& hashBucket = m_arrBuckets[ iBucket ];
		const UtlTSHashHandle_t hHash = Find( Key, hashBucket.m_pFirst, nullptr );
		return hHash ? hHash : Find( Key, hashBucket.m_pFirstUncommited, hashBucket.m_pFirst );
	}

	[[nodiscard]] int Count( ) const noexcept
	{
		return m_EntryMemory.iPeakAlloc;
	}

	int GetElements( int iFirstElement, int iCount, tElement* pElements ) const noexcept
	{
		int iIndex = 0;
		for ( CUtlMemoryPool::CBlob* pBlob = m_EntryMemory.pFirstBlob; pBlob; pBlob = pBlob->pNextBlob )
		{
			for ( int iListIndex = 0; iListIndex < m_EntryMemory.iBlocksPerBlob; iListIndex++ )
			{
				CUtlMemoryPool::FreeList_t& freeList = pBlob->arrFreeList[ iListIndex ];

				auto Element = reinterpret_cast< tElement >( freeList.pData );
				if ( !Element )
					continue;

				if ( --iFirstElement >= 0 )
					continue;

				pElements[ iIndex++ ] = Element;
				if ( iIndex >= iCount )
					return iIndex;
			}
		}

		return iIndex;
	}

	[[nodiscard]] tElement& Element( UtlTSHashHandle_t hHash ) noexcept
	{
		return reinterpret_cast< HashFixedData_t* >( hHash )->Data;
	}

	[[nodiscard]] const tElement& Element( UtlTSHashHandle_t hHash ) const noexcept
	{
		return reinterpret_cast< HashFixedData_t* >( hHash )->Data;
	}

	[[nodiscard]] tElement& operator[]( UtlTSHashHandle_t hHash ) noexcept
	{
		return reinterpret_cast< HashFixedData_t* >( hHash )->Data;
	}

	[[nodiscard]] const tElement& operator[]( UtlTSHashHandle_t hHash ) const noexcept
	{
		return reinterpret_cast< HashFixedData_t* >( hHash )->Data;
	}

	[[nodiscard]] tKey GetID( UtlTSHashHandle_t hHash ) const noexcept
	{
		return reinterpret_cast< HashFixedData_t* >( hHash )->Key;
	}
private:
	template< typename tData >
	struct HashFixedDataInternal_t
	{
		tKey Key;
		HashFixedDataInternal_t< tData >* pNext;
		tData Data;
	};

	using HashFixedData_t = HashFixedDataInternal_t< tElement >;

	struct HashBucket_t
	{
	private:
		[[maybe_unused]] std::byte m_pad0[ 0x18 ];
	public:
		HashFixedData_t* m_pFirst;
		HashFixedData_t* m_pFirstUncommited;
	};

	[[nodiscard]] UtlTSHashHandle_t Find( tKey Key, HashFixedData_t* pFirstElement, HashFixedData_t* pLastElement ) noexcept
	{
		for ( HashFixedData_t* pElement = pFirstElement; pElement != pLastElement; pElement = pElement->pNext )
		{
			if ( tHashFuncs::Compare( pElement->Key, Key ) )
				return reinterpret_cast< UtlTSHashHandle_t >( pElement );
		}

		return InvalidHandle( );
	}

	CUtlMemoryPool m_EntryMemory;
	HashBucket_t m_arrBuckets[ iBucketCount ];
	bool m_bNeedsCommit;
};

#endif // CS2_CHEAT_CUTLTSHASH_HPP
