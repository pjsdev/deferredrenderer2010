#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include "GLWindow.h"

namespace Ui
{
	class Application;
}

/// @file Application.h
/// @author Peter Smith
/// @version 1.0
/// @date 20/05/11
/// @class Application
/// @brief the main re-sizable window which contains the GLWidget
class Application : public QMainWindow
{
	Q_OBJECT
	
public:
	/// @brief ctor for the application
	/// @param [in] parent
	explicit Application( QWidget* parent = 0 );
	
	/// @brief dtor
	~Application();
	
private:

	/// @brief input response for key presses
	/// @param QKeyEvent pointer for retrieving the event info
	void keyPressEvent( QKeyEvent* );
	
	/// @brief the UI
	Ui::Application* m_ui;
	
	/// @brief our opengl window
	GLWindow* m_gl;
};

#endif // APPLICATION_H
