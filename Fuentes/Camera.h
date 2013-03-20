#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Types.h"

class Camera
{
private:
	//Variables usadas en modo free
	Vector3 m_Position;
	Vector3 m_Rotation;

public:
	Camera();

	void Move(Vector3 direction);
	void RotateX(CoordType angle);
	void RotateY(CoordType angle);
	void RotateZ(CoordType angle);
	void RotateXYZ(Vector3 angles );

	void MoveForwards(CoordType distance);
	void StrafeRight(CoordType distance);
	void MoveUp(CoordType distance);

	Vector3 GetPosition() { return m_Position; }
	void SetPosition(Vector3 v) { m_Position = v; }

	Vector3 GetRotation() { return m_Rotation; }
	void SetRotation(Vector3 v) { m_Rotation = v; }

	Vector3 GetViewDir();
};

#endif

