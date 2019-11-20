#include "Pyramid.h"

Mesh* Pyramid::mesh = NULL;

Pyramid::Pyramid(Vector3 _position, Vector3 _scale, int boundingRadius)
{

	this->root = new SceneNode(mesh);
	this->root->SetModelScale(_scale);
	this->root->SetTransform(Matrix4::Translation(_position) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
	this->root->SetBoundingRadius(boundingRadius);
	AddChild(root);
	timer = 1;
	PreTimer = 0;
}

Pyramid::Pyramid()
{
	root = new SceneNode(mesh);  //Forward position  Vector3 (8000, 1300 , 2100) 
	root->SetModelScale(Vector3(1500, 1500, 1500));  //Vector3(8000, 1300, 2100)) original location
	root->SetTransform(Matrix4::Translation(Vector3(8000, -100, 2100)) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
	root->SetBoundingRadius(500);
	AddChild(root);
	timer = 1;
	PreTimer = 0;
}

Pyramid::~Pyramid()
{
	delete mesh;
}

void Pyramid::Update(float msec)
{

	if (PreTimer < 4010)
	{
		PreTimer += 1;
		std::cout << PreTimer << endl;
	}

	if (timer < 1600 &&  PreTimer >= 4010)
	{
		root->SetTransform(Matrix4::Translation(Vector3(root->GetTransform().GetPositionVector().x, root->GetTransform().GetPositionVector().y + 1, root->GetTransform().GetPositionVector().z)) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
		timer += 1;
	}
	

	SceneNode::Update(msec);
}