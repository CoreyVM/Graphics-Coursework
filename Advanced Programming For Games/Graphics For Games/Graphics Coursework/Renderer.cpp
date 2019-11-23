#include "Renderer.h"


// Makes sure to always Update the Scene Node otherwise positions arent correct



Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{

#pragma region Create Static Meshes
	Dragon::CreateMesh();
	Tree::CreateTrunk();
	Tree::CreateSphere();
	Pyramid::CreatePyramid();
#pragma endregion
	
	heightMap = new HeightMap(TEXTUREDIR"Nmap.raw"); //Must used a normal map with the software GIMP and have a size of 256 * 267
	camera = new Camera(-40, 270, Vector3(-2100, 3300, 2000));
	quad = Mesh::GenerateQuad();
	
#pragma region Shader Initialisation
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 2500, (RAW_HEIGHT * HEIGHTMAP_Z / 2)),
	Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X));
	
	currentShader = new Shader(SHADERDIR "PerPixelLightVertex.glsl", SHADERDIR "PerPixelLightFragment.glsl");
	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	bumpShader = new Shader(SHADERDIR "PerPixelVertex.glsl", SHADERDIR "PerPixelFragment.glsl");
	shadowShader = new Shader(SHADERDIR "shadowVert.glsl", SHADERDIR "shadowFrag.glsl");
	sceneShader = new Shader(SHADERDIR "shadowscenevert.glsl", SHADERDIR "shadowscenefrag.glsl");

	if (!currentShader->LinkProgram() || !sceneShader->LinkProgram() || !shadowShader->LinkProgram() ||  !bumpShader->LinkProgram() || !reflectShader->LinkProgram() || !lightShader->LinkProgram() || !skyboxShader->LinkProgram()) { return; }
#pragma endregion

	hellData = new MD5FileData(MESHDIR "hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR "idle2.md5anim");
	hellNode->PlayAnim(MESHDIR "idle2.md5anim");

	cubeMap = SOIL_load_OGL_cubemap(   //nightwalker-id_rt.png
		TEXTUREDIR"nightwalker-id_ft.png", TEXTUREDIR"nightwalker-id_bk.png",
		TEXTUREDIR"nightwalker-id_up.png", TEXTUREDIR"nightwalker-id_dn.png",
		TEXTUREDIR"nightwalker-id_rt.png", TEXTUREDIR"nightwalker-id_lf.png",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0);

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"WaterSeam.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Sand.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"SandNormal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	modelMatrix = Matrix4::Translation(Vector3(100, 0, 100));

	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(quad->GetTexture(), true);

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	projMatrix = Matrix4::Perspective(1.0f, 30000.0f,
		(float)width / (float)height, 45.0f);
	
	
	root = new SceneNode();
//	root->AddChild(new Dragon());
//	root->AddChild(new EyeTower());
//	root->AddChild(new Tree());
	root->AddChild(new Pyramid());
	root->AddChild(new Pyramid(Vector3(-4000, -100, 2100), Vector3(1500, 1500, 1500), 10000));
	for (int i = 1; i < 4; ++i)
	{
		for (int x = 1; x < 6; ++x)
		{
			int temp;
			temp = rand();
			
			root->AddChild(new Tree(Vector3(700 * i, rand() % 300,  700 * x),temp, 180));
		}
	}
	root->AddChild(new Pyramid(Vector3(2250, -100, 8000), Vector3(1500, 1500, 1500),10000));
	root->AddChild(new Pyramid(Vector3(2250, -100, -4000), Vector3(1500, 1500, 1500),10000));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	waterRotate = 0;
	init = true;
}

Renderer ::~Renderer(void)
{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete heightMap;
	delete camera;
	delete root;

	delete skyboxShader;
	delete reflectShader;
	delete lightShader;
	delete bumpShader;
	delete shadowShader;
	delete sceneShader;

	delete quad;
	delete light;
	delete hellNode;
	delete hellData;

	Pyramid::DeletePyramid();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawSkyBox();
	DrawWater();
	DrawShadowScene();
	DrawCombinedScene();
	SwapBuffers();

}

void Renderer::DrawShadowScene()
{
	BuildNodesLists(root);
	SortNodeLists();
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
   glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(
		light->GetPosition(), Vector3(0, 0, 0));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();
	DrawHeightMap();
	DrawNodes();
	DrawMesh();

	

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	ClearNodeLists();
}

void Renderer::DrawCombinedScene()
{

	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	DrawHeightMap();
	DrawNodes();
	DrawMesh();

	glUseProgram(0);
	ClearNodeLists();
}
void Renderer::DrawHeightMap()
{
	modelMatrix = Matrix4::Translation(Vector3(0, 0, 0));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);


	UpdateShaderMatrices();
	heightMap->Draw();
}

void Renderer::DrawMesh()
{
//	modelMatrix.ToIdentity();
	modelMatrix = Matrix4::Translation(Vector3(400, 300, 400));
	Matrix4 tempMatrix = textureMatrix * modelMatrix;
	//	glUseProgram(currentShader->GetProgram());  
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) & (hellNode->GetWorldTransform() * Matrix4::Scale(hellNode->GetModelScale())));


	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&hellNode->GetColour());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)hellNode->GetMesh()->GetTexture());
	UpdateShaderMatrices();
	hellNode->Draw(*this);
}


void Renderer::DrawNode(SceneNode* n)
{
//	SetCurrentShader(sceneShader);
//	SetShaderLight(*light);

	
	if (n->GetMesh())
	{
		modelMatrix.ToIdentity();
		Matrix4 tempMatrix = textureMatrix * modelMatrix;
	//	glUseProgram(currentShader->GetProgram());  
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);
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
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X ) * 4;

	float heightY = 256 * HEIGHTMAP_Y / 3;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z * 4);

	modelMatrix =
		Matrix4::Translation(Vector3(0, 80, 0)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1, 0, 0));

	textureMatrix = Matrix4::Scale(Vector3(5, 5, 5)) *
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
