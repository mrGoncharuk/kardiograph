#include "GUI.hpp"


GUI::GUI(/* args */): clearColor(0.45f, 0.55f, 0.60f, 1.00f), running(true), showGenSignal(false), showFilterWindow(false)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return ;
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

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
   	window = SDL_CreateWindow("KardioGraph", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return ;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glsl_version);
    cardiocycle.calcEtalon();
	cardiocycle.generateSignal();
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void    GUI::mainloop()
{
    while (running)
    {
        events();
        update();
		render();
    }
}

void    GUI::events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            running = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            running = false;
    }
}

void	GUI::drawECG()
{
	static int cycles = cardiocycle.getCycleAmount();
	static float altPower = cardiocycle.getT().getAlternation();
	static float noisePower = cardiocycle.getNoisePower();
	static float alpha = 1;
	static int filterType = FILTER_EXP_SMOTH;
	static int meanType = MEAN_TYPE_FUTURE;

	ImGui::Begin("ECG", &showGenSignal);

	ImGui::PlotConfig conf;
	// conf.values.xs = x_data; // this line is optional
	conf.values.ys = cardiocycle.getSignal();
	conf.values.count = cardiocycle.getSignalDuration();
	conf.grid_x.size = cardiocycle.getSignalDuration();
	
	conf.scale.min = cardiocycle.getMinAmpl();
	conf.scale.max = cardiocycle.getMaxAmpl();
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = true;
	conf.grid_y.show = true;
	conf.frame_size = ImVec2(1000, 300);
	conf.line_thickness = 1.f;

	ImGui::Plot("plot", conf);
	ImGui::PushItemWidth(200);
	if (ImGui::InputInt("Cycle amount", &cycles))
	{
		if (cycles < 2)
			cycles = 2;
		if (cycles > 30)
			cycles = 30;
		cardiocycle.setCycleAmount(cycles);
		cardiocycle.generateSignal();
	}
	
	if (ImGui::SliderFloat("Alternation Power of T", &altPower, 0, 1.0f, "%.2f", 1))
	{
		cardiocycle.getT().setAlternation(altPower);
		cardiocycle.generateSignal();
	}
	
	if (ImGui::SliderFloat("Noise Power", &noisePower, 0, 1, "%.3f", 1))
	{
		cardiocycle.setNoisePower(noisePower);
		cardiocycle.generateSignal();
	}
	if (ImGui::Button("Filter Signal"))
		showFilterWindow = true;
	ImGui::End();


	if (showFilterWindow)
	{
		static int			windowWidth = 10;
		static const int	maxWindowWidth = 100;
		static float		h = 0.3;

		ImGui::Begin("Filter", &showFilterWindow);
		if (filter.getFilteredData() != NULL)
			conf.values.ys = filter.getFilteredData();
		ImGui::Plot("plot", conf);
		ImGui::RadioButton("Exponentional Smoothing", &filterType, FILTER_EXP_SMOTH);
		ImGui::RadioButton("Sliding Mean", &filterType, FILTER_SLIDING_MEAN);
		ImGui::RadioButton("Adaptive Smoothing", &filterType, FILTER_ADAPT_SMOTH);
		switch (filterType)
		{
		case FILTER_EXP_SMOTH:
			ImGui::PushItemWidth(300);
			if (ImGui::SliderFloat("Alpha", &alpha, 0, 1, "%.3f", 1))
			{
				filter.setData(cardiocycle.getSignal());
				filter.setDataSize(cardiocycle.getSignalDuration());
				filter.filterExpSmooth(alpha);
				conf.values.ys = filter.getFilteredData();
			}
			break;
		case FILTER_SLIDING_MEAN:


			if (ImGui::SliderInt("Window Width", &windowWidth, 1, maxWindowWidth, "%d"))
			{
				filter.setData(cardiocycle.getSignal());
				filter.setDataSize(cardiocycle.getSignalDuration());
				filter.filterSlidingMean(windowWidth, meanType);
				conf.values.ys = filter.getFilteredData();
			}
			if (ImGui::RadioButton("Smoothing with Previous signal value", &meanType, MEAN_TYPE_PREV) ||
			ImGui::RadioButton("Smoothing with Future signal value", &meanType, MEAN_TYPE_FUTURE))
			{
				filter.setData(cardiocycle.getSignal());
				filter.setDataSize(cardiocycle.getSignalDuration());
				filter.filterSlidingMean(windowWidth, meanType);
				conf.values.ys = filter.getFilteredData();
			}
			break;
		case FILTER_ADAPT_SMOTH:

			if (ImGui::SliderInt("Window Width", &windowWidth, 1, maxWindowWidth, "Window width: %d"))
			{
				filter.setData(cardiocycle.getSignal());
				filter.setDataSize(cardiocycle.getSignalDuration());
				filter.filterAdaptSmoth(windowWidth, h);
				conf.values.ys = filter.getFilteredData();
			}
			if (ImGui::SliderFloat("", &h, 0, 1, "Noise Power: %.3f", 1))
			{
				filter.setData(cardiocycle.getSignal());
				filter.setDataSize(cardiocycle.getSignalDuration());
				filter.filterAdaptSmoth(windowWidth, h);
				conf.values.ys = filter.getFilteredData();
			}
			break;
		default:
			break;
		}
		ImGui::End();
	}

}

void	GUI::update()
{
    ImGuiStyle& style = ImGui::GetStyle();
    static ImGuiStyle ref_saved_style;
	static int	rbValue = T_WAVE;
	static float width[2];
    static int      heart_rate = cardiocycle.getFH();
	width[0] = getCurrentWave(rbValue).getB1();
	width[1] = getCurrentWave(rbValue).getB2();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	ImGui::Begin("KardioGraph", NULL);
    ImGui::ShowDemoWindow();
    if (ImGui::ShowStyleSelector("Styles Selector"))
        ref_saved_style = style;
    ImGui::ColorEdit3("Skin", (float*)&clearColor);
    if (ImGui::SliderFloat("Amplitude", &getCurrentWave(rbValue).getAmplitude(), cardiocycle.getMinAmpl(), cardiocycle.getMaxAmpl(), "%.3f", 1.0f))
		cardiocycle.calcEtalon();
    if (ImGui::SliderFloat("Time", &getCurrentWave(rbValue).getTExtreme(), 0, cardiocycle.getEtalonDuration(), "%.3f", 1.0f))
		cardiocycle.calcEtalon();
    
    if (ImGui::SliderFloat2("Width", width, 1, 20, "%.3f", 1))
	{
		getCurrentWave(rbValue).setB1(width[0]);
		getCurrentWave(rbValue).setB2(width[1]);
		cardiocycle.calcEtalon();
	}
    if (ImGui::InputInt("Heart rate", &heart_rate))
    {
        if (heart_rate < 5)
            heart_rate = 5;
        else if (heart_rate > 120)
            heart_rate = 120;
        cardiocycle.setFH(heart_rate);
    }
	ImGui::RadioButton("P", &rbValue, P_WAVE); ImGui::SameLine(50);
	ImGui::RadioButton("Q", &rbValue, Q_WAVE); ImGui::SameLine(100);
	ImGui::RadioButton("R", &rbValue, R_WAVE); ImGui::SameLine(150);
	ImGui::RadioButton("S", &rbValue, S_WAVE); ImGui::SameLine(200);
	ImGui::RadioButton("T", &rbValue, T_WAVE);
	if (ImGui::Button("Generate Signal", ImVec2(125, 25)))
	{
		showGenSignal = true;
		cardiocycle.generateSignal();
	}
	ImGui::End();

	ImGui::Begin("CardioCycle", NULL);
    ImGui::PlotLines("CardioCycle", cardiocycle.getEtalon(), cardiocycle.getEtalonDuration(), 4, NULL,
                    cardiocycle.getMinAmpl(), cardiocycle.getMaxAmpl(), ImVec2(500, 500), 4);
	ImGui::End();

	if (showGenSignal)
		drawECG();
}

void	GUI::render()
{
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}


Wave	&GUI::getCurrentWave(int v)
{
	if (v == P_WAVE)
		return (cardiocycle.getP());
	else if (v == Q_WAVE)
		return (cardiocycle.getQ());
	else if (v == R_WAVE)
		return (cardiocycle.getR());
	else if (v == S_WAVE)
		return (cardiocycle.getS());
	else
		return (cardiocycle.getT());
}

SDL_Window *GUI::getWindow() { return (this->window); }
SDL_GLContext	GUI::getGLContext() { return (this->glContext); }
ImVec4			GUI::getClearColor() { return (this->clearColor); }

