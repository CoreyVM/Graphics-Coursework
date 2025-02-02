/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"
#include <vector>

class Camera	{
public:
	Camera(void){
		yaw		= 0.0f;
		pitch	= 0.0f;
	};

	Camera(float pitch, float yaw, Vector3 position){
		this->pitch		= pitch;
		this->yaw		= yaw;
		this->position	= position;
		
		cameraPoints.push_back(new Vector3(4000, 400, 400));
		cameraPoints.push_back(new Vector3(4800, 400, 3600));
		cameraPoints.push_back(new Vector3(280, 400, 5000));
		cameraPoints.push_back(new Vector3(-6155, 7380, 9420));

		cameraIndex = 0;
		removeIndex = false;
		FinishedTrack = false;
		usingTrack = true; // Was true
	}

	~Camera(void){};

	void UpdateCamera(float msec = 10.0f);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();

	//Gets position in world space
	Vector3 GetPosition() const { return position;}
	//Sets position in world space
	void	SetPosition(Vector3 val) { position = val;}

	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw;}
	//Sets yaw, in degrees
	void	SetYaw(float y) {yaw = y;}

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch;}
	//Sets pitch, in degrees
	void	SetPitch(float p) {pitch = p;}

	int cameraIndex;
	void ComparePositions();
	void MoveToPoint();

	void IncreaseYaw(int index);
	
protected:
	float	yaw;
	float	pitch;
	Vector3 position;

	bool usingTrack;
	bool removeIndex;
	bool FinishedTrack;

	vector<Vector3*> cameraPoints;
};