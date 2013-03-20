#include "Primitiva.h"


ResultadoRayo Primitiva::Intersecta(Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia) const
{
	ResultadoRayo retval = ResultadoRayoFallo;

	switch(m_TipoPrimitiva)
	{
		case TipoPrimitivaPlano:
		{
			CoordType d = DOT( m_Plano.N, rayoDireccion );

			if (d != (CoordType) 0)
			{
				CoordType dist = -(DOT( m_Plano.N, rayoOrigen ) + m_Plano.D) / d;
				if (dist > (CoordType) 0)
				{
					if (dist < distancia) 
					{
						distancia = dist;
						retval = ResultadoRayoImpacto;
					}
				}
			}
			break;
		}

		case TipoPrimitivaEsfera:
		{
			Vector3 v = rayoOrigen - m_Centro;
			CoordType b = -DOT(v, rayoDireccion);
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
			break;
		}
	}

	return retval;
}

Vector3 Primitiva::GetNormal(Vector3& posicion) const
{
	Vector3 N;

	switch(m_TipoPrimitiva)
	{
		case TipoPrimitivaPlano:
		{
			N = m_Plano.N;
			break;
		}

		case TipoPrimitivaEsfera:
		{
			N = (posicion - m_Centro) * m_RadioInv;
			break;
		}
	}

	return N;
}

int Primitiva::Intersectan(Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]) const
{
	int intersecciones = 0;

	switch(m_TipoPrimitiva)
	{
		case TipoPrimitivaPlano:
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				CoordType d = DOT( m_Plano.N, rayosDireccion[t]);

				if (d != (CoordType) 0)
				{
					CoordType dist = -(DOT(m_Plano.N, rayosOrigen[t]) + m_Plano.D) / d;
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
			break;
		}

		case TipoPrimitivaEsfera:
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				Vector3 v = rayosOrigen[t] - m_Centro;
				CoordType b = -DOT(v, rayosDireccion[t]);
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
								continue;
							}
						}
					}
				}

				resultados[t] = ResultadoRayoFallo;
			}
			break;
		}
	}

	return intersecciones;
}

int Primitiva::Intersectan(Vector3 &rayoOrigen, Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], CoordType (&distancias)[RAYOS_HORIZONTALES], ResultadoRayo (&resultados)[RAYOS_HORIZONTALES]) const
{
	int intersecciones = 0;

	switch(m_TipoPrimitiva)
	{
		case TipoPrimitivaPlano:
		{
			CoordType constante = -(DOT(m_Plano.N, rayoOrigen) + m_Plano.D);

			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				CoordType d = DOT( m_Plano.N, rayosDireccion[t]);

				if (d != (CoordType) 0)
				{
					CoordType dist = constante / d;
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
			break;
		}

		case TipoPrimitivaEsfera:
		{
			Vector3 v = rayoOrigen - m_Centro;;

			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				CoordType b = -DOT(v, rayosDireccion[t]);
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
								continue;
							}
						}
					}
				}

				resultados[t] = ResultadoRayoFallo;
			}
			break;
		}
	}

	return intersecciones;
}

void Primitiva::GetNormales(Vector3 (&posiciones)[RAYOS_HORIZONTALES], Vector3 (&normales)[RAYOS_HORIZONTALES]) const
{
	switch(m_TipoPrimitiva)
	{
		case TipoPrimitivaPlano:
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				normales[t] = m_Plano.N;
			break;
		}

		case TipoPrimitivaEsfera:
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				normales[t] = (posiciones[t] - m_Centro) * m_RadioInv;
			break;
		}
	}
}

void Primitiva::GetAABB(CoordType (&minimos)[3], CoordType (&maximos)[3])
{
	switch(m_TipoPrimitiva)
	{
		case TipoPrimitivaPlano:
		{
			if (m_Plano.N[0] != 0)
			{
				//Plano vertical perpendicular a la camara
				float x = m_Plano.N[0] * -m_Plano.D;

				minimos[1] = minimos[2] = -100.0f;
				maximos[2] = maximos[2] = 100.0f;

				minimos[0] = x - 0.2f;
				maximos[0] = x + 0.2f;
			}
			else if (m_Plano.N[1] != 0)
			{
				//Plano horizontal
				float y = m_Plano.N[1] * -m_Plano.D;

				minimos[0] = minimos[2] = -100.0f;
				maximos[0] = maximos[2] = 100.0f;

				minimos[1] = y - 0.2f;
				maximos[1] = y + 0.2f;
			}
			else
			{
				//Plano vertical mirando a la camara
				float z = m_Plano.N[2] * -m_Plano.D;

				minimos[0] = minimos[1] = -100.0f;
				maximos[0] = maximos[1] = 100.0f;

				minimos[2] = z - 0.2f;
				maximos[2] = z + 0.2f;
			}
			break;
		}

		case TipoPrimitivaEsfera:
		{
			for (int i = 0; i < 3; i++)
			{
				minimos[i] = m_Centro[i] - m_Radio;
				maximos[i] = m_Centro[i] + m_Radio;
			}
			break;
		}
	}
}
