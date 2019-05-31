#pragma once

namespace util {
	class Texture {
		unsigned texture;
		int height;
		int width;

	public:
		Texture(const char * name, bool mipmap = false);
		~Texture();

		void Render(float x, float y, float scale, float angle, float px, float py);
		void Use();
	};
};

