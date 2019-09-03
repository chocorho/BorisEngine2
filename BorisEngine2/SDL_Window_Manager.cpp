#include "SDL_Window_Manager.h"

BorisConsoleManager* SDL_Window_Manager::BorisConsoleManager = BorisConsoleManager::Instance();

SDL_Window_Manager* SDL_Window_Manager::instance = NULL;

SDL_Window_Manager::SDL_Window_Manager()
{

}

SDL_Window_Manager* SDL_Window_Manager::getInstance()
{
	if (!instance)
	{
		return instance = new SDL_Window_Manager();
	}
	return instance;
}

void SDL_Window_Manager::CheckSDLError(int line = -1)
{
	String error = SDL_GetError();
	if (error != "")
	{
		BorisConsoleManager->Print("SLD Error : " + error);
		if (line != -1)
		{
			BorisConsoleManager->Print("Line : " + std::to_string(line));
		}
		SDL_ClearError();
	}
}

bool SDL_Window_Manager::initWND(String strWNDTitle, int iWidth, int iHeight)
{
	return initWND(strWNDTitle, iWidth, iHeight, favicon);
}

bool SDL_Window_Manager::initWND(String windowTitle, int width, int height, Icon icon)
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		BorisConsoleManager->Print("Failed to load SDL");
		return false;
	}
	mainWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);
	// Check that everything worked out okay
	if (!mainWindow)
	{
		BorisConsoleManager->Print("Unable to create window.");
		CheckSDLError(__LINE__);
		return false;
	}
	// Get the renderer
	sdlRenderer = SDL_CreateRenderer(mainWindow, -1, 0);
	if (sdlRenderer)
	{
		BorisConsoleManager->Print("Renderer creation succeeded");
		SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 100, 255);
		SetSDLIcon(icon);
		return true;
	}
	BorisConsoleManager->Print("Renderer creation failed");
	return false;
}

SDL_Renderer* SDL_Window_Manager::getSDLRenderer()
{
	return sdlRenderer;
}

SDL_Window* SDL_Window_Manager::getSDLWindow()
{
	return mainWindow;
}

//https://caedesnotes.wordpress.com/2015/04/13/how-to-integrate-your-sdl2-window-icon-or-any-image-into-your-executable/
void SDL_Window_Manager::SetSDLIcon()
{
	SetSDLIcon(favicon);
}

void SDL_Window_Manager::SetSDLIcon(Icon iconStruct)
{
	// these masks are needed to tell SDL_CreateRGBSurface(From)
	// to assume the data it gets is byte-wise RGB(A) data
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		int shift = (my_icon.bytes_per_pixel == 3) ? 8 : 0;
		rmask = 0xff000000 >> shift;
		gmask = 0x00ff0000 >> shift;
		bmask = 0x0000ff00 >> shift;
		amask = 0x000000ff >> shift;
	#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = (iconStruct.bytes_per_pixel == 3) ? 0 : 0xff000000;
	#endif
	SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)iconStruct.pixel_data, iconStruct.width,
		iconStruct.height, iconStruct.bytes_per_pixel * 8, iconStruct.bytes_per_pixel*iconStruct.width,
		rmask, gmask, bmask, amask);

	SDL_SetWindowIcon(getSDLWindow(), icon);
	SDL_FreeSurface(icon);
}