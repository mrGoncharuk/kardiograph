#ifndef GUI_HPP
# define GUI_HPP

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#include "IMonitorDisplay.hpp"
#include "ECG.hpp"


#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

class GUI: public IMonitorDisplay
{
private:
	SDL_Window		*window;
	SDL_GLContext	glContext;
	ImVec4 			clearColor;
	bool			running;
	ECG				cardiocycle;
public:
	GUI();
	~GUI();
	GUI(GUI const &);
	GUI &operator =(GUI const &);

	SDL_Window		*getWindow();
	SDL_GLContext	getGLContext();
	ImVec4			getClearColor();
	void			mainloop();
	void			events();
	void			update();
	void			render();
};

#endif
