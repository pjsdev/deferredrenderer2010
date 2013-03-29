#include "FrameBufferObject.h"
#include "ngl/Types.h"
#include "iostream"

FrameBufferObject::FrameBufferObject( unsigned int _width, unsigned int _height ):
	m_width( _width ), m_height( _height )
{
}

FrameBufferObject::~FrameBufferObject()
{}

void FrameBufferObject::bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER_EXT, m_FBOid );
	glBindRenderbuffer( GL_RENDERBUFFER_EXT, m_RBid );
	
	glPushAttrib( GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport( 0, 0, m_width, m_height );
	
}

void FrameBufferObject::unbind()
{
	glPopAttrib();
	glBindFramebuffer( GL_FRAMEBUFFER_EXT, 0 );
	glBindRenderbuffer( GL_RENDERBUFFER_EXT, 0 );
}

void FrameBufferObject::checkStatus()
{
	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	
	switch( status ) {
		case GL_FRAMEBUFFER_COMPLETE:
			std::cout << "Complete FBO" << std::endl;
			break;
			
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "Unsupported FBO" << std::endl;
			break;
			
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "Incomplete Attatchments" << std::endl;
			break;
			
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "Missing attatchment" << std::endl;
			break;
			
		default:
			std::cout << "Weirdness afoot" << std::endl;
			break;
	}
}

void FrameBufferObject::bindTextureSlot( unsigned int _slot, GLuint _id )
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_FBOid );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _slot, GL_TEXTURE_2D, _id, 0 );
}

void FrameBufferObject::bindDepthTexture( GLuint textureId )
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_FBOid );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0 );
}

void FrameBufferObject::create( GLuint _textureId )
{
	// create a renderbuffer object to store depth info
	glGenRenderbuffers( 1, &m_RBid );
	glBindRenderbuffer( GL_RENDERBUFFER, m_RBid );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
	                       m_width, m_height );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	
	// create a framebuffer object
	glGenFramebuffers( 1, &m_FBOid );
	glBindFramebuffer( GL_FRAMEBUFFER, m_FBOid );
	
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0 );
	
	// attach the renderbuffer to depth attachment point
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	                           GL_RENDERBUFFER, m_RBid );
	                           
	checkStatus();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}

void FrameBufferObject::activateAllTargets()
{
	GLenum buffers[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};
	
	glDrawBuffers( 4, buffers );
}

void FrameBufferObject::activateTarget( unsigned int _id )
{
	glDrawBuffer( GL_COLOR_ATTACHMENT0 + _id );
}
