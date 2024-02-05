#include "GameWindow.h"

namespace ent {
	namespace ui {
		GameWindow::GameWindow() {
			setDefaultSettings(&m_displayMode);
			logger = util::Logger::getInstance();
			logger->addLog("Initializing GameWindow with default settings", util::level::DEBUG);
		}

		void GameWindow::init() {
			// Initializing GLFW
			if (glfwInit()) {
				logger->addLog("GLFW initialized successfully", util::level::INFO);
			} else {
				logger->addLog("Failed to initialize GLFW", util::level::FATAL);
			}

			// Setting GLFW parameters by default
			glfwDefaultWindowHints();

			// Configuring GLFW
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_displayMode.major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_displayMode.minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, m_displayMode.core ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, m_displayMode.isResizable);
			glfwWindowHint(GLFW_DECORATED, !m_displayMode.isBordless);
			//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1); // DEBUG VALUE | Enables window to have transparent background
			//glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, 1); // DEBUG VALUE | Enables window to pass through all input
			//glfwWindowHint(GLFW_FLOATING, 1); // DEBUG VALUE | Sets window always-on-top


			// Creating window
			GLFWmonitor* monitor = m_displayMode.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
			m_window = glfwCreateWindow(m_displayMode.screenWidth, m_displayMode.screenHeight, DEFAULT_TITLE, monitor, nullptr);

			if (m_window) {
				logger->addLog("Window created successfully", util::level::INFO);
			} else {
				logger->addLog("Failed to created window", util::level::FATAL);
			}

			// Initialize OpenGL
			glfwMakeContextCurrent((GLFWwindow*)m_window);

			if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				logger->addLog("OpenGL initialized successfully", util::level::INFO);
			} else {
				logger->addLog("Failed to initialize OpenGL", util::level::FATAL);
			}

			// Default clear colors
			glClearColor(0.176, 0.176, 0.176, 0.0);
			glClearColor(0.7, 0.7, 0.7, 0.0);
			glClearColor(0.1, 0.1, 0.1, 0.0);
			glClearDepth(1);

			// Initialize frame buffer
			glViewport(0, 0, m_displayMode.screenWidth, m_displayMode.screenHeight);

			// Fresh rate synchronization
			setSwapInterval(m_displayMode.swapInterval);

			// Some other initialization

			m_shouldQuit = false;
		}

		uint GameWindow::getWidth() {
			updateWindowSize();
			return m_displayMode.screenWidth;
		}

		uint GameWindow::getHeight() {
			updateWindowSize();
			return m_displayMode.screenHeight;
		}

		f32 GameWindow::getRatio() {
			updateWindowSize();
			return (float)m_displayMode.screenWidth / (float)m_displayMode.screenHeight;
		}

		void GameWindow::setWindowSize(ui32 w, ui32 h) {
			m_displayMode.screenWidth = w;
			m_displayMode.screenHeight = h;
		}

		void GameWindow::setDefaultSettings(GameDisplayMode* mode) {
			mode->screenWidth = DEFAULT_WINDOW_WIDHT;
			mode->screenHeight = DEFAULT_WINDOW_HEIGHT;
			mode->swapInterval = DEFAULT_SWAT_INTERFAL;
			mode->maxFPS = DEFAULT_MAX_FPS;
			mode->isResizable = true;
			mode->isBordless = false;
			mode->isFullscreen = false;
			mode->major = 3;
			mode->minor = 3;
			mode->core = true;
		}

		void GameWindow::updateWindowSize() {
			i32 width = 0;
			i32 height = 0;
			glfwGetWindowSize((GLFWwindow*)m_window, &width, &height);

			// Update viewport in case, if window size changed
			if (height != m_displayMode.screenHeight || width != m_displayMode.screenWidth) {
				glViewport(0, 0, m_displayMode.screenWidth, m_displayMode.screenHeight);
			}

			m_displayMode.screenWidth = width;
			m_displayMode.screenHeight = height;
		}
	}
}

