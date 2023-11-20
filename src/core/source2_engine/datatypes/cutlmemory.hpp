#ifndef CS2_CHEAT_CUTLMEMORY_HPP
#define CS2_CHEAT_CUTLMEMORY_HPP

#include <utility>

#include "../../../context/memory.hpp"

template< typename tMemory, typename... tArgs >
inline tMemory* Construct( tMemory* pMemory, tArgs... argList )
{
	return ::new ( pMemory ) tMemory( argList... );
}

template< typename tMemory >
inline tMemory* CopyConstruct( tMemory* pMemory, const tMemory& copyMemory )
{
	return ::new ( pMemory ) tMemory( copyMemory );
}

template< typename tMemory >
inline tMemory* MoveConstruct( tMemory* pMemory, tMemory&& moveMemory )
{
	return ::new ( pMemory ) tMemory( std::forward< tMemory >( moveMemory ) );
}

template< typename tMemory >
inline void Destruct( tMemory* pMemory )
{
	pMemory->~tMemory( );
}

template< typename tElement, typename tIndex = int >
class CUtlMemory
{
public:
	CUtlMemory( int iGrowSize = 0, int iInitSize = 0 ) noexcept;
	CUtlMemory( tElement* pMemory, int iNumElements ) noexcept;
	CUtlMemory( const tElement* pMemory, int iNumElements ) noexcept;
	~CUtlMemory( ) noexcept;

	CUtlMemory( const CUtlMemory& ) = delete;
	CUtlMemory& operator=( const CUtlMemory& ) = delete;

	CUtlMemory( CUtlMemory&& utlMemory ) noexcept;
	CUtlMemory& operator=( CUtlMemory&& utlMemory ) noexcept;

	void Init( int iGrowSize, int iInitSize = 0 ) noexcept;

	struct Iterator_t
	{
		constexpr explicit Iterator_t( tIndex nIndex ) noexcept
			: iIndex( nIndex )
		{
		}
		tIndex iIndex;

		[[nodiscard]] constexpr bool operator==( const Iterator_t it ) const noexcept
		{
			return iIndex == it.iIndex;
		}

		[[nodiscard]] constexpr bool operator!=( const Iterator_t it ) const noexcept
		{
			return iIndex != it.iIndex;
		}
	};

	[[nodiscard]] Iterator_t First( ) const noexcept
	{
		return Iterator_t( IsIndexValid( 0 ) ? 0 : InvalidIndex( ) );
	}

	[[nodiscard]] Iterator_t Next( const Iterator_t& it ) const noexcept
	{
		return Iterator_t( IsIndexValid( it.iIndex + 1 ) ? it.iIndex + 1 : InvalidIndex( ) );
	}

	[[nodiscard]] tIndex GetIndex( const Iterator_t& it ) const noexcept
	{
		return it.iIndex;
	}

	[[nodiscard]] constexpr bool IsIndexAfter( tIndex nIndex, const Iterator_t& it ) const noexcept
	{
		return nIndex > it.iIndex;
	}

	[[nodiscard]] bool IsValidIterator( const Iterator_t& it ) const noexcept
	{
		return IsIndexValid( it.iIndex );
	}

	[[nodiscard]] static constexpr Iterator_t InvalidIterator( ) noexcept
	{
		return Iterator_t( InvalidIndex( ) );
	}

	[[nodiscard]] tElement& operator[]( tIndex iIndex ) noexcept;
	[[nodiscard]] const tElement& operator[]( tIndex iIndex ) const noexcept;

	[[nodiscard]] tElement& Element( tIndex iIndex ) noexcept;
	[[nodiscard]] const tElement& Element( tIndex iIndex ) const noexcept;

	[[nodiscard]] bool IsIndexValid( tIndex iIndex ) const noexcept;

	void Swap( CUtlMemory< tElement, tIndex >& utlMemory ) noexcept;

	void ConvertToGrowableMemory( int iGrowSize ) noexcept;

	[[nodiscard]] int NumAllocated( ) const noexcept;
	[[nodiscard]] int Count( ) const noexcept;

	void Grow( int iNum = 1 ) noexcept;

	void EnsureCapacity( int iNum ) noexcept;

	void Purge( ) noexcept;
	void Purge( int iNumElements ) noexcept;

	[[nodiscard]] bool IsExternallyAllocated( ) const noexcept;
	[[nodiscard]] bool IsReadOnly( ) const noexcept;

	void SetGrowSize( int iSize ) noexcept;

	static constexpr tIndex INVALID_INDEX = static_cast< tIndex >( -1 );

	[[nodiscard]] static consteval tIndex InvalidIndex( ) noexcept
	{
		return INVALID_INDEX;
	}

	[[nodiscard]] tElement* Base( ) noexcept;
	[[nodiscard]] const tElement* Base( ) const noexcept;

	void SetExternalBuffer( tElement* pMemory, int iNumElements ) noexcept;
	void SetExternalBuffer( const tElement* pMemory, int iNumElements ) noexcept;
	void AssumeMemory( tElement* pMemory, int iSize ) noexcept;
	[[nodiscard]] tElement* Detach( ) noexcept;
	[[nodiscard]] void* DetachMemory( ) noexcept;
private:
	static constexpr int EXTERNAL_BUFFER_MARKER = -1;
	static constexpr int EXTERNAL_CONST_BUFFER_MARKER = -2;

	tElement* m_pMemory;
	int m_iAllocationCount;
	int m_iGrowSize;
};

template< typename tElement, typename tIndex >
CUtlMemory< tElement, tIndex >::CUtlMemory( int iGrowSize, int iInitSize ) noexcept
	: m_pMemory( nullptr )
	, m_iAllocationCount( iInitSize )
	, m_iGrowSize( iGrowSize )
{
	if ( m_iAllocationCount )
		m_pMemory = static_cast< tElement* >( CTX::pMemory->MemAlloc.AllocFunc( m_iAllocationCount * sizeof( tElement ) ) );
}

template< typename tElement, typename tIndex >
CUtlMemory< tElement, tIndex >::CUtlMemory( tElement* pMemory, int iNumElements ) noexcept
	: m_pMemory( pMemory )
	, m_iAllocationCount( iNumElements )
{
	m_iGrowSize = EXTERNAL_BUFFER_MARKER;
}

template< typename tElement, typename tIndex >
CUtlMemory< tElement, tIndex >::CUtlMemory( const tElement* pMemory, int iNumElements ) noexcept
	: m_pMemory( pMemory )
	, m_iAllocationCount( iNumElements )
{
	m_iGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template< typename tElement, typename tIndex >
CUtlMemory< tElement, tIndex >::~CUtlMemory( ) noexcept
{
	Purge( );
}

template< typename tElement, typename tIndex >
CUtlMemory< tElement, tIndex >::CUtlMemory( CUtlMemory&& utlMemory ) noexcept
	: m_pMemory( utlMemory.m_pMemory )
	, m_iAllocationCount( utlMemory.m_iAllocationCount )
	, m_iGrowSize( utlMemory.m_iGrowSize )
{
	utlMemory.m_pMemory = nullptr;
	utlMemory.m_iAllocationCount = 0;
	utlMemory.m_iGrowSize = 0;
}

template< typename tElement, typename tIndex >
CUtlMemory< tElement, tIndex >& CUtlMemory< tElement, tIndex >::operator=( CUtlMemory&& utlMemory ) noexcept
{
	tElement* pMemory = utlMemory.m_pMemory;
	int iAllocationCount = utlMemory.m_iAllocationCount;
	int iGrowSize = utlMemory.m_iGrowSize;

	utlMemory.m_pMemory = nullptr;
	utlMemory.m_nAllocationCount = 0;
	utlMemory.m_nGrowSize = 0;

	Purge( );

	m_pMemory = pMemory;
	m_iAllocationCount = iAllocationCount;
	m_iGrowSize = iGrowSize;

	return *this;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::Init( int iGrowSize, int iInitSize ) noexcept
{
	Purge( );

	m_iGrowSize = iGrowSize;
	m_iAllocationCount = iInitSize;

	if ( m_iAllocationCount )
		m_pMemory = static_cast< tElement* >( CTX::pMemory->MemAlloc.AllocFunc( m_iAllocationCount * sizeof( tElement ) ) );
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::Swap( CUtlMemory< tElement, tIndex >& utlMemory ) noexcept
{
	std::swap( m_iGrowSize, utlMemory.m_iGrowSize );
	std::swap( m_pMemory, utlMemory.m_pMemory );
	std::swap( m_iAllocationCount, utlMemory.m_iAllocationCount );
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::ConvertToGrowableMemory( int iGrowSize ) noexcept
{
	if ( !IsExternallyAllocated( ) )
		return;

	m_iGrowSize = iGrowSize;
	if ( m_iAllocationCount )
	{
		int iNumBytes = m_iAllocationCount * sizeof( tElement );
		tElement* pMemory = static_cast< tElement* >( CTX::pMemory->MemAlloc.AllocFunc( iNumBytes ) );
		std::memcpy( pMemory, m_pMemory, iNumBytes );
		m_pMemory = pMemory;
	}
	else
		m_pMemory = nullptr;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::SetExternalBuffer( tElement* pMemory, int iNumElements ) noexcept
{
	Purge( );

	m_pMemory = pMemory;
	m_iAllocationCount = iNumElements;

	m_iGrowSize = EXTERNAL_BUFFER_MARKER;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::SetExternalBuffer( const tElement* pMemory, int iNumElements ) noexcept
{
	Purge( );

	m_pMemory = pMemory;
	m_iAllocationCount = iNumElements;

	m_iGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::AssumeMemory( tElement* pMemory, int iSize ) noexcept
{
	Purge( );

	m_pMemory = pMemory;
	m_iAllocationCount = iSize;
}

template< typename tElement, typename tIndex >
void* CUtlMemory< tElement, tIndex >::DetachMemory( ) noexcept
{
	if ( IsExternallyAllocated( ) )
		return nullptr;

	void* pMemory = m_pMemory;
	m_pMemory = nullptr;
	m_iAllocationCount = 0;
	return pMemory;
}

template< typename tElement, typename tIndex >
tElement* CUtlMemory< tElement, tIndex >::Detach( ) noexcept
{
	return static_cast< tElement* >( DetachMemory( ) );
}

template< typename tElement, typename tIndex >
tElement& CUtlMemory< tElement, tIndex >::operator[]( tIndex iIndex ) noexcept
{
	return m_pMemory[ iIndex ];
}

template< typename tElement, typename tIndex >
const tElement& CUtlMemory< tElement, tIndex >::operator[]( tIndex iIndex ) const noexcept
{
	return m_pMemory[ iIndex ];
}

template< typename tElement, typename tIndex >
tElement& CUtlMemory< tElement, tIndex >::Element( tIndex iIndex ) noexcept
{
	return m_pMemory[ iIndex ];
}

template< typename tElement, typename tIndex >
const tElement& CUtlMemory< tElement, tIndex >::Element( tIndex iIndex ) const noexcept
{
	return m_pMemory[ iIndex ];
}

template< typename tElement, typename tIndex >
bool CUtlMemory< tElement, tIndex >::IsExternallyAllocated( ) const noexcept
{
	return ( m_iGrowSize < 0 );
}

template< typename tElement, typename tIndex >
bool CUtlMemory< tElement, tIndex >::IsReadOnly( ) const noexcept
{
	return ( m_iGrowSize == EXTERNAL_CONST_BUFFER_MARKER );
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::SetGrowSize( int iSize ) noexcept
{
	m_iGrowSize = iSize;
}

template< typename tElement, typename tIndex >
tElement* CUtlMemory< tElement, tIndex >::Base( ) noexcept
{
	return m_pMemory;
}

template< typename tElement, typename tIndex >
const tElement* CUtlMemory< tElement, tIndex >::Base( ) const noexcept
{
	return m_pMemory;
}

template< typename tElement, typename tIndex >
int CUtlMemory< tElement, tIndex >::NumAllocated( ) const noexcept
{
	return m_iAllocationCount;
}

template< typename tElement, typename tIndex >
int CUtlMemory< tElement, tIndex >::Count( ) const noexcept
{
	return m_iAllocationCount;
}

template< typename tElement, typename tIndex >
bool CUtlMemory< tElement, tIndex >::IsIndexValid( tIndex iIndex ) const noexcept
{
	return ( iIndex >= 0 ) && ( iIndex < m_iAllocationCount );
}

constexpr int UtlMemory_CalcNewAllocationCount( int iAllocationCount, int iGrowSize, int iNewSize, int iBytesItem ) noexcept
{
	if ( iGrowSize )
		iAllocationCount = ( ( 1 + ( ( iNewSize - 1 ) / iGrowSize ) ) * iGrowSize );
	else
	{
		if ( !iAllocationCount )
		{
			iAllocationCount = ( 31 + iBytesItem ) / iBytesItem;

			if ( iAllocationCount < iNewSize )
				iAllocationCount = iNewSize;
		}

		while ( iAllocationCount < iNewSize )
			iAllocationCount *= 2;
	}

	return iAllocationCount;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::Grow( int iNum ) noexcept
{
	if ( IsExternallyAllocated( ) )
		return;

	int iAllocationRequested = m_iAllocationCount + iNum;
	int iNewAllocationCount = UtlMemory_CalcNewAllocationCount( m_iAllocationCount, m_iGrowSize, iAllocationRequested, sizeof( tElement ) );

	if ( static_cast< int >( static_cast< tIndex >( iNewAllocationCount ) ) < iAllocationRequested )
	{
		if ( static_cast< int >( static_cast< tIndex >( iNewAllocationCount ) ) == 0 && static_cast< int >( static_cast< tIndex >( iNewAllocationCount - 1 ) ) >= iAllocationRequested )
			--iNewAllocationCount; // @note / xnxkzeu: deal with the common case of m_nAllocationCount = MAX_USHORT + 1
		else
		{
			if ( static_cast< int >( static_cast< tIndex >( iAllocationRequested ) != iAllocationRequested ) )
				return; // @note / xnxkzeu: we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
			while ( static_cast< int >( static_cast< tIndex >( iNewAllocationCount ) ) < iAllocationRequested )
				iNewAllocationCount = ( iNewAllocationCount + iAllocationRequested ) / 2;
		}
	}

	m_pMemory = static_cast< tElement* >( CTX::pMemory->MemAlloc.ReallocFunc( m_pMemory, iNewAllocationCount * sizeof( tElement ) ) );
	m_iAllocationCount = iNewAllocationCount;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::EnsureCapacity( int iNum ) noexcept
{
	if ( m_iAllocationCount >= iNum )
		return;

	if ( IsExternallyAllocated( ) )
		return;

	m_pMemory = static_cast< tElement* >( CTX::pMemory->MemAlloc.ReallocFunc( m_pMemory, iNum * sizeof( tElement ) ) );
	m_iAllocationCount = iNum;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::Purge( ) noexcept
{
	if ( IsExternallyAllocated( ) )
		return;

	if ( m_pMemory )
	{
		CTX::pMemory->MemAlloc.FreeFunc( m_pMemory );
		m_pMemory = nullptr;
	}

	m_iAllocationCount = 0;
}

template< typename tElement, typename tIndex >
void CUtlMemory< tElement, tIndex >::Purge( int iNumElements ) noexcept
{
	if ( iNumElements >= m_iAllocationCount )
		return;

	if ( iNumElements == 0 )
		return Purge( );

	if ( IsExternallyAllocated( ) )
		return;

	if ( !m_pMemory )
		return;

	m_pMemory = static_cast< tElement* >( CTX::pMemory->MemAlloc.ReallocFunc( m_pMemory, iNumElements * sizeof( tElement ) ) );
	m_iAllocationCount = iNumElements;
}

#endif
