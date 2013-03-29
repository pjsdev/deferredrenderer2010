#include "SpotLight.h"
#include "ngl/VBOPrimitives.h"
#include "ngl/Vector.h"
#include "ngl/Matrix.h"
#include "NGLMath.h"
#include "ngl/ShaderManager.h"
#include "ngl/Camera.h"

#include "math.h"

#include <cstring>

SpotLight::SpotLight( const ngl::Vector& _position, const ngl::Vector& _target, const float& _distance, const float& _angle, const ngl::Vector& _col, const ngl::Vector& _up )
{
	this->set( _position, _target, _distance, _angle, _col, _up );
}

SpotLight::~SpotLight() {}

bool SpotLight::collidesWithSphere( const ngl::Vector& _pos, const float& _rad )
{
	float radius = m_directionDistance.m_w * 0.5;
	
	ngl::Vector centre = ngl::Vector( m_directionDistance.m_x, m_directionDistance.m_y, m_directionDistance.m_z ) * ( m_directionDistance.m_w * 0.5 );
	
	ngl::Vector length = centre - _pos;
	float lengthSq = length.lengthSquared();
	
	float radiiSq = ( radius * radius ) * 1.5 /*scale up to avoid nasty culling */ + ( _rad * _rad );
	
	return lengthSq < radiiSq;
}

void SpotLight::draw( const std::string& _vaoName, const std::string& _shader )
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	( *shader )[_shader]->use();
	
	shader->setShaderParamFromMatrix( _shader, "u_ModelMatrix", m_world );
	shader->setShaderParamFromVector( _shader, "a_PositionAngle", m_positionAngle );
	shader->setShaderParamFromVector( _shader, "a_DirectionDistance", m_directionDistance );
	shader->setShaderParamFromVector( _shader, "a_Colour", m_colour );
	
	ngl::VBOPrimitives* prim = ngl::VBOPrimitives::instance();
	prim->draw( _vaoName );
}

void SpotLight::set(
    const ngl::Vector& _position,
    const ngl::Vector& _target,
    const float& _distance,
    const float& _angle,
    const ngl::Vector& _col,
    const ngl::Vector& _up
)
{
	const float xyscale = _distance * sinf( ( _angle * .5 ) * ngl::PI / 180 ) * 1.1f; // Scaling up to avoid hard edges eating the light
	
	ngl::Matrix scale, invView, trans;
	
	scale.scale( xyscale, xyscale, _distance );
	
	m_matrixCam = ngl::Camera( _position, _target, _up, ngl::PERSPECTIVE );
	
	m_lightView = Lookat( _position, _target, _up );
	invView = Inverse( m_lightView );
	
	//magic number to get ngl cone into position
	trans.translate( 0, 0, -1 );
	
	m_world =  invView * scale * trans;
	
	ngl::Vector direction = _target - _position;
	direction.normalize();
	
	m_positionAngle = _position;
	m_positionAngle.m_w = cos( ( _angle * .5 ) * ngl::PI / 180 );
	m_directionDistance = direction;
	m_directionDistance.m_w = _distance;
	m_colour = _col;
}
