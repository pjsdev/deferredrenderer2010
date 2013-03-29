#include "LightingEngine.h"
#include "ngl/VBOPrimitives.h"

LightingEngine::LightingEngine(): m_pointLightVAO( 0 )
{
}

void LightingEngine::accumulateLights( TextureEngine& _textureManager, const ngl::Vector& _camPos )
{
	glActiveTexture( GL_TEXTURE1 );
	_textureManager.bindTexture( "normal" );
	
	glActiveTexture( GL_TEXTURE2 );
	_textureManager.bindTexture( "albedo" );
	
	glActiveTexture( GL_TEXTURE3 );
	_textureManager.bindTexture( "position" );
	
	glActiveTexture( GL_TEXTURE4 );
	_textureManager.bindTexture( "shadow" );
	
	glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glEnable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glBlendFunc( GL_ONE, GL_ONE );
	glDepthMask( false );
	
	blendShadowSpots( _camPos );
	blendPointLights( _camPos );
	blendSpotLights( _camPos );
	
	glDisable( GL_BLEND );
	glDepthMask( true );
	glDisable( GL_CULL_FACE );
}

void LightingEngine::blendShadowSpots( const ngl::Vector& _camPos )
{
	for( unsigned int i = 0; i < m_shadowSpots.size(); i++ ) {
		//check for near plane clip
		if( m_shadowSpots[i].collidesWithSphere( _camPos, 10.0 ) ) {
			glCullFace( GL_FRONT );
			glDisable( GL_DEPTH_TEST );
			
			m_shadowSpots[i].draw( "spotlight", "ShadowSpot" );
			glEnable( GL_DEPTH_TEST );
			glCullFace( GL_BACK );
		}
		else {
			m_shadowSpots[i].draw( "spotlight", "ShadowSpot" );
		}
	}
}

void LightingEngine::blendPointLights( const ngl::Vector& _camPos )
{
	for( unsigned int i = 0; i < m_pointLights.size(); i++ ) {
		if( m_pointLights[i].collidesWithSphere( _camPos, 10.0 ) ) {
			glCullFace( GL_FRONT );
			glDisable( GL_DEPTH_TEST );
			m_pointLights[i].draw( m_pointLightVAO, "PointLight" );
			glEnable( GL_DEPTH_TEST );
			glCullFace( GL_BACK );
		}
		else {
			m_pointLights[i].draw( m_pointLightVAO, "PointLight" );
		}
	}
}

void LightingEngine::blendSpotLights( const ngl::Vector& _camPos )
{
	for( unsigned int i = 0; i < m_spotLights.size(); i++ ) {
		//check for near plane clip
		if( m_spotLights[i].collidesWithSphere( _camPos, 10.0 ) ) {
			glCullFace( GL_FRONT );
			glDisable( GL_DEPTH_TEST );
			m_spotLights[i].draw( "spotlight", "SpotLight" );
			glEnable( GL_DEPTH_TEST );
			glCullFace( GL_BACK );
		}
		else {
			m_spotLights[i].draw( "spotlight", "SpotLight" );
		}
	}
}

void LightingEngine::createLightGeometry()
{
	// vertex coords array
	GLfloat vertices[] = {
		1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, // v0-v1-v2-v3
		1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, // v0-v3-v4-v
		1, 1, 1, 1, 1, -1, -1, 1, -1, -1, 1, 1, // v0-v5-v6-v
		-1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1, // v1-v6-v7-v
		-1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, // v7-v4-v3-v
		1, -1, -1, -1, -1, -1, -1, 1, -1, 1, 1, -1 // v4-v7-v6-v5
	};
	
	// first we create a vertex array Object
	glGenVertexArrays( 1, &m_pointLightVAO );
	
	// now bind this to be the currently active one
	glBindVertexArray( m_pointLightVAO );
	// as they will be associated with the vertex array object
	GLuint vboID;
	glGenBuffers( 1, &vboID );
	// now we will bind an array buffer to the first one and load the data for the verts
	glBindBuffer( GL_ARRAY_BUFFER, vboID );
	glBufferData( GL_ARRAY_BUFFER, 24 * 3 * sizeof( GLfloat ), vertices, GL_STATIC_DRAW );
	// now we bind the vertex attribute pointer for this object in this case the
	// vertex data
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )["PointLight"]->vertexAttribPointer( "a_VertexPosition", 3, GL_FLOAT, 0, 0 );
	( *shader )["PointLight"]->enableAttribArray( "a_VertexPosition" );
	
	// finally switch back to the default so we don't overwrite
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
	
	ngl::VBOPrimitives* prim = ngl::VBOPrimitives::instance();
	prim->createVBOCone( "spotlight", 1.0, 1.0, 30.0, 1.0 );
}

void LightingEngine::addNamedPoint( const std::string& _name, PointLight& _point )
{
	m_namedPointLights.insert( std::pair<std::string, PointLight>( _name, _point ) );
}
void LightingEngine::addNamedSpot( const std::string& _name, SpotLight& _spot )
{
	m_namedSpotLights.insert( std::pair<std::string, SpotLight>( _name, _spot ) );
}
void LightingEngine::addNamedShadowSpot( const std::string& _name, ShadowSpot& _spot )
{
	m_namedShadowSpots.insert( std::pair<std::string, ShadowSpot>( _name, _spot ) );
}

void LightingEngine::addNamedPoint(
    const std::string& _name,
    const ngl::Vector& _centre,
    const float& _radius,
    const ngl::Vector& _intensity )
{
	PointLight tmp( _centre, _radius, _intensity );
	this->addNamedPoint( _name, tmp );
}

void LightingEngine::addNamedSpot(
    const std::string& _name,
    const ngl::Vector& _position,
    const ngl::Vector& _target,
    const float& _distance,
    const float& _angle,
    const ngl::Vector& _col,
    const ngl::Vector& _up )
{
	SpotLight tmp( _position, _target, _distance, _angle, _col, _up );
	this->addNamedSpot( _name, tmp );
}

void LightingEngine::addNamedShadowSpot(
    const std::string& _name,
    const ngl::Vector& _position,
    const ngl::Vector& _target,
    const float& _distance,
    const float& _angle,
    const ngl::Vector& _col,
    const ngl::Vector& _up )
{
	ShadowSpot tmp( _position, _target, _distance, _angle, _col, _up );
	this->addNamedSpot( _name, tmp );
}

void LightingEngine::addPointLight(
    const ngl::Vector& _centre,
    const float& _radius,
    const ngl::Vector& _intensity )
{
	PointLight tmp( _centre, _radius, _intensity );
	m_pointLights.push_back( tmp );
}
void LightingEngine::addSpotLight(
    const ngl::Vector& _position,
    const ngl::Vector& _target,
    const float& _distance,
    const float& _angle,
    const ngl::Vector& _col,
    const ngl::Vector& _up )
{
	SpotLight tmp( _position, _target, _distance, _angle, _col, _up );
	m_spotLights.push_back( tmp );
}
void LightingEngine::addShadowSpot(
    const ngl::Vector& _position,
    const ngl::Vector& _target,
    const float& _distance,
    const float& _angle,
    const ngl::Vector& _col,
    const ngl::Vector& _up )
{
	ShadowSpot tmp( _position, _target, _distance, _angle, _col, _up );
	m_shadowSpots.push_back( tmp );
}
