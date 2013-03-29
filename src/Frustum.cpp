#include "Frustum.h"
#include "ngl/Vector.h"

void Frustum::buildPlane( Plane& _plane, const ngl::Matrix& _mat, int _row, int _sign )
{
	_plane.a = _mat.m_m[0][3] + _sign * _mat.m_m[0][_row];
	_plane.b = _mat.m_m[1][3] + _sign * _mat.m_m[1][_row];
	_plane.c = _mat.m_m[2][3] + _sign * _mat.m_m[2][_row];
	_plane.d = _mat.m_m[3][3] + _sign * _mat.m_m[3][_row];
	
	normalizePlane( _plane );
}

void Frustum::generate( const ngl::Matrix& _mat )
{
	buildPlane( m_planes[e_left], _mat, 0, e_plus );
	buildPlane( m_planes[e_right], _mat, 0, e_minus );
	buildPlane( m_planes[e_bottom], _mat, 1, e_plus );
	buildPlane( m_planes[e_top], _mat, 1, e_minus );
	buildPlane( m_planes[e_near], _mat, 2, e_plus );
	buildPlane( m_planes[e_far], _mat, 2, e_minus );
}

bool Frustum::isAbovePlane( unsigned int _planeSide, const ngl::Vector& _point )const
{
	return m_planes[_planeSide].a * _point.m_x + m_planes[_planeSide].b * _point.m_y + m_planes[_planeSide].c * _point.m_z + m_planes[_planeSide].d >= 0;
}

bool Frustum::isInFrustum( const ngl::Vector& _point )const
{
	for ( unsigned int i = 0; i < 6; i++ ) {
		if ( !isAbovePlane( i, _point ) ) {
			return false;
		}
	}
	
	return true;
}

bool Frustum::isInFrustum( const ngl::Vector& _min, const ngl::Vector& _max )const
{
	ngl::Vector vmin, vmax;
	
	for ( unsigned int i = 0; i < 6; i++ ) {
		if ( m_planes[i].a > 0 ) {
			vmin.m_x = _min.m_x;
			vmax.m_x = _max.m_x;
		}
		else {
			vmin.m_x = _max.m_x;
			vmax.m_x = _min.m_x;
		}
		
		if ( m_planes[i].b > 0 ) {
			vmin.m_y = _min.m_y;
			vmax.m_y = _max.m_y;
		}
		else {
			vmin.m_y = _max.m_y;
			vmax.m_y = _min.m_y;
		}
		
		if ( m_planes[i].c > 0 ) {
			vmin.m_z = _min.m_z;
			vmax.m_z = _max.m_z;
		}
		else {
			vmin.m_z = _max.m_z;
			vmax.m_z = _min.m_z;
		}
		
		if ( !isAbovePlane( i, vmax ) ) {
			return false;
		}
	}
	
	return true;
	
}

void Frustum::normalizePlane( Plane& _plane )
{
	float mag;
	mag = sqrt( _plane.a * _plane.a + _plane.b * _plane.b + _plane.c * _plane.c );
	_plane.a = _plane.a / mag;
	_plane.b = _plane.b / mag;
	_plane.c = _plane.c / mag;
	_plane.d = _plane.d / mag;
	
}
