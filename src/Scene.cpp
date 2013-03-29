#include "Scene.h"
#include "string"
#include "ngl/TransformStack.h"
#include "ngl/ShaderManager.h"
#include "ngl/Material.h"
#include "ngl/Obj.h"
#include "ngl/NCCABinMesh.h"
#include <QImage>
//#include "ngl/VBOPrimitives.h"


#ifdef DARWIN
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

Scene::Scene( int _lightNum )
{
	m_ySpin = 0.0;
	m_pointlights.resize( _lightNum );
	m_lightVAO = 0;
}

Scene::~Scene()
{

}

void Scene::drawLights( const ngl::Vector& _camPos, unsigned int _pointLightVAO )
{
	glPointSize( 2.0 );
	glEnable( GL_POINT_SMOOTH );
	
	for( unsigned int i = 0; i < m_pointlights.size(); i++ ) {
		if( m_pointlights[i].collidesWithSphere( _camPos, 10.0 ) ) {
			glCullFace( GL_FRONT );
			glDisable( GL_DEPTH_TEST );
			m_pointlights[i].draw( _pointLightVAO, "PointLight" );
			glEnable( GL_DEPTH_TEST );
			glCullFace( GL_BACK );
		}
		else {
			m_pointlights[i].draw( _pointLightVAO, "PointLight" );
		}
		
		
		// draw the points to represent the demo lights
		ngl::ShaderManager* shader = ngl::ShaderManager::instance();
		( *shader )["DemoPL"]->use();
		
		shader->setShaderParam3f( "DemoPL",
		                          "u_Offset",
		                          m_pointlights[i].m_pos.m_x,
		                          m_pointlights[i].m_pos.m_y,
		                          m_pointlights[i].m_pos.m_z
		                        );
		                        
		glBindVertexArray( m_lightVAO );
		
		glDrawArrays( GL_POINTS, 0, 1 );
		
		glBindVertexArray( 0 );
	}
	
	glPointSize( 1.0 );
	glDisable( GL_POINT_SMOOTH );
}

void Scene::moveLights( const float& _time )
{
	float speed = 0.01;
	float amp = 2.7;
	
	float movement = sin( _time * speed );
	
	movement = ( movement * 0.5 ) + 0.5;
	amp += movement * 2.4;
	
	for( unsigned int i = 0; i < m_pointlights.size(); i++ ) {
		m_pointlights[i].m_pos = ngl::Vector( sin( i + _time * speed ) * amp, m_pointlights[i].m_pos.m_y, cos( i + _time * speed ) * amp ); //+ngl::Vector(5,0,5);
	}
}

void Scene::draw( const std::string& _shaderName, const Frustum& _f, bool _ySpin )
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	m_baseTrans.rotateY( m_ySpin );
	
	if( _ySpin ) {
		m_ySpin += 0.1;
	}
	
	( *shader )[_shaderName]->use();
	
	ngl::Material m( ngl::GOLD );
	m.use();
	
	//    for(unsigned int i = 0; i < m_gameObjects.size(); i++)
	//    {
	//        ngl::Matrix model ,rot, trans, scale;
	//        trans.translate(m_gameObjects[i].trans.m_x, m_gameObjects[i].trans.m_y, m_gameObjects[i].trans.m_z);
	//        rot.rotateY(m_gameObjects[i].rot.m_y);
	//        scale.scale(m_gameObjects[i].scale.m_x, m_gameObjects[i].scale.m_y, m_gameObjects[i].scale.m_z);
	
	//        model = trans*rot*scale;
	
	//        shader->setShaderParamFromMatrix(_shaderName,"u_ModelMatrix",model);
	
	//        //if( _f.isInFrustum(m_gameObjects[i].trans))
	//        //{
	//            glBindTexture(GL_TEXTURE_2D, m_gameObjects[i].texID);
	//            m_gameObjects[i].geo.draw();
	//        //}
	
	//    }
	
	//  glBindTexture(GL_TEXTURE_2D, m_blankTex);
	
	
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void Scene::loadTexture( const char* _texName, GLuint& _texID )
{
	QImage* image = new QImage();
	bool loaded = image->load( _texName );
	
	if( loaded == true ) {
		int width = image->width();
		int height = image->height();
		unsigned char* data = new unsigned char[width * height * 3];
		unsigned int index = 0;
		QRgb colour;
		
		for( int y = 0; y < height; ++y ) {
			for( int x = 0; x < width; ++x ) {
				colour = image->pixel( x, y );
				
				data[index++] = qRed( colour );
				data[index++] = qGreen( colour );
				data[index++] = qBlue( colour );
			}
		}
		
		glGenTextures( 1, &_texID );
		glBindTexture( GL_TEXTURE_2D, _texID );
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
		
		glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
}

void Scene::addGameObject(
    const std::string& _obj,
    const char* _tex,
    const ngl::Vector& _trans,
    const ngl::Vector& _rot,
    const ngl::Vector& _scale
)
{
	//GameObject tmp;
	
	
	//    tmp.geo = ngl::Obj(_obj);
	//    GLuint tmpTex;
	//    loadTexture(_tex, tmpTex);
	//    tmp.texID = tmpTex;
	//    tmp.scale = _scale;
	//    tmp.trans = _trans;
	//    tmp.rot = _rot;
	//    m_gameObjects.push_back(tmp);
}

void Scene::init()
{
	loadTexture( "textures/BLANK.tiff", m_blankTex );
	
	//addGameObject("models/TrollLowRes.obj", "textures/TrollColour.tiff", ngl::Vector(5,1.85,5), ngl::Vector(0,0,0), ngl::Vector(5,5,5));
	//addGameObject("models/ControlBox1.obj", "textures/ControlBox1.tiff", ngl::Vector(3.5,-1,-4), ngl::Vector(0,-90,0), ngl::Vector(0.1,0.1,0.1));
	//    addGameObject("models/ControlBox2.obj",
	//                  "textures/ControlBox2.tiff",
	//                  ngl::Vector(6.0f,-1.0f,-4.0f,1.0f),
	//                  ngl::Vector(0.0f,-90.0f,0.0f,1.0f),
	//                  ngl::Vector(0.1f,0.1f,0.1f,1.0f));
	//    addGameObject("models/TNT.obj",
	//                  "textures/TNT.tiff",
	//                  ngl::Vector(6.0f,0.25f,-4.0f,1.0f),
	//                  ngl::Vector(0.0f,-90.0f,0.0f,1.0f),
	//                  ngl::Vector(0.1f,0.1f,0.1f,1.0f));
	//    addGameObject("models/Street.obj",
	//                  "textures/Street.tiff",
	//                  ngl::Vector(0.0f,-1.0f,0.0f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(20.0f,20.0f,20.0f,1.0f));
	//    addGameObject("models/Ruins.obj",
	//                  "textures/Ruins.tiff",
	//                  ngl::Vector(-1.0f,-1.0f,5.0f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(0.5f,0.5f,0.5f,1.0f));
	//    addGameObject("models/Ruins2.obj",
	//                  "textures/Ruins.tiff",
	//                  ngl::Vector(1.0f,-1.0f,1.0f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(0.5f,0.5f,0.5f,1.0f));
	//    addGameObject("models/Minebeams.obj",
	//                  "textures/Minebeams.tiff",
	//                  ngl::Vector(5.2f,-2.0f,0.6f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(0.42f,0.4f,0.65f,1.0f));
	//    addGameObject("models/Plane.obj",
	//                  "textures/Sky.jpg",
	//                  ngl::Vector(5.2f,2.0f,-8.0f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(10.0f,10.0f,10.0f,1.0f));
	
	//    addGameObject("models/Killerroo.obj",
	//                  "textures/Sky.jpg",
	//                  ngl::Vector(5.2f,2.0f,-8.0f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(10.0f,10.0f,10.0f,1.0f));
	
	//    addGameObject("models/WaterTower.obj",
	//                  "textures/WaterTower.tiff",
	//                  ngl::Vector(9.2f,-2.0f,1.6f,1.0f),
	//                  ngl::Vector(0.0f,0.0f,0.0f,1.0f),
	//                  ngl::Vector(0.1f,0.1f,0.1f,1.0f));
	
	//    for(unsigned int i = 0; i < m_gameObjects.size(); i++)
	//    {
	//        m_gameObjects[i].geo.createVBO(GL_STATIC_DRAW);
	//    }
	
	
	
	/////////////////////////
	setupLights();
	std::cout << "Scene Created" << std::endl;
}

void Scene::setupLights()
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	for( unsigned int i = 0; i < m_pointlights.size(); i++ ) {
		ngl::Vector tmp;
		tmp = ngl::Vector( sin( i ) * 2.0, ( float )i * 0.06, cos( i ) * 2.0 ) + 0.5;
		m_pointlights[i] = PointLight( tmp + ngl::Vector( 5, 0, 5 ), 2.2, ngl::Vector( 1, 1, 1 ) );
	}
	
	////////////////////////
	//setup light points
	( *shader )["DemoPL"]->use();
	
	float vertices[] = {0.0, 0.0, 0.0};
	
	glGenVertexArrays( 1, &m_lightVAO );
	glBindVertexArray( m_lightVAO );
	
	GLuint vboIDLight;
	glGenBuffers( 1, &vboIDLight );
	
	glBindBuffer( GL_ARRAY_BUFFER, vboIDLight );
	glBufferData( GL_ARRAY_BUFFER, 3 * sizeof( GLfloat ), vertices, GL_STATIC_DRAW );
	
	( *shader )["DemoPL"]->vertexAttribPointer( "a_VertPos", 3, GL_FLOAT, 0, 0 );
	( *shader )["DemoPL"]->enableAttribArray( "a_VertPos" );
	
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
}
