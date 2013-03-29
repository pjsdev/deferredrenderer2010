#include "PostProcessor.h"
#include "Texture.h"
#include "ngl/ShaderManager.h"
#include "sstream"

PostProcessor::PostProcessor( int _w, int _h, GLuint _vao ):
	m_screenQuad( 0.0f, 0.0f, 0.5f, 1.0f ),
	m_width( _w ),
	m_height( _h ),
	m_fbo( _w, _h ),
	m_vao( _vao ),
	m_currentAttachment( 0 )
{
	Texture tmp( "tmp" );
	tmp.generateEmpty( _w, _h );
	m_fbo.create( tmp.id );
}

void PostProcessor::addEffect( const std::string& _effectName, Effect _e )
{
	m_effects.insert( std::pair<std::string, Effect>( _effectName, _e ) );
}

void PostProcessor::start()
{
	m_fbo.bind();
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void PostProcessor::doEffect( const std::string& _effectName )
{
	m_fbo.bindTextureSlot( m_currentAttachment, m_effects[_effectName].getOutputId() );
	m_fbo.activateTarget( m_currentAttachment );
	
	m_currentAttachment++;
	
	if( m_currentAttachment > 3 ) {
		m_currentAttachment = 0;
	}
	
	m_effects[_effectName].prepare();
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	m_screenQuad.draw( m_effects[_effectName].getShaderName(), m_vao );
	
	m_finalComposite = m_effects[_effectName].getOutputId();
}

