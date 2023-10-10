#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rendering/model.h>
#include <rendering/modelCreator.h>
#include <rendering/shader.h>
#include <camera/camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 760;

// camera
Camera camera(glm::vec3(9.0f, 9.0f, 38.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool hideCursor = true;

// time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//fps
float currentFPS = 0.0f;

// light variables
glm::vec3 lightDir(0.005f, -0.021f, -1.0f);
glm::vec3 lightAmbient(0.3f, 0.3f, 0.3f);
glm::vec3 lightDiffuse(0.7f, 0.7f, 0.7f);
glm::vec3 sunColor(1.0f, 0.73f, 0.0f);
float sunRadius = 0.052f;

// material variables
glm::vec3 materialDiffuse(0.15f, 0.4f, 0.5f);
glm::vec3 materialSpecular(0.72f, 0.57f, 0.3f);
float materialShininess = 64.0f;
float materialRefractiveIndex = 1.33f;
glm::vec3 materialHighlight(0.05f, 0.13f, 0.12f);
float materialHighlightOffset = 0.0f;

// sea variables
float seaSize = 250;
int seaResolution = 500;
int numWaves = 64;
float waveDirectionSeed = 128;
float initialAmplitude = 0.63;
float initialFrequency = 0.11;
float initialSpeed = 0.96;
float amplitudeIncrease = 0.74;
float frequencyIncrease = 1.22;
float speedIncrease = 0.970;

//skybox
vector<std::string> skyboxFaces
{
    "../res/textures/skybox/right.png",
    "../res/textures/skybox/left.png",
    "../res/textures/skybox/top.png",
    "../res/textures/skybox/bottom.png",
    "../res/textures/skybox/front.png",
    "../res/textures/skybox/back.png"
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // build and compile shaders
    // -------------------------
    Shader seaShader("../res/shaders/sea.vs", "../res/shaders/sea.fs");
    Shader skyboxShader("../res/shaders/skybox.vs", "../res/shaders/skybox.fs");

    // load models
    // -----------
    Model plane = *ModelCreator::CreatePlaneModel(seaSize, seaSize, seaResolution);

    float skyboxVertices[] = 
    {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    stbi_set_flip_vertically_on_load(false);
    unsigned int cubemapTexture = loadCubemap(skyboxFaces);

    // shader configuration
    // --------------------
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // ImGui initialization
    // --------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // fps
        currentFPS = 1.0f / deltaTime;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        seaShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        seaShader.setMatrix4("projection", projection);
        seaShader.setMatrix4("projection2", projection);
        seaShader.setMatrix4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        seaShader.setMatrix4("model", model);

        seaShader.setVector3("cameraPos", camera.Position);

        // skybox for sea
        seaShader.setInt("skybox", 0);

        // light variables
        seaShader.setVector3("light.direction", lightDir);
        seaShader.setVector3("light.ambient", lightAmbient);
        seaShader.setVector3("light.diffuse", lightDiffuse);

        // material variables
        seaShader.setVector3("material.diffuse", materialDiffuse);
        seaShader.setVector3("material.specular", materialSpecular);
        seaShader.setFloat("material.refractiveIndex", materialRefractiveIndex);
        seaShader.setFloat("material.shininess", materialShininess);
        seaShader.setVector3("material.highlight", materialHighlight);
        seaShader.setFloat("material.highlightOffset", materialHighlightOffset);

        // shader sea variables
        seaShader.setFloat("time", currentFrame);
        seaShader.setInt("numWaves", numWaves);
        seaShader.setFloat("directionSeed", waveDirectionSeed);
        seaShader.setFloat("initialAmplitude", initialAmplitude);
        seaShader.setFloat("initialFrequency", initialFrequency);
        seaShader.setFloat("initialSpeed", initialSpeed);
        seaShader.setFloat("amplitudeIncrease", amplitudeIncrease);
        seaShader.setFloat("frequencyIncrease", frequencyIncrease);
        seaShader.setFloat("speedIncrease", speedIncrease);

        plane.Draw(seaShader);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMatrix4("view", view);
        skyboxShader.setMatrix4("projection", projection);
        skyboxShader.setVector3("sunDirection", lightDir);
        skyboxShader.setVector3("sunColor", sunColor);
        skyboxShader.setFloat("sunRadius", sunRadius);
        skyboxShader.setFloat("time", currentFrame);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // start ImGui frame
        // -----------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // add ImGui widgets
        ImGui::Begin("Sea Variables");

        ImGui::Text("FPS: %.1f", currentFPS);

        if (ImGui::CollapsingHeader("Light"))
        {
            ImGui::SliderFloat3("Light Direction", &lightDir[0], -1.0f, 1.0f, "%.6f");
            ImGui::ColorEdit3("Light Ambient", &lightAmbient[0]);
            ImGui::ColorEdit3("Light Diffuse", &lightDiffuse[0]);
            ImGui::ColorEdit3("Sun Color", &sunColor[0]);
            ImGui::DragFloat("Sun Radius", &sunRadius, 0.001f);
        }

        if (ImGui::CollapsingHeader("Material"))
        {
            ImGui::ColorEdit3("Material Diffuse", &materialDiffuse[0]);
            ImGui::ColorEdit3("Material Specular", &materialSpecular[0]);
            ImGui::SliderFloat("Material Shininess", &materialShininess, 0.0f, 256.0f);
            ImGui::SliderFloat("Material Refractive Index", &materialRefractiveIndex, 1.0f, 2.0f);
            ImGui::ColorEdit3("Material Highlight", &materialHighlight[0]);
            ImGui::DragFloat("Material Highlight Offset", &materialHighlightOffset, 0.001f);
        }

        if (ImGui::CollapsingHeader("Sea"))
        {
            ImGui::SliderInt("Number of Waves", &numWaves, 1, 64);
            ImGui::InputFloat("Wave Direction Seed", &waveDirectionSeed);
            ImGui::DragFloat("Initial Amplitude", &initialAmplitude, 0.01f);
            ImGui::DragFloat("Amplitude Increase", &amplitudeIncrease, 0.01f);
            ImGui::DragFloat("Initial Frequency", &initialFrequency, 0.01f);
            ImGui::DragFloat("Frequency Increase", &frequencyIncrease, 0.01f);
            ImGui::DragFloat("Initial Speed", &initialSpeed, 0.01f);
            ImGui::DragFloat("Speed Increase", &speedIncrease, 0.01f);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        
        hideCursor = !hideCursor;

        if(hideCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
        
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if(hideCursor)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.isSprinting = true;
        else
            camera.isSprinting = false;
            
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if(hideCursor == false)
    {
        firstMouse = true;
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}  