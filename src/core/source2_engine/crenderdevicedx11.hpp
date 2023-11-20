#ifndef CS2_CHEAT_CRENDERDEVICEDX11_HPP
#define CS2_CHEAT_CRENDERDEVICEDX11_HPP

#include "../helpers/memory.hpp"

struct ID3D11Device;

class CRenderDeviceDx11
{
public:
	CRenderDeviceDx11( ) = delete;
	CRenderDeviceDx11( CRenderDeviceDx11&& ) = delete;
	CRenderDeviceDx11( const CRenderDeviceDx11& ) = delete;

	OFFSET( 0x28E50, GetD3DDevice, ID3D11Device* );
};

#endif // CS2_CHEAT_CRENDERDEVICEDX11_HPP
