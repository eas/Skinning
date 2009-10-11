// skinning.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "skinning.h"
#include "internals.h"

#include "graphics.h"
#include "colors.h"
#include "matrices.h"

#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

const LPCTSTR ShaderFileName = L"shader.vsh";
const float FrontClippingPlane = 0.5f;
const float BackClippingPlane = 1.0e13f;
const float SphereRadius = 10.0f;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void Render(D3D::GraphicDevice& device)
{
	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Colors::Gray, 1.0f, 0 );
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPTSTR,
                     int nCmdShow)
{
	Window mainWindow(hInstance, nCmdShow, &WndProc);

	D3DPRESENT_PARAMETERS params;
		ZeroMemory( &params, sizeof( params ) );

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat = D3DFMT_UNKNOWN;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;

	D3D::GraphicDevice graphicDevice( mainWindow.GetHWND(), params );
	graphicDevice.SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	D3D::VertexDeclaration vertexDeclaration(graphicDevice);
	vertexDeclaration.Use();

	D3D::Shader shader(graphicDevice, ShaderFileName);
	shader.Use();

	SpectatorCoords spectatorCoords( 20.0f, D3DX_PI / 2, -D3DX_PI / 2 );

	shader.SetWorldMatrix( UnityMatrix() );
	shader.SetProjectiveMatrix( ProjectiveMatrix(FrontClippingPlane, BackClippingPlane) );
	shader.SetViewMatrix(ViewMatrix(	spectatorCoords.GetCartesianCoords(),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f),
										D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));


	MSG msg;

	ZeroMemory(&msg, sizeof(msg));
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
		{
			Render(graphicDevice);
		}
    }

	_CrtDumpMemoryLeaks();

	return (int) msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		{
			D3D::Shader* pShader = NULL;
			SpectatorCoords* pSpectatorCoords = NULL;
			pSpectatorCoords = reinterpret_cast<SpectatorCoords*>(GetWindowLong(hWnd, 0));
			pShader = reinterpret_cast<D3D::Shader*>(GetWindowLong(hWnd, sizeof(LONG)));

			switch(wParam)
			{
			case VK_UP:
				pSpectatorCoords->DecTheta();
				break;
			case VK_DOWN:
				pSpectatorCoords->IncTheta();
				break;
			case VK_RIGHT:
				pSpectatorCoords->IncPhi();
				break;
			case VK_LEFT:
				pSpectatorCoords->DecPhi();
				break;
			case VK_NEXT:
			case 'S':
				pSpectatorCoords->IncR();
				break;
			case VK_PRIOR:
			case 'W':
				pSpectatorCoords->DecR();
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			pShader->SetViewMatrix(ViewMatrix(	pSpectatorCoords->GetCartesianCoords(),
												D3DXVECTOR3(0.0f, 0.0f, 0.0f),
												D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}