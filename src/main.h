//
// Created by VokamisAir on 3/18/21.
//

#include "common.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#endif

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#define numVAOs 1
#define numVBOs 2

const char *glsl_version;

GLuint vertexArrayObjects[numVAOs];
GLuint vertexBuffersObject[numVBOs];

GLuint frameBufferObject, renderBufferObject;
GLuint renderingProgram = 0;
GLuint renderedTexture = 0;
unsigned int textureColorBuffer;

static float cameraPositionX;
static float cameraPositionY;
static float cameraPositionZ;

static float cameraTargetX;
static float cameraTargetY;
static float cameraTargetZ;

static float cubeLocationX;
static float cubeLocationY;
static float cubeLocationZ;

static float pyramidLocationX;
static float pyramidLocationY;
static float pyramidLocationZ;

ImVec2 frameBufferSize;

GLuint viewLocation;
GLuint modelViewMatrixLocation;
GLuint projectionLocation;
GLuint timeFrameLocation;

int width;
int height;
static float aspect;
glm::mat4 perspectiveMatrix;
glm::mat4 translationMatrix;
glm::mat4 rotationMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelMatrix;
glm::mat4 modelViewMatrix;

float cameraHeight = 1.0f;
float cameraPitch = 45.0f;
float planeHeight = 0.0f;
