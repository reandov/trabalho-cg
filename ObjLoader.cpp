#include "ObjLoader.h"
#include<vector>

vec3 ObjLoader::getVertice(std::string s)
{
	float x, y, z;
	sscanf(s.c_str(), "v %f %f %f", &x, &y, &z);
	vec3 result(x, y, z);
	
	return result;
}

vec3 ObjLoader::getNormal(std::string s)
{
	float x, y, z;
	sscanf(s.c_str(), "vn %f %f %f", &x, &y, &z);
	vec3 result(x, y, z);
	
	return result;
}

face ObjLoader::getFace(std::string s)
{
	int v1, v2, v3, v4, f;
	sscanf(s.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v1, &f, &v2, &f, &v3, &f, &v4, &f);
	face result(v1, v2, v3, v4, f);
	
	return result;
}

void ObjLoader::loadOBJ(unsigned & id, const char * filePath)
{
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<face> faces;

	std::fstream arq(filePath);
	std::string line = "";

	if (!arq.is_open()) {
		std::cout << "ERRO::Nao foi possivel abrir o arquivo " << filePath << "\n";
	}

	while (getline(arq, line)) {
		if (line.find("v ") != std::string::npos) {
			vec3 tempVertice = getVertice(line);
			vertices.push_back(tempVertice);
		}
		else if (line.find("vn ") != std::string::npos) {
			vec3 tempNormal = getNormal(line);
			normals.push_back(tempNormal);
		}
		else if (line.find("f ") != std::string::npos) {
			face tempFace = getFace(line);
			faces.push_back(tempFace);
		}
	}

	std::cout << "Total Vertices: " << vertices.size() << "\n";
	std::cout << "Total Normals: " << normals.size() << "\n";
	std::cout << "Total Faces: " << faces.size() << "\n";

	id = glGenLists(1);

	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT, GL_FILL);
	for (int i = 0; i < faces.size(); i++) {
		unsigned index = faces[i].index - 1;
		int v1 = faces[i].vertice[0] - 1;
		int v2 = faces[i].vertice[1] - 1;
		int v3 = faces[i].vertice[2] - 1;
		int v4 = faces[i].vertice[3] - 1;

		glNormal3fv(&normals[index].x);
		glBegin(GL_QUADS);
		glVertex3fv(&vertices[v1].x);
		glVertex3fv(&vertices[v2].x);
		glVertex3fv(&vertices[v3].x);
		glVertex3fv(&vertices[v4].x);
		glEnd();
	}

	glEndList();
}