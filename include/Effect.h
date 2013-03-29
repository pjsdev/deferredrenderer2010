#ifndef EFFECT_H
#define EFFECT_H

#include "string"
#include "vector"
#include "ngl/Types.h"

class Effect
{
public:
	Effect() {}
	Effect( GLuint _texId, const std::string& _shaderName );
	~Effect();
	
	void addSourceTexture( const std::string& _samplerName, GLuint _texId );
	void prepare();
	GLuint getOutputId() {
		return m_output;
	}
	std::string getShaderName() {
		return m_shaderName;
	}
	
private:
	std::vector<GLuint> m_sources;
	GLuint m_output;
	std::string m_shaderName;
};

#endif // EFFECT_H
