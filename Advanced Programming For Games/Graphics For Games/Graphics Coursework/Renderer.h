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

class Renderer : public OGLRenderer
{
 public:

	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

//	void BuildNodeLists(SceneNode* from);
//	void SortNodeLists();
//	void ClearNodeLists();
//	void DrawNodes();


	void SwapTerrainTex();
	void DrawMesh();
	void DrawSkyBox();
	void DrawHeightMap();

	GLuint cubeMap;

 protected:
	HeightMap* heightMap;
	Camera* camera;
	SceneNode* root;
	
	MD5FileData* hellData;
	MD5Node* hellNode;

//	OBJMesh* dragonMesh;
	void  BuildNodesLists(SceneNode* from);
	void SortNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	void ClearNodeLists();


	Mesh* quad;
	Light* light;



	Shader* standShader;
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Frustum frameFrustum;

	vector<SceneNode*> nodeList; //Vector of scenenode objects
	vector<SceneNode*> transparentNodeList;
};