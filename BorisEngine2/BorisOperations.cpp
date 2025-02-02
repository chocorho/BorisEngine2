#include "BorisOperations.h"

namespace BorisOperations
{
	String BoolToString(bool b)
	{
		return b ? "true" : "false";
	}

	LPCSTR Char_to_LPCSTR(char c)
	{
		return new char[2] {c, '\0'};
	}

	String CharToString(char c)
	{
		return String(1, c);
	}

	bool CreateFolder(String dirname)
	{
		StdVec<String> dirParts = Split(dirname, String("\\"));
		if (FileExists(dirname) || dirParts.size() < 2)
		{
			return false;
		}
		String dir = dirParts[0];
		for (int i = 1; i < dirParts.size(); i++)
		{
			dir += "\\" + dirParts[i];
			if (!FileExists(dir))
			{
				if (!CreateDirectory(String_to_LPCSTR(dir), NULL))
				{
					return false;
				}
			}
		}
		return true;
	}

	SDL_Surface* CreateSurface(unsigned int bytesPerPixel, void *pixels, int width, int height, int depth, int pitch)
	{
		Uint32 rmask, gmask, bmask, amask;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				int shift = (bytesPerPixel == 3) ? 8 : 0;
				rmask = 0xff000000 >> shift;
				gmask = 0x00ff0000 >> shift;
				bmask = 0x0000ff00 >> shift;
				amask = 0x000000ff >> shift;
		#else // little endian, like x86
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = (bytesPerPixel == 3) ? 0 : 0xff000000;
		#endif
		return SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, rmask, gmask, bmask, amask);
	}

	bool FileExists(const String& filename)
	{
		struct stat buf;
		return stat(filename.c_str(), &buf) != -1;
	}

	float GetDistance(Vector2 a, Vector2 b)
	{
		float xdist = (a.X - b.X);
		float ydist = (a.Y - b.Y);
		return sqrt(pow(xdist, 2) + pow(ydist, 2));
	}

	SDL_Rect GetExpandedRect(SDL_Rect rect, int expansion)
	{
		return{ rect.x - expansion, rect.y - expansion, rect.w + (2 * expansion),rect.h + (4 * expansion) };
	}

	FloatRect GetFloatRect(SDL_Rect sdlRect)
	{
		return{ (float)sdlRect.x,(float)sdlRect.y,(float)sdlRect.w,(float)sdlRect.h };
	}

	SDL_Rect GetSDLRect(FloatRect frect)
	{
		return{ Round(frect.X),Round(frect.Y),Round(frect.W),Round(frect.H) };
	}

	LPCSTR Int_to_LPCSTR(int num)
	{
		//TODO: Optimize this.
		return String_to_LPCSTR(std::to_string(num));
	}

	float Lerp(float a, float b, float f)
	{
		return a + f*(b - a);
	}

	Vector2 Lerp(Vector2 a, Vector2 b, float f)
	{
		float x = Lerp(a.X, b.X, f);
		float y = Lerp(a.Y, b.Y, f);
		return{ x, y };
	}

	FloatRect Lerp(FloatRect a, FloatRect b, float f)
	{
		float x = Lerp(a.X, b.X, f);
		float y = Lerp(a.Y, b.Y, f);
		float w = Lerp(a.W, b.W, f);
		float h = Lerp(a.H, b.H, f);
		return{ x, y, w, h };
	}

	String PadNumber(int num, int idealSize)
	{
		String strnumber = std::to_string(num);
		while (strnumber.size() < (decltype(strnumber.size()))idealSize)
		{
			strnumber = "0" + strnumber;
		}
		return strnumber;
	}

	int RandomNumber(int min, int max)
	{
		return min + (rand() % (max - min + 1));
	}

	int Round(float value)
	{
		//return (int)ceil(value);
		return (int)round(value);
	}

	bool SDL_PointEquals(SDL_Point* a, SDL_Point* b)
	{
		if (a == b)
		{
			return true;
		}
		return (a->x == b->x) && (a->y == b->y);
	}

	StdVec<String> Split(String str, String delimiter)
	{
		StdVec<String> result;
		size_t pos = 0;
		String token;
		while ((pos = str.find(delimiter)) != String::npos)
		{
			token = str.substr(0, pos);
			result.push_back(token);
			str.erase(0, pos + delimiter.length());
		}
		result.push_back(str);
		return result;
	}

	LPCSTR String_to_LPCSTR(String str)
	{
		std::ostringstream ss;
		ss << str;
		return _strdup(ss.str().c_str());
	}

	SDL_Point Vector2ToSDLPoint(Vector2 vec2)
	{
		return{ Round(vec2.X),Round(vec2.Y) };
	}

	void WaitForMusicToStop()
	{
		int num = 0;
		Mix_HaltMusic();
		while (Mix_Playing(-1) != 0)
		{
			num++;
			std::cout << "Waiting for music to finish. Messaged " << num << " times." << std::endl;
			if (num > 200)
			{
				break;
			}
		}
	}

	bool LineIntersectsCircle(Vector2 lineStartPosition, Vector2 lineDirection, Circle circle)
	{
		return false;
	}
}