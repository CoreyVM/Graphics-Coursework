#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/camera.h"
#include "../../nclgl/HeightMap.h"
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\MD5Node.h"
#include "..\..\nclgl\MD5Mesh.h"
#include "..\..\nclgl\OBJMesh.h"
#include "..\..\nclgl\Light.h"
#include "Frustum.h"
#include "Plane.h"
#include "Dragon.h"
#include "EyeTower.h"
#include "Tree.h"
#include <algorithm>
#include "Pyramid.h"

  //was 2048
#define SHADOWSIZE 4096

class Renderer : public OGLRenderer
{
 public:

	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);
	void SwapTerrainTex();
	

	GLuint cubeMap;

 protected:
	HeightMap* heightMap;
	Camera* camera;
	SceneNode* root;
	
	MD5FileData* hellData;
	MD5Node* hellNode;
	void  BuildNodesLists(SceneNode* from);
	void SortNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void ClearNodeLists();
	void SetSpawnIndex();
	void DrawShadowScene();
	void DrawCombinedScene();
	void DrawWater();
	void DrawMesh();
	void DrawSkyBox();
	void DrawHeightMap();

	void SwapAnimation();

	Mesh* quad;
	Light* light;
	float waterRotate;

	GLuint shadowTex;
	GLuint shadowFBO;

	Shader* sceneShader;
	Shader* shadowShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* bumpShader;

	Frustum frameFrustum;
	int hellNodeIndex;
	int cameraTimer;
	vector<SceneNode*> nodeList; //Vector of scenenode objects
	vector<SceneNode*> transparentNodeList;
};

// 1  //  2

// 0     // 4