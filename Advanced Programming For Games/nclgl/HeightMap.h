#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "..\..\nclgl\Mesh.h"

#define RAW_WIDTH 256 //257 Original
#define RAW_HEIGHT 256

#define HEIGHTMAP_X 16
#define HEIGHTMAP_Z 16 //16
#define HEIGHTMAP_Y 1.25
#define HEIGHTMAP_TEX_X 1.0f / 16
#define HEIGHTMAP_TEX_Z 1.0f / 16

class HeightMap : public Mesh
{
public:
	HeightMap(std::string name);
	~HeightMap(void) {};
};