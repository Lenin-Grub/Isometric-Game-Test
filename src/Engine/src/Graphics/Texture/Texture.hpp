#pragma once
#include <string>

namespace smpl
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool loadFromFile(const std::string& file_path);

	private:

		unsigned int texture_id;
		int width;
		int height;
		int channels;
	};
}