#include "TextureEngine.h"

TextureEngine::TextureEngine() {}
TextureEngine::~TextureEngine() {}

void TextureEngine::addTexture( Texture& _t )
{
	m_textures.insert( std::pair<std::string, Texture>( _t.getName(), _t ) );
}

Texture TextureEngine::getTexture( const std::string& _name )
{
	return m_textures[_name];
}

GLuint TextureEngine::getTextureId( const std::string& _name )
{
	return m_textures[_name].id;
}

void TextureEngine::bindTexture( const std::string& _name )
{
	m_textures[_name].bind();
}

void TextureEngine::addEmptyf( const std::string& _name, int _w, int _h )
{
	Texture tmp( _name );
	tmp.generateEmptyf( _w, _h );
	this->addTexture( tmp );
}

void TextureEngine::addEmpty( const std::string& _name, int _w, int _h )
{
	Texture tmp( _name );
	tmp.generateEmpty( _w, _h );
	this->addTexture( tmp );
}

