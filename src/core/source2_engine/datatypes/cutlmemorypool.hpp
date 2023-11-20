#ifndef CS2_CHEAT_CUTLMEMORYPOOLBASE_HPP
#define CS2_CHEAT_CUTLMEMORYPOOLBASE_HPP

#include <cstdint>
#include <cstddef>

class CUtlMemoryPool
{
public:
	struct FreeList_t
	{
		FreeList_t* pNext = nullptr;
		FreeList_t* pPrev = nullptr;
		void* pData = nullptr;
	};

	class CBlob
	{
	public:
		CBlob* pNextBlob = nullptr;
		std::int32_t iNumBytes = 0;
		FreeList_t arrFreeList[ 1 ] = { }; // @note / xnxkzeu: it's actually iBlocksPerBlob-sized array.
	};

	int iBlockSize = 0;
	int iBlocksPerBlob = 0;

	int iGrowMode = 0;

	int iBlocksAllocated = 0;
	int iPeakAlloc = 0;

	std::uint16_t uAlignment = 0;
	std::uint16_t uNumBlobs = 0;

	// @note / xnxkzeu: not sure what next 4 fields are named and their types, but they contain all the info.
	FreeList_t* pHeadOfFreeList = nullptr;
	FreeList_t* pFreeList = nullptr;
	CBlob* pLastBlob;
	CBlob* pFirstBlob;

	int iMemAllocAttribute;

	std::uint8_t uUnknown;
};

#endif // CS2_CHEAT_CUTLMEMORYPOOLBASE_HPP
