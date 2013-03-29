#include "Effect.h"
#include "ngl/ShaderManager.h"
#include "iostream"


Effect::Effect( GLuint _texId, const std::string& _shaderName ): m_output( _texId ), m_shaderName( _shaderName )
{
}

Effect::~Effect() {}

void Effect::addSourceTexture( const std::string& _samplerName, GLuint _texId )
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )[m_shaderName]->use();
	shader->setShaderParam1i( m_shaderName, _samplerName, m_sources.size() );
	
	m_sources.push_back( _texId );
}

void Effect::prepare()
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )[m_shaderName]->use();
	
	for( unsigned int i = 0; i < m_sources.size(); i++ ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, m_sources[i] );
	}
}
