#include "Sprite.h"

TextureManager* Sprite::texturemanager = TextureManager::getInstance();
SoundManager* Sprite::soundmanager = SoundManager::GetInstance();
BorisConsoleManager* Sprite::BorisConsoleManager = BorisConsoleManager::Instance();

Sprite::Sprite(std::string textureName) : Sprite(texturemanager->GetTexture(textureName))
{

}

Sprite::Sprite(Texture* _texture)
{
	texture = _texture;
	int textureWidth = texture->getWidth();
	int textureHeight = texture->getHeight();
	SetDimensions(textureWidth, textureHeight);
	SetPosition({ 0,0,(float)textureWidth,(float)textureHeight });
	scale = { 1,1 };
	SetRotation(0);
}

void Sprite::MsgPosition()
{
	std::string msg = "X: " + std::to_string(position.X) + "; Y: " + std::to_string(position.Y) + "; W: " + std::to_string(position.W) + "; H: " + std::to_string(position.H) + ";";
	BorisConsoleManager->Print(msg);
	//std::stringstream ss;
	//ss << "X: " << position.X << " Y: " << position.Y << " W: " << position.W << " H: " << position.H;
	//BorisConsoleManager->Print(&ss);
	//cout << "X: " << position.X << " Y: " << position.Y << " W: " << position.W << " H: " << position.H << endl;
}

Sprite::~Sprite()
{
	delete texture;
}

//Make the sprite appear in the window at its given position.
void Sprite::Render()
{
	if (!rotation)
	{
		Render(&dimension, &GetPosition());
	}
	else
	{
		RenderRotated(&dimension, &GetPosition());
	}
}

void Sprite::Render(SDL_Rect* source, SDL_Rect* dest)
{
	if (GetSpriteType() == REGULAR)
	{
		texture->Render(source, dest);
	}
	else
	{
		texture->Render(NULL, NULL);
	}
}

void Sprite::RenderRotated(SDL_Rect* source, SDL_Rect* dest)
{
	if (GetSpriteType() == REGULAR)
	{
		texture->Render(source, dest, rotation, &centre);
	}
	else
	{
		texture->Render(NULL, NULL, rotation, &centre);
	}
}

SDL_Rect Sprite::GetPosition()
{
	return BorisOperations::GetSDLRect(position);
}

FloatRect Sprite::GetFloatPosition()
{
	return position;
}

Vector2 Sprite::GetVec2Position()
{
	return{ position.X,position.Y };
}

void Sprite::SetPosition(FloatRect _position)
{
	if (_position != position)
	{
		position = _position;
		centre.x = BorisOperations::Round(position.W / 2);
		centre.y = BorisOperations::Round(position.H / 2);
		SetRenderNow();
	}
}

void Sprite::SetPosition(float x, float y)
{
	SetPosition({ x,y,position.W,position.H });
}

void Sprite::SetPosition(Vector2 pos)
{
	SetPosition(pos.X, pos.Y);
}

void Sprite::LerpPosition(FloatRect destination,float f)
{
	SetPosition(BorisOperations::Lerp(position, destination, f));
}

void Sprite::LerpPosition(Vector2 destination, float f)
{
	SetPosition(BorisOperations::Lerp(GetVec2Position(), destination, f));
}

Texture* Sprite::GetTexture()
{
	return texture;
}

void Sprite::SetTexture(Texture* _texture)
{
	if (texture != _texture)
	{
		texture = _texture;
		SetDimensions(texture->getWidth(), texture->getHeight());
		SetRenderNow();
	}
}

void Sprite::SetTexture(std::string textureName)
{
	SetTexture(texturemanager->GetTexture(textureName));
}

SDL_Rect Sprite::GetDimensions()
{
	return dimension;
}

void Sprite::SetDimensions(SDL_Rect _dimension)
{
	//TODO: Fix this operator BS.
	//if (dimension != _dimension)
	{
		dimension = _dimension;
		width = dimension.w;
		height = dimension.h;
		SetRenderNow();
	}
}

void Sprite::SetDimensions(int _width, int _height)
{
	SetDimensions({ 0,0,_width,_height });
}

SDL_Point Sprite::GetCentre()
{
	return centre;
}

//void Sprite::SetCentre(SDL_Point _centre)
//{
//	centre = _centre;
//}

Vector2 Sprite::GetScale()
{
	return scale;
}

void Sprite::SetScale(Vector2 _scale)
{
	scale.X *= _scale.X;
	scale.Y *= _scale.Y;
	ScaleSprite();
}

void Sprite::SetScale(float x, float y)
{
	SetScale({ x,y });
}

void Sprite::SetAbsoluteScale(Vector2 _scale)
{
	scale.X = _scale.X;
	scale.Y = _scale.Y;
	ScaleSprite();
}

void Sprite::SetAbsoluteScale(float x, float y)
{
	SetAbsoluteScale({ x,y });
}

float Sprite::GetRotation()
{
	return rotation;
}

void Sprite::SetRotation(float _rotation)
{
	rotation = _rotation;
	SetRenderNow();
}

void Sprite::ScaleSprite()
{
	SetPosition({position.X,position.Y,dimension.w*scale.X,dimension.h*scale.Y});
}

//bool Sprite::IsActive()
//{
//	return active;
//}
//
//void Sprite::SetActive(bool _active)
//{
//	active = _active;
//}

void Sprite::Translate(Vector2 translation)
{
	SetPosition({( position.X + translation.X),(position.Y + translation.Y),position.W,position.H });
}

bool Sprite::CollidesWith(SDL_Rect* boundary)
{
	return SDL_HasIntersection(&GetPosition(), boundary);
}

bool Sprite::CollidesWith(Sprite* otherSprite)
{
	if (!IsActive() || !otherSprite->IsActive())
	{
		return false;
	}
	return CollidesWith(&BorisOperations::GetExpandedRect(otherSprite->GetPosition(),10));
}
//Left mouse button pressed on the position of the sprite.
bool Sprite::Clicked(SDL_Point* mouseposition)
{
	return IsActive() && SDL_PointInRect(mouseposition, &GetPosition());
}

void Sprite::SetSpriteType(SpriteType type)
{
	sprite_type = type;
	SetRenderNow();
}

SpriteType Sprite::GetSpriteType()
{
	return sprite_type;
}

float Sprite::GetDistanceFrom(Sprite* sprite)
{
	return BorisOperations::GetDistance(GetVec2Position(), sprite->GetVec2Position());
}