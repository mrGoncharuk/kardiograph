#ifndef KARDIOGRAPH_H
# define KARDIOGRAPH_H

# define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
# include <stdio.h>
# include <stdbool.h>
# include <SDL2/SDL.h>
# include "cimgui.h"
# include "imgui_impl_sdl.h"
# include "imgui_impl_opengl3.h"
// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
# if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#  include "GL/gl3w.h"    // Initialize with gl3wInit()
# elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#  include <GL/glew.h>    // Initialize with glewInit()
# elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#  include <glad/glad.h>  // Initialize with gladLoadGL()
# else
#  include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
# endif

# define EKG_AMOUNT 8
# define WIN_WIDTH 1024
# define WIN_HEIGHT 768

typedef struct		s_flags
{
	bool			running;
	bool			ig_window_shown;

}					t_flags;


typedef struct  	s_app
{
	SDL_Window  	*window;
	SDL_GLContext   gl_context;
    ImGuiIO         *io;
	float			ekgs[EKG_AMOUNT];
}				    t_app;



int		ft_init_all(t_app *app, t_flags *flags);
void    ft_mainloop(t_app *app, t_flags *flags);
void    ft_event_handler(t_flags *flags, const t_app *app);
void	ft_render(t_app *app);
void	ft_update(t_app *app, t_flags *flags);
void	ft_clean(t_app *app);


#endif