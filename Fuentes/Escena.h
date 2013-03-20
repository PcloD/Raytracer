#ifndef __ESCENA_H__
#define __ESCENA_H__

#include "Common.h"
#include <vector>
#include "Primitiva.h"
#include "Luz.h"
#include "KdTree.h"

typedef KdTree Separador;

class Escena
{
private:
	Primitiva** m_Primitivas;
	int m_MaximoPrimitivas;
	int m_CantidadPrimitivas;

	Luz** m_Luces;
	int m_MaximoLuces;
	int m_CantidadLuces;
	
	Separador* m_pSeparadores;

public:
	Escena();
	~Escena();

public:
	void AgregarPrimitiva(Primitiva* primitiva) { m_pSeparadores[0].AgregarPrimitiva(primitiva); m_Primitivas[m_CantidadPrimitivas++] = primitiva; }
	inline int GetCantidadPrimitivas() { return m_CantidadPrimitivas; }
	inline Primitiva* GetPrimitiva(int n) { return m_Primitivas[n]; }

	void AgregarLuz(Luz* luz) { m_Luces[m_CantidadLuces++] = luz; }
	inline int GetCantidadLuces() { return m_CantidadLuces; }
	inline Luz* GetLuz(int n) { return m_Luces[n]; }

	void CargarEscenaTest();
	void CargarEscenaTest2();
	void CargarEscenaScreenSaver();

	Separador* GetSeparador() { return &m_pSeparadores[0]; }
};

#endif
