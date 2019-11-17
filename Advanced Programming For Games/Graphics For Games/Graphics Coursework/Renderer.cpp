#include "Renderer.h"


// Makes sure to always Update the Scene Node otherwise positions arent correct



Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	Dragon::CreateMesh();
	
	heightMap = new HeightMap(TEXTUREDIR"Nmap.raw"); //Must used a normal map with the software GIMP and have a size of 256 * 267
	camera = new Camera(-40, 270, Vector3(-2100, 3300, 2000));
	quad = Mesh::GenerateQuad();
	
	
	//dragonMesh = new OBJMesh();
//	dragonMesh->LoadOBJMesh(MESHDIR"Dragon.obj");
	
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

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);

	
	root = new SceneNode();
	root->AddChild(new Dragon());
	root->AddChild(new EyeTower());


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init = true;
}

Renderer ::~Renderer(void)
{
	delete heightMap;
	delete camera;
	//delete root;
	delete skyboxShader;
	delete reflectShader;
	delete lightShader;
	delete standShader;
	delete quad;
	delete light;
	Dragon::DeleteMesh();
	currentShader = 0;
	//delete hellNode;
//	delete hellData;
}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	hellNode->Update(msec);
	root->Update(msec);
//	waterRotate += msec / 1000; Makes the water move in real time
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	DrawSkyBox();
	DrawHeightMap();
	DrawMesh();
	DrawNode(root);

//	DrawWater();
	glUseProgram(0);
	SwapBuffers();

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

//	modelMatrix = Matrix4::Translation(Vector3(1600, 500, 2500));
//	UpdateShaderMatrices();
//	DrawNode(root);
}


void Renderer::DrawNode(SceneNode* n)
{
//	modelMatrix.ToIdentity();
	SetCurrentShader(currentShader);
//	modelMatrix = Matrix4::Translation(Vector3(1800, 500, 2500));
//	UpdateShaderMatrices();
	if (n->GetMesh())
	{
		Matrix4 transform = n->GetWorldTransform() *  //Was  		Matrix4 transform =modelMatrix *
			Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(
			glGetUniformLocation(currentShader->GetProgram(),
				"modelMatrix"), 1, false, (float*)&transform);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float*)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTexture());
		n->Draw(*this);

	}

	for (vector < SceneNode* >::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i)
	{
	
		DrawNode(*i);
	}
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

void Renderer::SwapTerrainTex()
{
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"doge.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetTextureRepeating(heightMap->GetTexture(), true);
}