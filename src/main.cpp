#include <QApplication>
#include "Application.h"

int main( int argc, char** argv )
{
	QApplication a( argc, argv );
	
	// grab an instance of the OpenGL Format class
	QGLFormat glFormat = QGLFormat::defaultFormat();
	// enable the SampleBuffer to allow for multisample
	glFormat.setSampleBuffers( true );
	// set the number of Samples per pixel
	glFormat.setSamples( 4 );
	// now set this as the default for all OpenGL windows created
	QGLFormat::setDefaultFormat( glFormat );
	
	Application app;
	
	app.show();
	
	return a.exec();
}
