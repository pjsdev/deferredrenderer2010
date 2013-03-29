#ifndef TEXTURE_ENGINE_H
#define TEXTURE_ENGINE_H

#include "ngl/Types.h"
#include "string"
#include "map"
#include "Texture.h"

class TextureEngine
{
public:
	TextureEngine();
	~TextureEngine();
	
	void addTexture( Texture& _t );
	Texture getTexture( const std::string& _name );
	GLuint getTextureId( const std::string& _name );
	void bindTexture( const std::string& _name );
	void addEmptyf( const std::string& _name, int _w, int _h );
	void addEmpty( const std::string& _name, int _w, int _h );
	
private:
	std::map<std::string, Texture> m_textures;
	
};

#endif // FRUSTUM_H
