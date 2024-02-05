#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#include "..\Logger\Logger.h"
#include "..\dataTypes.h"
#include "..\Render\Shader.h"

namespace ent {
	namespace ui {
		/// Holds all state information relevant to a character as loaded using FreeType
		struct Character {
			unsigned int TextureID; // ID handle of the glyph texture
			glm::ivec2   Size;      // Size of glyph
			glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
			unsigned int Advance;   // Horizontal offset to advance to next glyph
		};

		// Rendrs text
		class Text {
		public:
			Text();
			~Text();
			Text(std::string fontPath);
			void init(std::string fontPath, ui32 charWidth = 0, ui32 charHeight = 14);
			void RenderText(render::Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
			void always_on_top(bool value);
			void setSize(ui32 charWidth = 0, ui32 charHeight = 14);
		private:
			FT_Library ft;
			std::map<GLchar, Character> Characters;
			unsigned int VAO, VBO;
			bool m_AlwaysOnTop;
			ui32 m_CharWidth;
			ui32 m_CharHeight;
			std::string m_FontPath;

			void clear();

			ent::util::Logger* logger;
		};
	}
}


