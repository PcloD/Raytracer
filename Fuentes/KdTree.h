#ifndef __KDTREE_H__
#define __KDTREE_H__

#include "Types.h"
#include "Primitiva.h"
#include <vector>

//#define KDTREE_ESTADISTICAS

class KdTree
{
public:
	struct Estadisticas
	{
		int cantidadDivisionesUnicas;
		int cantidadDivisionesConjuntas;
		int cantidadDivisionesFallidas;
	};

private:
	//KdTree* m_pPadre;
	KdTree* m_pHijos;

	//Plano de separacion de los hijos
	int m_Axis; //-1: no hay separacion, 0:x, 1:y, 2:z
	float m_Coordenada;

	//Primitivas
	//std::vector<Primitiva*> m_pPrimitivas;

	Primitiva** m_pPrimitivas;
	short m_CantidadPrimitivas;
	short m_MaximoPrimitivas;

	//Nivel
	//int m_Nivel;
	
	//Dimensiones (es un AABB)
	//Pongo estos 2 vectores en punteros para que el vector de KdTrees sea lo mas compacto posible
	CoordType* m_Minimos;
	CoordType* m_Maximos;

private:
	inline static bool ColisionanAABB(CoordType minimosA[3], CoordType maximosA[3],
							CoordType minimosB[3], CoordType maximosB[3]);

	inline static int ColisionAABBPlano(CoordType minimos[3], CoordType maximos[3],
								int axisPlano, float coordenadaPlano);

	inline static int ColisionaRayoPlano(Vector3& rayoOrigen, Vector3& rayoDireccion,
										  int axisPlano, float coordenadaPlano);

	inline static int ColisionaRayoPlano(Vector3& rayoOrigen, Vector3& rayoDireccion,
										  int axisPlano, float coordenadaPlano, float& distancia);

private:
	void Construir(CoordType minimos[3], CoordType maximos[3]);

	void Subdividir(KdTree* &pProximoKdTreeLibre);

public:
	KdTree(KdTree* pPadre = NULL);
	~KdTree();

	void Construir(CoordType dimension);

	void AgregarPrimitiva(Primitiva* primitiva);
	int GetCantidadPrimitivas() { return /*(int) m_pPrimitivas.size();*/ m_CantidadPrimitivas; }
	Primitiva* GetPrimitiva(int n) { return m_pPrimitivas[n]; }

	void Optimizar(KdTree* &pProximoKdTreeLibre, int nivel = 0);

	KdTree* BuscarNodoContenedorPunto(Vector3& punto);

	Primitiva* BuscarInterseccion(Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia, ResultadoRayo& resultadoRayo);

	void BuscarIntersecciones(Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]);

	void BuscarInterseccionesIniciales(Vector3 &rayoOrigen, Vector3 (&rayosDireccion)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_VERTICALES][RAYOS_HORIZONTALES]);
	void BuscarInterseccionesIniciales(Vector3 &rayoOrigen, Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]);

	bool ColisionConAlguienIgnorando(const Primitiva* primitivaAIgnorar, Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia);

	int ColisionConAlguienIgnorando(const Primitiva* primitivaAIgnorar, Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], bool (&colisiones)[RAYOS_HORIZONTALES]);
};

#ifdef KDTREE_ESTADISTICAS
extern KdTree::Estadisticas g_kdTreeEstadisticas;
#endif

#endif

