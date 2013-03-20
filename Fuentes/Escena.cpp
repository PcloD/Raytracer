#include "Escena.h"
#include "Primitiva.h"
#include "Esfera.h"
#include "Luz.h"
#include "Plano.h"

#define FUENTES_LUZ_VISIBLES

Escena::Escena()
{
	m_MaximoPrimitivas = 10000;
	m_Primitivas = new Primitiva*[m_MaximoPrimitivas];
	m_CantidadPrimitivas = 0;

	m_MaximoLuces = 1000;
	m_Luces = new Luz*[m_MaximoLuces];
	m_CantidadLuces = 0;

	//Creo todos los kd trees juntos en un solo vector para maximizar la coherencia de memoria
	m_pSeparadores = new Separador[10000];
	m_pSeparadores[0].Construir(1000.0f);
}

Escena::~Escena()
{
	delete[] m_pSeparadores;
}

void Escena::CargarEscenaTest()
{
	Material mat;

	// ground plane
	Primitiva* pPlano = Primitiva::NuevoPlano(Vector3( (CoordType) 0, (CoordType) 1, (CoordType) 0 ), (CoordType) 4.4f );
	mat.Reset();
	mat.SetReflection( (CoordType) 0 );
	mat.SetDiffuse( (CoordType) 1.0f );
	mat.SetSpecular( (CoordType) 0.0f );
	mat.SetColor( Color( (CoordType) 0.4f, (CoordType) 0.3f, (CoordType) 0.3f ) );
	pPlano->SetMaterial(mat);
	AgregarPrimitiva(pPlano);

	// background plane
	pPlano = Primitiva::NuevoPlano(Vector3( (CoordType) 0, (CoordType) 0, (CoordType) -1 ), (CoordType) 20.4f );
	mat.Reset();
	mat.SetReflection( (CoordType) 0 );
	mat.SetSpecular( (CoordType) 0.0f );
	mat.SetColor( Color( (CoordType) 1.0f, (CoordType) 0.3f, (CoordType) 0.3f ) );
	pPlano->SetMaterial(mat);
	AgregarPrimitiva(pPlano);

	// big sphere
	Primitiva* pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) 1, (CoordType) -0.8f, (CoordType) 3), (CoordType) 2.5f);
	mat.Reset();
	mat.SetReflection( (CoordType) 0.6f );
	mat.SetColor(Color((CoordType) 0.7f, (CoordType) 0.7f, (CoordType) 0.7f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);
	
	// small sphere
	pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) -5.5f, (CoordType) -0.5f, (CoordType) 7 ), (CoordType) 2);
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f );
	mat.SetDiffuse( (CoordType) 0.1f );
	mat.SetColor(Color( (CoordType) 0.7f, (CoordType) 0.7f, (CoordType) 1.0f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);

	for (int x = -9; x <= 9; x+= 9)
	{
		Primitiva* pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) x, (CoordType) 3.5f, (CoordType) 7 ), (CoordType) 2);
		mat.Reset();
		mat.SetReflection( (CoordType) 1.0f );
		mat.SetDiffuse( (CoordType) 0.1f );
		mat.SetColor(Color( (CoordType) 0.7f, (CoordType) 0.7f, (CoordType) 1.0f));
		pEsfera->SetMaterial(mat);
		AgregarPrimitiva(pEsfera);
	}
	

	// light source 1
	Luz* pLuz = new Luz(Vector3((CoordType) 0, (CoordType) 5, (CoordType) 5 ));
	mat.Reset();
	mat.SetColor(Color( (CoordType) 0.6f, (CoordType) 0.6f, (CoordType) 0.6f));
	pLuz->SetMaterial(mat);
	AgregarLuz(pLuz);

	#ifdef FUENTES_LUZ_VISIBLES
	//AgregarPrimitiva(pLuz);
	#endif

	// light source 2
	pLuz = new Luz(Vector3((CoordType) 2, (CoordType) 5, (CoordType) 1 ));
	mat.Reset();
	mat.SetColor(Color((CoordType) 0.7f, (CoordType) 0.7f, (CoordType) 0.9f));
	pLuz->SetMaterial(mat);
	AgregarLuz(pLuz);

	#ifdef FUENTES_LUZ_VISIBLES
	//AgregarPrimitiva(pLuz);
	#endif

	Separador* separadores = m_pSeparadores + 1;

	GetSeparador()->Optimizar(separadores);
}

void Escena::CargarEscenaTest2()
{
	Material mat;

	for (float x = -30; x <= 30; x+=10)
	{
		for (float y = -30; y <= 30; y+=10)
		{
			for (float z = -30; z <= 30; z+=10)
			{
				Primitiva* pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) x, (CoordType) y, (CoordType) z), (CoordType) 2);
				mat.Reset();
				mat.SetReflection( (CoordType) 1.0f );
				mat.SetDiffuse( (CoordType) 0.1f );
				mat.SetColor(Color( (CoordType) 0.7f, (CoordType) 0.7f, (CoordType) 1.0f));
				pEsfera->SetMaterial(mat);
				AgregarPrimitiva(pEsfera);
			}
		}
	}

	// light source 1
	Luz* pLuz = new Luz(Vector3((CoordType) 0, (CoordType) 35, (CoordType) -30 ));
	mat.Reset();
	mat.SetColor(Color( (CoordType) 0.6f, (CoordType) 0.6f, (CoordType) 0.6f));
	pLuz->SetMaterial(mat);
	AgregarLuz(pLuz);

	// light source 2
	pLuz = new Luz(Vector3((CoordType) 0, (CoordType) 35, (CoordType) 30 ));
	mat.Reset();
	mat.SetColor(Color((CoordType) 0.7f, (CoordType) 0.7f, (CoordType) 0.9f));
	pLuz->SetMaterial(mat);
	AgregarLuz(pLuz);

	Separador* separadores = m_pSeparadores + 1;

	GetSeparador()->Optimizar(separadores);
}

void Escena::CargarEscenaScreenSaver()
{
	Material mat;
	Primitiva* pPlano;
	Primitiva* pEsfera;
	Luz* pLuz;

	// ground plane
	pPlano = Primitiva::NuevoPlano(Vector3( (CoordType) 0, (CoordType) 1, (CoordType) 0 ), (CoordType) 4.4f );
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f);
	//mat.SetDiffuse( (CoordType) 1.0f );
	//mat.SetSpecular( (CoordType) 0.5f );
	mat.SetColor( Color( (CoordType) 0.8f, (CoordType) 0.8f, (CoordType) 0.8f ) );
	pPlano->SetMaterial(mat);
	AgregarPrimitiva(pPlano);

	// big sphere
	pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) 1, (CoordType) -0.8f, (CoordType) 3), (CoordType) 2.5f);
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f );
	mat.SetColor(Color((CoordType) 0.9f, (CoordType) 0.0f, (CoordType) 0.0f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);
	
	// small sphere
	pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) -5.5f, (CoordType) -0.5f, (CoordType) 7 ), (CoordType) 2);
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f );
	mat.SetDiffuse( (CoordType) 0.1f );
	mat.SetColor(Color( (CoordType) 0.9f, (CoordType) 0.9f, (CoordType) 0.0f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);

	pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) -9.0f, (CoordType) 3.5f, (CoordType) 7 ), (CoordType) 2);
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f );
	mat.SetDiffuse( (CoordType) 0.1f );
	mat.SetColor(Color( (CoordType) 0.0f, (CoordType) 0.9f, (CoordType) 0.0f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);
	
	pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) 0.0f, (CoordType) 3.5f, (CoordType) 7 ), (CoordType) 2);
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f );
	mat.SetDiffuse( (CoordType) 0.1f );
	mat.SetColor(Color( (CoordType) 0.0f, (CoordType) 0.9f, (CoordType) 0.9f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);

	pEsfera = Primitiva::NuevaEsfera(Vector3((CoordType) 9.0f, (CoordType) 3.5f, (CoordType) 7 ), (CoordType) 2);
	mat.Reset();
	mat.SetReflection( (CoordType) 1.0f );
	mat.SetDiffuse( (CoordType) 0.1f );
	mat.SetColor(Color( (CoordType) 0.9f, (CoordType) 0.0f, (CoordType) 0.9f));
	pEsfera->SetMaterial(mat);
	AgregarPrimitiva(pEsfera);

	// light source 1
	pLuz = new Luz(Vector3((CoordType) 0, (CoordType) 15, (CoordType) -10 ));
	mat.Reset();
	mat.SetColor(Color((CoordType) 1.9f, (CoordType) 1.9f, (CoordType) 1.9f));
	pLuz->SetMaterial(mat);
	AgregarLuz(pLuz);

	// light source 2
	pLuz = new Luz(Vector3((CoordType) 0, (CoordType) 15, (CoordType) 17 ));
	mat.Reset();
	mat.SetColor(Color((CoordType) 1.9f, (CoordType) 1.9f, (CoordType) 1.9f));
	pLuz->SetMaterial(mat);
	AgregarLuz(pLuz);
}
