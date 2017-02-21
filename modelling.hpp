#ifndef H_MODELLING
#define H_MODELLING

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"
#include "initShaders.h"
#include "loadTexture.hpp"

#include <memory>
#include <vector>

using namespace cg;

namespace cg {
class Modelling : public RenderingProtocol,
                  public enable_shared_from_this<Modelling> {
private:
  GLuint modelShader;
  GLuint objVBO, objVBA;
  GLuint objTexture;
  uint trianglesObj{0};

  using Point2i = struct { int y, x; };

  void createObj(vector<cg::Point2d> ctrlPts) {
    auto contour = cg::casteljau(ctrlPts, 0.01f);
    auto obj = cg::model(contour, 0.01f);

    auto md_h = contour.size();
    auto md_w = obj.size() / md_h;
    auto getVTX = [&](const Point2i p) -> glm::vec3 {
      int h = p.y, w = p.x;
      auto o = obj[h * md_w + (w % md_w)];
      return glm::vec3(o.x, o.y, o.z);
    };

    vector<glm::vec3> md_v; // triangle vertexes
    vector<glm::vec3> md_n; // triangle normals
    auto insertTriangle = [&](Point2i A, Point2i B, Point2i C) {
      auto a = getVTX(A);
      auto b = getVTX(B);
      auto c = getVTX(C);
      md_v.push_back(a);
      md_v.push_back(b);
      md_v.push_back(c);
      md_n.push_back(glm::normalize(glm::cross(c - a, b - a)));
    };
    for (auto h = 0; h < md_h - 1; ++h) {
      for (auto w = 0; w < md_w; ++w) {
        insertTriangle(Point2i{h, w}, Point2i{h + 1, w}, Point2i{h, w + 1});
        insertTriangle(Point2i{h + 1, w}, Point2i{h + 1, w + 1},
                       Point2i{h, w + 1});
      }
    }

    trianglesObj = md_v.size();
    auto sz_vertexes = 3 * md_v.size() * sizeof(GLfloat);
    auto sz_normals = 3 * md_n.size() * sizeof(GLfloat);

    glGenVertexArrays(1, &objVBA);
    glBindVertexArray(objVBA);

    glGenBuffers(1, &objVBO);
    glBindBuffer(GL_ARRAY_BUFFER, objVBO);
    glBufferData(GL_ARRAY_BUFFER, sz_vertexes + sz_normals, NULL,
                 GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sz_vertexes, md_v.data());
    glBufferSubData(GL_ARRAY_BUFFER, sz_vertexes, sz_normals, md_n.data());

    GLint attrP = glGetAttribLocation(modelShader, "aPosition");
    glVertexAttribPointer(attrP, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrP);

    GLint attrN = glGetAttribLocation(modelShader, "aNormal");
    glVertexAttribPointer(attrN, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrN);

    objTexture = loadTexture("pattern.jpg");

    cout << "model has " << trianglesObj / 3 << " triangles" << endl;
  }

  void displayObj() {
    glBindVertexArray(objVBA);

    // todo: manipulate object (rotate, translate, rescale)

    // position

    GLfloat bs = 2.0f;
    glm::mat4 ortho_box = glm::ortho(-bs, bs, -bs, bs, -bs, bs);

    GLfloat radius = 1.0f;
    GLfloat camY = abs(sin(M_PI_4)) * radius;
    GLfloat camZ = cos(glfwGetTime()) * radius;
    GLfloat camX = sin(glfwGetTime()) * radius;
    auto view = glm::lookAt(glm::vec3(camX, camY, camZ),
                            glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 mvp = ortho_box * view;
    int loc = glGetUniformLocation(modelShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));

    // light

    auto normalMat = glm::mat3(1.0);
    int loc_normal = glGetUniformLocation(modelShader, "uNormalMat");
    glUniformMatrix3fv(loc_normal, 1, GL_FALSE, glm::value_ptr(normalMat));

    int loc_ambient = glGetUniformLocation(modelShader, "Ambient");
    glUniform3f(loc_ambient, 0.1f, 0.1f, 0.1f);

    int loc_lightColor = glGetUniformLocation(modelShader, "LightColor");
    glUniform3f(loc_lightColor, 1.0, 1.0, 1.0);

    int loc_lightDirection =
        glGetUniformLocation(modelShader, "LightDirection");
    glUniform3f(loc_lightDirection, .0, 1.0, .0);

    int loc_halfVector = glGetUniformLocation(modelShader, "HalfVector");
    glUniform3f(loc_halfVector, .0, .1, .0);

    int loc_shininess = glGetUniformLocation(modelShader, "Shininess");
    glUniform1f(loc_shininess, .5);

    int loc_strenght = glGetUniformLocation(modelShader, "Strenght");
    glUniform1f(loc_strenght, .10);

    // todo: texture

    glDrawArrays(GL_TRIANGLES, 0, trianglesObj);
  }

public:
  void loadShaders() {
    glUseProgram(modelShader);
    cout << "loaded obj shaders" << endl;
  }
  Modelling() {
    modelShader = InitShader("modelShader.vert", "modelShader.frag");
  }
  ~Modelling() = default;

  void setCtrlPts(vector<cg::Point2d> ctrlPts) {
    trianglesObj = 0;
    if (ctrlPts.size() > 1)
      createObj(ctrlPts);
  }

  void display() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.0, .0, .2, .0);
    if (trianglesObj > 0)
      displayObj();
    glfwSwapBuffers();
  }

  void update() override {}
  void onMouseClick(float x, float y) override {}
  void onKeyPress(char c) override {}
};
}

#endif
