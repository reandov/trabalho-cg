#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include"Vetor.h"
#include"Material.h"

struct face {
	unsigned index;
	int vertice[4];

	face(int a, int b, int c, int d, int e) {
		vertice[0] = a; vertice[1] = b; 
		vertice[2] = c; vertice[3] = d;
		index = e;
	}
};

namespace ObjLoader {
	vec3 getVertice(std::string s);
	vec3 getNormal(std::string s);
	face getFace(std::string s);
	void loadOBJ(unsigned& id, const char* filePath);
};