#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <fstream>
#include <iterator>

namespace
{
const int SHADER_ERROR_LOG_SIZE = 500;
bool VerifyShaderCompile(GLuint newShaderObject)
{
    GLint compileStatus;
    OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}

bool VerifyProgramLink(GLuint shaderProgram)
{
    GLint linkStatus;
    OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
    if (linkStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
    time = 0.;
    vertexPositions = 
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.f},
            {-1.f, 1.f, 0.f, 1.f},
            {-1.f, 0.f, 0.f, 1.f}
        });
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

void Assignment2::SetupExample1()
{
    // Insert "Load and Compile Shaders" code here.
    const std::string vertFilename = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
    const std::string fragFilename = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
    std::ifstream fsV(vertFilename, std::ifstream::in);
    std::ifstream fsF(fragFilename, std::ifstream::in);
    if(!fsV.is_open()){
        std::cerr <<"ERROR: Cound not read shader from" << vertFilename << std::endl;
        return;
    };
    if(!fsV.is_open()){
        std::cerr <<"ERROR: Cound not read shader from" << fragFilename << std::endl;
        return;
    };
    std::string shaderVerText((std::istreambuf_iterator<char>(fsV)), std::istreambuf_iterator<char>());
    std::string shaderFragText((std::istreambuf_iterator<char>(fsF)), std::istreambuf_iterator<char>());
    const char* shaderVerTextStr = shaderVerText.c_str();
    const char* shaderFragTextStr = shaderFragText.c_str();
    GLuint vertObj = OGL_CALL(glCreateShader(GL_VERTEX_SHADER));
    GLuint fragObj = OGL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
    OGL_CALL(glShaderSource(vertObj, 1, &shaderVerTextStr, NULL));
    OGL_CALL(glShaderSource(fragObj, 1, &shaderFragTextStr, NULL));
    OGL_CALL(glCompileShader(vertObj));
    OGL_CALL(glCompileShader(fragObj));
    shaderProgram = OGL_CALL(glCreateProgram());
    OGL_CALL(glAttachShader(shaderProgram, vertObj));
    OGL_CALL(glAttachShader(shaderProgram, fragObj));
    glLinkProgram(shaderProgram);
 
    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.
    const GLuint vertexShaderId = vertObj;
    const GLuint fragmentShaderId = fragObj;
    const GLuint shaderProgramId = shaderProgram;

    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }

    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION

    // Insert "Setup Buffers" code here.
    OGL_CALL(glGenVertexArrays(1, &vao));
    OGL_CALL(glBindVertexArray(vao));
    GLuint vertexBuffer;
    OGL_CALL(glGenBuffers(1, &vertexBuffer));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    OGL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * 6, &vertexPositions[0], GL_STATIC_DRAW));
    OGL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
    OGL_CALL(glEnableVertexAttribArray(0));
}

void Assignment2::Tick(double deltaTime)
{
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.
    OGL_CALL(glUseProgram(shaderProgram));
    OGL_CALL(glBindVertexArray(vao));
    OGL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
    
    time += deltaTime;
   // std::cout << time << " ";
    GLint var = OGL_CALL(glGetUniformLocation(shaderProgram, "inputTime"));
    OGL_CALL(glUniform1f(var, time));
    
    
}
