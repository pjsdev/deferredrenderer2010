#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include "string"
#include "vector"
#include "ngl/Types.h"

/// @file FrameBufferObject.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class FrameBufferObject
/// @brief A glFBO wrapper for setting up and managing off screen rendering to multiple render targets
class FrameBufferObject
{
public:

	/// @brief empty ctor
	FrameBufferObject() {}
	
	/// @brief constructor that will set the dimensions of the FBO and init the rener target list
	/// @param [in] _width the FBO width
	/// @param [in] _height the FBO height
	/// @param [in] _numOfRT the number of render targets to have
	FrameBufferObject( unsigned int _width, unsigned int _height );
	
	/// @brief dtor
	~FrameBufferObject();
	
	/// @brief make this FBO the active one for drawing
	void bind();
	
	/// @brief make the default (screen) FBO active
	void unbind();
	
	/// @brief this method creates the fbo render targets and the glFBO itself
	void create( GLuint _textureId );
	
	/// @brief errors check this FBO
	void checkStatus();
	
	/// @brief accessor for FBO width
	/// @return m_width
	inline unsigned int getWidth() {
		return m_width;
	}
	
	/// @brief accessor for FBO height
	/// @return m_height
	inline unsigned int getHeight() {
		return m_height;
	}
	
	/// @brief acivates 4 targets for drawing
	void activateAllTargets();
	
	/// @brief bind a texture id to an FBO color attachment
	/// @param [in] _slot the color attachment
	/// @param [in] _id the texture id
	void bindTextureSlot( GLuint _slot, GLuint _id );
	
	void bindDepthTexture( GLuint textureId );
	
	/// @brief acivate a render target of your choice, starting from 0
	/// @param [in] _id the render targets color attachment id
	void activateTarget( GLuint _id );
private:

	/// @brief the FBO width
	GLuint m_width;
	
	/// @brief the FBO height
	GLuint m_height;
	
	/// @brief framebuffer id
	GLuint m_FBOid;
	
	/// @brief depth buffer id
	GLuint m_RBid;
};

#endif // FRAMEBUFFEROBJECT_H
