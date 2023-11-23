#ifndef CS2_CHEAT_CMODEL_HPP
#define CS2_CHEAT_CMODEL_HPP

#include "cutlvector.hpp"

class CRenderMesh;
class CPhysModel;

enum EBoneFlags : std::uint32_t
{
	FLAG_NO_BONE_FLAGS = 0,
	FLAG_BONEFLEXDRIVER = ( 1 << 2 ),
	FLAG_CLOTH = ( 1 << 3 ),
	FLAG_PHYSICS = ( 1 << 4 ),
	FLAG_ATTACHMENT = ( 1 << 5 ),
	FLAG_ANIMATION = ( 1 << 6 ),
	FLAG_MESH = ( 1 << 7 ),
	FLAG_HITBOX = ( 1 << 8 ),
	FLAG_BONE_USED_BY_VERTEX_LOD0 = ( 1 << 10 ),
	FLAG_BONE_USED_BY_VERTEX_LOD1 = ( 1 << 11 ),
	FLAG_BONE_USED_BY_VERTEX_LOD2 = ( 1 << 12 ),
	FLAG_BONE_USED_BY_VERTEX_LOD3 = ( 1 << 13 ),
	FLAG_BONE_USED_BY_VERTEX_LOD4 = ( 1 << 14 ),
	FLAG_BONE_USED_BY_VERTEX_LOD5 = ( 1 << 15 ),
	FLAG_BONE_USED_BY_VERTEX_LOD6 = ( 1 << 16 ),
	FLAG_BONE_USED_BY_VERTEX_LOD7 = ( 1 << 17 ),
	FLAG_BONE_MERGE_READ = ( 1 << 18 ),
	FLAG_BONE_MERGE_WRITE = ( 1 << 19 ),

	FLAG_ALL_BONE_FLAGS = 0xFFFFF,
};

class CModelSkeleton
{
public:
	CModelSkeleton( ) = delete;
	CModelSkeleton( CModelSkeleton&& ) = delete;
	CModelSkeleton( const CModelSkeleton& ) = delete;

private:
	[[maybe_unused]] std::byte m_pad0[ 0x38 ];
public:
	CUtlVector< const char* > vecBoneNames;
	CUtlVector< std::uint16_t > vecBoneParent;
private:
	[[maybe_unused]] std::byte m_pad1[ 0x18 ];
public:
	CUtlVector< EBoneFlags > vecBoneFlags;
};

class CModel
{
public:
	CModel( ) = delete;
	CModel( CModel&& ) = delete;
	CModel( const CModel& ) = delete;

private:
	[[maybe_unused]] std::byte m_pad0[ 0x8 ];
public:
	const char* szName;
private:
	[[maybe_unused]] std::byte m_pad1[ 0x4 ];
public:
	int iFlags;
	Math::Vector_t< float, 3 > vecHullMin;
	Math::Vector_t< float, 3 > vecHullMax;
	Math::Vector_t< float, 3 > vecViewMin;
	Math::Vector_t< float, 3 > vecViewMax;
private:
	[[maybe_unused]] std::byte m_pad2[ 0x28 ];
public:
	CUtlVector< CRenderMesh* > vecRenderMeshes;
private:
	[[maybe_unused]] std::byte m_pad3[ 0x48 ];
public:
	CPhysModel* pPhysModel;
private:
	[[maybe_unused]] std::byte m_pad4[ 0x48 ];
public:
	CModelSkeleton modelSkeleton;
};

#endif // CS2_CHEAT_CMODEL_HPP
