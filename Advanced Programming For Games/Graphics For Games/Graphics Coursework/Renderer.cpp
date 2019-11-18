#include "Renderer.h"


// Makes sure to always Update the Scene Node otherwise positions arent correct



Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	Dragon::CreateMesh();
	Tree::CreateTrunk();
	Tree::CreateSphere();
	
	
	heightMap = new HeightMap(TEXTUREDIR"Nmap.raw"); //Must used a normal map with the software GIMP and have a size of 256 * 267
	camera = new Camera(-40, 270, Vector3(-2100, 3300, 2000));
	quad = Mesh::GenerateQuad();
	
	
	currentShader = new Shader(SHADERDIR "SceneVertex.glsl",
		SHADERDIR "SceneFragment.glsl");
	
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f),
		500, (RAW_HEIGHT * HEIGHTMAP_Z / 2)),
		Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2);
	
	standShader = new  Shader(SHADERDIR"SceneVertex.glsl", 
		SHADERDIR"SceneFragment.glsl");

	hellData = new MD5FileData(MESHDIR "hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR "idle2.md5anim");
	hellNode->PlayAnim(MESHDIR "idle2.md5anim");

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl",
		SHADERDIR"reflectFragment.glsl");

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",
		SHADERDIR"skyboxFragment.glsl");

	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl",
		SHADERDIR"PerPixelFragment.glsl");

	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() || !skyboxShader->LinkProgram())
	{
		return;
	}

		quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);
	currentShader = new Shader(SHADERDIR "TexturedVertex.glsl", SHADERDIR "TexturedFragment.glsl");

	if (!currentShader->LinkProgram())
	{
		return;
	}

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	modelMatrix = Matrix4::Translation(Vector3(100, 0, 100));
	if (!heightMap->GetTexture())
	{
		return;
	}
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(quad->GetTexture(), true);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	
	root = new SceneNode();
//	root->AddChild(new Dragon());
//	root->AddChild(new EyeTower());
	root->AddChild(new Tree());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	waterRotate = 0;
	init = true;
}

Renderer ::~Renderer(void)
{
	delete heightMap;
	delete camera;
	delete root;
	delete skyboxShader;
	delete reflectShader;
	delete lightShader;
	delete standShader;
	delete quad;
	delete light;
	delete hellNode;
	delete hellData;

	Dragon::DeleteMesh();
	Tree::DeleteMeshes();
	currentShader = 0;

}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	hellNode->Update(msec);
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(msec);
	waterRotate += msec / 1000;
}

void Renderer::BuildNodesLists(SceneNode* from)
{
	if (frameFrustum.InsideFrustum(*from))
	{
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f)
		{
			transparentNodeList.push_back(from);
		}
		else
		{
			nodeList.push_back(from);
		}
	}

	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodesLists((*i));
	}
	
}

void Renderer::SortNodeLists()
{
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes()
{
	for (vector<SceneNode*>::const_iterator i = nodeList.begin();
		i != nodeList.end(); ++i)
	{
		DrawNode(*i);
	}
	for (vector < SceneNode* >::const_reverse_iterator i =
		transparentNodeList.rbegin();
		i != transparentNodeList.rend(); ++i)
	{
		DrawNode((*i));
	}
}
void Renderer::RenderScene() {
	BuildNodesLists(root);
	SortNodeLists();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(currentShader->GetProgram());
	//UpdateShaderMatrices();

	DrawSkyBox();
	DrawWater();
	DrawHeightMap();
	DrawMesh();
	DrawNodes();
	glUseProgram(0);
	SwapBuffers();
	ClearNodeLists();
}

void Renderer::DrawHeightMap()
{
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 1, (float*)&camera->GetPosition());

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1, (float*)&camera->GetPosition());

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	heightMap->Draw();
	glUseProgram(0);
}

void Renderer::DrawMesh()
{
	modelMatrix.ToIdentity();
	SetCurrentShader(currentShader);
	UpdateShaderMatrices();
	                                  //Fwd/Bwd   //Height  //Left and Right
	modelMatrix = Matrix4::Translation(Vector3(1600, 500, 2500));
	UpdateShaderMatrices();
	hellNode->Draw(*this);
}


void Renderer::DrawNode(SceneNode* n)
{
	
	if (n->GetMesh())
	{
		SetCurrentShader(currentShader);
		SetShaderLight(*light);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) & (n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale())));

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());

		n->Draw(*this);
	}

}

void Renderer::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::DrawSkyBox()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	
}

void Renderer::DrawWater()
{
	SetCurrentShader(lightShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2);

	float heightY = 256 * HEIGHTMAP_Y / 3;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1, 0, 0));

	textureMatrix = Matrix4::Scale(Vector3(10, 10, 10)) *
		Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	UpdateShaderMatrices();
	quad->Draw();
	glUseProgram(0);
}
void Renderer::SwapTerrainTex()
{
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"doge.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetTextureRepeating(heightMap->GetTexture(), true);
}