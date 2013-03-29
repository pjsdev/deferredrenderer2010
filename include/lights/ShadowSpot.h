#ifndef SHADOWSPOT_H
#define SHADOWSPOT_H

#include "SpotLight.h"
#include "ngl/Matrix.h"
#include "Frustum.h"

/// @file ShadowSpot.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class ShadowSpot
/// @brief a shadow casting spotlight
class ShadowSpot : public SpotLight
{
public:
	/// @brief
	/// @param [in] _position the light pos
	/// @param [in] _target the light lookat pos
	/// @param [in] _distance the reach of the light
	/// @param [in] _angle the cone angle
	/// @param [in] _col the light colour
	/// @param [in] _up the apporximated up vector
	ShadowSpot( const ngl::Vector& _position, const ngl::Vector& _target, const float& _distance, const float& _angle, const ngl::Vector& _col, const ngl::Vector& _up );
	
	ShadowSpot();
	
	/// @brief dtor
	~ShadowSpot() {}
	
	/// @brief access the shadowmap id
	unsigned int getMapId() {
		return m_mapHandle;
	}
	
	/// @brief construct the projection, inversprojection and projView matrices for this shadowspot
	void generateProjMatrices();
	
	/// @brief return the lights frustum
	inline Frustum getFrustum() {
		return m_frustum;
	} const
	
	/// @brief return the light projview
	inline ngl::Matrix getProjView() {
		return m_projView;
	} const
	
	/// @brief return the light projection matrix
	inline ngl::Matrix getProj() {
		return m_proj;
	} const
	
	/// @brief return the inverse projview
	inline ngl::Matrix getInverseProjView() {
		return m_invProjView;
	}
	
	/// @brief the shadowview matrix for shadow pass
	ngl::Matrix m_shadowView;
	
	/// @brief shadowmap handle
	unsigned int m_mapHandle;
	
private:
	/// @brief the lights frustum
	Frustum m_frustum;
	
	/// @brief the lights cone angle
	float m_angle;
	
	/// @brief the lights proj matrix
	ngl::Matrix m_proj;
	
	/// @brief the lights projview matrix
	ngl::Matrix m_projView;
	
	/// @brief the lights inverse projview matrix
	ngl::Matrix m_invProjView;
	
};

#endif // SHADOWSPOT_H
