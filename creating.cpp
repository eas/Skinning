#include "stdafx.h"

#include "creating.h"


#include "graphics.h"
#include "colors.h"

#include <vector>
#include <assert.h>
#include <algorithm>
#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

using namespace D3D;

Vertex SumWithWeight( const Vertex &v1, const Vertex &v2, float weight )
{
	DWORD r, g, b, a;
	DWORD color1 = v1.color;
	DWORD color2 = v2.color;
	a = static_cast<DWORD>( (color1 & 0xff000000)/0x1000000 * weight +
		(color2 & 0xff000000)/0x1000000 * (1-weight) );
	r = static_cast<DWORD>( (color1 & 0x00ff0000)/0x10000 * weight +
		(color2 & 0x00ff0000)/0x10000 * (1-weight) );
	g = static_cast<DWORD>( (color1 & 0x0000ff00)/0x100 * weight +
		(color2 & 0x0000ff00)/0x100 * (1-weight) );
	b = static_cast<DWORD>( (color1 & 0x000000ff) * weight +
		(color2 & 0x000000ff) * (1-weight) );

	return Vertex(	(v1.x*weight+v2.x*(1-weight)),
					(v1.y*weight+v2.y*(1-weight)),
					(v1.z*weight+v2.z*(1-weight)),
					D3DCOLOR_ARGB(a, r, g, b) );
}

void InitVertices(	Vertices& vertices, Indices& indices, 
					unsigned nPointsPerCircle, unsigned nPointsPerGeneratrix,
					float height, float R )
{
	const float angleStep = 2*D3DX_PI / nPointsPerCircle;
	const float heightStep = height / (nPointsPerGeneratrix-1);

	float h = 0;
	for( unsigned i = 0; i<nPointsPerGeneratrix; ++i )
	{
		float angle = 0;
		for( unsigned j = 0; j<nPointsPerCircle; ++j )
		{
			vertices.push_back( Vertex(	R*cosf(angle),
										h,
										R*sinf(angle),
										Colors::Random() ) );
			angle += angleStep;
		}
		h+=heightStep;
	}
	for( unsigned level = 0; level < nPointsPerGeneratrix-1; ++level )
	{
		for( unsigned number = 0; number < nPointsPerCircle; ++number )
		{
			indices.push_back( level*nPointsPerCircle + number);
			indices.push_back( (level+1)*nPointsPerCircle + number);
			indices.push_back( (level+1)*nPointsPerCircle + (number+1)%nPointsPerCircle);

			indices.push_back( level*nPointsPerCircle + number);
			indices.push_back( (level+1)*nPointsPerCircle + (number+1)%nPointsPerCircle);
			indices.push_back( level*nPointsPerCircle + (number+1)%nPointsPerCircle);
		}
	}
}