#pragma once
#include "../3dparty/imgui/imgui.h"
#include "../3dparty/imgui/imgui_impl_glfw.h"
#include "../3dparty/imgui/imgui_impl_opengl3.h"

namespace ent {
    namespace ui {
        namespace imgui {
			// Set custom IMGUI theme
			void SetupImGuiStyle() {
				// Fork of Quick minimal look style from ImThemes
				ImGuiStyle& style = ImGui::GetStyle();

				style.Alpha = 1.0f;
				style.DisabledAlpha = 0.300000011920929f;
				style.WindowPadding = ImVec2(6.5f, 4.400000095367432f);
				style.WindowRounding = 5.400000095367432f;
				style.WindowBorderSize = 1.0f;
				style.WindowMinSize = ImVec2(20.0f, 32.0f);
				style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
				style.WindowMenuButtonPosition = ImGuiDir_Right;
				style.ChildRounding = 0.0f;
				style.ChildBorderSize = 1.0f;
				style.PopupRounding = 8.800000190734863f;
				style.PopupBorderSize = 1.0f;
				style.FramePadding = ImVec2(8.600000381469727f, 3.5f);
				style.FrameRounding = 7.0f;
				style.FrameBorderSize = 0.0f;
				style.ItemSpacing = ImVec2(4.400000095367432f, 4.0f);
				style.ItemInnerSpacing = ImVec2(3.799999952316284f, 3.900000095367432f);
				style.CellPadding = ImVec2(5.0f, 7.400000095367432f);
				style.IndentSpacing = 8.199999809265137f;
				style.ColumnsMinSpacing = 5.400000095367432f;
				style.ScrollbarSize = 12.60000038146973f;
				style.ScrollbarRounding = 12.30000019073486f;
				style.GrabMinSize = 10.10000038146973f;
				style.GrabRounding = 5.5f;
				style.TabRounding = 11.5f;
				style.TabBorderSize = 0.0f;
				style.TabMinWidthForCloseButton = 0.0f;
				style.ColorButtonPosition = ImGuiDir_Right;
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

				style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
				style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1336977630853653f, 0.1091933771967888f, 0.1630901098251343f, 0.6180257797241211f);
				style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
				style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
				style.Colors[ImGuiCol_Border] = ImVec4(0.1019607856869698f, 0.1019607856869698f, 0.1019607856869698f, 0.5f);
				style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
				style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1607843190431595f, 0.1490196138620377f, 0.1921568661928177f, 0.7811158895492554f);
				style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
				style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
				style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
				style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
				style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
				style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
				style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5450980663299561f, 0.4666666686534882f, 0.7176470756530762f, 1.0f);
				style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4697708189487457f, 0.3866160809993744f, 0.6480686664581299f, 1.0f);
				style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5720282793045044f, 0.4584722816944122f, 0.8154506683349609f, 1.0f);
				style.Colors[ImGuiCol_Button] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3450980484485626f, 0.294117659330368f, 0.4588235318660736f, 1.0f);
				style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3137255012989044f, 0.2588235437870026f, 0.4274509847164154f, 1.0f);
				style.Colors[ImGuiCol_Header] = ImVec4(0.3176470696926117f, 0.2784313857555389f, 0.407843142747879f, 1.0f);
				style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4156862795352936f, 0.364705890417099f, 0.529411792755127f, 1.0f);
				style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4039215743541718f, 0.3529411852359772f, 0.5098039507865906f, 1.0f);
				style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
				style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_Tab] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3254902064800262f, 0.2862745225429535f, 0.4156862795352936f, 1.0f);
				style.Colors[ImGuiCol_TabActive] = ImVec4(0.4000000059604645f, 0.3490196168422699f, 0.5058823823928833f, 1.0f);
				style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
				style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
				style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
				style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
				style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
				style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
				style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
				style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
				style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
				style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
				style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
				style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2784313857555389f, 0.250980406999588f, 0.3372549116611481f, 1.0f);
				style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
				style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
				style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
			}

            // Initializes IMGUI with OpenGL & GLWF. GLSL version 330 core
            bool init(GLFWwindow* handle) {
                // Initializing IMGUI
                const char* glsl_version = "#version 330 core";
                // Setup Dear ImGui context
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

                // Setup Dear ImGui style
                //ImGui::StyleColorsDark();
				SetupImGuiStyle();
                // Setup Platform/Renderer backends
                ImGui_ImplGlfw_InitForOpenGL(handle, true);
                ImGui_ImplOpenGL3_Init(glsl_version);

                return true;
            }

            // Draws test menu
            void drawTestMenu(GLFWwindow* handle) {
                static char buff[256] = "";
                static  float f = 0;

                // Creating new frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // Adding data
                ImGui::Begin("Scene edit");
                ImGui::Text("Material parameters:");
                ImGui::SliderFloat("Material.shininess", &f, 0.1, 128);
                ImGui::InputText("Path:", &buff[0], 256, ImGuiInputTextFlags_EnterReturnsTrue);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();

                // Rendering frame
                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(handle, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            }
        }
	}
}
