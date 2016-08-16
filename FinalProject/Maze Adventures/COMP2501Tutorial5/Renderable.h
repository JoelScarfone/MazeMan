#include <vector>
#include <glew.h>
#include "MathHelper.h"
#include "Shader.h"

/*struct Vertex {
	float x, y, z;

	Vertex() {}

	Vertex(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
};

struct Color {
	float r, g, b, a;

	Color() {}

	Color(float _r, float _g, float _b, float _a) {
		r = _r; g = _g; b = _b; a = _a;
	}
};*/

class Renderable {
public:
	Renderable(Shader* _shader, int numverts, int numinds);
	~Renderable();
	void initGeometry(Vertex *vertices, Color *colors, GLushort *indices, Texture2D* textCoords, GLuint image);
	void renderSelf(Matrix4 &mvpMatrix);

protected:
	GLuint textureID;
	GLuint vao, vio;
	Shader* shader;
	GLuint mvpMatrixLoc;
	GLuint buffers[3];
	int numindices;
	int numvertices;
	GLuint  vertexLoc, textUnitLoc, colorLoc, tCoordLoc;
};

Renderable* createPlane(Shader* _shader, int subdivides = 1);
Renderable* createCube(Shader* _shader, GLuint image);
Renderable* createSphere(Shader* _shader, GLuint image, int rings, int sectors, float radius);
Renderable* createCylinder(Shader* _shader, GLuint image,int sectors, float height, float topRadius, float bottomRadius);