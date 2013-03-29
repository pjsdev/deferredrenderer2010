#include "NGLMath.h"


ngl::Matrix Lookat( const ngl::Vector& _eye, const ngl::Vector& _target, const ngl::Vector& _lookup )
{

	ngl::Vector minusEye = -_eye;
	ngl::Vector w = _eye - _target;
	ngl::Vector u = _lookup.cross( w );
	ngl::Vector v = w.cross( u );
	
	w.normalize();
	v.normalize();
	u.normalize();
	
	ngl::Matrix view;
	
	view.m_00 = u.m_x;
	view.m_01 = u.m_y;
	view.m_02 = u.m_z;
	view.m_03 = minusEye.dot( u );
	
	view.m_10 = v.m_x;
	view.m_11 = v.m_y;
	view.m_12 = v.m_z;
	view.m_13 = minusEye.dot( v );
	
	view.m_20 = w.m_x;
	view.m_21 = w.m_y;
	view.m_22 = w.m_z;
	view.m_23 = minusEye.dot( w );
	
	view.m_30 = 0.0;
	view.m_31 = 0.0;
	view.m_32 = 0.0;
	view.m_33 = 1.0;
	
	
	return view;
}

ngl::Matrix Adj( const ngl::Matrix& _mat )
{
	ngl::Matrix m;
	
	m.m_00 = _mat.m_m[1][1] * ( _mat.m_m[2][2] * _mat.m_m[3][3] - _mat.m_m[3][2] * _mat.m_m[2][3] ) + _mat.m_m[1][2] * ( _mat.m_m[3][1] * _mat.m_m[2][3] - _mat.m_m[2][1] * _mat.m_m[3][3] ) + _mat.m_m[1][3] * ( _mat.m_m[2][1] * _mat.m_m[3][2] - _mat.m_m[3][1] * _mat.m_m[2][2] );
	m.m_01 = _mat.m_m[1][0] * ( _mat.m_m[3][2] * _mat.m_m[2][3] - _mat.m_m[2][2] * _mat.m_m[3][3] ) + _mat.m_m[1][2] * ( _mat.m_m[2][0] * _mat.m_m[3][3] - _mat.m_m[3][0] * _mat.m_m[2][3] ) + _mat.m_m[1][3] * ( _mat.m_m[3][0] * _mat.m_m[2][2] - _mat.m_m[2][0] * _mat.m_m[3][2] );
	m.m_02 = _mat.m_m[1][0] * ( _mat.m_m[2][1] * _mat.m_m[3][3] - _mat.m_m[3][1] * _mat.m_m[2][3] ) + _mat.m_m[1][1] * ( _mat.m_m[3][0] * _mat.m_m[2][3] - _mat.m_m[2][0] * _mat.m_m[3][3] ) + _mat.m_m[1][3] * ( _mat.m_m[2][0] * _mat.m_m[3][1] - _mat.m_m[3][0] * _mat.m_m[2][1] );
	m.m_03 = _mat.m_m[1][0] * ( _mat.m_m[3][1] * _mat.m_m[2][2] - _mat.m_m[2][1] * _mat.m_m[3][2] ) + _mat.m_m[1][1] * ( _mat.m_m[2][0] * _mat.m_m[3][2] - _mat.m_m[3][0] * _mat.m_m[2][2] ) + _mat.m_m[1][2] * ( _mat.m_m[3][0] * _mat.m_m[2][1] - _mat.m_m[2][0] * _mat.m_m[3][1] );
	
	m.m_10 = _mat.m_m[0][1] * ( _mat.m_m[3][2] * _mat.m_m[2][3] - _mat.m_m[2][2] * _mat.m_m[3][3] ) + _mat.m_m[0][2] * ( _mat.m_m[2][1] * _mat.m_m[3][3] - _mat.m_m[3][1] * _mat.m_m[2][3] ) + _mat.m_m[0][3] * ( _mat.m_m[3][1] * _mat.m_m[2][2] - _mat.m_m[2][1] * _mat.m_m[3][2] );
	m.m_11 = _mat.m_m[0][0] * ( _mat.m_m[2][2] * _mat.m_m[3][3] - _mat.m_m[3][2] * _mat.m_m[2][3] ) + _mat.m_m[0][2] * ( _mat.m_m[3][0] * _mat.m_m[2][3] - _mat.m_m[2][0] * _mat.m_m[3][3] ) + _mat.m_m[0][3] * ( _mat.m_m[2][0] * _mat.m_m[3][2] - _mat.m_m[3][0] * _mat.m_m[2][2] );
	m.m_12 = _mat.m_m[0][0] * ( _mat.m_m[3][1] * _mat.m_m[2][3] - _mat.m_m[2][1] * _mat.m_m[3][3] ) + _mat.m_m[0][1] * ( _mat.m_m[2][0] * _mat.m_m[3][3] - _mat.m_m[3][0] * _mat.m_m[2][3] ) + _mat.m_m[0][3] * ( _mat.m_m[3][0] * _mat.m_m[2][1] - _mat.m_m[2][0] * _mat.m_m[3][1] );
	m.m_13 =  _mat.m_m[0][0] * ( _mat.m_m[2][1] * _mat.m_m[3][2] - _mat.m_m[3][1] * _mat.m_m[2][2] ) + _mat.m_m[0][1] * ( _mat.m_m[3][0] * _mat.m_m[2][2] - _mat.m_m[2][0] * _mat.m_m[3][2] ) + _mat.m_m[0][2] * ( _mat.m_m[2][0] * _mat.m_m[3][1] - _mat.m_m[3][0] * _mat.m_m[2][1] );
	
	m.m_20 = _mat.m_m[0][1] * ( _mat.m_m[1][2] * _mat.m_m[3][3] - _mat.m_m[3][2] * _mat.m_m[1][3] ) + _mat.m_m[0][2] * ( _mat.m_m[3][1] * _mat.m_m[1][3] - _mat.m_m[1][1] * _mat.m_m[3][3] ) + _mat.m_m[0][3] * ( _mat.m_m[1][1] * _mat.m_m[3][2] - _mat.m_m[3][1] * _mat.m_m[1][2] );
	m.m_21 = _mat.m_m[0][0] * ( _mat.m_m[3][2] * _mat.m_m[1][3] - _mat.m_m[1][2] * _mat.m_m[3][3] ) + _mat.m_m[0][2] * ( _mat.m_m[1][0] * _mat.m_m[3][3] - _mat.m_m[3][0] * _mat.m_m[1][3] ) + _mat.m_m[0][3] * ( _mat.m_m[3][0] * _mat.m_m[1][2] - _mat.m_m[1][0] * _mat.m_m[3][2] );
	m.m_22 = _mat.m_m[0][0] * ( _mat.m_m[1][1] * _mat.m_m[3][3] - _mat.m_m[3][1] * _mat.m_m[1][3] ) + _mat.m_m[0][1] * ( _mat.m_m[3][0] * _mat.m_m[1][3] - _mat.m_m[1][0] * _mat.m_m[3][3] ) + _mat.m_m[0][3] * ( _mat.m_m[1][0] * _mat.m_m[3][1] - _mat.m_m[3][0] * _mat.m_m[1][1] );
	m.m_23 = _mat.m_m[0][0] * ( _mat.m_m[3][1] * _mat.m_m[1][2] - _mat.m_m[1][1] * _mat.m_m[3][2] ) + _mat.m_m[0][1] * ( _mat.m_m[1][0] * _mat.m_m[3][2] - _mat.m_m[3][0] * _mat.m_m[1][2] ) + _mat.m_m[0][2] * ( _mat.m_m[3][0] * _mat.m_m[1][1] - _mat.m_m[1][0] * _mat.m_m[3][1] );
	
	m.m_30 = _mat.m_m[0][1] * ( _mat.m_m[2][2] * _mat.m_m[1][3] - _mat.m_m[1][2] * _mat.m_m[2][3] ) + _mat.m_m[0][2] * ( _mat.m_m[1][1] * _mat.m_m[2][3] - _mat.m_m[2][1] * _mat.m_m[1][3] ) + _mat.m_m[0][3] * ( _mat.m_m[2][1] * _mat.m_m[1][2] - _mat.m_m[1][1] * _mat.m_m[2][2] );
	m.m_31 = _mat.m_m[0][0] * ( _mat.m_m[1][2] * _mat.m_m[2][3] - _mat.m_m[2][2] * _mat.m_m[1][3] ) + _mat.m_m[0][2] * ( _mat.m_m[2][0] * _mat.m_m[1][3] - _mat.m_m[1][0] * _mat.m_m[2][3] ) + _mat.m_m[0][3] * ( _mat.m_m[1][0] * _mat.m_m[2][2] - _mat.m_m[2][0] * _mat.m_m[1][2] );
	m.m_32 = _mat.m_m[0][0] * ( _mat.m_m[2][1] * _mat.m_m[1][3] - _mat.m_m[1][1] * _mat.m_m[2][3] ) + _mat.m_m[0][1] * ( _mat.m_m[1][0] * _mat.m_m[2][3] - _mat.m_m[2][0] * _mat.m_m[1][3] ) + _mat.m_m[0][3] * ( _mat.m_m[2][0] * _mat.m_m[1][1] - _mat.m_m[1][0] * _mat.m_m[2][1] );
	m.m_33 = _mat.m_m[0][0] * ( _mat.m_m[1][1] * _mat.m_m[2][2] - _mat.m_m[2][1] * _mat.m_m[1][2] ) + _mat.m_m[0][1] * ( _mat.m_m[2][0] * _mat.m_m[1][2] - _mat.m_m[1][0] * _mat.m_m[2][2] ) + _mat.m_m[0][2] * ( _mat.m_m[1][0] * _mat.m_m[2][1] - _mat.m_m[2][0] * _mat.m_m[1][1] );
	
	return m;
}

float Det( const ngl::Matrix& _mat )
{
	// Our matrices are 4.4 only, so we can just write the full formula instead of a complex algorithm.
	return (
	           _mat.m_m[0][0] * _mat.m_m[1][1] * _mat.m_m[2][2] * _mat.m_m[3][3] - _mat.m_m[0][0] * _mat.m_m[1][1] * _mat.m_m[2][3] * _mat.m_m[3][2] + _mat.m_m[0][0] * _mat.m_m[1][2] * _mat.m_m[2][3] * _mat.m_m[3][1] - _mat.m_m[0][0] * _mat.m_m[1][2] * _mat.m_m[2][1] * _mat.m_m[3][3] + _mat.m_m[0][0] * _mat.m_m[1][3] * _mat.m_m[2][1] * _mat.m_m[3][2] - _mat.m_m[0][0] * _mat.m_m[1][3] * _mat.m_m[2][2] * _mat.m_m[3][1]
	           - _mat.m_m[1][0] * _mat.m_m[2][1] * _mat.m_m[3][2] * _mat.m_m[0][3] + _mat.m_m[1][0] * _mat.m_m[2][1] * _mat.m_m[0][2] * _mat.m_m[3][3] - _mat.m_m[1][0] * _mat.m_m[3][1] * _mat.m_m[0][2] * _mat.m_m[2][3] + _mat.m_m[1][0] * _mat.m_m[3][1] * _mat.m_m[2][2] * _mat.m_m[0][3] - _mat.m_m[1][0] * _mat.m_m[0][1] * _mat.m_m[2][2] * _mat.m_m[3][3] + _mat.m_m[1][0] * _mat.m_m[0][1] * _mat.m_m[3][2] * _mat.m_m[2][3]
	           + _mat.m_m[2][0] * _mat.m_m[3][1] * _mat.m_m[0][2] * _mat.m_m[1][3] - _mat.m_m[2][0] * _mat.m_m[3][1] * _mat.m_m[1][2] * _mat.m_m[0][3] + _mat.m_m[2][0] * _mat.m_m[0][1] * _mat.m_m[1][2] * _mat.m_m[3][3] - _mat.m_m[2][0] * _mat.m_m[0][1] * _mat.m_m[3][2] * _mat.m_m[1][3] + _mat.m_m[2][0] * _mat.m_m[1][1] * _mat.m_m[3][2] * _mat.m_m[0][3] - _mat.m_m[2][0] * _mat.m_m[1][1] * _mat.m_m[0][2] * _mat.m_m[3][3]
	           - _mat.m_m[3][0] * _mat.m_m[0][1] * _mat.m_m[1][2] * _mat.m_m[2][3] + _mat.m_m[3][0] * _mat.m_m[0][1] * _mat.m_m[2][2] * _mat.m_m[1][3] - _mat.m_m[3][0] * _mat.m_m[1][1] * _mat.m_m[2][2] * _mat.m_m[0][3] + _mat.m_m[3][0] * _mat.m_m[1][1] * _mat.m_m[0][2] * _mat.m_m[2][3] - _mat.m_m[3][0] * _mat.m_m[2][1] * _mat.m_m[0][2] * _mat.m_m[1][3] + _mat.m_m[3][0] * _mat.m_m[2][1] * _mat.m_m[1][2] * _mat.m_m[0][3]
	       );
}

ngl::Matrix Inverse( const ngl::Matrix& _mat )
{
	return Adj( _mat ) * ( 1.0 / Det( _mat ) );
}
