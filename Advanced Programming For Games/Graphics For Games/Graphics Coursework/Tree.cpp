#include "Tree.h"
Mesh* Tree::cube = NULL;
Mesh* Tree::sphere = NULL;

Tree::Tree()
{
	
	root = new SceneNode(cube);
	root->SetModelScale(Vector3(30, 130, 30));  //    root->SetModelScale(Vector3(30, 130, 30));
	root->SetTransform(Matrix4::Rotation(45, Vector3(310, 0, 35)));
	root->SetTransform(Matrix4::Translation(Vector3(1700, 300, 2200)));
	AddChild(root);
	CreateSceneBranches();
	CreateSceneBrushes();

	root->SetBoundingRadius(5000);
	

	rightBranch->SetBoundingRadius(5000);
	leftBranch->SetBoundingRadius(5000);


	rightBush->SetBoundingRadius(5000);
	leftBush->SetBoundingRadius(5000);
	topBush->SetBoundingRadius(5000);
	topLeftBush->SetBoundingRadius(5000);
	topRightBush->SetBoundingRadius(5000);

	frontBush1->SetBoundingRadius(5000);
	frontBush2->SetBoundingRadius(5000);
	frontBush3->SetBoundingRadius(5000);
	finaBush->SetBoundingRadius(5000);

	backBush1->SetBoundingRadius(5000);
	backBush2->SetBoundingRadius(5000);
	backBush3->SetBoundingRadius(5000);
	
	
}

void Tree::CreateSceneBranches()
{
	leftBranch = new SceneNode(cube);
	leftBranch->SetModelScale(Vector3(80, 10, 10));
	leftBranch->SetTransform(Matrix4::Translation(Vector3(-100, 200, 0)));
	root->AddChild(leftBranch);

	rightBranch = new SceneNode(cube);
	rightBranch->SetModelScale(Vector3(80, 10, 10));
	rightBranch->SetTransform(Matrix4::Translation(Vector3(100, 200, 0)));
	root->AddChild(rightBranch);
}

void Tree::CreateSceneBrushes()
{
#pragma region Creating Side Brushes
	leftBush = new SceneNode(sphere);
	leftBush->SetModelScale(Vector3(60, 60, 60));
	leftBush->SetTransform(Matrix4::Translation(Vector3(-180, 200, 0)));
	root->AddChild(leftBush);

	rightBush = new SceneNode(sphere);
	rightBush->SetModelScale(Vector3(60, 60, 60));
	rightBush->SetTransform(Matrix4::Translation(Vector3(180, 200, 0)));
	root->AddChild(rightBush);

	topBush = new SceneNode(sphere);
	topBush->SetModelScale(Vector3(60, 60, 60));
	topBush->SetTransform(Matrix4::Translation(Vector3(0, 280, 0)));
	root->AddChild(topBush);

	topLeftBush = new SceneNode(sphere);
	topLeftBush->SetModelScale(Vector3(60, 60, 60));
	topLeftBush->SetTransform(Matrix4::Translation(Vector3(-100, 280, 0)));

	topRightBush = new SceneNode(sphere);
	topRightBush->SetModelScale(Vector3(60, 60, 60));
	topRightBush->SetTransform(Matrix4::Translation(Vector3(100, 280, 0)));
	root->AddChild(topRightBush);
	root->AddChild(topLeftBush);
#pragma endregion 

#pragma region Creating The Front Brushes
	frontBush1 = new SceneNode(sphere);
	frontBush1->SetModelScale(Vector3(60, 60, 60));
	frontBush1->SetTransform(Matrix4::Translation(Vector3(0, 200, 40)));
	root->AddChild(frontBush1);

	frontBush2 = new SceneNode(sphere);
	frontBush2->SetModelScale(Vector3(60, 60, 60));
	frontBush2->SetTransform(Matrix4::Translation(Vector3(-90, 200, 40)));
	root->AddChild(frontBush2);

	frontBush3 = new SceneNode(sphere);
	frontBush3->SetModelScale(Vector3(60, 60, 60));
	frontBush3->SetTransform(Matrix4::Translation(Vector3(90, 200, 40)));
	root->AddChild(frontBush3);

	finaBush = new SceneNode(sphere);
	finaBush->SetModelScale(Vector3(60, 60, 60));
	finaBush->SetTransform(Matrix4::Translation(Vector3(0, 340, 0)));
	root->AddChild(finaBush);

#pragma endregion 

#pragma region Creating The Back Brushes
	backBush1 = new SceneNode(sphere);
	backBush1->SetModelScale(Vector3(60, 60, 60));
	backBush1->SetTransform(Matrix4::Translation(Vector3(0, 200, -40)));
	root->AddChild(backBush1);

	backBush2 = new SceneNode(sphere);
	backBush2->SetModelScale(Vector3(60, 60, 60));
	backBush2->SetTransform(Matrix4::Translation(Vector3(-90, 200, -40)));
	root->AddChild(backBush2);

	backBush3 = new SceneNode(sphere);
	backBush3->SetModelScale(Vector3(60, 60, 60));
	backBush3->SetTransform(Matrix4::Translation(Vector3(90, 200, -40)));
	root->AddChild(backBush3);
#pragma endregion
}
void Tree::Update(float msec)
{
	SceneNode::Update(msec);

}

