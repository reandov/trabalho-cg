#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector.h"
#include "Material.h"
#include "Texture.h"

// Basic enum to differentiate between triangles and quads
enum Poligon
{
	TRIANG = 3,
	QUAD = 4
};

// Face structure
struct face
{
	Poligon type;
	int vertice[4];
	int texCoord[4];
	int normal[4];

	face(Poligon type_,
		 int v1, int v2, int v3, int v4,
		 int vt1, int vt2, int vt3, int vt4,
		 int n1, int n2, int n3, int n4)
	{
		vertice[0] = v1;
		vertice[1] = v2;
		vertice[2] = v3;
		vertice[3] = v4;
		texCoord[0] = vt1;
		texCoord[1] = vt2;
		texCoord[2] = vt3;
		texCoord[3] = vt4;
		normal[0] = n1;
		normal[1] = n2;
		normal[2] = n3;
		normal[3] = n4;
		type = type_;
	}
};

// ObjLoader namespace
namespace ObjLoader
{
	vec3 getVertice(std::string s);
	vec2 getTexVertice(std::string s);
	vec3 getNormal(std::string s);
	face getFace(Poligon tipo_, std::string s);
	void loadOBJ(unsigned &id, Texture *texture, const char *filePath);
};
