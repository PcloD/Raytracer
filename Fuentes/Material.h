#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Common.h"

class Material
{
private:
	Color m_Color;
	CoordType m_Diff, m_Spec;
	CoordType m_Refl;

public:
	Material() : m_Color((CoordType) 0.2f, (CoordType) 0.2f, (CoordType) 0.2f), m_Diff((CoordType) 0.2f), m_Spec((CoordType) 0.8f ), m_Refl((CoordType) 0.0f) { }
	Material(Color color, CoordType diff, CoordType spec, CoordType refl) : m_Color(color), m_Diff(diff), m_Spec(spec), m_Refl(refl) { }

	void SetColor(Color color) { m_Color = color; }
	Color GetColor() const { return m_Color; }

	void SetReflection(CoordType refl) { m_Refl = refl; }
	CoordType GetReflection() { return m_Refl; }

	void SetDiffuse(CoordType diff) { m_Diff = diff; }
	CoordType GetDiffuse() const { return m_Diff; }

	void SetSpecular(CoordType spec) { m_Spec = spec; }
	CoordType GetSpecular() const { return m_Spec; }

	void Reset() { m_Color.set((CoordType) 0.2f, (CoordType) 0.2f, (CoordType) 0.2f); m_Diff = 0.2f;  m_Spec = 0.8f; m_Refl = 0.0f; }
};

#endif
