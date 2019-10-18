#include "kardiograph.h"

int		ft_init_window(t_app *app)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("failed to init: %s", SDL_GetError());
		return (-1);
	}
	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	
	app->window = SDL_CreateWindow(
			"EKG generator", 0, 0, 1024, 768,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
			);
	
	if (app->window == NULL)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return (-1);
	}
	app->gl_context = SDL_GL_CreateContext(app->window);
	SDL_GL_SetSwapInterval(1);	// enable vsync
	// check opengl version sdl uses
	//SDL_Log("opengl version: %s", (char*)glGetString(GL_VERSION));
	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#endif
	if (err)
	{
		SDL_Log("Failed to initialize OpenGL loader!");
		return 1;
	}

	// setup imgui
	igCreateContext(NULL);
	app->io = igGetIO();
	ImGui_ImplSDL2_InitForOpenGL(app->window, app->gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	igStyleColorsDark(NULL);
	return (0);
}

void	ft_init_flags(t_flags *flags)
{
	flags->ig_window_shown = true;
	flags->running = true;
}

int		ft_init_all(t_app *app, t_flags *flags)
{
	if (ft_init_window(app) < 0)
		return (-1);
	ft_init_flags(flags);
	for (int i = 0; i < EKG_AMOUNT; i++)
		app->ekgs[i] = 0;
	return (0);
}