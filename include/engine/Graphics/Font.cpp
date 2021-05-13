#include "Font.h"

void Font::create_font_textures() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (GLubyte c = 0; c < 128; c++) {
		// Font size (pixel) to scale
		float font_scale = stbtt_ScaleForPixelHeight(&fontInfo, 48);

		// Get bitmap and bitmap info of character
		int width, height, offset_x, offset_y;
		unsigned char* bitmap = stbtt_GetCodepointBitmap(
			&fontInfo, 										// Font information
			0.0f, 											// Scale in x axis
			font_scale, 									// Scale in y axis
			c, 												// Character
			&width, 										// Width of bitmap
			&height, 										// Height of bitmap
			&offset_x, 										// Left bearing of bitmap
			&offset_y 										// Top bearing of bitmap (negative value)
		);

		// Generate texture
		GLuint texture_text;
		glGenTextures(1, &texture_text);
		glBindTexture(GL_TEXTURE_2D, texture_text);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

		// Release bitmap
		stbtt_FreeBitmap(bitmap, fontInfo.userdata);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Get advance in x axis
		int advance_width = 0;
		stbtt_GetCodepointHMetrics(&fontInfo, c, &advance_width, nullptr);

		// Now store character for later use
		stbtt_char_t character =
		{
			texture_text,
			ivec2(width, height),
			ivec2(offset_x, -offset_y),			// flip y axis
			GLuint(advance_width * font_scale)
		};
		stbtt_char_list.insert(std::pair<GLchar, stbtt_char_t>(c, character));
	}
}