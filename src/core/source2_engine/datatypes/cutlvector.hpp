#ifndef CS2_CHEAT_CUTLVECTOR_HPP
#define CS2_CHEAT_CUTLVECTOR_HPP

#include "cutlmemory.hpp"

template< typename tElement, class tAllocator = CUtlMemory< tElement > >
class CUtlVector
{
public:
	explicit CUtlVector( int iGrowSize = 0, int iInitialCapacity = 0 ) noexcept;

	CUtlVector( tElement* pMemory, int iInitialCapacity = 0, int iInitialCount = 0 ) noexcept;
	~CUtlVector( ) noexcept;

	CUtlVector( const CUtlVector< tElement, tAllocator >& utlVector ) = delete;
	CUtlVector< tElement, tAllocator >& operator=( const CUtlVector< tElement, tAllocator >& utlVector ) noexcept;

	tElement& operator[]( int iIndex ) noexcept;
	const tElement& operator[]( int iIndex ) const noexcept;
	tElement& Element( int iIndex ) noexcept;
	const tElement& Element( int iIndex ) const noexcept;
	tElement& Head( ) noexcept;
	const tElement& Head( ) const noexcept;
	tElement& Tail( ) noexcept;
	const tElement& Tail( ) const noexcept;

	tElement* Base( ) noexcept
	{
		return m_Memory.Base( );
	};

	const tElement* Base( ) const noexcept
	{
		return m_Memory.Base( );
	};

	[[nodiscard]] int Count( ) const;

	[[nodiscard]] inline bool IsEmpty( ) const noexcept
	{
		return ( Count( ) == 0 );
	}

	bool IsValidIndex( int iIndex ) const;
	static constexpr int InvalidIndex( ) noexcept;

	int AddToHead( ) noexcept;
	int AddToTail( ) noexcept;
	int InsertBefore( int iIndex ) noexcept;
	int InsertAfter( int iIndex ) noexcept;

	int AddToHead( const tElement& Element ) noexcept;
	int AddToTail( const tElement& Element ) noexcept;
	int InsertBefore( int iIndex, const tElement& Element ) noexcept;
	int InsertAfter( int iIndex, const tElement& Element ) noexcept;

	int AddMultipleToHead( int iNum ) noexcept;
	int AddMultipleToTail( int iNum ) noexcept;
	int AddMultipleToTail( int iNum, const tElement* pElements ) noexcept;
	int InsertMultipleBefore( int iIndex, int iNum ) noexcept;
	int InsertMultipleBefore( int iIndex, int iNum, const tElement* pElements ) noexcept;
	int InsertMultipleAfter( int iIndex, int iNum ) noexcept;

	void SetSize( int iSize ) noexcept;
	void SetCount( int iCount ) noexcept;
	void SetCountNonDestructively( int iCount ) noexcept;

	void CopyArray( const tElement* pElements, int iSize ) noexcept;

	void Swap( CUtlVector< tElement, tAllocator >& utlVector ) noexcept;

	int AddVectorToTail( const CUtlVector< tElement, tAllocator >& utlVector ) noexcept;

	int Find( const tElement& Element ) const noexcept;
	bool HasElement( const tElement& Element ) const noexcept;

	void EnsureCapacity( int iNum ) noexcept;

	void FastRemove( int iIndex ) noexcept;
	void Remove( int iIndex ) noexcept;
	bool FindAndRemove( const tElement& Element ) noexcept;
	bool FindAndFastRemove( const tElement& Element ) noexcept;
	void RemoveMultiple( int iIndex, int iNum ) noexcept;
	void RemoveMultipleFromHead( int iNum ) noexcept;
	void RemoveMultipleFromTail( int iNum ) noexcept;
	void RemoveAll( ) noexcept;

	void Purge( ) noexcept;
	void PurgeAndDeleteElements( ) noexcept;

	void Compact( ) noexcept;

	void SetGrowSize( int iSize ) noexcept
	{
		m_Memory.SetGrowSize( iSize );
	}

	int NumAllocated( ) const noexcept;

	void Reverse( ) noexcept;

protected:

	void GrowVector( int iNum = 1 ) noexcept;

	void ShiftElementsRight( int iIndex, int iNum = 1 ) noexcept;
	void ShiftElementsLeft( int iIndex, int iNum = 1 ) noexcept;

	int m_iSize;
	tAllocator m_Memory;
};

template< typename tElement, class tAllocator >
CUtlVector< tElement, tAllocator >::CUtlVector( int iGrowSize, int iInitialCapacity ) noexcept
	: m_iSize( 0 )
	, m_Memory( iGrowSize, iInitialCapacity )
{
}

template< typename tElement, class tAllocator >
CUtlVector< tElement, tAllocator >::CUtlVector( tElement* pMemory, int iInitialCapacity, int iInitialCount ) noexcept
	: m_iSize( iInitialCount )
	, m_Memory( pMemory, iInitialCapacity )
{
}

template< typename tElement, class tAllocator >
CUtlVector< tElement, tAllocator >::~CUtlVector( ) noexcept
{
	Purge( );
}

template< typename tElement, class tAllocator >
CUtlVector< tElement, tAllocator >& CUtlVector< tElement, tAllocator >::operator=( const CUtlVector< tElement, tAllocator >& utlVector ) noexcept
{
	int iCount = utlVector.Count( );
	SetSize( iCount );
	for ( int iIndex = 0; iIndex < iCount; iIndex++ )
		( *this )[ iIndex ] = utlVector[ iIndex ];

	return *this;
}

template< typename tElement, class tAllocator >
tElement& CUtlVector< tElement, tAllocator >::operator[]( int iIndex ) noexcept
{
	return m_Memory[ iIndex ];
}

template< typename tElement, class tAllocator >
const tElement& CUtlVector< tElement, tAllocator >::operator[]( int iIndex ) const noexcept
{
	return m_Memory[ iIndex ];
}

template< typename tElement, class tAllocator >
tElement& CUtlVector< tElement, tAllocator >::Element( int iIndex ) noexcept
{
	return m_Memory[ iIndex ];
}

template< typename tElement, class tAllocator >
const tElement& CUtlVector< tElement, tAllocator >::Element( int iIndex ) const noexcept
{
	return m_Memory[ iIndex ];
}

template< typename tElement, class tAllocator >
tElement& CUtlVector< tElement, tAllocator >::Head( ) noexcept
{
	return m_Memory[ 0 ];
}

template< typename tElement, class tAllocator >
const tElement& CUtlVector< tElement, tAllocator >::Head( ) const noexcept
{
	return m_Memory[ 0 ];
}

template< typename tElement, class tAllocator >
tElement& CUtlVector< tElement, tAllocator >::Tail( ) noexcept
{
	return m_Memory[ m_iSize - 1 ];
}

template< typename tElement, class tAllocator >
const tElement& CUtlVector< tElement, tAllocator >::Tail( ) const noexcept
{
	return m_Memory[ m_iSize - 1 ];
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::Reverse( ) noexcept
{
	for ( int iIndex = 0; iIndex < m_iSize / 2; iIndex++ )
		std::swap( m_Memory[ iIndex ], m_Memory[ m_iSize - 1 - iIndex ] );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::Count( ) const
{
	return m_iSize;
}

template< typename tElement, class tAllocator >
bool CUtlVector< tElement, tAllocator >::IsValidIndex( int iIndex ) const
{
	return ( iIndex >= 0 ) && ( iIndex < m_iSize );
}

template< typename tElement, class tAllocator >
constexpr int CUtlVector< tElement, tAllocator >::InvalidIndex( ) noexcept
{
	return -1;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::GrowVector( int iNum ) noexcept
{
	if ( m_iSize + iNum > m_Memory.NumAllocated( ) )
		m_Memory.Grow( m_iSize + iNum - m_Memory.NumAllocated( ) );

	m_iSize += iNum;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::EnsureCapacity( int iNum ) noexcept
{
	m_Memory.EnsureCapacity( iNum );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::ShiftElementsRight( int iIndex, int iNum ) noexcept
{
	int iNumToMove = m_iSize - iIndex - iNum;
	if ( iNumToMove > 0 && iNum > 0 )
		std::memmove( &Element( iIndex + iNum ), &Element( iIndex ), iNumToMove * sizeof( tElement ) );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::ShiftElementsLeft( int iIndex, int iNum ) noexcept
{
	int iNumToMove = m_iSize - iIndex - iNum;
	if ( iNumToMove > 0 && iNum > 0 )
		std::memmove( &Element( iIndex ), &Element( iIndex + iNum ), iNumToMove * sizeof( tElement ) );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddToHead( ) noexcept
{
	return InsertBefore( 0 );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddToTail( ) noexcept
{
	return InsertBefore( m_iSize );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertAfter( int iIndex ) noexcept
{
	return InsertBefore( iIndex + 1 );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertBefore( int iIndex ) noexcept
{
	GrowVector( );
	ShiftElementsRight( iIndex );
	Construct( &Element( iIndex ) );
	return iIndex;
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddToHead( const tElement& Element ) noexcept
{
	return InsertBefore( 0, Element );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddToTail( const tElement& Element ) noexcept
{
	return InsertBefore( m_iSize, Element );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertAfter( int iIndex, const tElement& Element ) noexcept
{
	return InsertBefore( iIndex + 1, Element );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertBefore( int iIndex, const tElement& Element ) noexcept
{
	GrowVector( );
	ShiftElementsRight( iIndex );
	CopyConstruct( &this->Element( iIndex ), Element );
	return iIndex;
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddMultipleToHead( int iNum ) noexcept
{
	return InsertMultipleBefore( 0, iNum );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddMultipleToTail( int iNum ) noexcept
{
	return InsertMultipleBefore( m_iSize, iNum );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddMultipleToTail( int iNum, const tElement* pElements ) noexcept
{
	return InsertMultipleBefore( m_iSize, iNum, pElements );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertMultipleAfter( int iIndex, int iNum ) noexcept
{
	return InsertMultipleBefore( iIndex + 1, iNum );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::SetCount( int iCount ) noexcept
{
	RemoveAll( );
	AddMultipleToTail( iCount );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::SetSize( int iSize ) noexcept
{
	SetCount( iSize );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::SetCountNonDestructively( int iCount ) noexcept
{
	int iDelta = iCount - m_iSize;
	if ( iDelta > 0 )
		AddMultipleToTail( iDelta );
	else if ( iDelta < 0 )
		RemoveMultipleFromTail( iDelta );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::CopyArray( const tElement* pElements, int iSize ) noexcept
{
	SetSize( iSize );
	for ( int iIndex = 0; iIndex < iSize; iIndex++ )
		( *this )[ iIndex ] = pElements[ iIndex ];
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::Swap( CUtlVector< tElement, tAllocator >& utlVector ) noexcept
{
	m_Memory.Swap( utlVector.m_Memory );
	std::swap( m_iSize, utlVector.m_iSize );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::AddVectorToTail( const CUtlVector< tElement, tAllocator >& utlVector ) noexcept
{
	int iDestCount = Count( );
	int iSrcCount = utlVector.Count( );
	EnsureCapacity( iDestCount + iSrcCount );
	m_iSize += iSrcCount;
	for ( int nIndex = 0; nIndex < iSrcCount; nIndex++ )
		CopyConstruct( &Element( iDestCount + nIndex ), utlVector[ nIndex ] );

	return iDestCount;
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertMultipleBefore( int iIndex, int iNum ) noexcept
{
	if ( iNum == 0 )
		return iIndex;

	GrowVector( iNum );
	ShiftElementsRight( iIndex, iNum );

	for ( int iNewIndex = 0; iNewIndex < iNum; ++iNewIndex )
		Construct( &Element( iIndex + iNewIndex ) );

	return iIndex;
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::InsertMultipleBefore( int iIndex, int iNum, const tElement* pElements ) noexcept
{
	if ( iNum == 0 )
		return iIndex;

	GrowVector( iNum );
	ShiftElementsRight( iIndex, iNum );

	if ( !pElements )
	{
		for ( int iNewIndex = 0; iNewIndex < iNum; ++iNewIndex )
			Construct( &Element( iIndex + iNewIndex ) );
	}
	else
	{
		for ( int iNewIndex = 0; iNewIndex < iNum; ++iNewIndex )
			CopyConstruct( &Element( iIndex + iNewIndex ), pElements[ iNewIndex ] );
	}

	return iIndex;
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::Find( const tElement& Element ) const noexcept
{
	for ( int iIndex = 0; iIndex < Count( ); ++iIndex )
	{
		if ( this->Element( iIndex ) == Element )
			return iIndex;
	}

	return -1;
}

template< typename tElement, class tAllocator >
bool CUtlVector< tElement, tAllocator >::HasElement( const tElement& Element ) const noexcept
{
	return ( Find( Element ) >= 0 );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::FastRemove( int iIndex ) noexcept
{
	Destruct( &Element( iIndex ) );
	if ( m_iSize > 0 )
	{
		if ( iIndex != m_iSize - 1 )
			std::memcpy( &Element( iIndex ), &Element( iIndex - 1 ), sizeof( tElement ) );

		--m_iSize;
	}
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::Remove( int iIndex ) noexcept
{
	Destruct( &Element( iIndex ) );
	ShiftElementsLeft( iIndex );
	--m_iSize;
}

template< typename tElement, class tAllocator >
bool CUtlVector< tElement, tAllocator >::FindAndRemove( const tElement& Element ) noexcept
{
	int nIndex = Find( Element );
	if ( nIndex != -1 )
	{
		Remove( nIndex );
		return true;
	}
	return false;
}

template< typename tElement, class tAllocator >
bool CUtlVector< tElement, tAllocator >::FindAndFastRemove( const tElement& Element ) noexcept
{
	int nIndex = Find( Element );
	if ( nIndex != -1 )
	{
		FastRemove( nIndex );
		return true;
	}
	return false;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::RemoveMultiple( int iIndex, int iNum ) noexcept
{
	for ( int iElement = iIndex + iNum; --iElement >= iIndex; )
		Destruct( &Element( iElement ) );

	ShiftElementsLeft( iIndex, iNum );
	m_iSize -= iNum;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::RemoveMultipleFromHead( int iNum ) noexcept
{
	for ( int iIndex = iNum; --iIndex >= 0; )
		Destruct( &Element( iIndex ) );

	ShiftElementsLeft( 0, iNum );
	m_iSize -= iNum;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::RemoveMultipleFromTail( int iNum ) noexcept
{
	for ( int iIndex = m_iSize - iNum; iIndex < m_iSize; iIndex++ )
		Destruct( &Element( iIndex ) );

	m_iSize -= iNum;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::RemoveAll( ) noexcept
{
	for ( int iIndex = m_iSize; --iIndex >= 0; )
		Destruct( &Element( iIndex ) );

	m_iSize = 0;
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::Purge( ) noexcept
{
	RemoveAll( );
	m_Memory.Purge( );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::PurgeAndDeleteElements( ) noexcept
{
	for ( int iIndex = 0; iIndex < m_iSize; iIndex++ )
		delete Element( iIndex );

	Purge( );
}

template< typename tElement, class tAllocator >
void CUtlVector< tElement, tAllocator >::Compact( ) noexcept
{
	m_Memory.Purge( m_iSize );
}

template< typename tElement, class tAllocator >
int CUtlVector< tElement, tAllocator >::NumAllocated( ) const noexcept
{
	return m_Memory.NumAllocated( );
}

#endif // CS2_CHEAT_CUTLVECTOR_HPP
