#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <string>
#include "ngl/Vector.h"

/// @file PointLight.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class PointLight
/// @brief Deferred PointLight class using simple geometry to send to the GPU for per-pixel lighting
class PointLight
{
public:
	/// @brief ctor
	PointLight();
	
	/// @brief ctor taking the radius, centre and intensity colour of light
	/// @param [in] _centre the world space position of light
	/// @param [in] _radius the extent of lights influence
	/// @param [in] _intensity the colour value of the light
	PointLight( const ngl::Vector& _centre, const float& _radius, const ngl::Vector& _intensity );
	
	/// @brief dtor
	~PointLight();
	
	/// @brief method for drawing the pointlight
	/// @param [in] _vao the vao id of pointlight geometry
	/// @param [in] _shaderName the name of the shader to draw with
	void draw( unsigned int& _vao, const std::string& _shaderName );
	
	/// @brief check for collision with sphere
	/// @param [in] _pos sphere position
	/// @param [in] _radius sphere radius
	bool collidesWithSphere( const ngl::Vector& _pos, const float& _radius );
	
	/// @brief the light position, public for easier access
	ngl::Vector m_pos;
private:
	/// @brief the light radius
	float m_radius;
	
	/// @brief the light colour intensity
	ngl::Vector m_intensity;
};

#endif // POINTLIGHT_H
