#include "Pantalla.h"
#include "SDL_Image.h"

extern char g_pcRunningPath[512];

Pantalla::Pantalla()
{
	m_Screen = NULL;

	char pcPathCompleto[1024];

	sprintf(pcPathCompleto, "%s%s", g_pcRunningPath, "12P_Copperplate_Blue.png");

	SDL_Surface* pFont = IMG_Load(pcPathCompleto);

	m_Font = new SFont(pFont);
}

Pantalla::~Pantalla()
{
}

void Pantalla::Inicializar(int ancho, int alto, int bpp, bool fullscreen)
{
	if (fullscreen)
	{
		m_Screen = SDL_SetVideoMode(ancho, alto, bpp, SDL_SWSURFACE|SDL_ANYFORMAT|SDL_FULLSCREEN);
		SDL_ShowCursor(0);
	}
	else
		m_Screen = SDL_SetVideoMode(ancho, alto, bpp, SDL_SWSURFACE|SDL_ANYFORMAT);

    if ( m_Screen == NULL ) 
	{
        fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
			ancho,
			alto,
			bpp,
			SDL_GetError());
        exit(1);
    }

    printf("Set %dx%d at %d bits-per-pixel mode\n",
		m_Screen->w,
		m_Screen->h,
		m_Screen->format->BitsPerPixel);
}

void Pantalla::Finalizar()
{
}

void Pantalla::IniciarDibujado()
{
    /* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(m_Screen) ) {
        if ( SDL_LockSurface(m_Screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }
}

void Pantalla::FinalizarDibujado(int desdeY, int hastaY)
{
    if ( SDL_MUSTLOCK(m_Screen) ) {
        SDL_UnlockSurface(m_Screen);
    }
    /* Update just the part of the display that we've changed */
    SDL_UpdateRect(m_Screen, 0, desdeY, m_Screen->w, hastaY - desdeY);
}

void Pantalla::DibujarPixel(int x, int y, int r, int g, int b)
{
	//int pixel = SDL_MapRGB(m_Screen->format, r, g, b);

	int pixel = (r << 16) | (g << 8) | (b << 0);
    
	int bpp = m_Screen->format->BytesPerPixel;

	Uint8 *p = (Uint8 *)m_Screen->pixels + y * m_Screen->pitch + x * bpp;

    /*switch(bpp) 
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:*/
			*(Uint32 *)p = pixel;
	/*		break;
    }*/
}

void Pantalla::DibujarTexto(int x, int y, const char* pcTexto)
{
	m_Font->write(m_Screen, pcTexto, x, y);
}
