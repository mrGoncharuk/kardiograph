#include "kardiograph.h"



void    ft_mainloop(t_app *app, t_flags *flags)
{
    while (flags->running)
	{
		ft_event_handler(flags, app);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(app->window);
		igNewFrame();
		igBegin("Hello, world!", &(flags->ig_window_shown), 0);
		igText("This is some useful text");
		igSliderFloat("slider", app->ekgs, 0.0f, 1.0f, "%.3f", 1.0f);
		// put_sliders(app);
		igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
		igEnd();
		//ft_update(app, flags);
		SDL_GL_MakeCurrent(app->window, app->gl_context);
		glViewport(0, 0, (int)app->io->DisplaySize.x, (int)app->io->DisplaySize.y);
		// glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
		SDL_GL_SwapWindow(app->window);
		//ft_render(app);
	}
}