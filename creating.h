#pragma once
#include "graphics.h"
#include <vector>

typedef std::vector<D3D::Vertex> Vertices;
typedef std::vector<D3D::Index> Indices;

void InitVertices(	Vertices& vertices, Indices& indices, 
					unsigned nPointsPerCircle, unsigned nPointsPerGeneratrix,
					float height, float R );