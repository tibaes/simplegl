#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

GLuint loadTexture(const string &filename) {
  auto img = imread(filename);
  cout << "texture size: " << img.size() << endl;

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)img.size().width,
               (GLsizei)img.size().height, 0, GL_BGR, GL_UNSIGNED_BYTE,
               img.data);

  glGenerateMipmap(GL_TEXTURE_2D);

  return textureID;
}
