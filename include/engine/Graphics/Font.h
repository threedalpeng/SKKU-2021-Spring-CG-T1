#pragma once
#include <unordered_map>

#include "cg/cgut.h"
#include "cg/stb_truetype.h"

#include "engine/Graphics/Mesh.h"
#include "engine/Graphics/Shader.h"

struct stbtt_char_t
{
	GLuint	textureID;				// ID handle of the glyph texture
	ivec2	size;					// Size of glyph
	ivec2	bearing;				// Offset from baseline to left/top of glyph
	GLuint	advance;				// Horizontal offset to advance to next glyph
};

class Font {
public:
	bool loadFrom(std::string fontPath) {
		/* codes from text.cpp in */
		FILE* fp = fopen(fontPath.c_str(), "rb"); if (!fp) { printf("%s(): Unable to open %s\n", __func__, fontPath.c_str()); return false; }

		fseek(fp, 0, SEEK_END); size_t size = ftell(fp); fseek(fp, 0, SEEK_SET);
		unsigned char* font_buffer = (unsigned char*)malloc(size);
		if (!font_buffer)
			return false;
		std::size_t read_size = fread(font_buffer, 1, size, fp); // assert( read_size == size );
		fclose(fp);

		int result = stbtt_InitFont(&fontInfo, font_buffer, 0); if (!result) { printf("%s(): Failed to initialize stb_truetype.\n", __func__); return false; }
		create_font_textures();
		free(font_buffer);

		return true;
	}

	stbtt_char_t getCharacterInfo(char c) {
		return stbtt_char_list[c];
	}
private:
	stbtt_fontinfo fontInfo;
	std::unordered_map <GLchar, stbtt_char_t> stbtt_char_list;

	void create_font_textures();
};