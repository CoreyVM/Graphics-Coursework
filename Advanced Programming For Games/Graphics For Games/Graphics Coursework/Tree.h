#pragma once
#include "..\..\nclgl\OBJMesh.h"
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\Mesh.h"
#

class Tree : public SceneNode
{
public:
	Tree(Vector3 _position, int angle, int trunkSize);
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
		cube->CreateNormals();
		cube->CreateTangents();
		cube->CreateBufferData();

	}
	static void CreateSphere()
	{
		OBJMesh* s = new OBJMesh();
		s->LoadOBJMesh(MESHDIR"othersphere.obj");
		s->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"green.jpg",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		sphere = s;
		sphere->CreateNormals();
		sphere->CreateTangents();
		sphere->CreateBufferData();
	}

	static void DeleteMeshes()
	{
		delete cube;
		delete sphere;
	}

	void CreateSceneBranches();
	void CreateSceneBrushes();
	void SetBoundingRadius();

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
	Mesh newMesh;

};


