#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\OBJMesh.h"

class Pyramid : public SceneNode
{
public:
	Pyramid(Vector3 _position, Vector3 _scale, int boundingRadius);
	Pyramid();
	~Pyramid();

	static void CreatePyramid()
	{
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"pyramid.obj");
		m->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"PyramidTexture.png", 
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS));
		
		mesh = m;
		mesh = m;
		mesh->CreateNormals();
		mesh->CreateBufferData();
	}

	static void DeletePyramid()
	{
		delete mesh;
	}

	void Update(float msec);

protected:
	SceneNode* root;
	static Mesh* mesh;
	int PreTimer;
	int timer;
};

