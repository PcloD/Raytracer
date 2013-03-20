#include "SDL.h"
#include "Pantalla.h"
#include <stdio.h>
#include "Raytracer.h"
#include <windows.h>

bool g_bSalir = false;
SDL_sem* g_SemaforoDibujado = NULL;
bool g_bModoScreenSaver = false;
char g_pcRunningPath[512];

//casa:		7.99 fps (compilador INTEL / SSE2) (la mitad en el de MS)

//oficina:  4.677 fps

#define HILOS 2
#define BALANCEAR_HILOS

#define ESPERA_MAXIMO_HILO 20

#define RESOLUCION_ANCHO	640 //640
#define RESOLUCION_ALTO		480 //480

#define FULLSCREEN false

#define VELOCIDAD_ROTACION 90
#define VELOCIDAD_AVANCE 10



void Procesar();

int main(int argc, char *argv[]) 
{
	strcpy(g_pcRunningPath, argv[0]);

	for (int i = strlen(g_pcRunningPath) - 1; i >= 0; i--)
		if (g_pcRunningPath[i] == '\\' || 
			g_pcRunningPath[i] == '/')
		{
			g_pcRunningPath[i + 1] = '\0';
			break;
		}

	for (int i = 1; i < argc; i++)
		if (_stricmp(argv[i], "/s") == 0 ||
			_stricmp(argv[i], "-s") == 0)
		{
			g_bModoScreenSaver = true;
		}

	if (argc > 1 && g_bModoScreenSaver == false)
		return 0; //No se reconocio el parametro, no hago nada por las dudas

    printf("Initializing SDL.\n");
    
    if((SDL_Init(SDL_INIT_VIDEO/*|SDL_INIT_AUDIO*/)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

    printf("SDL initialized.\n");

	Procesar();

    printf("Quiting SDL.\n");
    
    /* Shutdown all subsystems */
    SDL_Quit();
    
    printf("Quiting....\n");

    exit(0);
}

struct InfoHilo
{
	SDL_Thread* id;
	SDL_sem* semaforo;
	int desdeY;
	int hastaY;
	Pantalla* pantalla;
	Escena* escena;
	Camera* camara;
	int tiempo;
};

int hiloDibujado(void* p)
{
	InfoHilo* pInfoHilo = (InfoHilo*) p;

	Raytracer raytracer;

	SDL_SemWait(pInfoHilo->semaforo);

	while(g_bSalir == false) 
	{
		int inicio = SDL_GetTicks();
		raytracer.Dibujar(pInfoHilo->escena, *pInfoHilo->pantalla, *pInfoHilo->camara, pInfoHilo->desdeY, pInfoHilo->hastaY);
		pInfoHilo->tiempo = SDL_GetTicks() - inicio;

		SDL_SemPost(g_SemaforoDibujado);
		SDL_SemWait(pInfoHilo->semaforo);
	}

	return 0;
}

void ProcesarTeclado(Camera& camara, float tiempoEvento)
{
	Uint8 *keystate = SDL_GetKeyState(NULL);
	
	if (keystate[SDLK_LEFT])
		camara.RotateY(tiempoEvento * -VELOCIDAD_ROTACION);

	if (keystate[SDLK_RIGHT])
		camara.RotateY(tiempoEvento * VELOCIDAD_ROTACION);

	if (keystate[SDLK_UP])
		camara.MoveForwards(tiempoEvento * VELOCIDAD_AVANCE);

	if (keystate[SDLK_DOWN])
		camara.MoveForwards(tiempoEvento * -VELOCIDAD_AVANCE);

	if (keystate[SDLK_PAGEUP])
		camara.RotateX(tiempoEvento * -VELOCIDAD_ROTACION);

	if (keystate[SDLK_PAGEDOWN])
		camara.RotateX(tiempoEvento * VELOCIDAD_ROTACION);
}

void ProcesarDemo(Camera& camara, Escena& escena, float tiempoEvento)
{
	static float anguloRotacionCamaraTotal = 0.0f;
	static float deltaLuz = -1000.0f;
	static float direccionLuz = 1;
	static Vector3 posicionInicialLuz1;
	static Vector3 posicionInicialLuz2;

	//Roto camara
	anguloRotacionCamaraTotal += 8.0f * tiempoEvento;

	if (anguloRotacionCamaraTotal > 360.0f)
		anguloRotacionCamaraTotal -= 360.0f;

	Vector3 mirarA(0, -7, 16);
	float mirarDitancia = 24;
	
	float x = -mirarA.x + mirarDitancia * sin(DegToRad(anguloRotacionCamaraTotal));
	float z = -mirarA.z + mirarDitancia * cos(DegToRad(anguloRotacionCamaraTotal));

	camara.SetPosition(Vector3(x, mirarA.y, z));
	camara.SetRotation(Vector3(15.0f, anguloRotacionCamaraTotal, 0));

	//Muevo luces
	if (deltaLuz == -1000.0f)
	{
		posicionInicialLuz1 = escena.GetLuz(0)->GetPosicion();
		posicionInicialLuz2 = escena.GetLuz(1)->GetPosicion();
		deltaLuz = 0;
	}

	escena.GetLuz(0)->SetPosition(posicionInicialLuz1 + Vector3(0,0,10) * deltaLuz);
	escena.GetLuz(1)->SetPosition(posicionInicialLuz2 - Vector3(0,0,10) * deltaLuz);

	deltaLuz += tiempoEvento * direccionLuz;

	if (deltaLuz > 4.0f)
	{
		deltaLuz = 4.0f;
		direccionLuz = -direccionLuz;
	}
	else if (deltaLuz < -4.0f)
	{
		deltaLuz = -4.0f;
		direccionLuz = -direccionLuz;
	}

}

void Procesar()
{
	Pantalla pantalla;
	Raytracer raytracer;
	Escena escena;
	Camera camara;

	if (g_bModoScreenSaver)
		escena.CargarEscenaScreenSaver();
	else
		escena.CargarEscenaTest();

	if (g_bModoScreenSaver)
		pantalla.Inicializar(320, 240, 32, true);
	else
		pantalla.Inicializar(RESOLUCION_ANCHO, RESOLUCION_ALTO, 32, FULLSCREEN);

	#ifdef KDTREE_ESTADISTICAS
	memset(&g_kdTreeEstadisticas, 0, sizeof(g_kdTreeEstadisticas));
	#endif

	SDL_Event event;

	#ifdef HILOS

	g_SemaforoDibujado = SDL_CreateSemaphore(0);

	InfoHilo infoHilos[HILOS];

	//creo los hilos
	int desdeY = 0;
	for (int hilo = 0; hilo < HILOS; hilo++)
	{
		infoHilos[hilo].desdeY = desdeY;
		desdeY += pantalla.GetAlto() / HILOS;
		infoHilos[hilo].hastaY = desdeY ;
		infoHilos[hilo].camara = &camara;
		infoHilos[hilo].pantalla = &pantalla;
		infoHilos[hilo].escena = &escena;
		infoHilos[hilo].semaforo = SDL_CreateSemaphore(0);

		infoHilos[hilo].id = SDL_CreateThread(&hiloDibujado, &infoHilos[hilo]);
	}

	#endif

	LARGE_INTEGER tiempoHighResolution;
	LARGE_INTEGER frecuenciaHighResolution;

	int tiempo = SDL_GetTicks();
	int tiempoInicioEvento = SDL_GetTicks();
	int frames = 0;
	float ultimosFPS = 0;

	QueryPerformanceCounter(&tiempoHighResolution);
	QueryPerformanceFrequency(&frecuenciaHighResolution);

	#ifdef HILOS

	pantalla.IniciarDibujado();

	for (int hilo = 0; hilo < HILOS; hilo++)
		SDL_SemPost(infoHilos[hilo].semaforo);

	int hilosDibujadoTerminados = 0;

	#endif

	while(g_bSalir == false)
	{
		while(SDL_PollEvent(&event))
		{  
			switch(event.type)
			{  
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							g_bSalir = true;
							break;
					}
					break;

				case SDL_QUIT:
					g_bSalir = true;
					break;
			}
		}

		if (!g_bSalir)
		{
			float tiempoEvento = (float) (SDL_GetTicks() - tiempoInicioEvento) / 1000.0f;
			tiempoInicioEvento = SDL_GetTicks();

			if (g_bModoScreenSaver)
				ProcesarDemo(camara, escena, tiempoEvento);
			else
				ProcesarTeclado(camara, tiempoEvento);

			#ifndef HILOS

			pantalla.IniciarDibujado();

			raytracer.Dibujar(&escena, pantalla, camara, 0, pantalla.GetAlto());

			if (g_bModoScreenSaver)
			{
				char pcTemp[512];

				sprintf(pcTemp, "FPS: %.3f\n", ultimosFPS);

				pantalla.DibujarTexto(0, 0, pcTemp);
			}

			pantalla.FinalizarDibujado(0, pantalla.GetAlto());

			frames++;

			#else

			for (int hilo = hilosDibujadoTerminados; hilo < HILOS; hilo++)
			{
				if (SDL_SemWaitTimeout(g_SemaforoDibujado, ESPERA_MAXIMO_HILO / HILOS) == 0)
					hilosDibujadoTerminados++;
				/*else
					break;*/
			}

			if (hilosDibujadoTerminados == HILOS)
			{
				if (g_bModoScreenSaver)
				{
					char pcTemp[512];

					sprintf(pcTemp, "FPS: %.3f\n", ultimosFPS);

					pantalla.DibujarTexto(0, 0, pcTemp);
				}

				pantalla.FinalizarDibujado(0, pantalla.GetAlto());

				hilosDibujadoTerminados = 0;

				#if HILOS == 2 && defined(BALANCEAR_HILOS)
				
				//Rebalanceo de la carga entre los hilos!!

				if (infoHilos[0].tiempo != infoHilos[1].tiempo)
				{
					int tiempoTotal = infoHilos[0].tiempo + infoHilos[1].tiempo;

					int tiempoPromedio = tiempoTotal / 2;

					int lineasHilo0 = infoHilos[0].hastaY;
					int lineasHilo1 = pantalla.GetAlto() - infoHilos[0].hastaY;

					if (infoHilos[0].tiempo == 0 || infoHilos[1].tiempo == 0)
					{
						lineasHilo0 = pantalla.GetAlto() / 2;
					}
					else
					{
						//Incremento la carga del hilo que tardó menos en forma proporcional con la diferencia
						//respecto del promedio
						if (infoHilos[0].tiempo > infoHilos[1].tiempo)
						{
							//El primer hilo tardó mas
							float diferencia = (float) infoHilos[0].tiempo / (float) tiempoPromedio;
							lineasHilo1 += (int) ((float) lineasHilo1 * (diferencia - 1.0f));
							
							lineasHilo0 = pantalla.GetAlto() - lineasHilo1;
						}
						else
						{
							//El segundo hilo tardó mas
							float diferencia = (float) infoHilos[1].tiempo / (float) tiempoPromedio;
							lineasHilo0 += (int) ((float) lineasHilo1 * (diferencia - 1.0f));
						}
					}

					//printf("Estadisticas:\n");
					//printf("\thilo1: %dms %d lineas\n", infoHilos[0].tiempo, infoHilos[0].hastaY - infoHilos[0].desdeY);
					//printf("\thilo2: %dms %d lineas\n", infoHilos[1].tiempo, infoHilos[1].hastaY - infoHilos[1].desdeY);

					//printf("diferencia hilo1 - hilo2: %dms\n", infoHilos[0].tiempo - infoHilos[1].tiempo);

					 //Me aseguro de que la cantidad de lineas que dibuja cada hilo
					//sea múltiplo de la cantidad de lineas verticales
					lineasHilo0 = (lineasHilo0 / RAYOS_VERTICALES) * RAYOS_VERTICALES;

					infoHilos[0].hastaY = lineasHilo0;
					infoHilos[1].desdeY = lineasHilo0;
				}
				
				#endif

				pantalla.IniciarDibujado();

				for (int hilo = 0; hilo < HILOS; hilo++)
					SDL_SemPost(infoHilos[hilo].semaforo);

				SDL_Delay(1);

				frames++;
			}

			#endif

			int ahora = SDL_GetTicks();

			if (ahora > tiempo + 1000)
			{
				LARGE_INTEGER nuevoTiempoHighResolution;

				QueryPerformanceCounter(&nuevoTiempoHighResolution);

				double intervalo = (nuevoTiempoHighResolution.QuadPart - tiempoHighResolution.QuadPart) / (double) frecuenciaHighResolution.QuadPart;
				float fps = frames / intervalo;

				printf("FPS: %.3f\n", fps);

				ultimosFPS = fps;

#ifdef RAYTRACER_ESTADISTICAS
#ifndef HILOS
				Raytracer::Estadisticas est = raytracer.ObtenerEstadisticas();

				printf("Total Rayos p/frame: %d, Prim: %d, Sec: %d, SecSimp: %d, SecMult: %d\n", 
					est.cantidadRayos / frames,
					est.cantidadRayosPrincipales / frames,
					est.cantidadRayosSecundarios / frames,
					est.cantidadRayosSecundariosSimples / frames,
					est.cantidadRayosSecundariosMultiples / frames
					);

				printf("Total intersecciones p/frame: %d\n",
					est.cantidadChequeosIntersecciones / frames);

				raytracer.ResetearEstadisticas();
#endif
#endif

#ifdef KDTREE_ESTADISTICAS
#ifndef HILOS
				printf("KdTree - Divisiones Conjuntas: %d, Unicas: %d, Fallidas: %d\n", 
					g_kdTreeEstadisticas.cantidadDivisionesConjuntas / frames,
					g_kdTreeEstadisticas.cantidadDivisionesUnicas / frames,
					g_kdTreeEstadisticas.cantidadDivisionesFallidas / frames
					);
								
				memset(&g_kdTreeEstadisticas, 0, sizeof(g_kdTreeEstadisticas));
#endif
#endif

				QueryPerformanceCounter(&nuevoTiempoHighResolution);

				frames = 0;
				tiempo = ahora;
				tiempoHighResolution = nuevoTiempoHighResolution;
			}
		}
	}

#ifdef HILOS
	for (int hilo = 0; hilo < HILOS; hilo++)
		SDL_SemPost(infoHilos[hilo].semaforo);

	for (int hilo = 0; hilo < HILOS; hilo++)
		SDL_WaitThread(infoHilos[hilo].id, NULL);
#endif
}

