#include "KdTree.h"
#include "RayBox.h"

#define MAXIMO_PRIMITIVAS 8
#define OPTIMIZAR_ESPACIOS_VACIOS
#define NIVEL_MAXIMO 30

#ifdef KDTREE_ESTADISTICAS
KdTree::Estadisticas g_kdTreeEstadisticas;
#endif

KdTree::KdTree(KdTree* pPadre)
{
	//m_pPadre = pPadre;

	m_pHijos = NULL;
	//m_pHijos[1] = NULL;

	m_CantidadPrimitivas = 0;

	m_Axis = -1;

	m_pPrimitivas = NULL;
	m_MaximoPrimitivas = 0;

	m_Minimos = new CoordType[3];
	m_Maximos = new CoordType[3];
}

KdTree::~KdTree()
{
	delete[] m_Minimos;
	delete[] m_Maximos;
}

void KdTree::Construir(CoordType dimension)
{
	CoordType minimos[3], maximos[3];

	minimos[0] = minimos[1] = minimos[2] = -dimension / 2;
	maximos[0] = maximos[1] = maximos[2] = dimension / 2;

	Construir(minimos, maximos);
}

void KdTree::Construir(CoordType minimos[3], CoordType maximos[3])
{
	//m_Nivel = nivel;

	for (int i = 0; i < 3; i++)
	{
		m_Minimos[i] = minimos[i];
		m_Maximos[i] = maximos[i];
	}
}

void KdTree::Subdividir(KdTree* &pProximoKdTreeLibre)
{
	CoordType minimos[3], maximos[3];

	//Elijo el plano de separación

	GetPrimitiva(0)->GetAABB(minimos, maximos);

	for (int a = 1; a < GetCantidadPrimitivas(); a++)
	{
		CoordType minimos2[3], maximos2[3];

		GetPrimitiva(a)->GetAABB(minimos2, maximos2);

		for (int i = 0; i < 3; i++)
		{
			minimos[i] = min(minimos[i], minimos2[i]);
			maximos[i] = max(maximos[i], maximos2[i]);
		}
	}

	//Me aseguro que los minimos / maximos esten dentro de los limites del nodo
	//Puede no ser asi si hay primitivas que desborden del nodo
	for (int i = 0; i < 3; i++)
	{
		if (minimos[i] < m_Minimos[i])
			minimos[i] = m_Minimos[i];

		if (maximos[i] > m_Maximos[i])
			maximos[i] = m_Maximos[i];
	}

	float tamanios[3];

	for (int i = 0; i < 3; i++)
		tamanios[i] = maximos[i] - minimos[i];

	int pMax = 0; //0:x 1:y 2:z
	float max = tamanios[pMax];

	if (tamanios[0] == tamanios[1] &&
		tamanios[1] == tamanios[2])
	{
		static int planoSeparacionAUsar = 0;

		pMax = (planoSeparacionAUsar++) % 3;
	}
	else
	{
		for (int i = 1; i < 3; i++)
			if (tamanios[i] > max)
			{
				max = tamanios[i];
				pMax = i;
			}
	}

	//Divido en el plano que corresponda
	float mitad = max / 2.0f + minimos[pMax];

#ifdef OPTIMIZAR_ESPACIOS_VACIOS
	
	//Calculo el volumen del espacio con primitivas
	float volumenOcupado = 1;

	for (int i = 0; i < 3; i++)
		volumenOcupado *= maximos[i] - minimos[i];

	//Calculo el volumen de espacio de los 6 sectores posiblemente vacios alrededor de los minimos / maximos internos
	float volumenesVacios[3][2]; //3 dimensiones (x, y, z), y 2 posiciones (izq / der, arr / aba, atr / adel)

	for (int d = 0; d < 3; d++)
	{
		float ds[3];

		for (int i = 0; i < 3; i++)
			ds[i] = m_Maximos[i] - m_Minimos[i];

		for (int p = 0; p < 2; p++)
		{
			if (p == 0)
				ds[d] = minimos[d] - m_Minimos[d];
			else
				ds[d] = m_Maximos[d] - maximos[d];

			volumenesVacios[d][p] = ds[0] * ds[1] * ds[2];
		}
	}

	//Busco el maximo volumen vacio
	int dimensionVaciaMaxima = 0;
	int posicionVaciaMaxima = 0;
	float volumenVacioMaximo = 0;

	for (int d = 0; d < 3; d++)
	{
		for (int p = 0; p < 2; p++)
		{
			if (volumenesVacios[d][p] > volumenVacioMaximo)
			{
				volumenVacioMaximo = volumenesVacios[d][p];
				dimensionVaciaMaxima = d;
				posicionVaciaMaxima = p;
			}
		}
	}

	if (volumenVacioMaximo > volumenOcupado)
	{
		//Me conviene separar en vacio / ocupado!
		
		//Uso el plano del volumen vacio maximo

		pMax = dimensionVaciaMaxima;

		if (posicionVaciaMaxima == 0)
			mitad = minimos[pMax] - 0.001f;
		else
			mitad = maximos[pMax] + 0.001f;
	}

#endif

	m_Axis = pMax;
	m_Coordenada = mitad;

	//Separo!
	for (int i = 0; i < 3; i++)
	{
		minimos[i] = m_Minimos[i];
		maximos[i] = m_Maximos[i];
	}

	//Primer subdivision (menor al plano)
	minimos[pMax] = m_Minimos[pMax];
	maximos[pMax] = mitad;

	m_pHijos = pProximoKdTreeLibre;
	pProximoKdTreeLibre += 2;

	//m_pHijos[0] = pProximoKdTreeLibre++;/*  new KdTree(this);*/
	//m_pHijos[0].m_pPadre = this;
	m_pHijos[0].Construir(minimos, maximos);
	
	//Segunda subdivision (mayor al plano)
	minimos[pMax] = mitad;
	maximos[pMax] = m_Maximos[pMax];

	//m_pHijos[1] = pProximoKdTreeLibre++; /*new KdTree(this);*/
	//m_pHijos[1].m_pPadre = this;
	m_pHijos[1].Construir(minimos, maximos);
}

void KdTree::Optimizar(KdTree* &pProximoKdTreeLibre, int nivel)
{
	CoordType aabbMinimos[3];
	CoordType aabbMaximos[3];

	for (int i = 0; i < nivel; i++)
		printf(" ");

	if (m_Axis < 0 && GetCantidadPrimitivas() > MAXIMO_PRIMITIVAS && nivel < NIVEL_MAXIMO)
	{
		printf("Nueva separacion\n");

		Subdividir(pProximoKdTreeLibre);

		for (int p = 0; p < GetCantidadPrimitivas(); p++)
		{
			Primitiva* prim = GetPrimitiva(p);

			prim->GetAABB(aabbMinimos, aabbMaximos);

			switch(ColisionAABBPlano(aabbMinimos, aabbMaximos, m_Axis, m_Coordenada))
			{
				case 0:
					//colisiona con ambos:
					m_pHijos[0].AgregarPrimitiva(prim);
					m_pHijos[1].AgregarPrimitiva(prim);
					break;

				case 1:
					//solo colisiona con el menor
					m_pHijos[0].AgregarPrimitiva(prim);
					break;

				case 2:
					//solo colisiona con el mayor
					m_pHijos[1].AgregarPrimitiva(prim);
					break;
			}
		}
		
		for (int i = 0; i < 2; i++)
			m_pHijos[i].Optimizar(pProximoKdTreeLibre, nivel + 1);

		/*if (m_pHijos[0].m_Axis < 0 && m_pHijos[0].GetCantidadPrimitivas() == 0 || 
			m_pHijos[1].m_Axis < 0 && m_pHijos[1].GetCantidadPrimitivas() == 0)
		{
			//Separe al pedo :-(

			//No puedo recuperar con delete porque no lo cree con new!
			//delete m_pHijos[0];
			//delete m_pHijos[1];

			//m_pHijos[0] = NULL;
			//m_pHijos[1] = NULL;

			m_pHijos = NULL;

			m_Axis = -1;

			printf("Deshacer separación\n");
		}
		else
		{*/
			m_CantidadPrimitivas = 0;

			delete[] m_pPrimitivas;

			m_pPrimitivas = NULL;
			m_MaximoPrimitivas = 0;

			
		/*}*/
	}
	else
	{
		printf("Nivel: %d, Primitivas: %d\n", nivel, GetCantidadPrimitivas());
	}
}

void KdTree::AgregarPrimitiva(Primitiva* primitiva)
{
	/*m_pPrimitivas.push_back(primitiva);*/

	if (m_pPrimitivas == NULL)
	{
		m_MaximoPrimitivas = 8;
		m_pPrimitivas = (Primitiva**) malloc(sizeof(Primitiva*) * m_MaximoPrimitivas);
	}

	if (m_CantidadPrimitivas + 1 == m_MaximoPrimitivas)
	{
		m_MaximoPrimitivas *= 2;

		m_pPrimitivas = (Primitiva**) realloc(m_pPrimitivas, sizeof(Primitiva*) * m_MaximoPrimitivas);
	}

	m_pPrimitivas[m_CantidadPrimitivas] = primitiva;
	m_CantidadPrimitivas++;
}

bool KdTree::ColisionanAABB(CoordType minimosA[3], CoordType maximosA[3],
					CoordType minimosB[3], CoordType maximosB[3])
{
	for (int i = 0; i < 3; i++)
		if (minimosB[i] > maximosA[i] ||
			minimosA[i] > maximosB[i])
			return false;

	return true;
}

//Devuelve 1 si es menor al plano, 2 si es mayor al plano, y 0 si colisiona con el plano
int KdTree::ColisionAABBPlano(CoordType minimos[3], CoordType maximos[3],
					int axisPlano, float coordenadaPlano)
{
	if (maximos[axisPlano] < coordenadaPlano)
		return 1;

	if (minimos[axisPlano] > coordenadaPlano)
		return 2;

	return 0;
}

//Devolver 1 si es menor, 0 si colisiona, 2 si es mayor
int KdTree::ColisionaRayoPlano(Vector3& rayoOrigen, Vector3& rayoDireccion,
									  int axisPlano, float coordenadaPlano)
{
	CoordType d = rayoDireccion[axisPlano];

	if (d)
	{
		CoordType dist = -(rayoOrigen[axisPlano] - coordenadaPlano) / d;

		if (dist > (CoordType) 0)
			return 0;

		if (d > 0)
			return 2;
		else
			return 1;
	}

	return 0;
}

int KdTree::ColisionaRayoPlano(Vector3& rayoOrigen, Vector3& rayoDireccion,
									  int axisPlano, float coordenadaPlano, float& distancia)
{
	CoordType planoD = -coordenadaPlano;

	CoordType d = rayoDireccion[axisPlano];

	if (d)
	{
		CoordType dist = -(rayoOrigen[axisPlano] + planoD) / d;

		if (dist > (CoordType) 0)
		{
			distancia = dist;
			return 0;
		}

		if (d > 0)
			return 2;
		else
			return 1;
	}
	else
	{
		distancia = 0;
	}

	return 0;
}

Primitiva* KdTree::BuscarInterseccion(Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia, ResultadoRayo& resultadoRayo)
{
	KdTree* pTree = this;

	Primitiva* prim = NULL;

	while (pTree->m_Axis >= 0) 
	{
		int n = ColisionaRayoPlano(rayoOrigen, rayoDireccion, pTree->m_Axis, pTree->m_Coordenada);

		if (n == 1)
			pTree = &pTree->m_pHijos[0];
		else if (n == 2)
			pTree = &pTree->m_pHijos[1];
		else /*if (n == 0)*/
		{
			prim = pTree->m_pHijos[0].BuscarInterseccion(rayoOrigen, rayoDireccion, distancia, resultadoRayo);

			Primitiva* pr = pTree->m_pHijos[1].BuscarInterseccion(rayoOrigen, rayoDireccion, distancia, resultadoRayo);

			if (pr != NULL)
				prim = pr;

			return prim;
		}
	}
	
	for (int s = 0; s < pTree->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = pTree->GetPrimitiva(s);

		ResultadoRayo res;

		if (res = pr->Intersecta(rayoOrigen, rayoDireccion, distancia)) 
		{
			prim = pr;
			resultadoRayo = res;
		}
	}

	return prim;
}

void KdTree::BuscarIntersecciones(Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES])
{
	KdTree* pTree = this;

	while (pTree->m_Axis >= 0)
	{
		//TODO: Ver si puedo aplicar alguna heuristica para dejar de buscar antes!!

		int totalNs[3];

		totalNs[0] = totalNs[1] = totalNs[2] = 0;

		int ns[RAYOS_HORIZONTALES];

		for (int i = 0; i < RAYOS_HORIZONTALES; i++)
			totalNs[ns[i] = ColisionaRayoPlano(rayosOrigen[i], rayosDireccion[i], pTree->m_Axis, pTree->m_Coordenada)]++;

		if (totalNs[1] == RAYOS_HORIZONTALES)
		{
			//Todos menores

			pTree = &pTree->m_pHijos[0];
		} 
		else if (totalNs[2] == RAYOS_HORIZONTALES)
		{
			//Todos mayores

			pTree = &pTree->m_pHijos[1];
		}
		else if (totalNs[0] == RAYOS_HORIZONTALES)
		{
			//Todos en el medio

			pTree->m_pHijos[0].BuscarIntersecciones(rayosOrigen, rayosDireccion, distancias, primitivas, resultados);
			pTree->m_pHijos[1].BuscarIntersecciones(rayosOrigen, rayosDireccion, distancias, primitivas, resultados);
			return;
		}
		else
		{
			//Mezclado, tengo que hacerlo por separado

			for (int n = 0; n < RAYOS_HORIZONTALES; n++)
			{
				switch(ns[n])
				{
					case 0:
					{
						Primitiva* pr = pTree->m_pHijos[0].BuscarInterseccion(rayosOrigen[n], rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						
						pr = pTree->m_pHijos[1].BuscarInterseccion(rayosOrigen[n], rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						break;
					}

					case 1:
					{
						Primitiva* pr = pTree->m_pHijos[0].BuscarInterseccion(rayosOrigen[n], rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						break;
					}

					case 2:
					{
						Primitiva* pr = pTree->m_pHijos[1].BuscarInterseccion(rayosOrigen[n], rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						break;
					}
				}
			}
			return;
		}
	}

	// find the nearest intersection

	for (int s = 0; s < pTree->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = pTree->GetPrimitiva(s);

		ResultadoRayo res[RAYOS_HORIZONTALES];

		int intersecciones = pr->Intersectan(rayosOrigen, rayosDireccion, distancias, res);

		if (intersecciones)
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				if (res[t] != ResultadoRayoFallo)
				{
					primitivas[t] = pr;
					resultados[t] = res[t];
				}
		}
	}
}

void KdTree::BuscarInterseccionesIniciales(Vector3 &rayoOrigen, Vector3 (&rayosDireccion)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_VERTICALES][RAYOS_HORIZONTALES])
{
	KdTree* pTree = this;

	while (pTree->m_Axis >= 0)
	{
		int nSuperiorIzquierdo = ColisionaRayoPlano(rayoOrigen, rayosDireccion[0][0], pTree->m_Axis, pTree->m_Coordenada);
		int nInferiorDerecho = ColisionaRayoPlano(rayoOrigen, rayosDireccion[RAYOS_VERTICALES - 1][RAYOS_HORIZONTALES - 1], pTree->m_Axis, pTree->m_Coordenada);

		if (nSuperiorIzquierdo == nInferiorDerecho)
		{
			if (nSuperiorIzquierdo == 1)
			{
				//Todos menores
				#ifdef KDTREE_ESTADISTICAS
				g_kdTreeEstadisticas.cantidadDivisionesUnicas++;
				#endif

				pTree = &pTree->m_pHijos[0];
			} 
			else if (nSuperiorIzquierdo == 2)
			{
				//Todos mayores
				#ifdef KDTREE_ESTADISTICAS
				g_kdTreeEstadisticas.cantidadDivisionesUnicas++;
				#endif

				pTree = &pTree->m_pHijos[1];
			}
			else
			{
				//Todos en el medio
				#ifdef KDTREE_ESTADISTICAS
				g_kdTreeEstadisticas.cantidadDivisionesConjuntas++;
				#endif

				pTree->m_pHijos[0].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion, distancias, primitivas, resultados);
				pTree->m_pHijos[1].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion, distancias, primitivas, resultados);

				return;
			}
		}
		else
		{
			#ifdef KDTREE_ESTADISTICAS
			g_kdTreeEstadisticas.cantidadDivisionesFallidas++;
			#endif

			//Mezclados

			int ns[RAYOS_VERTICALES][RAYOS_HORIZONTALES];
			int totalNsSeparadas[RAYOS_VERTICALES][3];

			memset(totalNsSeparadas, 0, sizeof(totalNsSeparadas));

			CoordType planoD = -pTree->m_Coordenada;
			CoordType constante = -(rayoOrigen[pTree->m_Axis] + planoD);

			if (constante > 0)
			{
				float* p = (float*) &rayosDireccion[0][0];
				p += pTree->m_Axis;

				for (int y = 0; y < RAYOS_VERTICALES; y++)
					for (int i = 0; i < RAYOS_HORIZONTALES; i++)
					{
						CoordType d = *p;

						int n;

						if (d < 0)
							n = 1;
						else
							n = 0;

						ns[y][i] = n;

						totalNsSeparadas[y][n]++;

						p += 4;
					}
			}
			else if (constante < 0)
			{
				float* p = (float*) &rayosDireccion[0][0];
				p += pTree->m_Axis;

				for (int y = 0; y < RAYOS_VERTICALES; y++)
					for (int i = 0; i < RAYOS_HORIZONTALES; i++)
					{
						CoordType d = *p;

						int n;

						if (d > 0)
							n = 2;
						else
							n = 0;

						ns[y][i] = n;

						totalNsSeparadas[y][n]++;
					}
			}
			else
			{
				float* p = (float*) &rayosDireccion[0][0];
				p += pTree->m_Axis;

				for (int y = 0; y < RAYOS_VERTICALES; y++)
					for (int i = 0; i < RAYOS_HORIZONTALES; i++)
					{
						CoordType d = *p;

						int n = 1;

						if (d > 0)
							n = 2;
						else
							n = 1;

						ns[y][i] = n;

						totalNsSeparadas[y][n]++;
					}
			}

			for (int y = 0; y < RAYOS_VERTICALES; y++)
			{
				if (totalNsSeparadas[y][1] == RAYOS_HORIZONTALES)
				{
					//Subconjunto menores

					pTree->m_pHijos[0].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion[y], distancias[y], primitivas[y], resultados[y]);
				}
				else if (totalNsSeparadas[y][2] == RAYOS_HORIZONTALES)
				{
					//Subconjunto mayores
					pTree->m_pHijos[1].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion[y], distancias[y], primitivas[y], resultados[y]);
				}
				else if (totalNsSeparadas[y][0] == RAYOS_HORIZONTALES)
				{
					//Subconjunto en el medio
					pTree->m_pHijos[0].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion[y], distancias[y], primitivas[y], resultados[y]);
					pTree->m_pHijos[1].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion[y], distancias[y], primitivas[y], resultados[y]);
				}
				else
				{
					//Mezclado, tengo que hacerlo por separado
					for (int n = 0; n < RAYOS_HORIZONTALES; n++)
					{
						switch(ns[y][n])
						{
							case 0:
							{
								Primitiva* pr = pTree->m_pHijos[0].BuscarInterseccion(rayoOrigen, rayosDireccion[y][n], distancias[y][n], resultados[y][n]);

								if (pr != NULL)
									primitivas[y][n] = pr;
								
								pr = pTree->m_pHijos[1].BuscarInterseccion(rayoOrigen, rayosDireccion[y][n], distancias[y][n], resultados[y][n]);

								if (pr != NULL)
									primitivas[y][n] = pr;
								break;
							}

							case 1:
							{
								Primitiva* pr = pTree->m_pHijos[0].BuscarInterseccion(rayoOrigen, rayosDireccion[y][n], distancias[y][n], resultados[y][n]);

								if (pr != NULL)
									primitivas[y][n] = pr;
								break;
							}

							case 2:
							{
								Primitiva* pr = pTree->m_pHijos[1].BuscarInterseccion(rayoOrigen, rayosDireccion[y][n], distancias[y][n], resultados[y][n]);

								if (pr != NULL)
									primitivas[y][n] = pr;
								break;
							}
						}
					}
				}
			}

			return;
		}
	}
	
	// find the nearest intersection
	for (int s = 0; s < pTree->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = pTree->GetPrimitiva(s);

		for (int y = 0; y < RAYOS_VERTICALES; y++)
		{
			ResultadoRayo res[RAYOS_HORIZONTALES];

			int intersecciones = pr->Intersectan(rayoOrigen, rayosDireccion[y], distancias[y], res);

			if (intersecciones)
			{
				for (int t = 0; t < RAYOS_HORIZONTALES; t++)
					if (res[t] != ResultadoRayoFallo)
					{
						primitivas[y][t] = pr;
						resultados[y][t] = res[t];
					}
			}
		}
	}
}

void KdTree::BuscarInterseccionesIniciales(Vector3 &rayoOrigen, Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES])
{
	KdTree* pTree = this;

	while (pTree->m_Axis >= 0)
	{
		int nIzquierda = ColisionaRayoPlano(rayoOrigen, rayosDireccion[0], pTree->m_Axis, pTree->m_Coordenada);
		int nDerecha = ColisionaRayoPlano(rayoOrigen, rayosDireccion[RAYOS_HORIZONTALES - 1], pTree->m_Axis, pTree->m_Coordenada);

		if (nIzquierda == nDerecha)
		{
			if (nIzquierda == 1)
			{
				//Todos menores

				pTree = &pTree->m_pHijos[0];
			} 
			else if (nIzquierda == 2)
			{
				//Todos mayores

				pTree = &pTree->m_pHijos[1];
			}
			else
			{
				//Todos en el medio

				pTree->m_pHijos[0].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion, distancias, primitivas, resultados);
				pTree->m_pHijos[1].BuscarInterseccionesIniciales(rayoOrigen, rayosDireccion, distancias, primitivas, resultados);
				
				return;
			}
		}
		else
		{
			int ns[RAYOS_HORIZONTALES];

			ns[0] = nIzquierda;
			ns[RAYOS_HORIZONTALES - 1] = nDerecha;

			for (int i = 1; i < RAYOS_HORIZONTALES - 1; i++)
				ns[i] = ColisionaRayoPlano(rayoOrigen, rayosDireccion[i], pTree->m_Axis, pTree->m_Coordenada);

			//Mezclado, tengo que hacerlo por separado

			for (int n = 0; n < RAYOS_HORIZONTALES; n++)
			{
				switch(ns[n])
				{
					case 0:
					{
						Primitiva* pr = pTree->m_pHijos[0].BuscarInterseccion(rayoOrigen, rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						
						pr = pTree->m_pHijos[1].BuscarInterseccion(rayoOrigen, rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						break;
					}

					case 1:
					{
						Primitiva* pr = pTree->m_pHijos[0].BuscarInterseccion(rayoOrigen, rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						break;
					}

					case 2:
					{
						Primitiva* pr = pTree->m_pHijos[1].BuscarInterseccion(rayoOrigen, rayosDireccion[n], distancias[n], resultados[n]);

						if (pr != NULL)
							primitivas[n] = pr;
						break;
					}
				}
			}

			return;
		}
	}
	
	// find the nearest intersection

	for (int s = 0; s < pTree->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = pTree->GetPrimitiva(s);

		ResultadoRayo res[RAYOS_HORIZONTALES];

		int intersecciones = pr->Intersectan(rayoOrigen, rayosDireccion, distancias, res);

		if (intersecciones)
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				if (res[t] != ResultadoRayoFallo)
				{
					primitivas[t] = pr;
					resultados[t] = res[t];
				}
		}
	}
}

bool KdTree::ColisionConAlguienIgnorando(const Primitiva* primitivaAIgnorar, Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia)
{
	KdTree* pTree = this;

	while (pTree->m_Axis >= 0) {

		int n = ColisionaRayoPlano(rayoOrigen, rayoDireccion, pTree->m_Axis, pTree->m_Coordenada);

		if (n == 1)
			pTree = &pTree->m_pHijos[0];
		else if (n == 2)
			pTree = &pTree->m_pHijos[1];
		else /*if (n == 0)*/
		{
			if (pTree->m_pHijos[0].ColisionConAlguienIgnorando(primitivaAIgnorar, rayoOrigen, rayoDireccion, distancia))
				return true;

			if (pTree->m_pHijos[1].ColisionConAlguienIgnorando(primitivaAIgnorar, rayoOrigen, rayoDireccion, distancia))
				return true;

			return false;
		}
	}

	for (int s = 0; s < pTree->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = pTree->GetPrimitiva(s);

		if (pr != primitivaAIgnorar && pr->Intersecta(rayoOrigen, rayoDireccion, distancia))
			return true;
	}

	return false;
}

int KdTree::ColisionConAlguienIgnorando(const Primitiva* primitivaAIgnorar, Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], bool (&colisiones)[RAYOS_HORIZONTALES])
{
	KdTree *pTree = this;

	int totalColisiones = 0;

	while (pTree->m_Axis >= 0)
	{
		int totalNs[3];

		totalNs[0] = totalNs[1] = totalNs[2] = 0;

		int ns[RAYOS_HORIZONTALES];

		for (int i = 0; i < RAYOS_HORIZONTALES; i++)
		{
			ns[i] = ColisionaRayoPlano(rayosOrigen[i], rayosDireccion[i], pTree->m_Axis, pTree->m_Coordenada);

			totalNs[ns[i]]++;
		}

		if (totalNs[1] == RAYOS_HORIZONTALES)
		{
			//Todos menores

			pTree = &pTree->m_pHijos[0];
		} 
		else if (totalNs[2] == RAYOS_HORIZONTALES)
		{
			//Todos mayores

			pTree = &pTree->m_pHijos[1];
		}
		else if (totalNs[0] == RAYOS_HORIZONTALES)
		{
			//Todos intersectan

			totalColisiones = pTree->m_pHijos[0].ColisionConAlguienIgnorando(primitivaAIgnorar, rayosOrigen, rayosDireccion, distancias, colisiones);

			if (totalColisiones != RAYOS_HORIZONTALES)
				totalColisiones += pTree->m_pHijos[1].ColisionConAlguienIgnorando(primitivaAIgnorar, rayosOrigen, rayosDireccion, distancias, colisiones);

			return totalColisiones;
		}
		else
		{
			//Mezclado, tengo que hacerlo por separado

			for (int n = 0; n < RAYOS_HORIZONTALES; n++)
			{
				switch(ns[n])
				{
					case 0:
					{
						if (colisiones[n] == false)
						{
							if (pTree->m_pHijos[0].ColisionConAlguienIgnorando(primitivaAIgnorar, rayosOrigen[n], rayosDireccion[n], distancias[n]))
							{
								colisiones[n] = true;
								totalColisiones++;
							}
							else if (pTree->m_pHijos[1].ColisionConAlguienIgnorando(primitivaAIgnorar, rayosOrigen[n], rayosDireccion[n], distancias[n]))
							{
								colisiones[n] = true;
								totalColisiones++;
							}
						}
						break;
					}

					case 1:
					{
						if (colisiones[n] == false)
						{
							if (pTree->m_pHijos[0].ColisionConAlguienIgnorando(primitivaAIgnorar, rayosOrigen[n], rayosDireccion[n], distancias[n]))
							{
								colisiones[n] = true;
								totalColisiones++;
							}
						}
						break;
					}

					case 2:
					{
						if (colisiones[n] == false)
						{
							if (pTree->m_pHijos[1].ColisionConAlguienIgnorando(primitivaAIgnorar, rayosOrigen[n], rayosDireccion[n], distancias[n]))
							{
								colisiones[n] = true;
								totalColisiones++;
							}
						}
						break;
					}
				}
			}

			return totalColisiones;
		}
	}

	for (int s = 0; s < pTree->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = pTree->GetPrimitiva(s);

		if (pr != primitivaAIgnorar)
		{
			ResultadoRayo res[RAYOS_HORIZONTALES];

			int intersecciones = pr->Intersectan(rayosOrigen, rayosDireccion, distancias, res);

			if (intersecciones)
			{
				for (int t = 0; t < RAYOS_HORIZONTALES; t++)
					if (res[t] != ResultadoRayoFallo && colisiones[t] == false)
					{
						colisiones[t] = true;
						totalColisiones++;
					}
			}

			if (totalColisiones == RAYOS_HORIZONTALES)
				break;
		}
	}

	return totalColisiones;
}

KdTree* KdTree::BuscarNodoContenedorPunto(Vector3& punto)
{
	if (m_Axis < 0)
		return this;

	if (punto[m_Axis] < m_Coordenada)
		return m_pHijos[0].BuscarNodoContenedorPunto(punto);
	else if (punto[m_Axis] > m_Coordenada)
		return m_pHijos[1].BuscarNodoContenedorPunto(punto);
	else
		return this;
}


