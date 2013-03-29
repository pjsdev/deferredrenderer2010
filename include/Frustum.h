#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "ngl/Matrix.h"

/// @file Frustum.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class Frustum
/// @brief A generic frustum class that can be generated from any Projview, contains basic point and Bbox culling methods
/// Modified from :-
/// Mathieu Sanchez (2010)
class Frustum
{
private:

	/// @brief a basic structure for plane equation ax+by+cz+d = 0
	struct Plane {
		float a, b, c, d;
	};
	
	/// @brief enum for identifying the 6 sides of the frustum
	/// @enum planeSides
	enum planeSides {
	    e_left = 0,
	    e_right = 1,
	    e_bottom = 2,
	    e_top = 3,
	    e_near = 4,
	    e_far = 5
	};
	
	/// @brief enum used to denote which way the plane is facing
	/// @enum planeSign
	enum planeSign {
	    e_minus = -1,
	    e_plus = 1
	};
	
	/// @brief the 6 sides of the frustum
	Plane m_planes[6];
	
	/// @brief build a plane
	/// @param [inout] _plane the plane to buil
	/// @param [in] _mat the matrix to build it from
	/// @param [in] _row the matrix row to build from
	/// @param [in] _sign the direction of the plane normal
	void buildPlane( Plane& io_plane, const ngl::Matrix& _mat, int _row, int _sign );
	
	/// @brief normalise the plane
	/// @param [inout] the plane to be normalised
	void normalizePlane( Plane& _plane );
	
public:
	/// @brief empty ctor
	Frustum() {}
	
	/// @brief method to generate the frustum from a projView matrix
	/// @param [in] _mat the matrix to build from
	void generate( const ngl::Matrix& _mat );
	
	/// @brief collision check for a point against a frustum plane
	/// @param [in] _planeSide the side of the frustum to check
	/// @param [in] _point the point to check
	bool isAbovePlane( unsigned int _planeSide, const ngl::Vector& _point )const;
	
	/// @brief bbox check inside frustum
	/// @param [in] _min the lower extent of bbox
	/// @param [in] _max the upper extent of bbox
	bool isInFrustum( const ngl::Vector& _min, const ngl::Vector& _max )const;
	
	/// @brief point check inside frustum
	/// @param [in] _point the point to check
	bool isInFrustum( const ngl::Vector& _point )const;
	
};

#endif // FRUSTUM_H
