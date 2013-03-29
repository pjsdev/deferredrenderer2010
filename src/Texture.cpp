#include "Texture.h"
#include "QImage"

Texture::Texture( const std::string& _name ): id( 0 ), m_width( 0 ), m_height( 0 ), m_name( _name ) {}
Texture::~Texture() {}

void Texture::generateEmpty( int _width, int _height )
{
	m_height = _height;
	m_width = _width;
	
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	
	// Set the texture's stretching properties
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glGenerateMipmap( GL_TEXTURE_2D );
	
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,  m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
}

void Texture::generateEmptyf( int _width, int _height )
{
	m_height = _height;
	m_width = _width;
	
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	
	// Set the texture's stretching properties
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, m_width, m_height, 0, GL_RGBA, GL_FLOAT, 0 );
	
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::generateDepth( int _width, int _height )
{
	m_height = _height;
	m_width = _width;
	
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	
	// Set the texture's stretching properties
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE  );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE  );
	glGenerateMipmap( GL_TEXTURE_2D );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,  m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
}

void Texture::generateFromFile( char* _filePath )
{
	QImage* image = new QImage();
	bool loaded = image->load( _filePath );
	
	if( loaded == true ) {
		int width = image->width();
		int height = image->height();
		unsigned char* data = new unsigned char[width * height * 3];
		unsigned int index = 0;
		QRgb colour;
		
		for( int y = 0; y < height; ++y ) {
			for( int x = 0; x < width; ++x ) {
				colour = image->pixel( x, y );
				
				data[index++] = qRed( colour );
				data[index++] = qGreen( colour );
				data[index++] = qBlue( colour );
			}
		}
		
		glGenTextures( 1, &id );
		glBindTexture( GL_TEXTURE_2D, id );
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		
		glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
}
