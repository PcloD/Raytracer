#include "Raytracer.h"
#include "Common.h"
#include "Rayo.h"
#include "Primitiva.h"
#include "Esfera.h"
#include "Luz.h"

//con octree y antes de multiples rayos: 9.62 fps
//despues de multiples rayos: 10.90 fps
//despues de optimizar el llamado si se chocó contra 1 misma primitiva todas las veces: 11.10 fps

#define RAYTRACE_DEPTH 6

#define USAR_DIFFUSE
#define USAR_SPECULAR
#define USAR_SPECULAR_APROX
#define USAR_REFLECTION
#define USAR_SOMBRAS

//sin REFL ni SOMB: 3.44
//con REFL: 0.89
//con SOMB: 1.100

#define USAR_KDTREE
#define USAR_RAYOS_PRIMARIOS

//fdangelo: El SUPERSAMPLING por el momento no se ve bien debido al no supersampleo de los pixels límite del recuadro dibujado

//#define USAR_SUPERSAMPLING

Raytracer::Raytracer()
{
	m_pEscena = NULL;
	ResetearEstadisticas();
}

Raytracer::~Raytracer()
{
}

void Raytracer::ResetearEstadisticas() 
{ 
	memset(&m_Estadisticas, 0, sizeof(m_Estadisticas));
}

void Raytracer::Dibujar(Escena *pEscena, Pantalla& pantalla, Camera& camara, int desdeY, int hastaY)
{
	Matrix44 matCamara = IdentityMatrix44();
	
	matCamara *= RotateRadMatrix44(Vector3(0, 1, 0), DegToRad(camara.GetRotation().y));
	matCamara *= RotateRadMatrix44(Vector3(1, 0, 0), DegToRad(camara.GetRotation().x));
	matCamara *= RotateRadMatrix44(Vector3(0, 0, 1), DegToRad(camara.GetRotation().z));

	m_pEscena = pEscena;

	Vector3 origen( (CoordType) 0, (CoordType) 0, (CoordType) -7 );

	//pantalla.IniciarDibujado();

	CoordType WX1 = (CoordType) -4, WX2 = (CoordType) 4; /*Ancho pantalla [-4, 4]*/
	CoordType WY1 = (CoordType) 3, WY2 = (CoordType) -3; /*Alto pantalla [3,-3]*/

	CoordType DX = (WX2 - WX1) / (CoordType) pantalla.GetAncho();
	CoordType DY = (WY2 - WY1) / (CoordType) pantalla.GetAlto();

	CoordType SY = WY1 + DY * desdeY;

	Vector3 origenDesplazado(origen - camara.GetPosition());

	//KdTree* pRaizOrigen = m_pEscena->GetSeparador()->BuscarNodoContenedorPunto(origenDesplazado);

	KdTree* pRaizOrigen = m_pEscena->GetSeparador();

	for (int y = desdeY; y < hastaY; y += RAYOS_VERTICALES)
	{
		CoordType SX = WX1;

		int ancho = pantalla.GetAncho();

		for (int x = 0; x < ancho; x += RAYOS_HORIZONTALES)
		{
			Vector3 _MM_ALIGN16 rayosOrigen[RAYOS_VERTICALES][RAYOS_HORIZONTALES];
			Vector3 _MM_ALIGN16 rayosDireccion[RAYOS_VERTICALES][RAYOS_HORIZONTALES];
			Color _MM_ALIGN16 colores[RAYOS_VERTICALES][RAYOS_HORIZONTALES];
			Primitiva* primitivas[RAYOS_VERTICALES][RAYOS_HORIZONTALES];

			CoordType startSX = SX;

			SY = WY1 + DY * y;

			for (int y2 = 0; y2 < RAYOS_VERTICALES; y2++)
			{
				SX = startSX;

				for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				{
					rayosDireccion[y2][t] = matCamara * (Vector3((CoordType) SX, (CoordType) SY, (CoordType) 0.0f) - origen).normalize();
					rayosOrigen[y2][t] = origenDesplazado;
					SX += DX;
				}

				SY += DY;
			}


#ifdef USAR_RAYOS_PRIMARIOS

			TrazarRayosPrimarios(pRaizOrigen, rayosOrigen, rayosDireccion, colores, primitivas, RAYTRACE_DEPTH);
#else

			for (int y2 = 0; y2 < RAYOS_VERTICALES; y2++)
			{
				for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				{
					float distancia = 100000;

					TrazarRayo(rayosOrigen[y2][t], rayosDireccion[y2][t], colores[y2][t], RAYTRACE_DEPTH, distancia);
				}
			}

#endif

			for (int y2 = 0; y2 < RAYOS_VERTICALES; y2++)
				for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				{
					Vector3 c = colores[y2][t];

					#ifdef USAR_SUPERSAMPLING

					if (y2 > 0 && y2 < RAYOS_VERTICALES - 1)
					{
						if (t > 0 && t < RAYOS_HORIZONTALES - 1)
						{
							if (primitivas[y2][t] != primitivas[y2-1][t] ||
								primitivas[y2][t] != primitivas[y2+1][t] ||
								primitivas[y2][t] != primitivas[y2][t-1] ||
								primitivas[y2][t] != primitivas[y2][t+1])
							{
								c = colores[y2-1][t];
								c += colores[y2+1][t];
								c += colores[y2][t+1];
								c += colores[y2][t-1];
															
								c *= 0.25f;
							}
						}
						else
						{
							if (primitivas[y2][t] != primitivas[y2-1][t] ||
								primitivas[y2][t] != primitivas[y2+1][t])
							{
								c = colores[y2-1][t] + colores[y2+1][t];
								c *= 0.5f;
							}
						}
					}
					else
					{
						if (t > 0 && t < RAYOS_HORIZONTALES - 1)
						{
							if (primitivas[y2][t] != primitivas[y2][t-1] ||
								primitivas[y2][t] != primitivas[y2][t+1])
							{
								c = colores[y2][t-1] + colores[y2][t+1];
								c *= 0.5f;
							}
						}
					}

					/*if (y2 > 0 && y2 < RAYOS_VERTICALES - 1 &&
						t > 0 && t < RAYOS_HORIZONTALES - 1)
					{
						if (primitivas[y2][t] != primitivas[y2-1][t] ||
							primitivas[y2][t] != primitivas[y2+1][t] ||
							primitivas[y2][t] != primitivas[y2][t-1] ||
							primitivas[y2][t] != primitivas[y2][t+1])
						{
							//Filtro bilineal.. la verdad que no queda tan bien, en especial porque solo lo hago
							//en un subconjunto de la imagen (los bordes no tienen antialiasing :-( )
							//Para hacerlo bien tendria que almacenar la asociación de cada pixel con sus primitivas
							//y recien despues hacer el filtro..

							c = colores[y2-1][t-1];
							c += colores[y2-1][t+1];
							c += colores[y2+1][t+1];
							c += colores[y2+1][t-1];
														
							c *= 1.0f / 4.0f;
						}
					}*/

					#endif

					c *= 256;

					int r = min(c.x, 255);
					int g = min(c.y, 255);
					int b = min(c.z, 255);

					pantalla.DibujarPixel(x + t, y + y2, r, g, b);
				}
		}
	}

	//pantalla.FinalizarDibujado(desdeY, hastaY);
}

Primitiva* Raytracer::TrazarRayo(Vector3& rayoOrigen, Vector3& rayoDireccion, Color& color, int profundidad, CoordType& distancia)
{
	if (profundidad == 0)
		return NULL;

#ifdef RAYTRACER_ESTADISTICAS
	m_Estadisticas.cantidadRayosSecundarios++;
	m_Estadisticas.cantidadRayosSecundariosSimples++;
	m_Estadisticas.cantidadRayos++;
#endif

	//trace primary ray
	distancia = 100000.0f;
	Vector3 pi;
	Primitiva* prim = NULL;

	ResultadoRayo resultado;

	// find the nearest intersection

#ifdef USAR_KDTREE

	prim = m_pEscena->GetSeparador()->BuscarInterseccion(rayoOrigen, rayoDireccion, distancia, resultado);

#else

	for (int s = 0; s < m_pEscena->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = m_pEscena->GetPrimitiva(s);

		ResultadoRayo res;

#ifdef RAYTRACER_ESTADISTICAS
		m_Estadisticas.cantidadChequeosIntersecciones++;
#endif

		if (res = pr->Intersecta(rayoOrigen, rayoDireccion, distancia)) 
		{
			prim = pr;
			resultado = res;
		}
	}

#endif
	
	if (prim != NULL) 
		ProcesarInterseccion(prim, rayoOrigen, rayoDireccion, distancia, color, profundidad);
	
	// return pointer to primitive hit by primary ray
	return prim;
}

void Raytracer::TrazarRayosPrimarios(KdTree* pRaizOrigen, Vector3 (&rayosOrigen)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Color (&colores)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_VERTICALES][RAYOS_HORIZONTALES], int profundidad)
{
#ifdef RAYTRACER_ESTADISTICAS
	m_Estadisticas.cantidadRayosPrincipales+=RAYOS_VERTICALES*RAYOS_HORIZONTALES;
	m_Estadisticas.cantidadRayos+=RAYOS_VERTICALES*RAYOS_HORIZONTALES;
#endif

	//trace primary rays

	CoordType _MM_ALIGN16 distancias[RAYOS_VERTICALES][RAYOS_HORIZONTALES];
	ResultadoRayo _MM_ALIGN16 resultados[RAYOS_VERTICALES][RAYOS_HORIZONTALES];
	bool _MM_ALIGN16 mismaPrimitiva[RAYOS_VERTICALES];

	memset(mismaPrimitiva, 0, sizeof(mismaPrimitiva));
	memset(primitivas, 0, sizeof(primitivas));
	
	for (int y = 0; y < RAYOS_VERTICALES; y++)
		for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			distancias[y][t] = 100000.0f;

#ifdef USAR_KDTREE

	pRaizOrigen->BuscarInterseccionesIniciales(rayosOrigen[0][0], rayosDireccion, distancias, primitivas, resultados);

	for (int y = 0; y < RAYOS_VERTICALES; y++)
	{
		bool mismaPrimitiva = false;

		if (primitivas[y][0] != NULL)
		{
			mismaPrimitiva = true;

			Primitiva* pr = primitivas[y][0];

			for (int i = 1; i < RAYOS_HORIZONTALES; i++)
			{
				if (primitivas[y][i] != pr)
				{
					mismaPrimitiva = false;
					break;
				}
			}
		}

		if (mismaPrimitiva)
		{
			ProcesarInterseccion(primitivas[y][0], rayosOrigen[y], rayosDireccion[y], colores[y], profundidad, distancias[y]);
		}
		else
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				Primitiva* prim = primitivas[y][t];

				if (prim != NULL)
					ProcesarInterseccion(prim, rayosOrigen[y][t], rayosDireccion[y][t], distancias[y][t], colores[y][t], profundidad);
			}
		}
	}


#else

	// find the nearest intersections
	for (int s = 0; s < m_pEscena->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = m_pEscena->GetPrimitiva(s);

		for (int y = 0; y < RAYOS_VERTICALES; y++)
		{
			ResultadoRayo res[RAYOS_HORIZONTALES];

			int intersecciones;

#ifdef RAYTRACER_ESTADISTICAS
			m_Estadisticas.cantidadChequeosIntersecciones += RAYOS_HORIZONTALES;
#endif
			if (intersecciones = pr->Intersectan(rayosOrigen[y], rayosDireccion[y], distancias[y], res)) 
			{
				mismaPrimitiva[y] = (intersecciones == RAYOS_HORIZONTALES);

				if (mismaPrimitiva[y])
				{
					for (int t = 0; t < RAYOS_HORIZONTALES; t++)
					{
						primitivas[y][t] = pr;
						resultados[y][t] = res[t];
					}
				}
				else
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
	
	for (int y = 0; y < RAYOS_VERTICALES; y++)
	{
		// handle intersections
		if (mismaPrimitiva[y])
		{
			ProcesarInterseccion(primitivas[y][0], rayosOrigen[y], rayosDireccion[y], colores[y], profundidad, distancias[y]);
		}
		else
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				Primitiva* prim = primitivas[y][t];

				if (prim != NULL)
					ProcesarInterseccion(prim, rayosOrigen[y][t], rayosDireccion[y][t], distancias[y][t], colores[y][t], profundidad);
			}
		}
	}
#endif

}

void Raytracer::TrazarRayos(Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], Color (&colores)[RAYOS_HORIZONTALES], int profundidad, CoordType (&distancias)[RAYOS_HORIZONTALES], Primitiva* (&primitivas)[RAYOS_HORIZONTALES])
{
	if (profundidad == 0)
		return;

#ifdef RAYTRACER_ESTADISTICAS
	m_Estadisticas.cantidadRayosSecundarios+=RAYOS_HORIZONTALES;
	m_Estadisticas.cantidadRayosSecundariosMultiples+=RAYOS_HORIZONTALES;
	m_Estadisticas.cantidadRayos+=RAYOS_HORIZONTALES;
#endif

	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
	{
		distancias[t] = 100000.0f;
		primitivas[t] = NULL;
	}

#ifdef USAR_KDTREE

	ResultadoRayo resultados[RAYOS_HORIZONTALES];

	m_pEscena->GetSeparador()->BuscarIntersecciones(rayosOrigen, rayosDireccion, distancias, primitivas, resultados);

	bool mismaPrimitiva = false;

	if (primitivas[0] != NULL)
	{
		mismaPrimitiva = true;

		Primitiva* pr = primitivas[0];

		for (int i = 1; i < RAYOS_HORIZONTALES; i++)
		{
			if (primitivas[i] != pr)
			{
				mismaPrimitiva = false;
				break;
			}
		}
	}

	if (mismaPrimitiva)
	{
		ProcesarInterseccion(primitivas[0], rayosOrigen, rayosDireccion, colores, profundidad, distancias);
	}
	else
	{
		for (int t = 0; t < RAYOS_HORIZONTALES; t++)
		{
			Primitiva* prim = primitivas[t];

			if (prim != NULL)
				ProcesarInterseccion(prim, rayosOrigen[t], rayosDireccion[t], distancias[t], colores[t], profundidad);
		}
	}

#else

	//trace primary ray

	ResultadoRayo resultados[RAYOS_HORIZONTALES];
	bool mismaPrimitiva = false;

	// find the nearest intersections
	for (int s = 0; s < m_pEscena->GetCantidadPrimitivas(); s++)
	{
		Primitiva* pr = m_pEscena->GetPrimitiva(s);

		ResultadoRayo res[RAYOS_HORIZONTALES];

		int intersecciones;

#ifdef RAYTRACER_ESTADISTICAS
		m_Estadisticas.cantidadChequeosIntersecciones += RAYOS_HORIZONTALES;
#endif

		if (intersecciones = pr->Intersectan(rayosOrigen, rayosDireccion, distancias, res)) 
		{
			mismaPrimitiva = (intersecciones == RAYOS_HORIZONTALES);

			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				if (res[t] != ResultadoRayoFallo)
				{
					primitivas[t] = pr;
					resultados[t] = res[t];
				}
		}
	}
	
	// handle intersections
	if (mismaPrimitiva)
	{
		ProcesarInterseccion(primitivas[0], rayosOrigen, rayosDireccion, colores, profundidad, distancias);
	}
	else
	{
		for (int t = 0; t < RAYOS_HORIZONTALES; t++)
		{
			Primitiva* prim = primitivas[t];

			if (prim != NULL)
				ProcesarInterseccion(prim, rayosOrigen[t], rayosDireccion[t], distancias[t], colores[t], profundidad);
		}
	}
#endif
}

void Raytracer::ProcesarInterseccion(const Primitiva* prim, Vector3& rayoOrigen, Vector3& rayoDireccion, CoordType& distancia, Color& color, int profundidad)
{
	// determine color at point of intersection
	Vector3 pi = rayoOrigen + rayoDireccion * distancia;
	Vector3 N = prim->GetNormal(pi);

	// trace lights
	for (int l = 0; l < m_pEscena->GetCantidadLuces(); l++ )
	{
		Luz* luz = m_pEscena->GetLuz(l);

		// handle point light source
		CoordType shade = (CoordType) 1;

		#ifdef USAR_SOMBRAS

		if (true) //Usado para aislar las variables locales
		{
			Vector3 L = luz->GetPosicion() - pi;
			CoordType tdist = LENGTH( L );
			L *= (1.0f / tdist);
			Vector3 rOrigen = pi + L * EPSILON;
			Vector3 rDireccion = L;

#ifdef USAR_KDTREE

			if (m_pEscena->GetSeparador()->ColisionConAlguienIgnorando(prim, rOrigen, rDireccion, tdist))
				shade = 0;

			#ifdef RAYTRACER_ESTADISTICAS
				m_Estadisticas.cantidadRayosSecundarios++;
				m_Estadisticas.cantidadRayosSecundariosSimples++;
				m_Estadisticas.cantidadRayos++;
			#endif

#else

			for (int s = 0; s < m_pEscena->GetCantidadPrimitivas(); s++ )
			{
				Primitiva* pr = m_pEscena->GetPrimitiva(s);

				if (pr != prim)
				{
#ifdef RAYTRACER_ESTADISTICAS
					m_Estadisticas.cantidadChequeosIntersecciones++;
#endif
					
					if (pr->Intersecta(rOrigen, rDireccion, tdist))
					{
						shade = 0;
						break;
					}
				}
			}

#endif
		}

		#endif

		if (shade > 0)
		{
			Vector3 L = (luz->GetPosicion() - pi).normalize();
			CoordType dotLuzNormal = DotProduct(N, L);

			#ifdef USAR_DIFFUSE

			// calculate diffuse shading
			if (prim->GetMaterial().GetDiffuse() > (CoordType) 0)
			{
				if (dotLuzNormal > (CoordType) 0)
				{
					CoordType diff = dotLuzNormal * prim->GetMaterial().GetDiffuse() * shade;
					// add diffuse component to ray color
					color += diff * prim->GetMaterial().GetColor() * luz->GetMaterial().GetColor();
				}
			}

			#endif

			#ifdef USAR_SPECULAR

			// determine specular component
			if (prim->GetMaterial().GetSpecular() > (CoordType) 0)
			{
				// point light source: sample once for specular highlight
				Vector3 R = L - 2.0f * dotLuzNormal * N;
				CoordType dot = DOT(rayoDireccion, R );
				if (dot > (CoordType) 0 )
				{
					#ifndef USAR_SPECULAR_APROX
					CoordType spec = pow( dot, 20 ) * prim->GetMaterial().GetSpecular() * shade;
					#else
					CoordType spec = dot * prim->GetMaterial().GetSpecular() * shade / (50 - 50 * dot + dot);
					#endif

					// add specular component to ray color
					color += spec * luz->GetMaterial().GetColor();
				}
			}

			#endif
		}
	}

	#ifdef USAR_REFLECTION

	// calculate reflection
	CoordType refl = prim->GetMaterial().GetReflection();
	if (refl > 0.0f)
	{
		if (profundidad > 1) 
		{
			Vector3 R = rayoDireccion - 2.0f * DOT( rayoDireccion, N ) * N;

			Color rcol;
			CoordType dist;
			Vector3 rayoOrigen = pi + R * EPSILON;
			Vector3 rayoDireccion = R;
			
			TrazarRayo(rayoOrigen, rayoDireccion, rcol, profundidad - 1, dist);

			color += refl * rcol * prim->GetMaterial().GetColor();
		}
	}

	#endif
}

void Raytracer::ProcesarInterseccion(const Primitiva* prim, Vector3 (&rayosOrigen)[RAYOS_HORIZONTALES], Vector3 (&rayosDireccion)[RAYOS_HORIZONTALES], Color (&colores)[RAYOS_HORIZONTALES], int profundidad, CoordType (&distancias)[RAYOS_HORIZONTALES])
{
	Vector3* raysOrig = rayosOrigen;
	Vector3* raysDir = rayosDireccion;

	// determine color at point of intersection

	Vector3 _MM_ALIGN16 pis[RAYOS_HORIZONTALES];
	Vector3 _MM_ALIGN16 Ns[RAYOS_HORIZONTALES];

	for (int t = 0; t < RAYOS_HORIZONTALES; t++)
		Ns[t] = prim->GetNormal(pis[t] = rayosOrigen[t] + rayosDireccion[t] * distancias[t]);

	//Cache de intersecciones por prim

	// trace lights
	for (int l = 0; l < m_pEscena->GetCantidadLuces(); l++ )
	{
		Luz* luz = m_pEscena->GetLuz(l);

		// handle point light source
		CoordType _MM_ALIGN16 shades[RAYOS_HORIZONTALES];
		for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			shades[t] = 1.0f;

		bool allShaded = false;

		#ifdef USAR_SOMBRAS

		if (true) //Usado para aislar las variables locales
		{
			Vector3 _MM_ALIGN16 rSecundarioOrigen[RAYOS_HORIZONTALES];
			Vector3 _MM_ALIGN16 rSecundarioDireccion[RAYOS_HORIZONTALES];
			CoordType rSecundarioDist[RAYOS_HORIZONTALES];

			//TODO: Ver si puedo hacer que el compilador vectorice esto!
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				Vector3 L = luz->GetPosicion() - pis[t];
				CoordType tdist = LENGTH( L );
				L *= (1.0f / tdist);

				rSecundarioOrigen[t] = pis[t] + L * EPSILON;
				rSecundarioDireccion[t] = L;
				rSecundarioDist[t] = tdist;
			}

			int totalShaded = 0;

			#ifdef USAR_KDTREE
			
			bool colisiones[RAYOS_HORIZONTALES];

			memset(colisiones, 0, sizeof(colisiones));

			m_pEscena->GetSeparador()->ColisionConAlguienIgnorando(prim, rSecundarioOrigen, rSecundarioDireccion, rSecundarioDist, colisiones);

			for (int n = 0; n < RAYOS_HORIZONTALES; n++)
			{
				if (colisiones[n])
				{
					shades[n] = 0;
					totalShaded++;
				}
			}

			#ifdef RAYTRACER_ESTADISTICAS
				m_Estadisticas.cantidadRayosSecundarios+=RAYOS_HORIZONTALES;
				m_Estadisticas.cantidadRayosSecundariosMultiples+=RAYOS_HORIZONTALES;
				m_Estadisticas.cantidadRayos+=RAYOS_HORIZONTALES;
			#endif

			#else
			
			for (int s = 0; s < m_pEscena->GetCantidadPrimitivas(); s++)
			{
				Primitiva* pr = m_pEscena->GetPrimitiva(s);
				
				ResultadoRayo resultados[RAYOS_HORIZONTALES];

				int n;

#ifdef RAYTRACER_ESTADISTICAS
				m_Estadisticas.cantidadChequeosIntersecciones++;
#endif

				if (pr != prim && (n = pr->Intersectan(rSecundarioOrigen, rSecundarioDireccion, rSecundarioDist, resultados)))
				{
					if (n == RAYOS_HORIZONTALES)
					{
						allShaded = true;
						break;
					}
					else
					{
						for (int t = 0; t < RAYOS_HORIZONTALES; t++)
							if (resultados[t] != 0 && shades[t] != 0)
							{
								totalShaded++;
								shades[t] = 0;
							}
					}
				}
			}

			#endif

			if (totalShaded == RAYOS_HORIZONTALES)
				allShaded = true;
		}

		#endif

		if (allShaded == false)
		{
			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				CoordType shade = shades[t];

				if (shade > 0)
				{
					Vector3& pi = pis[t];
					Vector3& N = Ns[t];
					Vector3& rayoOrigen = rayosOrigen[t];
					Vector3& rayoDireccion = rayosDireccion[t];

					Vector3 L = (luz->GetPosicion() - pi).normalize();
					CoordType dotLuzNormal = DotProduct(N, L );
					Vector3 color;

					#ifdef USAR_DIFFUSE

					// calculate diffuse shading
					if (prim->GetMaterial().GetDiffuse() > (CoordType) 0)
					{
						if (dotLuzNormal > (CoordType) 0)
						{
							CoordType diff = dotLuzNormal * prim->GetMaterial().GetDiffuse() * shade;
							// add diffuse component to ray color
							color += diff * prim->GetMaterial().GetColor() * luz->GetMaterial().GetColor();
						}
					}

					#endif

					#ifdef USAR_SPECULAR

					// determine specular component
					if (prim->GetMaterial().GetSpecular() > 0)
					{
						// point light source: sample once for specular highlight
						Vector3 R = L - 2.0f * dotLuzNormal * N;
						CoordType dot = DOT(rayoDireccion, R );
						if (dot > (CoordType) 0)
						{
							#ifndef USAR_SPECULAR_APROX
							CoordType spec = pow( dot, 20 ) * prim->GetMaterial().GetSpecular() * shade;
							#else
							CoordType spec = dot * prim->GetMaterial().GetSpecular() * shade / (50 - 50 * dot + dot);
							#endif

							// add specular component to ray color
							color += spec * luz->GetMaterial().GetColor();
						}
					}

					#endif

					colores[t] += color;
				}
			}
		}			
	}

	#ifdef USAR_REFLECTION

	// calculate reflection
	CoordType refl = prim->GetMaterial().GetReflection();
	if (refl > 0.0f)
	{
		if (profundidad > 1) 
		{
			Vector3 _MM_ALIGN16 rayosReflejadosOrigen[RAYOS_HORIZONTALES];
			Vector3 _MM_ALIGN16 rayosReflejadosDireccion[RAYOS_HORIZONTALES];
			Vector3 _MM_ALIGN16 coloresReflejados[RAYOS_HORIZONTALES];
			CoordType distanciasReflejadas[RAYOS_HORIZONTALES];
			Primitiva* primitivasReflejadas[RAYOS_HORIZONTALES];

			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
			{
				Vector3& N = Ns[t];
				Vector3& rayoDireccion = rayosDireccion[t];

				rayosReflejadosOrigen[t] = pis[t] + (rayosReflejadosDireccion[t] = (rayoDireccion - 2.0f * DOT( rayoDireccion, N ) * N)) * EPSILON;
			}

			TrazarRayos(rayosReflejadosOrigen, rayosReflejadosDireccion, coloresReflejados, profundidad - 1, distanciasReflejadas, primitivasReflejadas);

			Vector3 constante = refl * prim->GetMaterial().GetColor();

			for (int t = 0; t < RAYOS_HORIZONTALES; t++)
				colores[t] += constante * coloresReflejados[t];
		}
	}

	#endif
}
