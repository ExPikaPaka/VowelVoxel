#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../3dparty/stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3dparty/stb_image/stb_image_write.h"
#include <Logger.h>
#include "UI/GameWindow.h"
#include "UI/IMGUI_UI.h"
#include "Render/Shader.h"
#include "Render/Camera.h"
#include "Object/Mesh.h"
#include "Object/Model.h"
#include "Generation/Arrow.h"
#include "Algorithm/Ray.h"
#include "ColorSpace/Colors.h"
#include "Algorithm/entMath.h"
#include "World/SVO.h"
#include "UI/Text.h"
#include <thread>
#include <chrono>
#include "World/SVOConverter.h"
#include "World/MeshToSVO.h"
#include "World/World.h"
#include <future>
#include "Utility/Timer.h"


void keyboardUpdate(GLFWwindow* window, ent::render::Camera& camera);
int main();
void mouseUpdate(GLFWwindow* window, ent::render::Camera& camera, bool zero = false);
void viewFrameUpdate(GLFWwindow* window);



int main() {
    /* ------------------------------ Logger initialization ------------------------------ */
    ent::util::Logger* logger = logger->getInstance();
    logger->setLogLevel(ent::util::level::DEBUG);
    logger->setLogToConsole(true);
    logger->setLogToFile(true);
    logger->addLog("Starting new session", ent::util::level::INFO);


    /* ------------------------------ Window initialization ------------------------------ */
    ent::ui::GameWindow window;
    window.setWindowSize(1920, 1000);
    window.init();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* ------------------------------ IMGUI initialization ------------------------------ */
    ent::ui::imgui::init((GLFWwindow*)window.getHandle());


    /* ------------------------------ Shader initialization ------------------------------ */
    ent::render::Shader objectShader;
    std::string vShaderPath = "assets/shaders/objectTex.vert";
    std::string fShaderPath = "assets/shaders/objectTex.frag";
    objectShader.load(vShaderPath, fShaderPath);

    ent::render::Shader redShader;
    vShaderPath = "assets/shaders/objectTex.vert";
    fShaderPath = "assets/shaders/object.frag";
    redShader.load(vShaderPath, fShaderPath);

    ent::render::Shader normalShader;
    vShaderPath = "assets/shaders/normalShader.vert";
    fShaderPath = "assets/shaders/normalShader.frag";
    normalShader.load(vShaderPath, fShaderPath);

    // Text shaders
    ent::render::Shader textShader;
    vShaderPath = "assets/shaders/text.vert";
    fShaderPath = "assets/shaders/text.frag";
    textShader.load(vShaderPath, fShaderPath);
    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(window.getWidth()), 0.0f, static_cast<float>(window.getHeight()));
    textShader.use();
    textShader.setMat4("projection", textProjection);


    /* ------------------------------ Model initialization ------------------------------ */
    ent::model::Model object("assets/models/xyzCubeMesh.obj");
    object.setTextureFilter(GL_LINEAR);
    glEnable(GL_DEPTH_TEST);
        
    ent::model::Model arrow = ent::generation::genArrow({ -1,  -1,  3.5}, { -1, -1, 4.5 }, 6, 0.1, 0.1);
    ent::model::Mesh light = ent::generation::genCylinder({ -1,  -1,  3.5 }, { -1, -1, 4.5 }, 6, 0.5, true);

    std::vector<ent::model::Model> arrows;
    f32v3 lightPos(0);
    lightPos.z = 4;
    lightPos.x = 4;
    lightPos.y = -9;

    /* ------------------------------ Camera initialization ------------------------------ */
    f32m4 view = f32m4(0);
    f32m4 model = f32m4(0);
    f32m4 projection;
    ent::render::Camera camera;
    camera.speed = 2.0;
    camera.fov = 90;

    bool hideMouse = false;
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glPointSize(10);

    /* ------------------------------ Font initialization ------------------------------ */
    ent::ui::Text textRenderer;
    textRenderer.init("assets/fonts/Carlito-Bold.ttf", 0, 20);
    textRenderer.always_on_top(true);


    /* ------------------------------ Testing initialization ------------------------------ */
   



    ent::world::SVO<ent::world::Voxel> world(7);

    ent::world::Voxel simple_voxel;
    simple_voxel.id = 0;
    simple_voxel.color = { 255, 100, 0 };

    // Loading from bmp file
    int w, h, c;
    int voxCount = 0;
    for (ui32 layers = 0; layers < 1; layers++) {
        std::string filename = "assets/SVO_demo/shape4/layer-" + std::to_string(layers) + ".bmp";
        //std::string filename = "assets/SVO_demo/test_3-" + std::to_string(layers) + ".bmp";
        filename = "assets/SVO_demo/depth-7-test.bmp";

        std::cout << "Loading file \"" << filename << "\"\n";

        // Loading image
        unsigned char* array = stbi_load(filename.c_str(), &w, &h, &c, 0);
        
        i32 cID = 0;

        if (array) {
            std::cout << "Success\n";
            std::cout << "Image size: [" << w << ", " << h << "]. Total " << c << " channels.\n";
            // Inserting data into world
            for (ui32 j = 0; j < w; j++) {
                for (ui32 i = 0; i < w * 3; i += 3) {
                    if (array[i + j * w * 3]) {
                        int x = i / 3;
                        int y = w - 1 - j;
                        int z = layers;
                       // if (x < 10) {
                            
                        i32 r = 1;
                        i32 g = 1;
                        i32 b = 1;

                        r = array[cID + 0];
                        g = array[cID + 1];
                        b = array[cID + 2];

                        cID += 3;
                        simple_voxel.color = { r, g, b };

                        //if (array[i + j * w * 3] != 255 && array[i + j * w * 3 + 1] != 0) {
                            world.set({ x, y, z }, simple_voxel);
                            voxCount++;
                        //}

                           
                       // }
                           
                    }
                }
            }
            stbi_image_free(array);
        }
    }

    for (ui32 z = 0; z < 10; z++) {
        for (ui32 y = 0; y < 10; y++) {
            for (ui32 x = 0; x < 10; x++) {
                simple_voxel.color.r = (f32)x * 28.41;
                simple_voxel.color.g = (f32)y * 28.41;
                simple_voxel.color.b = (f32)z * 28.41;

                //world.set({ x, y, z }, simple_voxel);
            }
        }
    }


    std::cout << "Total voxels: " << voxCount << "\n";

    world.calculate();
    world.setSize(4);

    // New convertor test
    ent::world::SVOConverter converter;
    ent::model::Mesh worldModelMesh;
    ent::model::Model worldModel;
    
    auto start_time = std::chrono::high_resolution_clock::now();

    for (ui32 i = 0; i < 1; i++) {
        worldModelMesh = converter.convert(world, 7);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    // Print the elapsed time
    std::cout << "Conversion time: " << duration << " milliseconds" << std::endl;
    std::cout << "Mesh vertices: " << worldModelMesh.vertices.size()<< "\n";
    std::cout << "Mesh triangles: " << worldModelMesh.indices.size() / 3<< "\n";


 
    worldModelMesh.setPosition({ 1, 1, 0 });

    worldModel.addMesh(worldModelMesh);

    i32 c1 = 0;
    i32 x1 = 0;
    i32 y1 = 0;
    i32 z1 = 0;

    i32 level = 5;

    camera.speed = 2.6;

    glLineWidth(2);


    ent::model::Model arr;

    ent::world::World lia;


    for (ui32 z = 0; z < 1; z++) {
        for (ui32 y = 0; y < 5; y++) {
            for (ui32 x = 0; x < 5; x++) {
                simple_voxel.color.r = (f32)x * 28.41;
                simple_voxel.color.g = (f32)y * 28.41;
                simple_voxel.color.b = (f32)z * 28.41;

                //lia.gen({ x, y, z });
            }
        }
    }


    ent::util::Timer timer;

    while (!glfwWindowShouldClose((GLFWwindow*)window.getHandle())) {
        // Handle events /////////////////////////////////////////////////////////////////////
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //////////////////////////////////////////////////////////////////////////////////////
       
        // Update ////////////////////////////////////////////////////////////////////////////
        // View update
        viewFrameUpdate((GLFWwindow*)window.getHandle());

        // Mouse update
        if (hideMouse) mouseUpdate((GLFWwindow*)window.getHandle(), camera);

        if (glfwGetKey((GLFWwindow*)window.getHandle(), GLFW_KEY_C) == GLFW_PRESS) {
            glfwSetInputMode((GLFWwindow*)window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseUpdate((GLFWwindow*)window.getHandle(), camera, true);
            hideMouse = true;
        }
        if (glfwGetKey((GLFWwindow*)window.getHandle(), GLFW_KEY_V) == GLFW_PRESS) {
            glfwSetInputMode((GLFWwindow*)window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            hideMouse = false;
        }

        // Keyboard update
        keyboardUpdate((GLFWwindow*)window.getHandle(), camera);

        // Light position update
        lightPos = f32v3(-1, -1, 4);
        if (glfwGetKey((GLFWwindow*)window.getHandle(), GLFW_KEY_G) == GLFW_PRESS) {
            lightPos = camera.position;
        }
        //////////////////////////////////////////////////////////////////////////////////////

        f32 camDist = sqrt(camera.position.x * camera.position.x + camera.position.y * camera.position.y + camera.position.z * camera.position.z);
        
        f32v3 p = { 1, 1, 0 };
        p += worldModel.getCenter();
        //camera.lookAt(p);

        if (glfwGetKey((GLFWwindow*)window.getHandle(), GLFW_KEY_X) == GLFW_PRESS) {
      

            if (!timer.active()) {
                level--;
                if (level < 1) level = 1;
                worldModelMesh = converter.convert(world, level);

                worldModelMesh.setPosition({ 1, 1, 0 });

                worldModel.clear();

                worldModel.addMesh(worldModelMesh);
                timer.setTimer(1000);
            }
        }

        if (glfwGetKey((GLFWwindow*)window.getHandle(), GLFW_KEY_Z) == GLFW_PRESS) {
            if (!timer.active()) {
                level++;
                if (level > 5) level = 5;
                worldModelMesh = converter.convert(world, level);

                worldModelMesh.setPosition({ 1, 1, 0 });

                worldModel.clear();

                worldModel.addMesh(worldModelMesh);
                timer.setTimer(1000);
            }
        }

        objectShader.use();
        
        model = f32m4(1);
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.fov), (float)((float)window.getWidth() / (float)window.getHeight()), 0.0001f, 1000000.0f);

        objectShader.use();
        objectShader.setMat4("model", model);
        objectShader.setMat4("view", view);
        objectShader.setMat4("projection", projection);

		objectShader.setVec3("material.ambient", 0.3, 0.3, 0.3);
		objectShader.setVec3("material.diffuse", 0.9, 0.9, 0.9);
		objectShader.setVec3("material.specular", 0.4, 0.4, 0.4);
		objectShader.setFloat("material.shininess", 16);
		objectShader.setFloat("material.emissivity", 0);
		objectShader.setInt("lights_count", 1);

		for (int i = 0; i < 1; i++) {
			objectShader.setVec3((std::string("light[" + std::to_string(i)) + std::string("].ambient")).c_str(), 1.0f, 1.0f, 1.0f);
			objectShader.setVec3((std::string("light[" + std::to_string(i)) + std::string("].diffuse")).c_str(), 1.0f, 1.0f, 1.0f);
			objectShader.setVec3((std::string("light[" + std::to_string(i)) + std::string("].specular")).c_str(), 1.0f, 1.0f, 1.0f);
			objectShader.setVec3((std::string("light[" + std::to_string(i)) + std::string("].position")).c_str(), lightPos);
			objectShader.setVec3((std::string("light[" + std::to_string(i)) + std::string("].direction")).c_str(), 0.0f, 1.0f, 0.0f);
			objectShader.setFloat((std::string("light[" + std::to_string(i)) + std::string("].linear")).c_str(), 0.0f);
			objectShader.setFloat((std::string("light[" + std::to_string(i)) + std::string("].constant")).c_str(), 0.0f);
			objectShader.setFloat((std::string("light[" + std::to_string(i)) + std::string("].quadratic")).c_str(), 0.0f);
			objectShader.setInt((std::string("light[" + std::to_string(i)) + std::string("].type")).c_str(), 0.0f);
			objectShader.setFloat((std::string("light[" + std::to_string(i)) + std::string("].cutoff")).c_str(), cos(glm::radians(50.0f)));
			objectShader.setFloat((std::string("light[" + std::to_string(i)) + std::string("].outerCutoff")).c_str(), cos(glm::radians(0.0f)));
		}
        objectShader.setVec3("viewPos", camera.position);


        model = glm::translate(model, { 0.1, 0, 0.1 });
        objectShader.setMat4("model", model);
        object.draw(objectShader);

        model = f32m4(1);
        objectShader.setMat4("model", model);


        // Rendering new world mesh
        worldModel.draw(objectShader);
        //lia.draw(objectShader);



        redShader.use();
        redShader.setVec3("material.ambient", 0.2, 0.2, 0.2);
        redShader.setVec3("material.diffuse", 0.6, 0.6, 0.6);
        redShader.setVec3("material.specular", 0.2, 0.2, 0.2);
        redShader.setFloat("material.shininess", 1);
        redShader.setFloat("material.emissivity", 0);
        redShader.setInt("lights_count", 1);
        
        redShader.setVec3((std::string("light[" + std::to_string(0)) + std::string("].ambient")).c_str(), 1.0f, 1.0f, 1.0f);
        redShader.setVec3((std::string("light[" + std::to_string(0)) + std::string("].diffuse")).c_str(), 1.0f, 1.0f, 1.0f);
        redShader.setVec3((std::string("light[" + std::to_string(0)) + std::string("].specular")).c_str(), 1.0f, 1.0f, 1.0f);
        redShader.setVec3((std::string("light[" + std::to_string(0)) + std::string("].position")).c_str(), lightPos);
        redShader.setVec3((std::string("light[" + std::to_string(0)) + std::string("].direction")).c_str(), 0.0f, 1.0f, 0.0f);
        redShader.setFloat((std::string("light[" + std::to_string(0)) + std::string("].linear")).c_str(), 0.0f);
        redShader.setFloat((std::string("light[" + std::to_string(0)) + std::string("].constant")).c_str(), 0.0f);
        redShader.setFloat((std::string("light[" + std::to_string(0)) + std::string("].quadratic")).c_str(), 0.0f);
        redShader.setInt((std::string("light[" + std::to_string(0)) + std::string("].type")).c_str(), 0.0f);
        redShader.setFloat((std::string("light[" + std::to_string(0)) + std::string("].cutoff")).c_str(), cos(glm::radians(50.0f)));
        redShader.setFloat((std::string("light[" + std::to_string(0)) + std::string("].outerCutoff")).c_str(), cos(glm::radians(0.0f)));
        redShader.setVec3("viewPos", camera.position);

        model = f32m4(1);
        redShader.setMat4("model", model);
        redShader.setMat4("view", view);
        redShader.setMat4("projection", projection);

        static i32 f = 0;
        //arrow.draw(redShader);
        //model = glm::translate(model, lightPos);
        arr.draw(redShader);
        redShader.setMat4("model", model);
        light.draw(redShader);
        //wMesh.draw(redShader);


        std::string position;
        position = "";
        position += "X [";
        position += std::to_string(camera.position.x);
        position.erase(position.size() - 3);
        position += "]";
        textRenderer.RenderText(textShader, position.c_str(), 10, 1000 - 20, 1.0f, glm::vec3(0.5, 0.8f, 0.2f) * 0.7f);

        position = "";
        position += "Y [";
        position += std::to_string(camera.position.y);
        position.erase(position.size() - 3);
        position += "]";
        textRenderer.RenderText(textShader, position.c_str(), 10, 1000 - 45, 1.0f, glm::vec3(0.5, 0.8f, 0.2f) * 0.7f);

        position = "";
        position += "Z [";
        position += std::to_string(camera.position.z);
        position.erase(position.size() - 3);
        position += "]";
        textRenderer.RenderText(textShader, position.c_str(), 10, 1000 - 70, 1.0f, glm::vec3(0.5, 0.8f, 0.2f) * 0.7f);

        std::string vX = "X [" + std::to_string((int)((float)(camera.position.x * world.getDimension()) / world.getSize())) + "]";
        std::string vY = "Y [" + std::to_string((int)((float)(camera.position.y * world.getDimension()) / world.getSize())) + "]";
        std::string vZ = "Z [" + std::to_string((int)((float)(camera.position.z * world.getDimension()) / world.getSize())) + "]";
        textRenderer.RenderText(textShader, vX.c_str(), 160, 1000 - 20, 1.0f, glm::vec3(1.5, 0.8f, 0.2f) * 0.7f);
        textRenderer.RenderText(textShader, vY.c_str(), 160, 1000 - 45, 1.0f, glm::vec3(1.5, 0.8f, 0.2f) * 0.7f);
        textRenderer.RenderText(textShader, vZ.c_str(), 160, 1000 - 70, 1.0f, glm::vec3(1.5, 0.8f, 0.2f) * 0.7f);

        // Render ////////////////////////////////////////////////////////////////////////////
        glfwSwapBuffers((GLFWwindow*)window.getHandle());
        //////////////////////////////////////////////////////////////////////////////////////
    }


    logger->addLog("Ending session\n\n", ent::util::level::INFO);
    return 0;
}

void mouseUpdate(GLFWwindow* window, ent::render::Camera& camera, bool zero) {
    static f64 x_old = 0, y_old = 0, x = 0, y = 0;
    static bool start = true;
    if (zero) start = true;

    glfwGetCursorPos(window, &x, &y);
    if (start) {
        x_old = x;
        y_old = y;
        start = false;
    }
  
    camera.processMouseMovement(x - x_old, y - y_old);
    x_old = x;
    y_old = y;
}

void viewFrameUpdate(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        glDisable(GL_CULL_FACE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }
}

void keyboardUpdate(GLFWwindow* window, ent::render::Camera& camera) {
    static float previous = glfwGetTime();
    float now = glfwGetTime();

    float deltaTime = now - previous;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processMovement(ent::render::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processMovement(ent::render::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processMovement(ent::render::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processMovement(ent::render::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processMovement(ent::render::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processMovement(ent::render::DOWN, deltaTime);

    previous = now;
}