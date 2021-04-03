#include <vector>
#include "../headers/ObjLoader.h"

// If readed line contains the v char, it will be a vertice
vec3 ObjLoader::getVertice(std::string s)
{
	float x, y, z;
	sscanf(s.c_str(), "v %f %f %f", &x, &y, &z);
	vec3 result(x, y, z);

	return result;
}

// If readed line contains the vt char, it will be a texture coordinate
vec2 ObjLoader::getTexVertice(std::string s)
{
	float x, y;
	sscanf(s.c_str(), "vt %f %f", &x, &y);
	vec2 result(x, y);

	return result;
}

// If readed line contains the vn char, it will be a normal
vec3 ObjLoader::getNormal(std::string s)
{
	float x, y, z;
	sscanf(s.c_str(), "vn %f %f %f", &x, &y, &z);
	vec3 result(x, y, z);

	return result;
}

// If readed line contains the f char, it will be a face
face ObjLoader::getFace(Poligon type_, std::string s)
{
	// Variables to store all the necessary information to create and connect a face
	int v1, v2, v3, v4, vt1, vt2, vt3, vt4, n1, n2, n3, n4;

	// If the figure is a triangle v4, vt4 and n4 will be nullified
	if (type_ == Poligon::TRIANG)
	{
		v4 = -1;
		vt4 = -1;
		n4 = -1;

		sscanf(s.c_str(),
			   "f %d/%d/%d %d/%d/%d %d/%d/%d",
			   &v1, &vt1, &n1,
			   &v2, &vt2, &n2,
			   &v3, &vt3, &n3);
	}
	// If the figure is a quad all info will be stored
	else if (type_ == Poligon::QUAD)
	{
		sscanf(s.c_str(),
			   "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
			   &v1, &vt1, &n1,
			   &v2, &vt2, &n2,
			   &v3, &vt3, &n3,
			   &v4, &vt4, &n4);
	}

	face result(type_, v1, v2, v3, v4, vt1, vt2, vt3, vt4, n1, n2, n3, n4);

	return result;
}

// Reads the file and applies the texture
void ObjLoader::loadOBJ(unsigned &id, Texture *texture, const char *filePath)
{
	// vec3, vec2 and face arrays
	std::vector<vec3> vertices;
	std::vector<vec2> texCoords;
	std::vector<vec3> normals;
	std::vector<face> faces;

	// Reads the file at given filepath
	std::fstream arq(filePath);
	std::string line = "";

	// If failed during file reading, throws error message
	if (!arq.is_open())
	{
		std::cout << "ERRO::Nao foi possivel abrir o arquivo " << filePath << "\n";
	}

	// Reads the whole .obj file populating the arrays and saving useful info
	while (getline(arq, line))
	{
		if (line.find("v ") != std::string::npos)
		{
			vec3 tempVertice = getVertice(line);
			vertices.push_back(tempVertice);
		}
		else if (line.find("vt ") != std::string::npos)
		{
			vec2 tempTexCoord = getTexVertice(line);
			texCoords.push_back(tempTexCoord);
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
			Poligon type_ = (Poligon)nSpace;
			face tempFace = getFace(type_, line);
			faces.push_back(tempFace);
		}
	}

	// Uncomment to get acess to model info
	// std::cout << "Total Vertices: " << vertices.size() << "\n";
	// std::cout << "Total TexCoords: " << texCoords.size() << "\n";
	// std::cout << "Total Normals: " << normals.size() << "\n";
	// std::cout << "Total Faces: " << faces.size() << "\n";

	// Create a modelID
	id = glGenLists(1);

	// Compile model at given id
	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT, GL_FILL);

	// Connects everything
	for (int i = 0; i < faces.size(); i++)
	{
		// note: -1 in here is because .obj don't start at 0
		int v1 = faces[i].vertice[0] - 1;
		int v2 = faces[i].vertice[1] - 1;
		int v3 = faces[i].vertice[2] - 1;
		int v4 = faces[i].vertice[3] - 1;
		int vt1 = faces[i].texCoord[0] - 1;
		int vt2 = faces[i].texCoord[1] - 1;
		int vt3 = faces[i].texCoord[2] - 1;
		int vt4 = faces[i].texCoord[3] - 1;
		int n1 = faces[i].normal[0] - 1;
		int n2 = faces[i].normal[1] - 1;
		int n3 = faces[i].normal[2] - 1;
		int n4 = faces[i].normal[3] - 1;

		// If the poligon is a triangle
		if (faces[i].type == Poligon::TRIANG)
		{
			// Bind the texture
			texture->Bind();

			// Start "drawing" the figure based on given vertices positions, normals and texture coordinates
			glBegin(GL_TRIANGLES);

			glNormal3fv(&normals[n1].x);
			glTexCoord2f(texCoords[vt1].x, texCoords[vt1].y);
			glVertex3fv(&vertices[v1].x);

			glNormal3fv(&normals[n2].x);
			glTexCoord2f(texCoords[vt2].x, texCoords[vt2].y);
			glVertex3fv(&vertices[v2].x);

			glNormal3fv(&normals[n3].x);
			glTexCoord2f(texCoords[vt3].x, texCoords[vt3].y);
			glVertex3fv(&vertices[v3].x);

			glEnd();

			// Unbind the texture
			texture->UnBind();
		}
		// If the poligon is a quad
		else if (faces[i].type == Poligon::QUAD)
		{
			// Bind the texture
			texture->Bind();

			// Start "drawing" the figure based on given vertices positions, normals and texture coordinates
			glBegin(GL_QUADS);

			glNormal3fv(&normals[n1].x);
			glTexCoord2f(texCoords[vt1].x, texCoords[vt1].y);
			glVertex3fv(&vertices[v1].x);

			glNormal3fv(&normals[n2].x);
			glTexCoord2f(texCoords[vt2].x, texCoords[vt2].y);
			glVertex3fv(&vertices[v2].x);

			glNormal3fv(&normals[n3].x);
			glTexCoord2f(texCoords[vt3].x, texCoords[vt3].y);
			glVertex3fv(&vertices[v3].x);

			glNormal3fv(&normals[n4].x);
			glTexCoord2f(texCoords[vt4].x, texCoords[vt4].y);
			glVertex3fv(&vertices[v4].x);
			glEnd();
			texture->UnBind();
		}
	}

	glEndList();
}
