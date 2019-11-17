#include "Dragon.h"

Mesh* Dragon::mesh = NULL;

Dragon::Dragon(void)
{
	body = new SceneNode(mesh);
    body->SetModelScale(Vector3(10, 10, 10));
	
	body->SetTransform(Matrix4::Translation(Vector3(2000, 500, 2500)));
	AddChild(body);

	body->SetBoundingRadius(100);
}