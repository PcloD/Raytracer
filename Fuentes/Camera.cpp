#include "Camera.h"
#include "Quaternion.h"

/***************************************************************************************/

Vector3 ToVector3(Vector4 vec)
{
	return Vector3(vec.x, vec.y, vec.z);
}

Matrix33 ToMatrix33(Matrix44 mat)
{
	 return Matrix33(ToVector3(mat[0]), ToVector3(mat[1]), ToVector3(mat[2]));
}

Camera::Camera()
{
	m_Position.set(0.0f, 0.0f, 0.0f);
	m_Rotation.set(0.0f, 0.0f, 0.0f);
}

Vector3 Camera::GetViewDir()
{
	Vector3 Step1, Step2;
	//Rotate around Y-axis:
	Step1.x = cos( (m_Rotation.y + 90.0f) * PIRADIANES);
	Step1.z = -sin( (m_Rotation.y + 90.0f) * PIRADIANES);
	//Rotate around X-axis:
	CoordType cosX = cos (m_Rotation.x * PIRADIANES);
	Step2.x = Step1.x * cosX;
	Step2.z = Step1.z * cosX;
	Step2.y = sin(m_Rotation.x * PIRADIANES);
	//Rotation around Z-axis not yet implemented, so:

	return(Step2);
}
void Camera::Move (Vector3 direction)
{
	m_Position += direction;
}

void Camera::RotateY (CoordType angle)
{
	m_Rotation.y += angle;
}

void Camera::RotateX (CoordType angle)
{
	m_Rotation.x += angle;
}

void Camera::RotateZ (CoordType angle)
{
	m_Rotation.z += angle;
}

void Camera::MoveForwards( CoordType distance )
{
	Vector3 dir = GetViewDir();

	dir *= distance;

	m_Position += dir;
}

void Camera::StrafeRight ( CoordType distance )
{
	Vector3 dir = GetViewDir();

	dir.y = 0.0f;
	dir = Normalized(dir);

	Vector3 MoveVector;

	MoveVector.z = -dir.x * -distance;
	MoveVector.y = 0.0;
	MoveVector.x = dir.z * -distance;

	m_Position += MoveVector;
}

void Camera::MoveUp(CoordType distance)
{
	m_Position += Vector3(0.0f, distance, 0.0f);
}
