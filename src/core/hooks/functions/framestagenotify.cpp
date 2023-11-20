#include "../hooks.hpp"

#include "../../helpers/render.hpp"

#include "../../source2_engine/isource2client.hpp"

#include "../../../features/esp.hpp"

void Core::CHooks::FrameStageNotify( ISource2Client* pSource2Client, int iFrameStage ) noexcept
{
	pfnFrameStageNotify( pSource2Client, iFrameStage );

	if ( iFrameStage == FRAME_RENDER_START )
	{
		Core::pRender->BeginFrame( );

		Features::pESP->Render( );

		Core::pRender->FinishFrame( );
	}
}