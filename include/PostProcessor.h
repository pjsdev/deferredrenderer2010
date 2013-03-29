#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "FrameBufferObject.h"
#include "TextureEngine.h"
#include "ScreenQuad.h"
#include "Effect.h"
#include "map"

class PostProcessor
{
public:
	PostProcessor() {}
	PostProcessor( int _w, int _h, GLuint _vao );
	~PostProcessor() {}
	
	void init();
	void addEffect( const std::string& _effectName, Effect _e );
	void start();
	void finish() {
		m_fbo.unbind();
	}
	void doEffect( const std::string& _effectName );
	
	GLuint getFinalComposite() {
		return m_finalComposite;
	}
	
private:
	GLuint m_finalComposite;
	ScreenQuad m_screenQuad;
	std::map<std::string, Effect> m_effects;
	int m_width, m_height;
	FrameBufferObject m_fbo;
	GLuint m_vao;
	int m_currentAttachment;
};

#endif // POSTPROCESSOR_H
