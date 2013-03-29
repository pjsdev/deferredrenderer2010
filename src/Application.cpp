#include "Application.h"
#include "ui_Application.h"

Application::Application( QWidget* parent ) :
	QMainWindow( parent ),
	m_ui( new Ui::Application )
{
	m_ui->setupUi( this );
	m_gl = new GLWindow( this );
	m_ui->s_ApplicationGridLayout->addWidget( m_gl, 0, 0, 2, 1 );
	connect( m_ui->m_wireFrame, SIGNAL( toggled( bool ) ), m_gl, SLOT( toggleWireframe( bool ) ) );
	connect( m_ui->m_showShadowMap, SIGNAL( toggled( bool ) ), m_gl, SLOT( toggleShadowMap( bool ) ) );
	connect( m_ui->m_debug, SIGNAL( toggled( bool ) ), m_gl, SLOT( toggleDebug( bool ) ) );
	
	connect( m_ui->m_turntable, SIGNAL( toggled( bool ) ), m_gl, SLOT( toggleTurntable( bool ) ) );
	
	connect( m_ui->m_mainDisplayCB, SIGNAL( currentIndexChanged( int ) ), m_gl, SLOT( setMainDisplay( int ) ) );
	connect( m_ui->m_kaSpin, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setKa( double ) ) );
	connect( m_ui->m_kdSpin, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setKd( double ) ) );
	connect( m_ui->m_dVecX, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setDVecX( double ) ) );
	connect( m_ui->m_dVecY, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setDVecY( double ) ) );
	connect( m_ui->m_dVecZ, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setDVecZ( double ) ) );
	
	connect( m_ui->m_ambientR, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setAmbientColourR( double ) ) );
	connect( m_ui->m_ambientG, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setAmbientColourG( double ) ) );
	connect( m_ui->m_ambientB, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setAmbientColourB( double ) ) );
	
	connect( m_ui->m_dirR, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setDColourR( double ) ) );
	connect( m_ui->m_dirG, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setDColourG( double ) ) );
	connect( m_ui->m_dirB, SIGNAL( valueChanged( double ) ), m_gl, SLOT( setDColourB( double ) ) );
	
	connect( m_ui->m_Nbarrier, SIGNAL( valueChanged( double ) ), m_gl, SLOT( changeNormBarrier( double ) ) );
	connect( m_ui->m_Zbarrier, SIGNAL( valueChanged( double ) ), m_gl, SLOT( changeDepthBarrier( double ) ) );
	connect( m_ui->m_Nweight, SIGNAL( valueChanged( double ) ), m_gl, SLOT( changeNormWeight( double ) ) );
	connect( m_ui->m_Zweight, SIGNAL( valueChanged( double ) ), m_gl, SLOT( changeDepthWeight( double ) ) );
	connect( m_ui->m_kernel, SIGNAL( valueChanged( double ) ), m_gl, SLOT( changeKernel( double ) ) );
}

void Application::keyPressEvent(
    QKeyEvent* _event
)
{
	switch ( _event->key() ) {
		case Qt::Key_Escape :
			QApplication::exit( EXIT_SUCCESS );
			break;
			
		default :
			break;
	}
	
	m_gl->processKeyPress( _event );
}

Application::~Application()
{
	delete m_ui;
}
