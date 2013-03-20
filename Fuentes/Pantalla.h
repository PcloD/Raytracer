#ifndef __PANTALLA_H__
#define __PANTALLA_H__

#include "SDL.h"
#include "Font.hpp"

class Pantalla
{
private:
	SDL_Surface *m_Screen;
	SFont *m_Font;

public:
	Pantalla();
	~Pantalla();

public:
	void Inicializar(int ancho, int alto, int bpp, bool fullscreen);
	void Finalizar();

	void IniciarDibujado();
	void FinalizarDibujado(int desdeY, int hastaY);
	
	int GetAncho() { return m_Screen->w; }
	int GetAlto() { return m_Screen->h; }
	
	void DibujarPixel(int x, int y, int r, int g, int b);

	void DibujarTexto(int x, int y, const char* pcTexto);
};

#endif
