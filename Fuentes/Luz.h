#ifndef __LUZ_H__
#define __LUZ_H__

#include "Esfera.h"

class Luz
{
private:
	Vector3 m_Posicion;
	Material m_Material;

public:
	Luz(Vector3 posicion) : m_Posicion(posicion) { }

	Material GetMaterial() const { return m_Material; }
	void SetMaterial(Material mat) { m_Material = mat; }

	Vector3 GetPosicion() const { return m_Posicion; }
	void SetPosition(Vector3 posicion) { m_Posicion = posicion; }
};

#endif
