#include "kardiograph.h"

void	ft_clean(t_app *app)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	igDestroyContext(NULL);

	SDL_GL_DeleteContext(app->gl_context);
	if (app->window != NULL)
	{
		SDL_DestroyWindow(app->window);
		app->window = NULL;
	}
	SDL_Quit();
}