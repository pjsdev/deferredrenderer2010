#include "GLWindow.h"
#include <iostream>
#include "ngl/Vector.h"
#include "ngl/Light.h"
#include "ngl/NGLInit.h"
#include "ngl/VBOPrimitives.h"
#include "ngl/ShaderManager.h"
#include "FrameBufferObject.h"
#include "ScreenQuad.h"
#include "NGLMath.h"
#include "Texture.h"
#include "ngl/Material.h"
#include "ngl/Util.h"
#include <QImage>
#include "LightingEngine.h"
#include "sstream"

#define WIDTH 1024
#define HEIGHT 768

//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(
    QWidget* _parent
) :
	QGLWidget( _parent ),
	m_viewportQuad( 0.0f, 0.0f, 0.5f, 1.0f ),
	m_fbo( WIDTH, HEIGHT ),
	m_shadowBuffer( 512, 512 ),
	m_cam( ngl::Vector( 0, 6, 10 ), ngl::Vector( 0, 0, 4 ), ngl::Vector( 0, 1, 0 ), ngl::PERSPECTIVE ),
	m_debugQuad1( -0.75, -0.8, 0.0, 0.2 ),
	m_debugQuad2( -0.25, -0.8, 0.0, 0.2 ),
	m_debugQuad3( 0.25, -0.8, 0.0, 0.2 ),
	m_debugQuad4( 0.75, -0.8, 0.0, 0.2 ),
	m_debugQuad5( -0.75, 0.75, 0.0, 0.2 )
{

	// set this widget to have the initial keyboard focus
	setFocus();
	// re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
	this->resize( WIDTH, HEIGHT );
	
	m_rotate = false;
	// mouse rotation values set to 0
	m_spinXFace = 0;
	m_spinYFace = 0;
	
	m_wireframe = false;
	m_debug = true;
	m_turntable = false;
	m_showShadowMap = false;
	m_currentDisplay = 0;
	m_Ka = 0.5;
	m_Kd = 0.7;
	
	m_directionalVec = ngl::Vector( 1, 0, 0 );
	m_ambientColour = ngl::Vector( 1, 1, 1 );
	m_directionalColour = ngl::Vector( 1, 1, 1 );
	
	//win32
	m_AAbarrierDepth = 0.5;
	m_AAbarrierNorm = 0.6;
	m_AAweightNorm = 0.15;
	m_AAweightDepth = 0.15;
	m_AAkernel = 0.4;
	
	m_time = 0.0;
	m_timeStep = 1.0;
	
}

void GLWindow::initializeGL()
{
	//init ngl
	ngl::NGLInit* init = ngl::NGLInit::instance();
#ifdef WIN32
	glewInit();
#endif
	init->initGlew();
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	// enable depth testing for drawing
	glEnable( GL_DEPTH_TEST );
	
	glEnable( GL_MULTISAMPLE );
	
	for( int i = 0; i < 5; i++ ) {
		glActiveTexture( GL_TEXTURE0 + i );
		glEnable( GL_TEXTURE_2D );
	}
	
	m_cam.setShape( 45, float( WIDTH / HEIGHT ), 0.1, 300, ngl::PERSPECTIVE );
	m_frustum.generate( m_cam.getProjection() * m_cam.getModelView() );
	
	//init shaders
	setupShaders();
	
	//init scene
	//m_scene.init();
	
	m_lightingManager.createLightGeometry();
	
	//geometry for unit screen quad
	createScreenQuad();
	
	setupTextures();
	
	postProcessSetup();
	
	//init fbos
	m_fbo.create( m_textureManager.getTextureId( "constant" ) );
	m_shadowBuffer.create( m_textureManager.getTextureId( "shadow" ) );
	
	m_fbo.bindTextureSlot( 1, m_textureManager.getTextureId( "normal" ) );
	m_fbo.bindTextureSlot( 2, m_textureManager.getTextureId( "albedo" ) );
	m_fbo.bindTextureSlot( 3, m_textureManager.getTextureId( "position" ) );
	
	m_lightingManager.addPointLight( ngl::Vector( 2.0, 0.0, 4.0 ), 4.0, ngl::Vector( 0.7, 0.7, 0.7 ) );
	m_lightingManager.addPointLight( ngl::Vector( -2.0, 0.0, -4.0 ), 4.0, ngl::Vector( 1.0, 0.7, 0.7 ) );
	m_lightingManager.addPointLight( ngl::Vector( 2.0, 0.0, -4.0 ), 4.0, ngl::Vector( 0.7, 0.7, 1.0 ) );
	m_lightingManager.addPointLight( ngl::Vector( -2.0, 0.0, 4.0 ), 4.0, ngl::Vector( 0.7, 0.7, 0.0 ) );
	m_lightingManager.addPointLight( ngl::Vector( 3.0, 0.0, 0.0 ), 4.0, ngl::Vector( 1.0, 0.0, 1.0 ) );
	m_lightingManager.addPointLight( ngl::Vector( -3.0, 0.0, 0.0 ), 4.0, ngl::Vector( 0.0, 1.0, 1.0 ) );
	
	m_lightingManager.addShadowSpot( ngl::Vector( 0, 6, 10 ), ngl::Vector( 0, 0, 4 ), 30.0, 45.0, ngl::Vector( 0.8, 0.8, 0.4 ), ngl::Vector( 0, 1, 0 ) );
	
	obj = new ngl::Obj( "models/Troll.obj" );
	obj->createVBO( GL_STATIC_DRAW );
	
	//loadMesh("models/Troll.obj");
	
	//m_go.loadMesh("models/Troll.obj");
	
	//set current main diaply texturer
	m_currentDisplay = m_textureManager.getTextureId( "constant" );
	
	//start update timer for lights
	//m_updatetimer = startTimer(10);
}


void GLWindow::shadowPass()
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )["ShadowBuffer"]->use();
	
	shader->setShaderParamFromMatrix( "ShadowBuffer", "u_LightView", m_lightingManager.getShadowSpot( 0 ).m_shadowView );
	shader->setShaderParamFromMatrix( "ShadowBuffer", "u_LightProj", m_lightingManager.getShadowSpot( 0 ).getProj() );
	
	glClearColor( 0.f, 0.f, 0.f, 1.0f );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	
	//bind shadow buffer
	m_shadowBuffer.bind();
	{
		m_shadowBuffer.activateTarget( 0 );
		
	}
	m_shadowBuffer.unbind();
	
	glDisable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
}

void GLWindow::loadMesh( const std::string& _meshName )
{
	std::cout << "loading mesh\n";
	// load the obj
	ngl::Obj mesh( "models/Troll.obj" );
	std::cout << "checking triangular\n";
	
	// this is only going to work for tri meshes so check
	if( ! mesh.isTriangular() ) {
		std::cout << "only works for tri meshes\n";
		exit( EXIT_FAILURE );
	}
	
	std::cout << "getting lists\n";
	
	// get the obj data so we can process it locally
	std::vector <ngl::Vector> verts = mesh.getVertexList();
	std::vector <ngl::Face> faces = mesh.getFaceList();
	std::vector <ngl::Vector> tex = mesh.getTextureCordList();
	std::vector <ngl::Vector> normals = mesh.getNormalList();
	
	std::cout << "got mesh data\n";
	// now we are going to process and pack the mesh into an ngl::VertexArrayObject
	std::vector <vertData> vboMesh;
	vertData d;
	unsigned int nFaces = faces.size();
	unsigned int nNorm = normals.size();
	unsigned int nTex = tex.size();
	// loop for each of the faces
	
	for( unsigned int i = 0; i < nFaces; ++i ) {
		// now for each triangle in the face (remember we ensured tri above)
		for( int j = 0; j < 3; ++j ) {
			// pack in the vertex data first
			d.x = verts[faces[i].m_vert[j]].m_x;
			d.y = verts[faces[i].m_vert[j]].m_y;
			d.z = verts[faces[i].m_vert[j]].m_z;
			
			// now if we have norms of tex (possibly could not) pack them as well
			if( nNorm > 0 && nTex > 0 ) {
				d.nx = normals[faces[i].m_norm[j]].m_x;
				d.ny = normals[faces[i].m_norm[j]].m_y;
				d.nz = normals[faces[i].m_norm[j]].m_z;
				
				d.u = tex[faces[i].m_tex[j]].m_x;
				d.v = tex[faces[i].m_tex[j]].m_y;
				
			}
			// now if neither are present (only verts like Zbrush models)
			else if( nNorm == 0 && nTex == 0 ) {
				d.nx = 0;
				d.ny = 0;
				d.nz = 0;
				d.u = 0;
				d.v = 0;
			}
			// here we've got norms but not tex
			else if( nNorm > 0 && nTex == 0 ) {
				d.nx = normals[faces[i].m_norm[j]].m_x;
				d.ny = normals[faces[i].m_norm[j]].m_y;
				d.nz = normals[faces[i].m_norm[j]].m_z;
				d.u = 0;
				d.v = 0;
				
			}
			// here we've got tex but not norm least common
			else if( nNorm == 0 && nTex > 0 ) {
				d.nx = 0;
				d.ny = 0;
				d.nz = 0;
				d.u = tex[faces[i].m_tex[j]].m_x;
				d.v = tex[faces[i].m_tex[j]].m_y;
			}
			
			// now we calculate the tangent / bi-normal (tangent) based on the article here
			// http://www.terathon.com/code/tangent.html
			
			ngl::Vector c1 = normals[faces[i].m_norm[j]].cross( ngl::Vector( 0.0, 0.0, 1.0 ) );
			ngl::Vector c2 = normals[faces[i].m_norm[j]].cross( ngl::Vector( 0.0, 1.0, 0.0 ) );
			ngl::Vector tangent;
			ngl::Vector binormal;
			
			if( c1.length() > c2.length() ) {
				tangent = c1;
			}
			else {
				tangent = c2;
			}
			
			// now we normalize the tangent so we don't need to do it in the shader
			tangent.normalize();
			// now we calculate the binormal using the model normal and tangent (cross)
			binormal = normals[faces[i].m_norm[j]].cross( tangent );
			// normalize again so we don't need to in the shader
			binormal.normalize();
			d.tx = tangent.m_x;
			d.ty = tangent.m_y;
			d.tz = tangent.m_z;
			d.bx = binormal.m_x;
			d.by = binormal.m_y;
			d.bz = binormal.m_z;
			// finally add it to our mesh VAO structure
			vboMesh.push_back( d );
		}
	}
	
	// first we grab an instance of our VOA class as a TRIANGLE_STRIP
	vao = ngl::VertexArrayObject::createVOA( GL_TRIANGLES );
	// next we bind it so it's active for setting data
	vao->bind();
	
	unsigned int meshSize = vboMesh.size();
	// now we have our data add it to the VAO, we need to tell the VAO the following
	// how much (in bytes) data we are copying
	// a pointer to the first element of data (in this case the address of the first element of the
	// std::vector
	
	vao->setData( meshSize * sizeof( vertData ), vboMesh[0].u );
	// in this case we have packed our data in interleaved format as follows
	// u,v,nx,ny,nz,x,y,z
	// If you look at the shader we have the following attributes being used
	// attribute vec3 inVert; attribute 0
	// attribute vec2 inUV; attribute 1
	// attribute vec3 inNormal; attribure 2
	// so we need to set the vertexAttributePointer so the correct size and type as follows
	// vertex is attribute 0 with x,y,z(3) parts of type GL_FLOAT, our complete packed data is
	// sizeof(vertData) and the offset into the data structure for the first x component is 5 (u,v,nx,ny,nz)..x
	
	vao->setVertexAttributePointer( 0, 3, GL_FLOAT, sizeof( vertData ), 5 );
	// uv same as above but starts at 0 and is attrib 1 and only u,v so 2
	
	vao->setVertexAttributePointer( 1, 2, GL_FLOAT, sizeof( vertData ), 0 );
	// normal same as vertex only starts at position 2 (u,v)-> nx
	vao->setVertexAttributePointer( 2, 3, GL_FLOAT, sizeof( vertData ), 2 );
	
	// tangent same as vertex only starts at position 8 (u,v)-> nx
	vao->setVertexAttributePointer( 3, 3, GL_FLOAT, sizeof( vertData ), 8 );
	
	// bi-tangent (or Binormal) same as vertex only starts at position 11 (u,v)-> nx
	vao->setVertexAttributePointer( 4, 3, GL_FLOAT, sizeof( vertData ), 11 );
	
	// now we have set the vertex attributes we tell the VAO class how many indices to draw when
	// glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
	// specify less (in steps of 3))
	vao->setNumIndices( meshSize );
	
	// finally we have finished for now so time to unbind the VAO
	//vao->unbind();
}



void GLWindow::geometryPass()
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	( *shader )["Gbuffer"]->use();
	
	ngl::Material m( ngl::GOLD );
	m.use();
	
	m_fbo.bind();
	{
		m_fbo.activateAllTargets();
		
		glClearColor( 0.f, 0.f, 0.f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		//vao->bind();
		//vao->draw();
		//vao->unbind();
		
		obj->draw();
		
	}
	m_fbo.unbind();
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void GLWindow::debugPass()
{
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glActiveTexture( GL_TEXTURE0 );
	
	m_textureManager.bindTexture( "constant" );
	m_debugQuad1.draw( "ScreenQuad", m_screenQuadVAO );
	m_textureManager.bindTexture( "normal" );
	m_debugQuad2.draw( "ScreenQuad", m_screenQuadVAO );
	m_textureManager.bindTexture( "albedo" );
	m_debugQuad3.draw( "ScreenQuad", m_screenQuadVAO );
	m_textureManager.bindTexture( "position" );
	m_debugQuad4.draw( "ScreenQuad", m_screenQuadVAO );
	
	if( m_showShadowMap ) {
		m_textureManager.bindTexture( "shadow" );
		m_debugQuad5.draw( "ScreenQuad", m_screenQuadVAO );
	}
	
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void GLWindow::lightAccumulation()
{
	m_fbo.bind();
	{
		m_fbo.activateTarget( 0 );
		
		ngl::Transformation trans;
		trans.setRotation( m_spinXFace, m_spinYFace, 0 );
		
		//get the camera pos in WS
		ngl::Matrix invView = Inverse( m_cam.getModelView() * trans.getMatrix() );
		ngl::Vector camPos = invView * ngl::Vector( 0, 0, 0, 1 );
		
		m_lightingManager.accumulateLights( m_textureManager, camPos );
	}
	m_fbo.unbind();
	
}

void GLWindow::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	ngl::Transformation trans;
	trans.setRotation( m_spinXFace, m_spinYFace, 0 );
	
	ngl::Matrix projView = m_cam.getProjection() * ( m_cam.getModelView() * trans.getMatrix() );
	ngl::Matrix view = m_cam.getModelView() * trans.getMatrix();
	
	m_frustum.generate( projView );
	
	if( m_wireframe ) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	else {
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	/******************
	 Shadow pass
	 *********************/
	//shadowPass();
	/**************************************/
	
	/******************
	  Gpass
	  ********************/
	( *shader )["Gbuffer"]->use();
	shader->setShaderParamFromMatrix( "Gbuffer", "u_ViewMatrix", view );
	shader->setShaderParam3f( "Gbuffer", "u_AmbientLight", m_ambientColour.m_x, m_ambientColour.m_y, m_ambientColour.m_z );
	shader->setShaderParam3f( "Gbuffer", "u_DirectionalLightVec", m_directionalVec.m_x, m_directionalVec.m_y, m_directionalVec.m_z );
	shader->setShaderParam3f( "Gbuffer", "u_DirectionalLight", m_directionalColour.m_x, m_directionalColour.m_y, m_directionalColour.m_z );
	shader->setShaderParam3f( "Gbuffer", "u_KaKdKs", m_Ka, m_Kd, 0.0 );
	geometryPass();
	/*************************************/
	
	/******************
	  light accumulation
	  *********************/
	( *shader )["PointLight"]->use();
	shader->setShaderParamFromMatrix( "PointLight", "u_ViewMatrix", view );
	
	( *shader )["DemoPL"]->use();
	shader->setShaderParamFromMatrix( "DemoPL", "u_ViewMatrix", view );
	
	( *shader )["SpotLight"]->use();
	shader->setShaderParamFromMatrix( "SpotLight", "u_ViewMatrix", view );
	
	( *shader )["ShadowSpot"]->use();
	shader->setShaderParamFromMatrix( "ShadowSpot", "u_ViewMatrix", view );
	shader->setShaderParamFromMatrix( "ShadowSpot", "u_LightView", m_lightingManager.getShadowSpot( 0 ).m_shadowView );
	shader->setShaderParamFromMatrix( "ShadowSpot", "u_LightProj", m_lightingManager.getShadowSpot( 0 ).getProj() );
	
	lightAccumulation();
	
	/***************************************/
	
	/******************
	  Post-process
	  ********************/
	postProcessUniforms();
	postProcess();
	/***************************************/
	
	
	/******************
	  debug gpass
	  ********************/
	if( m_debug ) {
		( *shader )["ScreenQuad"]->use();
		//debugPass();
	}
	
	/***************************************/
	
	/******************
	  final
	  *********************/
	
	( *shader )["ScreenQuad"]->use();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D,  m_currentDisplay );
	
	m_viewportQuad.draw( "ScreenQuad", m_screenQuadVAO );
	glBindTexture( GL_TEXTURE_2D, 0 );
	
	/*************************************/
	
}

void GLWindow::addStandardEffect( const std::string& _effectName,
                                  const std::string& _outputName,
                                  const std::string& _shaderName,
                                  const std::string& _sourceName )
{
	Effect effect( m_textureManager.getTextureId( _outputName ), _shaderName );
	effect.addSourceTexture( "u_Sampler", m_textureManager.getTextureId( _sourceName ) );
	m_postProcessor.addEffect( _effectName, effect );
}

void GLWindow::postProcessSetup()
{
	m_postProcessor = PostProcessor( WIDTH, HEIGHT, m_screenQuadVAO );
	
	Effect antiAlias( m_textureManager.getTextureId( "antialias" ), "AntiAlias" );
	antiAlias.addSourceTexture( "u_ColorMap", m_textureManager.getTextureId( "constant" ) );
	antiAlias.addSourceTexture( "u_NormalMap", m_textureManager.getTextureId( "normal" ) );
	antiAlias.addSourceTexture( "u_PositionMap", m_textureManager.getTextureId( "position" ) );
	m_postProcessor.addEffect( "AntiAlias", antiAlias );
	
	Effect depthOfField( m_textureManager.getTextureId( "DoF" ), "DoF" );
	depthOfField.addSourceTexture( "u_ColorMap", m_textureManager.getTextureId( "antialias" ) );
	depthOfField.addSourceTexture( "u_BlurMap", m_textureManager.getTextureId( "gaussian" ) );
	depthOfField.addSourceTexture( "u_PositionMap", m_textureManager.getTextureId( "position" ) );
	m_postProcessor.addEffect( "DoF", depthOfField );
	
	addStandardEffect( "HorizontalBlur", "horizontalBlur", "HorizontalBlur", "antialias" );
	addStandardEffect( "VerticalBlur", "gaussian", "VerticalBlur", "horizontalBlur" );
	addStandardEffect( "Grayscale", "grayscale", "Grayscale", "antialias" );
	addStandardEffect( "Sepia", "sepia", "Sepia", "antialias" );
	
	addStandardEffect( "BrightPass", "brightPass", "BrightPass", "antialias" );
	addStandardEffect( "BloomHBlur", "bloomHBlur", "HorizontalBlur", "brightPass" );
	addStandardEffect( "BloomGaussian", "bloomGaussian", "VerticalBlur", "bloomHBlur" );
	
	Effect bloom( m_textureManager.getTextureId( "bloom" ), "Bloom" );
	bloom.addSourceTexture( "u_BrightMap", m_textureManager.getTextureId( "bloomGaussian" ) );
	bloom.addSourceTexture( "u_ColorMap", m_textureManager.getTextureId( "antialias" ) );
	m_postProcessor.addEffect( "Bloom", bloom );
}

void GLWindow::postProcessUniforms()
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	( *shader )["AntiAlias"]->use();
	
	shader->setShaderParam4f( "AntiAlias",
	                          "u_barrierWeights",
	                          m_AAbarrierNorm,
	                          m_AAbarrierDepth,
	                          m_AAweightNorm,
	                          m_AAweightDepth
	                        );
	                        
	shader->setShaderParam1f( "AntiAlias", "u_kernel", m_AAkernel );
	
	( *shader )["DoF"]->use();
	
	shader->setShaderParam1f( "DoF", "u_FocalDepth", 0.5 );
	ngl::Transformation trans;
	trans.setRotation( m_spinXFace, m_spinYFace, 0 );
	
	ngl::Matrix projView = m_cam.getProjection() * ( m_cam.getModelView() * trans.getMatrix() );
	shader->setShaderParamFromMatrix( "DoF", "u_ProjView", projView );
}

void GLWindow::postProcess()
{
	m_postProcessor.start();
	{
		//always do AA
		m_postProcessor.doEffect( "AntiAlias" );
		
		//grab a blur texture
		m_postProcessor.doEffect( "HorizontalBlur" );
		m_postProcessor.doEffect( "VerticalBlur" );
		
		//do depth of field
		m_postProcessor.doEffect( "DoF" );
		
		m_postProcessor.doEffect( "BrightPass" );
		m_postProcessor.doEffect( "BloomHBlur" );
		m_postProcessor.doEffect( "BloomGaussian" );
		m_postProcessor.doEffect( "Bloom" );
		
		//m_postProcessor.doEffect("Grayscale");
		//m_postProcessor.doEffect("Sepia");
	}
	m_postProcessor.finish();
}

void GLWindow::setMainDisplay( int _mode )
{
	switch( _mode ) {
		case 0:
			m_currentDisplay =  m_textureManager.getTextureId( "constant" );
			break;
			
		case 1:
			m_currentDisplay =  m_textureManager.getTextureId( "normal" );
			break;
			
		case 2:
			m_currentDisplay =  m_textureManager.getTextureId( "albedo" );
			break;
			
		case 3:
			m_currentDisplay =  m_textureManager.getTextureId( "position" );
			break;
			
		case 4:
			m_currentDisplay =  m_textureManager.getTextureId( "antialias" );
			break;
			
		case 5:
			m_currentDisplay =  m_textureManager.getTextureId( "gaussian" );
			break;
			
		case 6:
			m_currentDisplay =  m_postProcessor.getFinalComposite();
			break;
			
		default:
			break;
	}
	
	updateGL();
}

void GLWindow::genShader( const std::string& _name, const std::string& _VSname, const std::string& _FSname, const std::vector<std::string>& _attributes )
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	shader->createShaderProgram( _name );
	//load shader sources
	shader->attachShader( "vertex", ngl::VERTEX );
	shader->attachShader( "fragment", ngl::FRAGMENT );
	shader->loadShaderSource( "vertex", _VSname );
	shader->loadShaderSource( "fragment", _FSname );
	//compile
	shader->compileShader( "vertex" );
	shader->compileShader( "fragment" );
	//link
	shader->attachShaderToProgram( _name, "vertex" );
	shader->attachShaderToProgram( _name, "fragment" );
	
	for( unsigned int i = 0; i < _attributes.size(); i++ ) {
		shader->bindAttribute( _name, i, _attributes[i] );
	}
	
	shader->linkProgramObject( _name );
	//use
	( *shader )[_name]->use();
	
}

void GLWindow::setupTextures()
{
	m_textureManager.addEmptyf( "constant", WIDTH, HEIGHT );
	m_textureManager.addEmptyf( "normal", WIDTH, HEIGHT );
	m_textureManager.addEmptyf( "albedo", WIDTH, HEIGHT );
	m_textureManager.addEmptyf( "position", WIDTH, HEIGHT );
	
	m_textureManager.addEmpty( "antialias", WIDTH, HEIGHT );
	
	m_textureManager.addEmpty( "grayscale", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "sepia", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "horizontalBlur", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "DoF", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "gaussian", WIDTH, HEIGHT );
	
	m_textureManager.addEmpty( "bloomHBlur", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "bloom", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "bloomGaussian", WIDTH, HEIGHT );
	m_textureManager.addEmpty( "brightPass", WIDTH, HEIGHT );
	
	Texture tmp( "shadow" );
	tmp.generateEmpty( 512, 512 );
	m_textureManager.addTexture( tmp );
}

void GLWindow::setupShaders()
{
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	
	std::vector<std::string> attrs;
	attrs.push_back( "a_VertexPosition" );
	attrs.push_back( "a_TextureCoord" );
	
	genShader( "ScreenQuad", "shaders/ScreenQuad.vs", "shaders/ScreenQuad.fs", attrs );
	
	genShader( "Grayscale", "shaders/ScreenQuad.vs", "shaders/postProcess/GrayScale.fs", attrs );
	
	genShader( "Sepia", "shaders/ScreenQuad.vs", "shaders/postProcess/Sepia.fs", attrs );
	
	genShader( "BrightPass", "shaders/ScreenQuad.vs", "shaders/postProcess/BrightPass.fs", attrs );
	
	genShader( "Bloom", "shaders/ScreenQuad.vs", "shaders/postProcess/Bloom.fs", attrs );
	
	genShader( "HorizontalBlur", "shaders/ScreenQuad.vs", "shaders/postProcess/HorizontalBlur.fs", attrs );
	shader->setShaderParam1f( "HorizontalBlur", "u_pixel", 1.0 / ( float )WIDTH );
	
	genShader( "VerticalBlur", "shaders/ScreenQuad.vs", "shaders/postProcess/VerticalBlur.fs", attrs );
	shader->setShaderParam1f( "VerticalBlur", "u_pixel", 1.0 / ( float )HEIGHT );
	
	genShader( "AntiAlias", "shaders/ScreenQuad.vs", "shaders/postProcess/AntiAlias.fs", attrs );
	shader->setShaderParam2f( "AntiAlias", "u_pixel", 1.0 / 768.0, 1.0 / ( float )WIDTH );
	
	genShader( "DoF", "shaders/ScreenQuad.vs", "shaders/postProcess/DoF.fs", attrs );
	
	attrs.clear();
	
	attrs.push_back( "inVert" );
	attrs.push_back( "inUV" );
	attrs.push_back( "inNormal" );
	attrs.push_back( "inTangent" );
	attrs.push_back( "inBinormal" );
	
	genShader( "Gbuffer", "shaders/Gbuffer.vs", "shaders/Gbuffer.fs", attrs );
	shader->setShaderParamFromMatrix( "Gbuffer", "u_ProjectionMatrix", m_cam.getProjection() );
	
	attrs.clear();
	
	genShader( "ShadowBuffer", "shaders/SpotShadowPass.vs", "shaders/SpotShadowPass.fs", attrs );
	
	attrs.push_back( "a_VertexPosition" );
	genShader( "PointLight", "shaders/lights/PointLight.vs", "shaders/lights/PointLight.fs", attrs );
	shader->setShaderParamFromMatrix( "PointLight", "u_ProjectionMatrix", m_cam.getProjection() );
	shader->setShaderParam1i( "PointLight", "u_NormalMap", 1 );
	shader->setShaderParam1i( "PointLight", "u_AlbedoMap", 2 );
	shader->setShaderParam1i( "PointLight", "u_PositionMap", 3 );
	
	attrs.clear();
	genShader( "SpotLight", "shaders/lights/SpotLight.vs", "shaders/lights/SpotLight.fs", attrs );
	shader->setShaderParamFromMatrix( "SpotLight", "u_ProjectionMatrix", m_cam.getProjection() );
	shader->setShaderParam1i( "SpotLight", "u_NormalMap", 1 );
	shader->setShaderParam1i( "SpotLight", "u_AlbedoMap", 2 );
	shader->setShaderParam1i( "SpotLight", "u_PositionMap", 3 );
	
	genShader( "ShadowSpot", "shaders/lights/ShadowSpot.vs", "shaders/lights/ShadowSpot.fs", attrs );
	shader->setShaderParamFromMatrix( "ShadowSpot", "u_ProjectionMatrix", m_cam.getProjection() );
	shader->setShaderParam1i( "ShadowSpot", "u_NormalMap", 1 );
	shader->setShaderParam1i( "ShadowSpot", "u_AlbedoMap", 2 );
	shader->setShaderParam1i( "ShadowSpot", "u_PositionMap", 3 );
	shader->setShaderParam1i( "ShadowSpot", "u_ShadowMap", 4 );
	
	attrs.push_back( "a_VertPos" );
	genShader( "DemoPL", "shaders/VisiblePLDemo.vs", "shaders/VisiblePLDemo.fs", attrs );
	shader->setShaderParamFromMatrix( "DemoPL", "u_ProjectionMatrix", m_cam.getProjection() );
	attrs.clear();
	
}

void GLWindow::createScreenQuad()
{
	// vertex coords array
	GLfloat vertices[] = {
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		1.0, 1.0, 0.0,
		-1.0, 1.0, 0.0  // v0-v1-v2-v3
	};
	
	GLfloat texture[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1 // v0-v1-v2-v3
	};
	
	// first we create a vertex array Object
	glGenVertexArrays( 1, &m_screenQuadVAO );
	
	// now bind this to be the currently active one
	glBindVertexArray( m_screenQuadVAO );
	// now we create two VBO's one for each of the objects these are only used here
	// as they will be associated with the vertex array object
	GLuint vboID[2];
	glGenBuffers( 2, &vboID[0] );
	// now we will bind an array buffer to the first one and load the data for the verts
	glBindBuffer( GL_ARRAY_BUFFER, vboID[0] );
	glBufferData( GL_ARRAY_BUFFER, 4 * 3 * sizeof( GLfloat ), vertices, GL_STATIC_DRAW );
	// now we bind the vertex attribute pointer for this object in this case the
	// vertex data
	ngl::ShaderManager* shader = ngl::ShaderManager::instance();
	( *shader )["ScreenQuad"]->use();
	( *shader )["ScreenQuad"]->vertexAttribPointer( "a_VertexPosition", 3, GL_FLOAT, 0, 0 );
	( *shader )["ScreenQuad"]->enableAttribArray( "a_VertexPosition" );
	
	// now we repeat for the UV data using the second VBO
	glBindBuffer( GL_ARRAY_BUFFER, vboID[1] );
	glBufferData( GL_ARRAY_BUFFER, 4 * 2 * sizeof( GLfloat ), texture, GL_STATIC_DRAW );
	// now bind
	( *shader )["ScreenQuad"]->vertexAttribPointer( "a_TextureCoord", 2, GL_FLOAT, 0, 0 );
	( *shader )["ScreenQuad"]->enableAttribArray( "a_TextureCoord" );
	// finally switch back to the default so we don't overwrite
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
}

void GLWindow::mouseMoveEvent (
    QMouseEvent* _event
)
{
	if( m_rotate && _event->buttons() == Qt::LeftButton ) {
		m_spinYFace = ( m_spinYFace + ( _event->x() - m_origX ) ) % 360 ;
		m_spinXFace = ( m_spinXFace + ( _event->y() - m_origY ) ) % 360 ;
		m_origX = _event->x();
		m_origY = _event->y();
	}
	
	// re-draw GL
	updateGL();
}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
    QMouseEvent* _event
)
{
	if( _event->button() == Qt::LeftButton ) {
		m_origX = _event->x();
		m_origY = _event->y();
		m_rotate = true;
	}
	
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
    QMouseEvent* _event
)
{
	if ( _event->button() == Qt::LeftButton ) {
		m_rotate = false;
	}
}

void GLWindow::processKeyPress( QKeyEvent* _event )
{
	switch ( _event->key() ) {
		default :
			break;
	}
}

void GLWindow::timerEvent(
    QTimerEvent* _event
)
{
	if( _event->timerId() == m_updatetimer ) {
	
		m_time += m_timeStep;
		//m_scene.moveLights(m_time);
		updateGL();
	}
}
