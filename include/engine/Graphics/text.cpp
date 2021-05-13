#include "cg/cgmath.h"	// slee's simple math library
#include "cg/cgut.h"	// slee's OpenGL utility
#define STB_TRUETYPE_IMPLEMENTATION
#include "cg/stb_truetype.h"
#include <iostream>

// stb_truetype object
stbtt_fontinfo font_info;

GLuint		VAO;			// vertex array for text objects
GLuint		program_text;	// GPU program for text render

static const char*	font_path = "../bin/fonts/consola.ttf";				// from Windows/font/
static const char*	vert_text_path = "../bin/shaders/text.vert";		// text vertex shaders
static const char*	frag_text_path = "../bin/shaders/text.frag";		// text fragment shaders

struct stbtt_char_t
{
	GLuint	textureID;				// ID handle of the glyph texture
	ivec2	size;					// Size of glyph
	ivec2	bearing;				// Offset from baseline to left/top of glyph
	GLuint	advance;				// Horizontal offset to advance to next glyph
};

std::map<GLchar, stbtt_char_t> stbtt_char_list;

void create_font_textures()
{
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	for (GLubyte c = 0; c < 128; c++)
	{	
		// Font size (pixel) to scale
		float font_scale = stbtt_ScaleForPixelHeight( &font_info, 48 );

		// Get bitmap and bitmap info of character
		int width, height, offset_x, offset_y;
		unsigned char* bitmap = stbtt_GetCodepointBitmap( 
			&font_info, 									// Font information
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
		glGenTextures( 1, &texture_text );
		glBindTexture( GL_TEXTURE_2D, texture_text );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap );

		// Release bitmap
		stbtt_FreeBitmap( bitmap, font_info.userdata );

		// Set texture options
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// Get advance in x axis
		int advance_width = 0;
		stbtt_GetCodepointHMetrics( &font_info, c, &advance_width, nullptr );

		// Now store character for later use
		stbtt_char_t character =
		{
			texture_text,
			ivec2( width, height ),
			ivec2( offset_x, -offset_y ),			// flip y axis
			GLuint( advance_width * font_scale )
		};
		stbtt_char_list.insert( std::pair<GLchar, stbtt_char_t>( c, character ) );
	}
}

bool init_text()
{
	FILE* fp = fopen( font_path, "rb" ); if(!fp){ printf( "%s(): Unable to open %s\n", __func__, font_path ); return false; }
	
	fseek( fp, 0, SEEK_END ); size_t size = ftell( fp ); fseek( fp, 0, SEEK_SET );
	unsigned char* font_buffer = (unsigned char*) malloc(size);
	std::size_t read_size = fread( font_buffer, 1, size, fp ); // assert( read_size == size );
	fclose( fp );

	// Initialize stb truetype
	int result = stbtt_InitFont( &font_info, font_buffer, 0 ); if(!result){ printf( "%s(): Failed to initialize stb_truetype.\n", __func__ ); return false; }
	create_font_textures();
	free(font_buffer);

	program_text = cg_create_program( vert_text_path, frag_text_path );
	if(!program_text) return false;

	vertex vertices[6] = 
	{
		{ vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
		{ vec3(0,0,0), vec3(0,0,0), vec2(0.0f,1.0f) },
		{ vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
		{ vec3(0,1,0), vec3(0,0,0), vec2(0.0f,0.0f) },
		{ vec3(1,0,0), vec3(0,0,0), vec2(1.0f,1.0f) },
		{ vec3(1,1,0), vec3(0,0,0), vec2(1.0f,0.0f) } 
	};

	GLuint vertex_buffer;
	glGenBuffers( 1, &vertex_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

	VAO = cg_create_vertex_array( vertex_buffer ); if(!VAO){ printf( "%s(): VAO==nullptr\n", __func__ ); return false; }
	std::cout << VAO << " " << program_text << std::endl;
	return true;
}

void render_text( std::string text, GLint _x, GLint _y, GLfloat scale, vec4 color, GLfloat dpi_scale )
{
	// apply dpi_scale
	if(dpi_scale>1.0f)
	{
		_x = int(_x*dpi_scale);
		_y = int(_y*dpi_scale);
		scale *= dpi_scale;
	}

	// Activate corresponding render state	
	ivec2 window_size = cg_default_window_size();
	GLfloat x = GLfloat(_x);
	GLfloat y = GLfloat(_y);
	glUseProgram(program_text);
	glBindVertexArray(VAO);

	glUniform4f(glGetUniformLocation(program_text, "text_color"), color.r, color.g, color.b, color.a);
	glActiveTexture(GL_TEXTURE0);
	
	mat4 text_offset = mat4( 1/(window_size.x/2.0f), 0.0f, 0.0f,-1.0f,		// view space conversion
						 0.0f, 1/(window_size.y/2.0f), 0.0f, 1.0f,
						 0.0f, 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 0.0f, 1.0f );

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		stbtt_char_t ch = stbtt_char_list[*c];

		mat4 text_size		= mat4(	scale * float(ch.size.x),0.0f,0.0f,0.0f,
									0.0f,scale * float(ch.size.y),0.0f,0.0f,
									0.0f,0.0f,1.0f,0.0f,
									0.0f,0.0f,0.0f,1.0f
									);

		mat4 text_translate = mat4( 1.0f,0.0f,0.0f,	x  + scale * float(ch.bearing.x),
									0.0f,1.0f,0.0f,	-y + scale * float(-(ch.size.y - ch.bearing.y)),
									0.0f,0.0f,1.0f,	0.0f,
									0.0f,0.0f,0.0f,	1.0f
									);

		mat4 text_matrix = mat4();
		
		text_matrix = text_offset * text_translate * text_size * text_matrix;
		glUniformMatrix4fv(glGetUniformLocation(program_text, "text_matrix"), 1, GL_TRUE, text_matrix);

		// Render glyph texture over quad
 		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph
		x += ch.advance * scale;
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}