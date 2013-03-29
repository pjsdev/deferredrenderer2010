#include "ShadowSpot.h"
#include "ngl/ShaderManager.h"
#include "ngl/VBOPrimitives.h"
#include "NGLMath.h"
#include "ngl/Matrix.h"
#include "ngl/Util.h"

ShadowSpot::ShadowSpot( const ngl::Vector& _position,
                        const ngl::Vector& _target,
                        const float& _distance,
                        const float& _angle,
                        const ngl::Vector& _col,
                        const ngl::Vector& _up )
{
	//m_shadowMapId = 0;
	this->set( _position, _target, _distance, _angle, _col, _up );
	m_shadowView = m_matrixCam.getModelView();//Lookat(_position, _target, _up, true);
	m_angle = _angle;
	
	generateProjMatrices();
}


/// Based on Jon Maceys code (2011)
void ShadowSpot::generateProjMatrices()
{
	float aspectRatio = float( 1024 / 768 );
	
	float near = 0.1;
	float far = 300.0;
	
	// note 1/tan == cotangent
	float f = 1.0 / tan( ngl::radians( m_angle ) / 2.0 );
	
	m_proj.identity();
	
	m_proj.m_m[0][0] = f / aspectRatio;
	m_proj.m_m[1][1] = f;
	
	m_proj.m_m[2][2] = ( far + near ) / ( near - far );
	m_proj.m_m[3][2] = ( 2 * far * near ) / ( near - far );
	
	m_proj.m_m[2][3] = -1;
	
	m_projView = m_proj * m_lightView;
	m_frustum.generate( m_projView );
}
