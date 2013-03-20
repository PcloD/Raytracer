#ifndef __ESFERA_H__
#define __ESFERA_H__

#include "Primitiva.h"
/*
class Esfera : public Primitiva
{
private:
	Vector3 m_Centro;
	CoordType m_Radio, m_RadioSq, m_RadioInv;

public:
	Esfera(Vector3 centro, CoordType radio) : 
	  Primitiva(Primitiva::TipoPrimitivaEsfera), m_Centro(centro), 
		  m_Radio(radio), m_RadioSq(radio * radio), m_RadioInv(1.0f / radio) { }

protected:
	Esfera(Primitiva::TipoPrimitiva tipoPrimitiva, Vector3 centro, CoordType radio) : 
	  Primitiva(tipoPrimitiva), m_Centro(centro), 
		  m_Radio(radio), m_RadioSq(radio * radio), m_RadioInv(1.0f / radio) { }

public:
	Vector3 GetCentro() { return m_Centro; }
	CoordType GetRadio() { return m_Radio; }

public:
	ResultadoRayo Intersecta(Rayo& rayo, CoordType& distancia);
	Vector3 GetNormal(Vector3& posicion);

	int Intersectan(Rayo (&rayos)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]);
	void GetNormales(Vector3 (&posiciones)[RAYOS_HORIZONTALES], Vector3 (&normales)[RAYOS_HORIZONTALES]);
};
*/
#endif
