#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float msec) {

	ComparePositions();


	//Update the mouse by how much
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	msec *= 5.0f;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * msec;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += msec;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= msec;
	}
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() {
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};

void Camera::ComparePositions()
{
	if (cameraIndex < cameraPoints.size())
	{
		if (position != *cameraPoints[cameraIndex])
		{
			removeIndex = false;
			MoveToPoint();
		}
		else if (position == *cameraPoints[cameraIndex])
		{
			cameraIndex++;
		
		}
	}
}

void Camera::MoveToPoint()
{
	if (cameraPoints[0] != NULL && !FinishedTrack)
	{
		Vector3 currentPoint = *cameraPoints[cameraIndex];
		if (position.x < cameraPoints[cameraIndex]->x)
		{
			position.x += 5;
		}
		else if (position.x > cameraPoints[cameraIndex]->x)
		{
			position.x -= 5;
		}

		if (position.y > cameraPoints[cameraIndex]->y)
		{
			position.y -= 5;
		}

		else	if (position.y < cameraPoints[cameraIndex]->y)
		{
			position.y += 5;
		}

		if (position.z > cameraPoints[cameraIndex]->z)
		{
			position.z -= 5;
		}
		else	if (position.z < cameraPoints[cameraIndex]->z)
		{
			position.z += 5;
		}
	}

}
