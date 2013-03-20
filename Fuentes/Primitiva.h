#ifndef __PRIMITIVA_H__
#define __PRIMITIVA_H__

#include "Common.h"
#include "Material.h"
#include "Rayo.h"

class Primitiva
{
public:
	enum TipoPrimitiva
	{
		TipoPrimitivaPlano,
		TipoPrimitivaEsfera
	};

protected:
	//Propiedades Generales
	TipoPrimitiva m_TipoPrimitiva;
	Material m_Material;

	//Propiedades de Plano
	Plane m_Plano;

	//Propiedades de Esfera y Luz
	Vector3 m_Centro;
	CoordType m_Radio, m_RadioSq, m_RadioInv;

protected:
	Primitiva(TipoPrimitiva tipoPrimitiva) : m_TipoPrimitiva(tipoPrimitiva) { }

public:
	static Primitiva* NuevoPlano(Vector3 normal, CoordType d)
	{
		Primitiva* p = new Primitiva(Primitiva::TipoPrimitivaPlano);
		p->m_Plano.N = normal;
		p->m_Plano.D = d;

		return p;
	}

	static Primitiva* NuevaEsfera(Vector3 centro, CoordType radio)
	{
		Primitiva* p = new Primitiva(Primitiva::TipoPrimitivaEsfera);

		p->m_Centro = centro;
		p->m_Radio = radio;
		p->m_RadioSq = radio * radio;
		p->m_RadioInv = 1.0f / radio;

		return p;
	}

public:
	TipoPrimitiva GetTipoPrimitiva() const { return m_TipoPrimitiva; }

	Material GetMaterial() const { return m_Material; }
	void SetMaterial(Material mat) { m_Material = mat; }

public:
	ResultadoRayo Intersecta(Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia) const;
	Vector3 GetNormal(Vector3& posicion) const;

	//Devolver la cantidad de intersecciones
	int Intersectan(Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]) const;
	int Intersectan(Vector3 &rayoOrigen, Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]) const;
	void GetNormales(Vector3 (&posiciones)[RAYOS_HORIZONTALES], Vector3 (&normales)[RAYOS_HORIZONTALES]) const;

	void GetAABB(CoordType (&minimos)[3], CoordType (&maximos)[3]);

//Metodos especificos de Esfera y Luz
public:
	Vector3 GetCentro() { return m_Centro; }
	CoordType GetRadio() { return m_Radio; }
};

#endif
