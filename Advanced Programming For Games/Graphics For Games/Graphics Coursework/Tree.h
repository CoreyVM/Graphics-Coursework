#pragma once
#include "..\..\nclgl\OBJMesh.h"
#include "..\..\nclgl\SceneNode.h"
#

class Tree : public SceneNode
{
public:

	Tree();
	~Tree(void)
	{
		delete root;
		delete leftBranch;
		delete rightBranch;
		delete leftBush;
		delete rightBush;
	};

	static void CreateTrunk()
	{
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj"); //TreeBark.JPG
		m->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"wood.png", // wood.png
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		cube = m;

	}
	static void CreateSphere()
	{
		OBJMesh* s = new OBJMesh();
		s->LoadOBJMesh(MESHDIR"othersphere.obj");
		s->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"green.jpg",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sphere = s;

	}

	static void DeleteMeshes()
	{
		delete cube;
		delete sphere;
	}

	void CreateSceneBranches();
	void CreateSceneBrushes();
	void Update(float msec);

protected:
	SceneNode* root;

	SceneNode* leftBranch;
	SceneNode* rightBranch;


	SceneNode* leftBush;
	SceneNode* rightBush;
	SceneNode* topBush;
	SceneNode* topLeftBush;
	SceneNode* topRightBush;

	SceneNode* frontBush1;
	SceneNode* frontBush2;
	SceneNode* frontBush3;

	SceneNode* backBush1;
	SceneNode* backBush2;
	SceneNode* backBush3;
	SceneNode* finaBush;

	static Mesh* cube;
	static Mesh* sphere;

};


