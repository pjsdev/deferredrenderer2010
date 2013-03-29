#include "ScreenQuad.h"
#include "ngl/ShaderManager.h"
#include "ngl/Types.h"
#include "ngl/TransformStack.h"

#ifdef DARWIN
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

ScreenQuad::ScreenQuad() {}
ScreenQuad::ScreenQuad( const float& _x, const float& _y, const float& _depth, const float& _extent )
{
	m_x = _x;
	m_y = _y;
	m_depth = _depth;
	m_extent = _extent;
}
ScreenQuad::~ScreenQuad() {}

void ScreenQuad::draw( const std::string& _shaderName, unsigned int& _vao )
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )[_shaderName]->use();
	
	shader->setShaderParam4f( _shaderName, "u_CentreDepthExtent",
	                          m_x,
	                          m_y,
	                          m_depth,
	                          m_extent );
	                          
	// now we bind back our vertex array object and draw
	glBindVertexArray( _vao );		// select first VAO
	
	glDrawArrays( GL_QUADS, 0, 4 );	// draw first object
	
	// go back to default just incase
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
}
