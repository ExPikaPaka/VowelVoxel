#pragma once
#include "..\Logger\Logger.h"
#include "..\dataTypes.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ent {
	namespace ui {

		#define DEFAULT_WINDOW_WIDHT 800
		#define DEFAULT_WINDOW_HEIGHT 600
		#define DEFAULT_SWAT_INTERFAL GameSwapInterval::V_SYNC
		#define DEFAULT_MAX_FPS 60
		#define DEFAULT_TITLE "Ent engine window"

		/* Typeless window handle
		*	GLFW - GLFWwindow*
		*   SDL - SDL_GLContext*
		*/
		typedef void* WindowHandle;

		enum class GameSwapInterval {
			UNLIMITED_FPS = 0, // No screen syncronization or FPS limiting is performed
			V_SYNC = 1, // Syncronize with monitor frame rate
			LOW_SYNC = 2, // Skip every second frame (half of monitor frame rate)
		};

		struct GameDisplayMode {
			uint screenWidth; // buffer width
			uint screenHeight; // buffer height

			bool isFullscreen;
			bool isBordless;
			bool isResizable;

			GameSwapInterval swapInterval; // Synchronization value for the screen refresh rate 
			uint maxFPS; // Desired max FPS, if the FPS limiter is used

			ui32 major; // Major version for graphics context (ex. GL:3.3)
			ui32 minor; // Minor version for graohics context (ex. GL:3.0)
			bool core; // Set true if only OpenGL Core is used
		};

		class GameWindow {
		public:
			/*  Initializes window with default values
			*	Doesn't create physical window
			*/
			GameWindow();

			/* Creates the window and initializes context
			*
			*/
			void init();
			
			/* Returns true if 'quit' signal was received */
			bool shouldQuit() const { return m_shouldQuit; }


			// Access display settings
			uint getWidth();
			uint getHeight();
			f32 getRatio();
			bool isFullscreen() const { return m_displayMode.isFullscreen; }
			bool isBordless() const { return m_displayMode.isBordless; }
			bool isResizable() const { return m_displayMode.isResizable; }
			uint getMaxFPS() const { return m_displayMode.maxFPS; }
			WindowHandle getHandle() const { return m_window; }

			// Change display settings
			void setPosition(int x, int y);
			void setWindowSize(ui32 w, ui32 h);
			void setFullscreen(bool state);
			void setBordless(bool state);
			void setResizable(bool state);
			void setMaxFPS(uint value) { m_displayMode.maxFPS = value; }
			void setSwapInterval(GameSwapInterval mode) { m_displayMode.swapInterval = mode; glfwSwapInterval((i32)mode); }
			void setTitle(const char title);

		private:
			static void setDefaultSettings(GameDisplayMode* mode);

			/* Singleton logger for logging with different levels */
			util::Logger* logger;

			GameDisplayMode m_displayMode; // Current display settings of the window
			bool m_shouldQuit = false; // Game exit flag
			WindowHandle m_window = nullptr; // Window handle

			void updateWindowSize();
		};
	}
}
