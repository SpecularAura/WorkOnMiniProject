#include "Main_include.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera* camera);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::vector<float> CircleVertexArrayData(int angleCount);
std::vector<unsigned int> CircleVertexDrawOrder(int angleCount);

constexpr float INVERSE_ASPECT_RATIO = 9.0f/16.0f;
constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = INVERSE_ASPECT_RATIO * SCR_WIDTH;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD \n";
        glfwTerminate();
        return -1;
    }
    //glEnable(GL_DEPTH_TEST);

    Shader t1Shader("Shaders/vertex_shader.glvs", "Shaders/fragment_shader.glfs");

    unsigned int VBO, VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    std::vector<float> circledata = CircleVertexArrayData(30);
    std::vector<unsigned int> indexdata = CircleVertexDrawOrder(30);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (circledata.size() * sizeof(float)), circledata.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexdata.size() * sizeof(int)), indexdata.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    //glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //load and create texture 
    //-----------------------
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width,height,nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("media/container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }  
    else
    {
        std::cout<<"Failed to load data";
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("media/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load data";
    }
    stbi_image_free(data);

    Camera mainCamera{glm::vec3(0.0f, 0.0f, 3.0f)};
    glfwSetWindowUserPointer(window, (void*)&mainCamera);


    t1Shader.use();
    t1Shader.setUniform("texture1", 0);
    t1Shader.setUniform("texture2", 1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, &mainCamera);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 projection{1.0f};
        projection = glm::perspective(glm::radians(mainCamera.GetZoom()), 
                                      (16.0f/9.0f), 0.1f, 100.0f);

        t1Shader.setUniform("projection", projection);
        glm::mat4 view{1.0f};
        view = mainCamera.ViewMatrix();  
        
        t1Shader.setUniform("view", view);

        glm::mat4 model{1.0f};
        float cosGetTime{(float)glm::cos(glfwGetTime())};
        float bounce{0.125f * (1.0f - cosGetTime)};
        model = glm::translate(model, glm::vec3(0.0f, cosGetTime, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f - bounce, 0.5f));

        t1Shader.setUniform("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexdata.size(), GL_UNSIGNED_INT, 0);
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, Camera* camera)
{
    static float lastframe{0.0f};
    static float currentframe{0.0f};
    static float deltatime{0.0f};
    static int state{GLFW_CURSOR_NORMAL};

    currentframe = (float)glfwGetTime();
    deltatime = currentframe - lastframe;
    lastframe = currentframe;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {glfwSetWindowShouldClose(window, true);}
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, state);
        state = (state == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::FORWARD, deltatime);}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::BACKWARD, deltatime);}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::LEFT, deltatime);}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {camera->MoveCamera(CameraMovement::RIGHT, deltatime);}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse{true};
    static float lastX{(float)SCR_WIDTH / 2.0f};
    static float lastY{(float)SCR_HEIGHT / 2.0f};
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

    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    camera->RotateCamera(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    camera->ZoomCamera(yoffset);
}

std::vector<float> CircleVertexArrayData(int angleCount)
{
    if (angleCount <= 0)
    {
        std::cout<<"Please Put a positive INTEGER (>= 1)";
    }
    std::vector<float> circleData;
    float radius{1.0f};
    float angleStep{2 * glm::pi<float>() / (float)angleCount};

    float x, y;
    float tempx, tempy;
    float s, t;
    float r, angle;
    for (int i = 0; i <= angleCount; i++)
    {
        angle = i * angleStep;
        tempx = cos(angle); // tempx and tempy form a circle with radius 1
        tempy = sin(angle);
        x = radius * tempx;
        y = radius * tempy;
        std::cout << i << "=> x:" << x << " y:" << y << "\n";
        circleData.push_back(x);
        circleData.push_back(y);
        circleData.push_back(0.0f);

        s = tempx / 2 + 0.5f; // Shift the origin of the circle one unit to the right 
        t = tempy / 2 + 0.5f;
        circleData.push_back(s);
        circleData.push_back(t);
    }

    circleData.push_back(0.0f);
    circleData.push_back(0.0f);
    circleData.push_back(0.0f); // add the origin 
    s = 0.5f;
    t = 0.5f;
    circleData.push_back(s);
    circleData.push_back(t);

    return circleData;

}

std::vector<unsigned int> CircleVertexDrawOrder(int angleCount)
{
    std::vector<unsigned int> indices;
    int originLoc{angleCount + 1};
    for (int i = 0; i < angleCount; i++)
    {
        std::cout << originLoc << ", " << i << ", " << (i+1) << "\n";
        indices.push_back(originLoc);
        indices.push_back(i);
        indices.push_back(i+1);
    }

    return indices;    
}