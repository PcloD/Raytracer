#include "Esfera.h"

/*
ResultadoRayo Esfera::Intersecta(Rayo& rayo, CoordType& distancia)
{
	ResultadoRayo retval = ResultadoRayoFallo;

	Vector3 v = rayo.GetOrigen() - m_Centro;
	CoordType b = -DOT(v, rayo.GetDireccion());
	CoordType det = (b * b) - DOT( v, v ) + m_RadioSq;

	if (det > (CoordType) 0)
	{
		det = sqrt( det );
		CoordType i1 = b - det;
		CoordType i2 = b + det;

		if (i2 > (CoordType) 0)
		{
			if (i1 < (CoordType) 0) 
			{
				//if (i2 < distancia) 
				//{
				//	distancia = i2;
				//	retval = ResultadoRayoImpactoInterior;
				//}
			}
			else
			{
				if (i1 < distancia)
				{
					distancia = i1;
					retval = ResultadoRayoImpacto;
				}
			}
		}
	}

	return retval;
}

Vector3 Esfera::GetNormal(Vector3& posicion)
{
	return (posicion - m_Centro) * m_RadioInv;
}

int Esfera::Intersectan(Rayo (&rayos)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES])
{
	int intersecciones = 0;

	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
		resultados[t] = ResultadoRayoFallo;

	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
	{
		Vector3 v = rayos[t].GetOrigen() - m_Centro;
		CoordType b = -DOT(v, rayos[t].GetDireccion());
		CoordType det = (b * b) - DOT(v, v) + m_RadioSq;

		if (det > (CoordType) 0)
		{
			det = sqrt( det );
			CoordType i1 = b - det;
			CoordType i2 = b + det;

			if (i2 > (CoordType) 0)
			{
				if (i1 < (CoordType) 0) 
				{
					//if (i2 < distancias[t]) 
					//{
					//	distancias[t] = i2;
					//	resultados[t] = ResultadoRayoImpactoInterior;
					//	intersecciones++;
					//}
				}
				else
				{
					if (i1 < distancias[t])
					{
						distancias[t] = i1;
						resultados[t] = ResultadoRayoImpacto;
						intersecciones++;
					}
				}
			}
		}
	}

	return intersecciones;
}

void Esfera::GetNormales(Vector3 (&posiciones)[RAYOS_HORIZONTALES], Vector3 (&normales)[RAYOS_HORIZONTALES])
{
	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
		normales[t] = GetNormal(posiciones[t]);
}
*/