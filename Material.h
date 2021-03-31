#pragma once
#include <GL/gl.h>

class Material
{
private:
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;

public:
	Material(
		float ambi_r, float ambi_g, float ambi_b,
		float diff_r, float diff_g, float diff_b,
		float spec_r, float spec_g, float spec_b,
		float shin);
	~Material();
	void ativar();
};
