#include "TextureManager.h"

TextureManager* TextureManager::_instance = NULL;

BorisConsoleManager* TextureManager::BorisConsoleManager = BorisConsoleManager::Instance();

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	DeleteTextures();
}

TextureManager* TextureManager::getInstance()
{
	if (!_instance)
	{
		return _instance = new TextureManager();
	}
	return _instance;
}

Texture* TextureManager::AddTexture(String textureName, LPCSTR filename)//(LPCSTR textureName, LPCSTR filename)
{
	Texture* texture = new Texture(filename, sdlRenderer);
	return AddTexture(textureName, texture);
}

Texture* TextureManager::AddTexture(String textureName, Texture* texture)//(LPCSTR textureName, Texture* texture)
{
	Texture* t = GetTexture(textureName);
	if (!t)
	{
		textureList.insert(make_pair(textureName, texture));
		return texture;
	}
	String str = "Unable to add" + textureName + "because a texture of that name has already been added.";
	BorisConsoleManager->Print(str);
	return texture;
}

Texture* TextureManager::GetTexture(String textureName)
{
	Dictionary<String, Texture*>::iterator txt = textureList.find(textureName);
	if (txt != textureList.end())
	{
		return txt->second;
	}
	return NULL;
}

Texture* TextureManager::GetTextureOrBlank(String textureName)
{
	Texture* t = GetTexture(textureName);
	if (t)
	{
		return t;
	}
	return BlankTexture();
}

void TextureManager::DeleteTextures()
{
	for (Dictionary<String, Texture*>::iterator txt = textureList.begin(); txt != textureList.end(); txt++)
	{
		if (txt->second)
		{
			delete txt->second;
			txt->second = NULL;
		}
	}
	textureList.clear();
}

void TextureManager::SetRenderer(SDL_Renderer* renderer)
{
	sdlRenderer = renderer;
	if (textureList.size() > 0)
	{
		for (Dictionary<String, Texture*>::iterator i = textureList.begin(); i != textureList.end(); i++)
		{
			i->second->SetSDLRenderer(renderer);
		}
	}
}

int TextureManager::Count()
{
	return (int)textureList.size();
}

Texture* TextureManager::BlankTexture()
{
	String nanKey = "[NAN Texture]";
	Texture* t = GetTexture(nanKey);
	if (t)
	{
		return t;
	}
	return AddTexture(nanKey,Texture::NANTexture(sdlRenderer));
}