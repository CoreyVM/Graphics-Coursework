#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\OBJMesh.h"

class Dragon : public SceneNode
{
public:
	Dragon(void);
	~Dragon(){};

	static void CreateMesh()
	{
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"Dragon.obj");
		m->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"DragonTexture1.png", 
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		mesh = m;
	}
	static void DeleteMesh()
	{
		delete mesh;
	}

protected:
	static Mesh* mesh;
	SceneNode* body;
};

