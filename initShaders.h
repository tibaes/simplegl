#ifndef __INIT_SHADERS__
#define __INIT_SHADERS__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

typedef struct Shader {
  const char *filename;
  GLenum type;
  GLchar *source;
} tShader;

static char *readShaderSource(const char *shaderFile);

GLuint InitShader(const char *vShaderFile, const char *fShaderFile);

#endif //__INIT_SHADERS__
