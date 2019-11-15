#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	heightMap = new HeightMap(TEXTUREDIR"Nmap.raw"); //Must used a normal map with the software GIMP and have a size of 256 * 267
	camera = new Camera(-40, 270, Vector3(-2100, 3300, 2000));


	hellData = new MD5FileData(MESHDIR "hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR "idle2.md5anim");
	hellNode->PlayAnim(MESHDIR "idle2.md5anim");


	currentShader = new Shader(SHADERDIR "TexturedVertex.glsl", SHADERDIR "TexturedFragment.glsl");

	if (!currentShader->LinkProgram())
	{
		return;
	}

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	modelMatrix = Matrix4::Translation(Vector3(100, 0, 100));
	root = new SceneNode();
	


	
	if (!heightMap->GetTexture())
	{
		return;
	}
	SetTextureRepeating(heightMap->GetTexture(), true);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	init = true;
}

Renderer ::~Renderer(void)
{
	delete heightMap;
	delete camera;
	delete root;
	//delete hellNode;
//	delete hellData;
}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	hellNode->Update(msec);
	root->Update(msec);
}

void Renderer::RenderScene() {
	
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	//UpdateShaderMatrices();

	DrawMesh();
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	heightMap->Draw();


	modelMatrix = Matrix4::Translation(Vector3(400, 200, 600));
	UpdateShaderMatrices();
	hellNode->Draw(*this);
	                                     //Fwd/Bwd   //Height  //Left and Right
	modelMatrix = Matrix4::Translation(Vector3(1600, 200, 2500));
	UpdateShaderMatrices();
	hellNode->Draw(*this);

	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}

void Renderer::DrawMesh()
{
	modelMatrix.ToIdentity();
	Matrix4 tempMatrix = textureMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	UpdateShaderMatrices();


	for (int y = 0; y < 2; ++y) { //Used for spawning multiple enemies at an increwaing position
		for (int x = 0; x < 2; ++x) 
		{
		
			
		}
	}
	

}

void Renderer::BuildNodeLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from))
	{
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
		{
		//	transparentNodeList.push_back(from);
		}
		else
		{
			nodeList.push_back(from);
		}
	}

	for (vector < SceneNode* >::const_iterator i = from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists()
{
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes()
{
	for (vector<SceneNode*>::const_iterator i = nodeList.begin();
		i != nodeList.end(); ++i)
	{

		modelMatrix = Matrix4::Translation(Vector3(50, 0, 50));
		DrawNode(*i);
	}
}

void Renderer::DrawNode(SceneNode* n)
{
	
	if (n->GetMesh())
	{
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) & (n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale())));

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());

		n->Draw(*this);
	}
}

void Renderer::ClearNodeLists()
{
	nodeList.clear();
}