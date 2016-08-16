#include "Renderable.h"
#include <Soil.h>


Renderable::Renderable(Shader* _shader, int numverts, int numinds)
{
	shader = _shader;
	numvertices = numverts;
	numindices = numinds;
}

Renderable::~Renderable(){	
	shader = 0;
	glDeleteBuffers(3, buffers);
	glDeleteVertexArrays(1, &vao);
}

void Renderable::initGeometry(Vertex *vertices, Color *colors, GLushort *indices, Texture2D* textCoords, GLuint image){

	textureID = image;

	GLuint vertexLoc, colorLoc;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate slots for the vertex and color buffers
	glGenBuffers(3, buffers);

	// bind buffer for vertices and copy data into buffer
	vertexLoc = glGetAttribLocation(shader->id(), "position");
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, numvertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);

	

	tCoordLoc = glGetAttribLocation(shader->id(), "textCoord");
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, numvertices * sizeof(Texture2D), textCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(tCoordLoc);
	glVertexAttribPointer(tCoordLoc, 2, GL_FLOAT, 0, 0, 0);

	// Typical Texture Generation Using Data From The Bitmap

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textUnitLoc = glGetUniformLocation(shader->id(), "texUnit");
	

	// bind buffer for indices and copy data into buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLushort), indices, GL_STATIC_DRAW);
	vio = buffers[2];

	mvpMatrixLoc = glGetUniformLocation(shader->id(), "mvpMatrix");
}

void Renderable::renderSelf(Matrix4 &mvpMatrix){



	shader->bind();			
	glUniformMatrix4fv(mvpMatrixLoc, 1, GL_TRUE, (GLfloat *)&mvpMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(textUnitLoc, 0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
	glDrawElements(
		GL_TRIANGLES,        // mode
		numindices,			 // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0             // element array buffer offset
		);


}

//Plane
//Plane::Plane(Shader* _shader, int subdivides){

//}

Renderable * createCube(Shader* _shader, GLuint image)
{
	
	const int numvertices = 24;
	const int numindices = 36;

	Renderable* model = new Renderable(_shader, numvertices, numindices);

	Vertex vertices[numvertices] = { Vertex(-1, -1, 1), Vertex(1, -1, 1), Vertex(-1, 1, 1), Vertex(1, 1, 1),  //front face  v0,v1,v2,v3
		Vertex(1, -1, -1), Vertex(-1, -1, -1), Vertex(1, 1, -1), Vertex(-1, 1, -1),   //back face   v4,v5,v6,v7
		Vertex(1, -1, 1), Vertex(1, -1, -1), Vertex(1, 1, 1), Vertex(1, 1, -1),   //right face  v1 v4 v3 v6
		Vertex(-1, -1, -1), Vertex(-1, -1, 1), Vertex(-1, 1, -1), Vertex(-1, 1, 1),   //left face   v5 v0 v7 v2
		Vertex(-1, -1, -1), Vertex(1, -1, -1), Vertex(-1, -1, 1), Vertex(1, -1, 1),  //bottom face v5 v4 v0 v1
		Vertex(-1, 1, 1), Vertex(1, 1, 1), Vertex(-1, 1, -1), Vertex(1, 1, -1) }; //top face    v2 v3 v7 v6

	Color colors[numvertices] = { Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 0, 0, 1), //red
		Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(0, 1, 0, 1),  //green
		Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(0, 0, 1, 1),  //blue
		Color(1, 1, 0, 1), Color(1, 1, 0, 1), Color(1, 1, 0, 1), Color(1, 1, 0, 1),  //yellow
		Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 0, 1, 1),  //magenta
		Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(0, 1, 1, 1) }; //cyan

	GLushort indices[numindices] = { 0, 1, 2, 2, 1, 3,   //front face
		4, 5, 6, 6, 5, 7,   //back face
		8, 9, 10, 10, 9, 11,   //right face
		12, 13, 14, 14, 13, 15,   //left face
		16, 17, 18, 18, 17, 19,   //bottom face
		20, 21, 22, 22, 21, 23 }; //top face

	Texture2D textCoords[24] = { Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f),
		Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f) ,
		Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f) ,
		Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f) ,
		Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f) ,
		Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f) };

	model->initGeometry(vertices, colors, indices, textCoords, image);

	return model;
}


//Sphere
Renderable* createSphere(Shader *_shader, GLuint image, int rings, int sectors, float radius){
	int numvertices = rings * sectors;
	int numindices = rings * sectors * 4 * 3;
	Renderable* model = new Renderable(_shader, numvertices, numindices);

	auto R = 1. / (float)(rings - 1);
	auto S = 1. / (float)(sectors - 1);

	std::vector<Vertex> vertices;
	vertices.resize(numvertices);
	std::vector<Color> colors;
	colors.resize(numvertices);

	for (auto r = 0; r < rings; r++) {
		for (auto s = 0; s < sectors; s++) {
			auto y = sin(-Math::HALF_PI + Math::PI * r * R);
			auto x = cos(2 * Math::PI * s * S) * sin(Math::PI * r * R);
			auto z = sin(2 * Math::PI * s * S) * sin(Math::PI * r * R);

			vertices[r*sectors + s] = Vertex(x * radius, y*radius, z*radius);
			colors[r*sectors + s] = Color(x, y, z, 1);
		}
	}

	std::vector<GLushort> indices;
	indices.resize(numindices);
	auto i = indices.begin();
	for (auto r = 0; r < rings - 1; r++) for (auto s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + s;
		*i++ = (r + 1) * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
		*i++ = (r + 1) * sectors + (s + 1);

	}

	Texture2D textCoords[24] = { Texture2D(0.0f, 1.0f), Texture2D(1.0f, 1.0f), Texture2D(0.0f, 0.0f), Texture2D(1.0, 0.0f),};

	model->initGeometry(&vertices[0], &colors[0], &indices[0], textCoords, image);
	return model;
}

//Cylinder
Renderable *createCylinder(Shader* _shader, GLuint image, int sectors, float height, float topRadius, float bottomRadius){
	
	auto const numvertices = (sectors + 1) * 2;
	auto const numindices = (sectors - 2) * 3 * 2 + sectors * 6;
	Renderable* model = new Renderable(_shader, numvertices, numindices);
	
	auto half = numvertices / 2;
	auto S = 1. / (float)(sectors);

	Vertex* vertices = new Vertex[numvertices];
	Color *colors = new Color[numvertices];

	for (auto s = 0; s < half; s++) {
		auto x = cos(2 * Math::PI * s * S);
		auto z = sin(2 * Math::PI * s * S);

		vertices[s] = Vertex(x*topRadius, height/2, z*topRadius);
		vertices[half + s] = Vertex(x*bottomRadius, -height/2, z*bottomRadius);
		colors[s] = Color(x, 1, z, 1);
		colors[half + s] = Color(x, 0, z, 1);

	}



	std::vector<GLushort> indices;
	indices.resize(numindices);
	auto i = indices.begin();
	for (auto s = 1; s < sectors - 1; s++) {//top
		*i++ = 0;
		*i++ = s + 1;
		*i++ = s;
	}
	for (auto s = 1; s < sectors - 1; s++) {//bottom
		*i++ = half;
		*i++ = half + s;
		*i++ = half + s + 1;
	}
	for (auto s = 0; s < sectors; s++) {//side
		*i++ = s;
		*i++ = s + 1;
		*i++ = half + s;
		*i++ = half + s;
		*i++ = s + 1;

		*i++ = half + s + 1;
	}

	Texture2D textCoords[24] = { Texture2D(1.0f / 4.0f, 2.0f / 3.0f), Texture2D(2.0f / 4.0f, 2.0f / 3.0f), Texture2D(1.0f / 4.0f, 1.0f / 3.0f), Texture2D(2.0f / 4.0f, 1.0f / 3.0f),
		Texture2D(3.0f / 4.0f, 2.0f / 3.0f), Texture2D(1.0f, 2.0f / 3.0f), Texture2D(3.0f / 4.0f, 1.0f / 3.0f), Texture2D(1.0f, 1.0f / 3.0f),
		Texture2D(2.0f / 4.0f, 2.0f / 3.0f), Texture2D(3.0f / 4.0f, 2.0f / 3.0f), Texture2D(2.0f / 4.0f, 1.0f / 3.0f), Texture2D(3.0f / 4.0f, 1.0f / 3.0f),
		Texture2D(0.0f, 2.0f / 3.0f), Texture2D(1.0f / 4.0f, 2.0f / 3.0f), Texture2D(0.0f, 1.0f / 3.0f), Texture2D(1.0f / 4.0f, 1.0f / 3.0f),
		Texture2D(1.0f / 4.0f, 1.0f), Texture2D(2.0f / 4.0f, 1.0f), Texture2D(1.0f / 4.0f, 2.0f / 3.0f), Texture2D(2.0f / 4.0f, 2.0f / 3.0f),
		Texture2D(1.0f / 4.0f, 1.0f / 3.0f), Texture2D(2.0f / 4.0f, 1.0f / 3.0f), Texture2D(1.0f / 4.0f, 0.0f), Texture2D(2.0f / 4.0f, 0.0f) };

	model->initGeometry(vertices, colors, &indices[0], textCoords, image);
	return model;
}
