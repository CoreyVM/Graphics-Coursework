#include "Tree.h"
Mesh* Tree::cube = NULL;
Mesh* Tree::sphere = NULL;

Tree::Tree()
{
	root = new SceneNode(cube);
	root->SetModelScale(Vector3(30, 130, 30));
	root->SetTransform(Matrix4::Translation(Vector3(2600, 100, 1500)));
	AddChild(root);
	
	leftBranch = new SceneNode(cube);
	leftBranch->SetModelScale(Vector3(80, 10, 10));
	leftBranch->SetTransform(Matrix4::Translation(Vector3(-100, 200, 0)));
	root->AddChild(leftBranch);

	rightBranch = new SceneNode(cube);
	rightBranch->SetModelScale(Vector3(80, 10, 10));
	rightBranch->SetTransform(Matrix4::Translation(Vector3(100, 200, 0)));
	root->AddChild(rightBranch);

	leftBush = new SceneNode(sphere);
	leftBush->SetModelScale(Vector3(60, 60, 60));
	leftBush->SetTransform(Matrix4::Translation(Vector3(-200, 200, 0)));
	root->AddChild(leftBush);
	
	rightBush = new SceneNode(sphere);
	rightBush->SetModelScale(Vector3(60, 60, 60));
	rightBush->SetTransform(Matrix4::Translation(Vector3(200, 200, 0)));
	root->AddChild(rightBush);


	root->SetBoundingRadius(10);
	rightBush->SetBoundingRadius(10);
	leftBush->SetBoundingRadius(10);

	rightBranch->SetBoundingRadius(10);
	leftBranch->SetBoundingRadius(10);
	
}

void Tree::Update(float msec)
{
	SceneNode::Update(msec);

}