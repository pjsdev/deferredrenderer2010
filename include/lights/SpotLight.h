#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "ngl/Vector.h"
#include "ngl/Matrix.h"
#include "ngl/Camera.h"


/// @file SpotLight.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class SpotLight
/// @brief Spotlight for deferred lighting
class SpotLight
{
public:

	SpotLight() {}
	
	/// @brief ctor
	/// @param [in] _position the light pos
	/// @param [in] _target the light lookat pos
	/// @param [in] _distance the reach of the light
	/// @param [in] _angle the cone angle
	/// @param [in] _col the light colour
	/// @param [in] _up the apporximated up vector
	SpotLight( const ngl::Vector& _position, const ngl::Vector& _target, const float& _distance, const float& _angle, const ngl::Vector& _col, const ngl::Vector& _up );
	
	/// @brief
	~SpotLight();
	
	/// @brief set method for the lights values
	/// @param [in] _position the light pos
	/// @param [in] _target the light lookat pos
	/// @param [in] _distance the reach of the light
	/// @param [in] _angle the cone angle
	/// @param [in] _col the light colour
	/// @param [in] _up the apporximated up vector
	void set( const ngl::Vector& _position, const ngl::Vector& _target, const float& _distance, const float& _angle, const ngl::Vector& _col, const ngl::Vector& _up );
	
	/// @brief check for collision with sphere
	/// @param [in] _pos sphere position
	/// @param [in] _radius sphere radius
	bool collidesWithSphere( const ngl::Vector& _pos, const float& _rad );
	
	/// @brief method for drawing the spotlight
	/// @param [in] _vaoName the ngl vboPrimitive of spotlight geometry
	/// @param [in] _shaderName the name of the shader to draw with
	void draw( const std::string& _vaoName, const std::string& _shader );
	
	/// @brief access the light view matrix
	inline ngl::Matrix getView() {
		return m_lightView;
	}
	
	/// @brief acces the light world matrix
	inline ngl::Matrix getWorld() {
		return m_world;
	}
	
protected:

	/// @brief a temporary use of ngl camera for shadow matrices
	ngl::Camera m_matrixCam;
	
	/// @brief the lights view matrix
	ngl::Matrix m_lightView;
	
private:

	/// @brief radius of spotlight for near plane clipping
	float m_radius;
	
	/// @brief world matrix of spotlight
	ngl::Matrix m_world;
	
	/// @brief the position [xyz] and angle [w] of light
	ngl::Vector m_positionAngle;
	
	/// @brief the direction [xyz] and distance [w] of light
	ngl::Vector m_directionDistance;
	
	/// @brief the light colour
	ngl::Vector m_colour;
};

#endif // SPOTLIGHT_H
