#include "EyeTower.h"

EyeTower::EyeTower()
{
	CreateTowerBase();
	CreateEyeBall();

	root = new SceneNode(towerBase);
	root->SetModelScale(Vector3(100, 400, 100));
	root->SetTransform(Matrix4::Translation(Vector3(2600, 100, 1500)));
	AddChild(root);
	
	eye = new SceneNode(eyeMesh);
	eye->SetModelScale(Vector3(100, 100, 100));
	eye->SetTransform(Matrix4::Translation(Vector3(0, 800, 0)));
	root->AddChild(eye);

	root->SetBoundingRadius(300);
	eye->SetBoundingRadius(150);
}

void EyeTower::Update(float msec)
{
	SceneNode::Update(msec);
}