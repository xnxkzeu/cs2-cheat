#ifndef CS2_CHEAT_CSKELETONINSTANCE_HPP
#define CS2_CHEAT_CSKELETONINSTANCE_HPP

#include "cgamescenenode.hpp"
#include "cstronghandle.hpp"
#include "ctransform.hpp"

class CModel;

class CModelState
{
public:
	CModelState( ) = delete;
	CModelState( CModelState&& ) = delete;
	CModelState( const CModelState& ) = delete;

	SCHEMA_VARIABLE( "CModelState->m_hModel", GetModel, CStrongHandle< CModel > );

	OFFSET( 0x80, GetBoneTransformation, CTransform* );
};

class CSkeletonInstance
	: public CGameSceneNode
{
public:
	CSkeletonInstance( ) = delete;
	CSkeletonInstance( CSkeletonInstance&& ) = delete;
	CSkeletonInstance( const CSkeletonInstance& ) = delete;

	SCHEMA_VARIABLE( "CSkeletonInstance->m_modelState", GetModelState, CModelState );
};

#endif // CS2_CHEAT_CSKELETONINSTANCE_HPP
