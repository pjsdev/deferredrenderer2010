#ifndef LIGHTING_ENGINE_H
#define LIGHTING_ENGINE_H

#include "PointLight.h"
#include "SpotLight.h"
#include "ShadowSpot.h"
#include "ngl/ShaderManager.h"
#include "TextureEngine.h"

class LightingEngine
{
public:
	LightingEngine();
	~LightingEngine() {}
	
	inline void addPointLight( PointLight& _point ) {
		m_pointLights.push_back( _point );
		std::cout << m_pointLights.size() << std::endl;
	}
	inline void addSpotLight( SpotLight& _spot ) {
		m_spotLights.push_back( _spot );
	}
	inline void addShadowSpot( ShadowSpot& _spot ) {
		m_shadowSpots.push_back( _spot );
	}
	
	void addPointLight(
	    const ngl::Vector& _centre,
	    const float& _radius,
	    const ngl::Vector& _intensity );
	    
	void addSpotLight(
	    const ngl::Vector& _position,
	    const ngl::Vector& _target,
	    const float& _distance,
	    const float& _angle,
	    const ngl::Vector& _col,
	    const ngl::Vector& _up );
	    
	void addShadowSpot(
	    const ngl::Vector& _position,
	    const ngl::Vector& _target,
	    const float& _distance,
	    const float& _angle,
	    const ngl::Vector& _col,
	    const ngl::Vector& _up );
	    
	void addNamedPoint(
	    const std::string& _name,
	    const ngl::Vector& _centre,
	    const float& _radius,
	    const ngl::Vector& _intensity );
	    
	void addNamedSpot(
	    const std::string& _name,
	    const ngl::Vector& _position,
	    const ngl::Vector& _target,
	    const float& _distance,
	    const float& _angle,
	    const ngl::Vector& _col,
	    const ngl::Vector& _up );
	    
	void addNamedShadowSpot(
	    const std::string& _name,
	    const ngl::Vector& _position,
	    const ngl::Vector& _target,
	    const float& _distance,
	    const float& _angle,
	    const ngl::Vector& _col,
	    const ngl::Vector& _up );
	    
	void addNamedPoint( const std::string& _name, PointLight& _point );
	void addNamedSpot( const std::string& _name, SpotLight& _spot );
	void addNamedShadowSpot( const std::string& _name, ShadowSpot& _spot );
	
	inline SpotLight& getNamedSpot( const std::string& _name ) {
		return m_namedSpotLights[_name];
	}
	inline PointLight& getNamedPoint( const std::string& _name ) {
		return m_namedPointLights[_name];
	}
	inline ShadowSpot& getNamedShadowSpot( const std::string& _name ) {
		return m_namedShadowSpots[_name];
	}
	
	inline SpotLight& getSpot( int _index ) {
		return m_spotLights[_index];
	}
	inline PointLight& getPoint( int _index ) {
		return m_pointLights[_index];
	}
	inline ShadowSpot& getShadowSpot( int _index ) {
		return m_shadowSpots[_index];
	}
	
	inline void setGlobalAmbient( const ngl::Vector& _colour ) {
		m_globalAmbient = _colour;
	}
	inline void setGlobalDirectional( const ngl::Vector& _colour, const ngl::Vector& _direction ) {
		m_globalDirectional = _colour;
		m_globalDirectionalVec = _direction;
	}
	
	inline ngl::Vector getGlobalAmbient() {
		return m_globalAmbient;
	}
	inline ngl::Vector getDirectionalColour() {
		return m_globalDirectional;
	}
	inline ngl::Vector getDirectionalVec() {
		return m_globalDirectionalVec;
	}
	
	void accumulateLights( TextureEngine& _textureManager, const ngl::Vector& _camPos );
	void createLightGeometry();
private:

	unsigned int m_pointLightVAO;
	
	void blendPointLights( const ngl::Vector& _camPos );
	void blendShadowSpots( const ngl::Vector& _camPos );
	void blendSpotLights( const ngl::Vector& _camPos );
	
	std::vector<PointLight> m_pointLights;
	std::vector<SpotLight> m_spotLights;
	std::vector<ShadowSpot> m_shadowSpots;
	
	std::map<std::string, PointLight> m_namedPointLights;
	std::map<std::string, SpotLight> m_namedSpotLights;
	std::map<std::string, ShadowSpot> m_namedShadowSpots;
	
	ngl::Vector m_globalAmbient;
	ngl::Vector m_globalDirectional;
	ngl::Vector m_globalDirectionalVec;
};

#endif // LIGHTINGMANAGER_H
