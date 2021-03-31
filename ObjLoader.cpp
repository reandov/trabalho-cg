#include "ObjLoader.h"
#include <vector>

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

face ObjLoader::getFace(Poligono tipo_, std::string s)
{
	int v1, v2, v3, v4, vt, n1, n2, n3, n4;

	if (tipo_ == Poligono::TRIANG)
	{
		v4 = -1;
		n4 = -1;

		sscanf(s.c_str(),
			   "f %d/%d/%d %d/%d/%d %d/%d/%d",
			   &v1, &vt, &n1,
			   &v2, &vt, &n2,
			   &v3, &vt, &n3);
	}
	else if (tipo_ == Poligono::QUAD)
	{
		sscanf(s.c_str(),
			   "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
			   &v1, &vt, &n1,
			   &v2, &vt, &n2,
			   &v3, &vt, &n3,
			   &v4, &vt, &n4);
	}

	face result(tipo_, v1, v2, v3, v4, n1, n2, n3, n4);

	return result;
}

void ObjLoader::loadOBJ(unsigned &id, const char *filePath)
{
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<face> faces;

	std::fstream arq(filePath);
	std::string line = "";

	if (!arq.is_open())
	{
		std::cout << "ERRO::Nao foi possivel abrir o arquivo " << filePath << "\n";
	}

	while (getline(arq, line))
	{
		if (line.find("v ") != std::string::npos)
		{
			vec3 tempVertice = getVertice(line);
			vertices.push_back(tempVertice);
		}
		else if (line.find("vn ") != std::string::npos)
		{
			vec3 tempNormal = getNormal(line);
			normals.push_back(tempNormal);
		}
		else if (line.find("f ") != std::string::npos)
		{
			short nSpace = 0;
			for (char c : line)
			{
				if (c == ' ')
				{
					nSpace++;
				}
			}
			Poligono tipo_ = (Poligono)nSpace;
			face tempFace = getFace(tipo_, line);
			faces.push_back(tempFace);
		}
	}

	std::cout << "Total Vertices: " << vertices.size() << "\n";
	std::cout << "Total Normals: " << normals.size() << "\n";
	std::cout << "Total Faces: " << faces.size() << "\n";

	id = glGenLists(1);

	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT, GL_FILL);
	for (int i = 0; i < faces.size(); i++)
	{
		int v1 = faces[i].vertice[0] - 1;
		int v2 = faces[i].vertice[1] - 1;
		int v3 = faces[i].vertice[2] - 1;
		int v4 = faces[i].vertice[3] - 1;
		int n1 = faces[i].normal[0] - 1;
		int n2 = faces[i].normal[1] - 1;
		int n3 = faces[i].normal[2] - 1;
		int n4 = faces[i].normal[3] - 1;

		if (faces[i].tipo == Poligono::TRIANG)
		{
			glBegin(GL_TRIANGLES);
			glNormal3fv(&normals[n1].x);
			glVertex3fv(&vertices[v1].x);
			glNormal3fv(&normals[n2].x);
			glVertex3fv(&vertices[v2].x);
			glNormal3fv(&normals[n3].x);
			glVertex3fv(&vertices[v3].x);
			glEnd();
		}
		else if (faces[i].tipo == Poligono::QUAD)
		{
			glBegin(GL_QUADS);
			glNormal3fv(&normals[n1].x);
			glVertex3fv(&vertices[v1].x);
			glNormal3fv(&normals[n2].x);
			glVertex3fv(&vertices[v2].x);
			glNormal3fv(&normals[n3].x);
			glVertex3fv(&vertices[v3].x);
			glNormal3fv(&normals[n4].x);
			glVertex3fv(&vertices[v4].x);
			glEnd();
		}
	}

	glEndList();
}
