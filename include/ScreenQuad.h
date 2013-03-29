#ifndef SCREEN_QUAD_H
#define SCREEN_QUAD_H

#include "ngl/Vector.h"
#include "ngl/TransformStack.h"

/// @file ScreenQuad.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class ScreenQuad
/// @brief A screenquad for drawing textures onto quads for projection
class ScreenQuad
{
public:
	/// @brief ctor
	ScreenQuad();
	
	/// @brief construct our screen quad with screen pos and extent
	/// @param [in] _x screen X
	/// @param [in] _y screeb Y
	/// @param [in] _extent size of quad
	/// @param [in] _depth the screen depth
	ScreenQuad( const float& _x, const float& _y, const float& _extent, const float& _depth );
	
	/// @brief dtor
	~ScreenQuad();
	
	void genVAO();
	
	/// @brief draw the quad
	/// @param [in] _shaderName our screenquad shader
	/// @param [in] _our screenquad vao id
	void draw( const std::string& _shaderName, unsigned int& _vao );
private:
	/// @brief quad screen x value
	float m_x;
	/// @brief quad screen y value
	float m_y;
	/// @brief quads size
	float m_extent;
	/// @brief quad screen z value
	float m_depth;
};

#endif // ScreenQuad_H
