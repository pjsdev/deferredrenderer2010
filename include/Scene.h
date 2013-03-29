#ifndef SCENE_H
#define SCENE_H

#include "string"
#include "ngl/TransformStack.h"
#include "ngl/Matrix.h"
#include "Frustum.h"
#include "ngl/Obj.h"
#include "PointLight.h"
#include "ngl/NCCABinMesh.h"
#include "ngl/Obj.h"

//struct GameObject{
//    ngl::Obj geo;
//    GLuint texID;
//    ngl::Vector trans,rot,scale;
//};

/// @file Scene.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class Scene
/// @brief A demo scene class for exhibiting this deferred renderer
class Scene
{
public:

	/// @brief ctor
	/// @param [in] _lightNum the number of lights for demo scene
	Scene( int _lightNum );
	
	/// @brief dtor
	~Scene();
	
	/// @brief initialise demo scene geometry
	void init();
	
	/// @brief move method for updating
	/// @param [in] _time the current time since beginning
	void moveLights( const float& _time );
	
	/// @brief draw our pointlight vector
	/// @param [in] _camPos used for clip testing
	/// @param [in] _pointLightVAO the vao id of pointlight geometry
	void drawLights( const ngl::Vector& _camPos, unsigned int _pointLightVAO );
	
	void loadTexture( const char* _texName, GLuint& _texID );
	
	/// @brief the geometry drawing routine
	/// @param [in] _shaderName the main shader to draw with
	/// @param [in] _f the frustum to clip against
	void draw( const std::string& _shaderName, const Frustum& _f, bool turntable );
private:
	void addGameObject(
	    const std::string& _obj,
	    const char* _tex,
	    const ngl::Vector& _trans,
	    const ngl::Vector& _rot,
	    const ngl::Vector& _scale
	);
	
	void setupLights();
	
	GLuint m_blankTex;
	
	//std::vector<GameObject> m_gameObjects;
	
	/// @brief our lightVAO id
	unsigned int m_lightVAO;
	
	/// @brief the demo lights vector
	std::vector<PointLight> m_pointlights;
	
	float m_ySpin;
	
	ngl::Matrix m_baseTrans;
};

#endif // SCENE_H
