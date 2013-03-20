#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

#include "Pantalla.h"
#include "Common.h"
#include "Rayo.h"
#include "Escena.h"
#include "Camera.h"

#define RAYTRACER_ESTADISTICAS

class Primitiva;

class Raytracer
{
public:
	struct Estadisticas
	{
		int cantidadRayos;
		
		int cantidadRayosPrincipales;
		int cantidadRayosSecundarios;
		int cantidadRayosSecundariosSimples;
		int cantidadRayosSecundariosMultiples;

		int cantidadChequeosIntersecciones;
	};

private:
	Escena* m_pEscena;
	Estadisticas m_Estadisticas;

public:
	Raytracer();
	~Raytracer();

private:
	inline void ProcesarInterseccion(const Primitiva* prim, Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia, Color& color, int profundidad);
	inline void ProcesarInterseccion(const Primitiva* prim, Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], Color (&colores)[RAYOS_HORIZONTALES], int profundidad, CoordType (&distancias)[RAYOS_HORIZONTALES]);

public:
	void Dibujar(Escena* pEscena, Pantalla& pantalla, Camera& camara, int desdeY, int hastaY);

	inline Primitiva* TrazarRayo(Vector3& rayoOrigen, Vector3& rayoDireccion, Color& color, int profundidad, CoordType& distancia);
	inline void TrazarRayos(Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], Color (&colores)[RAYOS_HORIZONTALES], int profundidad, CoordType (&distancias)[RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_HORIZONTALES]);

	inline void TrazarRayosPrimarios(KdTree* pRaizOrigen, Vector3 (&rayosOrigen)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Color (&colores)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], int profundidad);

	void ResetearEstadisticas();
	Estadisticas ObtenerEstadisticas() { return m_Estadisticas; }
};


#endif
