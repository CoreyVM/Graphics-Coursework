#pragma once

 #include "Vector4.h"
 #include "Vector3.h"

class Light
{
public:

	Light() {};
	Light(Vector3 _position, Vector4 _colour, float _radius)
	{
		this-> position = _position;
		this-> colour   = _colour;
		this-> radius   = _radius;
	}

	~Light(void) {};

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 val) { colour = val; }


protected:
	Vector3 position;
	Vector4 colour;
	float radius;
};
