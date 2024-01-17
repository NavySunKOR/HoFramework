#include <dxgi.h>    // DXGIFactory
#include <dxgi1_4.h> // DXGIFactory4

#include "BaseRenderModule.h"
#include "Application.h"

void HBaseRenderModule::Run()
{
	Update();
	Render();
}

void HBaseRenderModule::Update()
{

}

void HBaseRenderModule::Render()
{
    // IA: Input-Assembler stage
	// VS: Vertex Shader
	// PS: Pixel Shader
	// RS: Rasterizer stage
	// OM: Output-Merger stage

}
