#include "PointLight.h"
#include "ngl/ShaderManager.h"
#include "ngl/Types.h"
#include "ngl/TransformStack.h"
#include "ngl/VBOPrimitives.h"
#include "ngl/Util.h"

PointLight::PointLight() {}
PointLight::PointLight( const ngl::Vector& _centre, const float& _radius, const ngl::Vector& _intensity )
{
	m_pos.set( _centre.m_x, _centre.m_y, _centre.m_z );
	m_radius = _radius;
	m_intensity = _intensity;
}
PointLight::~PointLight() {}

bool PointLight::collidesWithSphere( const ngl::Vector& _pos, const float& _radius )
{
	ngl::Vector length = m_pos - _pos;
	float lengthSq = length.lengthSquared();
	
	float radiiSq = ( m_radius * m_radius ) + ( _radius * _radius );
	
	return lengthSq < radiiSq;
}

void PointLight::draw( unsigned int& _vao, const std::string& _shaderName )
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )[_shaderName]->use();
	
	shader->setShaderParam3f( _shaderName,
	                          "u_RGBIntensity",
	                          m_intensity.m_x,
	                          m_intensity.m_y,
	                          m_intensity.m_z );
	                          
	shader->setShaderParam4f( _shaderName,
	                          "u_WSCentreRadius",
	                          m_pos.m_x,
	                          m_pos.m_y,
	                          m_pos.m_z,
	                          m_radius
	                        );
	                        
	// now we bind back our vertex array object and draw
	glBindVertexArray( _vao );
	
	glDrawArrays( GL_QUADS, 0, 24 );
	
	//go back to default just incase
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
}
