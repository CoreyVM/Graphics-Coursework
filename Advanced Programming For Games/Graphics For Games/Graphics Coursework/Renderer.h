#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/camera.h"
#include "../../nclgl/HeightMap.h"
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\MD5Node.h"
#include "..\..\nclgl\MD5Mesh.h"
#include "..\..\nclgl\OBJMesh.h"
#include "Frustum.h"
#include "Plane.h"
#include <algorithm>

class Renderer : public OGLRenderer
{
 public:

	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	void DrawMesh();

 protected:
	HeightMap* heightMap;
	Camera* camera;
	SceneNode* root;
	MD5FileData* hellData;
	MD5Node* hellNode;

	Frustum frameFrustum;

	vector<SceneNode*> nodeList; //Vector of scenenode objects
};