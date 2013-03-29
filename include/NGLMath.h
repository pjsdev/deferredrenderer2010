#ifndef NGLMATH_H
#define NGLMATH_H

#include "ngl/Matrix.h"
#include "ngl/Vector.h"
#include "math.h"

/// @brief cotangent declaration
/// @param [in] _i float value to find cotan of
inline float cotan( const float& _i )
{
	return( 1 / tan( _i ) );
}

/// @brief method for constructing view matrix based on a approximated 'up' vector
/// @param [in] _eye the position of the source
/// @param [in] _target the position the source is looking at
/// @param [in] _lookup the approximated 'up' vector for this source
/// @return ngl::Matrix view, view matrix for this lookat
ngl::Matrix Lookat( const ngl::Vector& _eye, const ngl::Vector& _target, const ngl::Vector& _lookup );

/// @brief calculated adjoint of matrix
/// @param [in] _mat the matrix to calculate from
/// @return adjoint
ngl::Matrix Adj( const ngl::Matrix& _mat );

/// @brief calculated determinant of matrix
/// @param [in] _mat the matrix to calculate from
/// @return determinant
float Det( const ngl::Matrix& _mat );

/// @brief the inverse of a matrix
/// @param [in] _mat the matrix to calculate from
/// @return inverse
ngl::Matrix Inverse( const ngl::Matrix& _mat );

#endif // NGLMATH_H
