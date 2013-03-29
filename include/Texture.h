#ifndef TEXTURE_H
#define TEXTURE_H

#include "ngl/Types.h"
#include "string"

class Texture
{
public:
	GLuint id;
	Texture() {}
	Texture( const std::string& _name );
	~Texture();
	
	void generateEmpty( int _width, int _height );
	void generateEmptyf( int _width, int _height );
	void generateDepth( int _width, int _height );
	
	void generateFromFile( char* _filePath );
	
	void bind() {
		glBindTexture( GL_TEXTURE_2D, id );
	}
	
	std::string getName() {
		return m_name;
	}
	
private:
	int m_width, m_height;
	std::string m_name;
};

#endif // FRUSTUM_H
