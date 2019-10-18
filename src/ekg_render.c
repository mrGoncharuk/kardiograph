#include "kardiograph.h"

void	ft_render(t_app *app)
{
	ImVec4 clearColor;
	clearColor.x = 1.f;
	clearColor.y = 2.f;
	clearColor.z = 3.f;
	clearColor.w = 1.00f;
	igRender();
	SDL_GL_MakeCurrent(app->window, app->gl_context);
	glViewport(0, 0, (int)app->io->DisplaySize.x, (int)app->io->DisplaySize.y);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
	SDL_GL_SwapWindow(app->window);

}