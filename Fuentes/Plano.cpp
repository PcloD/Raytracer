#include "Plano.h"
/*

ResultadoRayo Plano::Intersecta(Rayo& rayo, CoordType& distancia)
{
	CoordType d = DOT( m_Plano.N, rayo.GetDireccion() );

	if (d != (CoordType) 0)
	{
		CoordType dist = -(DOT( m_Plano.N, rayo.GetOrigen() ) + m_Plano.D) / d;
		if (dist > (CoordType) 0)
		{
			if (dist < distancia) 
			{
				distancia = dist;
				return ResultadoRayoImpacto;
			}
		}
	}

	return ResultadoRayoFallo;
}

Vector3 Plano::GetNormal(Vector3&)
{
	return m_Plano.N;
}

int Plano::Intersectan(Rayo (&rayos)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES])
{
	int intersecciones = 0;

	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
	{
		CoordType vDot = DOT(m_Plano.N, rayos[t].GetOrigen());
		CoordType d = DOT( m_Plano.N, rayos[t].GetDireccion());

		if (d != (CoordType) 0)
		{
			CoordType dist = -(vDot + m_Plano.D) / d;
			if (dist > (CoordType) 0)
			{
				if (dist < distancias[t]) 
				{
					distancias[t] = dist;
					intersecciones++;
					resultados[t] = ResultadoRayoImpacto;
					continue;
				}
			}
		}

		resultados[t] = ResultadoRayoFallo;
	}

	return intersecciones;
}

void Plano::GetNormales(Vector3 (&posiciones)[RAYOS_HORIZONTALES], Vector3 (&normales)[RAYOS_HORIZONTALES])
{
	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
		normales[t] = GetNormal(posiciones[t]);
}
*/