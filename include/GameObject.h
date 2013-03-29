#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ngl/Types.h"
#include "string"
#include "ngl/VertexArrayObject.h"

// a simple structure to hold our vertex data
struct vertData {
	GLfloat u; // tex cords from obj
	GLfloat v; // tex cords
	GLfloat nx; // normal from obj mesh
	GLfloat ny;
	GLfloat nz;
	GLfloat x; // position from obj
	GLfloat y;
	GLfloat z;
	GLfloat tx; // tangent calculated by us
	GLfloat ty;
	GLfloat tz;
	GLfloat bx; // binormal (bi-tangent really) calculated by us
	GLfloat by;
	GLfloat bz;
};

class GameObject
{
public:
	GameObject() {}
	~GameObject() {}
	
	void loadMesh( const std::string& _meshName );
	
	ngl::VertexArrayObject* vao;
};

#endif // FRUSTUM_H
