#include "Pyramid.h"

Mesh* Pyramid::mesh = NULL;

Pyramid::Pyramid(Vector3 _position, Vector3 _scale)
{

	this->root = new SceneNode(mesh);
	this->root->SetModelScale(_scale);
	this->root->SetTransform(Matrix4::Translation(_position) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
	this->root->SetBoundingRadius(500);
	AddChild(root);
	timer = 1;
}

Pyramid::Pyramid()
{
	root = new SceneNode(mesh);  //Forward position  Vector3 (8000, 1300 , 2100) 
	root->SetModelScale(Vector3(1500, 1500, 1500));  //Vector3(8000, 1300, 2100)) original location
	root->SetTransform(Matrix4::Translation(Vector3(8000, -100, 2100)) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
	root->SetBoundingRadius(500);
	AddChild(root);
	timer = 1;
}

Pyramid::~Pyramid()
{
	delete mesh;
}

void Pyramid::Update(float msec)
{
	if (timer < 1600)
	{
		root->SetTransform(Matrix4::Translation(Vector3(root->GetTransform().GetPositionVector().x, root->GetTransform().GetPositionVector().y + 1, root->GetTransform().GetPositionVector().z)) * Matrix4::Rotation(270, Vector3(1, 0, 0)));
		timer += 1;
	}
	

	SceneNode::Update(msec);
}