#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\OBJMesh.h"


class EyeTower : public SceneNode
{
public:
	EyeTower();
	~EyeTower(void) {
		delete root;
		delete eye;
	};

	void CreateTowerBase()
	{
		OBJMesh* tower = new OBJMesh();
		tower->LoadOBJMesh(MESHDIR"cube.obj");  
		tower->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"doge.jpg",
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		towerBase = tower;
	}

	void CreateEyeBall()
	{
		OBJMesh* _eye = new OBJMesh();
		_eye->LoadOBJMesh(MESHDIR"DragonEye.obj"); //DragonEyeTexture.png
		_eye->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"DragonEyeTexture.png",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		eyeMesh = _eye;
	}

	void Update(float msec);
protected:
	SceneNode* root;
	SceneNode* eye;
	 Mesh* towerBase;
	 Mesh* eyeMesh;
};

