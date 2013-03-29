#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#include "Effect.h"
#include "PostProcessor.h"
#include "ngl/Camera.h"
#include "ngl/TransformStack.h"
#include "ngl/Obj.h"
#include "ScreenQuad.h"
#include "FrameBufferObject.h"
#include "Frustum.h"
#include "TextureEngine.h"
#include "LightingEngine.h"
#include "GameObject.h"
#include "ngl/VertexArrayObject.h"

// must be included after our stuff becuase GLEW needs to be first
#include <QtOpenGL>

/// @file GLWindow.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class GLWindow
/// @brief The GLWidget for updating the drawing of our scene
class GLWindow : public QGLWidget
{
	Q_OBJECT
public :

	/// @brief Constructor for GLWindow
	/// @param [in] _parent the parent window to create the GL context in
	GLWindow(
	    QWidget* _parent
	);
	
	/// @brief GL key events processing
	/// @param [in] _event the event infomation
	void processKeyPress( QKeyEvent* _event );
	
public slots:

	/// @brief a slot to toggle wireframe mode
	/// @param [in] _mode the mode to wireframe to
	inline void toggleWireframe( bool _mode ) {
		m_wireframe = _mode;
		updateGL();
	}
	
	/// @brief a slot to toggle showing shadowmap
	/// @param [in] _mode the mode to set shadow map to
	inline void toggleShadowMap( bool _mode ) {
		m_showShadowMap = _mode;
		updateGL();
	}
	
	/// @brief set the debug display
	/// @param [in] _mode the mode to set debug diaplsy to
	inline void toggleDebug( bool _mode ) {
		m_debug = _mode;
		updateGL();
	}
	
	
	/// @brief set the ambient coefficient for the scene
	/// @param [in] _value the new Ka value
	inline void setKa( double _value ) {
		m_Ka = ( float )_value;
		updateGL();
	}
	
	/// @brief set the diffuse coefficient for the scene
	/// @param [in] _value the new value for Kd
	inline void setKd( double _value ) {
		m_Kd = ( float )_value;
		updateGL();
	}
	
	/// @brief set the global directional light for the scene
	/// @param [in] _value the new directional light vec X dir
	inline void setDVecX( double _value ) {
		m_directionalVec.m_x = _value;
		updateGL();
	}
	
	/// @brief set the global directional light for the scene
	/// @param [in] _value the new directional light vec Y dir
	inline void setDVecY( double _value ) {
		m_directionalVec.m_y = _value;
		updateGL();
	}
	
	/// @brief set the global directional light for the scene
	/// @param [in] _value the new directional light vec Z dir
	inline void setDVecZ( double _value ) {
		m_directionalVec.m_z = _value;
		updateGL();
	}
	
	/// @brief set the global ambient light col for the scene
	/// @param [in] _value the new R ambient light colour
	inline void setAmbientColourR( double _value ) {
		m_ambientColour.m_x = _value;
		updateGL();
	}
	
	/// @brief set the global ambient light col for the scene
	/// @param [in] _value the new G ambient light colour
	inline void setAmbientColourG( double _value ) {
		m_ambientColour.m_y = _value;
		updateGL();
	}
	
	/// @brief set the global ambient light col for the scene
	/// @param [in] _value the new B ambient light colour
	inline void setAmbientColourB( double _value ) {
		m_ambientColour.m_z = _value;
		updateGL();
	}
	
	/// @brief set the global directional light col for the scene
	/// @param [in] _value the new R ambient light colour
	inline void setDColourR( double _value ) {
		m_directionalColour.m_x = _value;
		updateGL();
	}
	
	/// @brief set the global directional light col for the scene
	/// @param [in] _value the new G ambient light colour
	inline void setDColourG( double _value ) {
		m_directionalColour.m_y = _value;
		updateGL();
	}
	
	/// @brief set the global directional light col for the scene
	/// @param [in] _value the new B ambient light colour
	inline void setDColourB( double _value ) {
		m_directionalColour.m_z = _value;
		updateGL();
	}
	
	inline void toggleTurntable( bool _mode ) {
		m_turntable = _mode;
		updateGL();
	}
	
	inline void changeNormWeight( double _value ) {
		m_AAweightNorm = ( float )_value;
		updateGL();
	}
	
	inline void changeDepthWeight( double _value ) {
		m_AAweightDepth = ( float )_value;
		updateGL();
	}
	
	inline void changeNormBarrier( double _value ) {
		m_AAbarrierNorm = ( float )_value;
		updateGL();
	}
	
	inline void changeDepthBarrier( double _value ) {
		m_AAbarrierDepth = ( float )_value;
		updateGL();
	}
	
	inline void changeKernel( double _value ) {
		m_AAkernel = ( float )_value;
		updateGL();
	}
	
	/// @brief the mode for the main screen quad, binds our render targets to it
	/// @param [in] _displayMode the new mode of display
	void setMainDisplay( int _displayMode );
	
protected:

	float m_AAbarrierNorm, m_AAbarrierDepth, m_AAweightNorm, m_AAweightDepth, m_AAkernel;
	
	/// @brief a helper function for generating ngl shaders
	/// @param [in] _name the shaders desired name
	/// @param [in] _VSname the vertex shader name
	/// @param [in] _FSname the frag sjader name
	/// @param [in] _attributes a vector of strings for the shaders attributes
	void genShader( const std::string& _name, const std::string& _VSname, const std::string& _FSname, const std::vector<std::string>& _attributes );
	
	/// @brief the current main display texture
	int m_currentDisplay;
	
	/// @brief used to store the x rotation mouse value
	int m_spinXFace;
	
	/// @brief used to store the y rotation mouse value
	int m_spinYFace;
	
	/// @brief flag to indicate if the mouse button is pressed when dragging
	bool m_rotate;
	
	/// @brief the previous x mouse value
	int m_origX;
	
	/// @brief the previous y mouse value
	int m_origY;
	
	/// @brief called upon creating GLWindow, setup for Glew and other scene elements
	void initializeGL();
	
	/// @brief Re-size method, disabled in this application
	void resizeGL( const int _w, const int _h ) {}
	
	/// @brief this is the main gl drawing routine
	void paintGL();
	
	/// @brief timer event for updating the demo scene
	/// @param [in] _event qt event info
	void timerEvent( QTimerEvent* _event );
	
	/// @brief construct the geometry once for our screen quad
	void createScreenQuad();
	
	/// @brief the transformation stack
	ngl::TransformStack m_transformStack;
	
private:


	/// @brief this method is called every time a mouse is moved
	/// @param _event the Qt Event structure
	void mouseMoveEvent (
	    QMouseEvent* _event
	);
	/// @brief this method is called everytime the mouse button is pressed
	/// @param _event the Qt Event structure
	void mousePressEvent (
	    QMouseEvent* _event
	);
	/// @brief this method is called everytime the mouse button is released
	/// @param _event the Qt Event structure
	void mouseReleaseEvent (
	    QMouseEvent* _event
	);
	
	/// @brief setup our application shaders
	void setupShaders();
	
	void setupTextures();
	
	/// @brief draw m_scene
	//void drawScene();
	/// @brief accumulate light onto our pixels
	void lightAccumulation();
	/// @brief shadow pass for our shadowspot
	void shadowPass();
	/// @brief geometry for main gbuffer
	void geometryPass();
	/// @brief debug pass for drawing our debug quads
	void debugPass();
	
	void postProcess();
	void postProcessUniforms();
	void postProcessSetup();
	
	void addStandardEffect( const std::string& _effectName,
	                        const std::string& _outputName,
	                        const std::string& _shaderName,
	                        const std::string& _sourceName );
	                        
	TextureEngine m_textureManager;
	LightingEngine m_lightingManager;
	
	/// @brief update timer for demo scene
	int m_updatetimer;
	/// @brief current time variable
	float m_time;
	/// @brief the increment for out time variable
	float m_timeStep;
	/// @brief the ambient coefficient of the gpass
	float m_Ka;
	/// @brief the diffuse coefficient of the gpass
	float m_Kd;
	/// @brief wireframe mode on/off
	bool m_wireframe;
	/// @brief debug mode on/off
	bool m_debug;
	/// @brief the demo scene
	//Scene m_scene;
	/// @brief our main viewport quad
	ScreenQuad m_viewportQuad;
	/// @brief out camera frustum
	Frustum m_frustum;
	/// @brief maine fbo for gpass
	FrameBufferObject m_fbo;
	/// @brief shadow fbo for shadowpass
	FrameBufferObject m_shadowBuffer;
	
	/// @brief our ngl camera
	ngl::Camera m_cam;
	
	/// @brief out point light vao id
	unsigned int m_pointLightVAO;
	/// @brief spotlight name for ngl vboprimitives
	std::string m_spotLightName;
	/// @brief our screen quad vao id
	unsigned int m_screenQuadVAO;
	
	/// @brief debug quad bottom left
	ScreenQuad m_debugQuad1;
	/// @brief debug quad bottom middle-left
	ScreenQuad m_debugQuad2;
	/// @brief debug quad bottom middle-right
	ScreenQuad m_debugQuad3;
	/// @brief debug quad bottom right
	ScreenQuad m_debugQuad4;
	/// @brief debug quad top left
	ScreenQuad m_debugQuad5;
	/// @brief debug quad top right
	ScreenQuad m_debugQuad6;
	
	/// @brief the global directional vector
	ngl::Vector m_directionalVec;
	
	/// @brief the global directional vector
	ngl::Vector m_ambientColour;
	
	/// @brief the global directional vector
	ngl::Vector m_directionalColour;
	
	bool m_showShadowMap;
	
	bool m_turntable;
	
	PostProcessor m_postProcessor;
	
	GameObject m_go;
	
	ngl::VertexArrayObject* vao;
	
	ngl::Obj* obj;
	
	void loadMesh( const std::string& _meshName );
	
};

#endif
