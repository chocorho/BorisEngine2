#include "SoundManager.h"

SoundManager* SoundManager::_instance = NULL;

BorisConsoleManager* SoundManager::BorisConsoleManager = BorisConsoleManager::Instance();

SoundManager::SoundManager()
{
	Initialise();
}

bool SoundManager::Initialise()
{
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		std::string str = "SDL_Init_AUDIO Failed: " + std::string(SDL_GetError());
		BorisConsoleManager->Print(str);
		//std::stringstream ss;
		//ss << "SDL_Init_AUDIO Failed: " << SDL_GetError();
		//BorisConsoleManager->Print(&ss);
		//cout << "SDL_Init_AUDIO Failed: " << SDL_GetError() << endl;
		return false;
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
	{
		std::string str = "Mix_OpenAudio Failed: " + std::string(SDL_GetError());
		BorisConsoleManager->Print(str);
		//std::stringstream ss;
		//ss << "Mix_OpenAudio Failed: " << SDL_GetError();
		//BorisConsoleManager->Print(&ss);
		//cout << "Mix_OpenAudio Failed: " << SDL_GetError() << endl;
		return false;
	}
	return true;
}

SoundManager::~SoundManager()
{
	DeleteSounds();
	Mix_CloseAudio();
	Mix_Quit();
}

SoundManager* SoundManager::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new SoundManager();
	}
	return _instance;
}

Sound* SoundManager::GetSound(std::string soundname)//(LPCSTR soundname)
{
	std::map<std::string, Sound*>::iterator sound = sounds.find(soundname);
	//map<LPCSTR, Sound*>::iterator sound = sounds.find(soundname);
	if (sound != sounds.end())
	{
		return sound->second;
	}
	return NULL;
}

void SoundManager::AddSound(std::string soundname, LPCSTR filename, SoundType soundtype)//(LPCSTR soundname, LPCSTR filename, SoundType soundtype)
{
	if (!GetSound(soundname))
	{
		Sound* newsound = new Sound(filename,soundtype);
		sounds.insert(make_pair(soundname, newsound));
	}
}

void SoundManager::DeleteSounds()
{
	for (std::map<std::string, Sound*>::iterator sound = sounds.begin(); sound != sounds.end(); ++sound)//(map<LPCSTR, Sound*>::iterator sound = sounds.begin(); sound != sounds.end(); ++sound)
	{
		delete sound->second;
	}
}