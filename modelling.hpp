#ifndef H_MODELLING
#define H_MODELLING

#include "RenderingProtocol.hpp"
#include "ctrlPts.hpp"
#include "initShaders.h"

#include <memory>
#include <vector>

using namespace cg;

namespace cg {
class Modelling : public RenderingProtocol,
                  public enable_shared_from_this<Modelling> {
private:
  GLuint modelShader;
  GLuint objVBO, objVBA;
  uint trianglesObj{0};

  void createObj(vector<cg::Point2d> ctrlPts) {
    auto contour = cg::casteljau(ctrlPts);
    auto obj = cg::model(contour);

    auto md_h = contour.size();
    auto md_w = obj.size() / md_h;
    auto getVTX = [&](int h, int w) { return obj[h * md_w + (w % md_w)]; };

    vector<Point3d> md_v;   // triangle vertexes
    vector<glm::vec3> md_n; // triangle normals
    auto insertTriangle = [&](Point3d a, Point3d b, Point3d c) {
      md_v.push_back(a);
      md_v.push_back(b);
      md_v.push_back(c);
      auto aa = glm::vec3(a.x, a.y, a.z);
      auto bb = glm::vec3(b.x, b.y, b.z);
      auto cc = glm::vec3(c.x, c.y, c.z);
      md_n.push_back(glm::normalize(glm::cross(cc - aa, bb - aa)));
    };
    for (auto h = 0; h < md_h - 1; ++h) {
      for (auto w = 0; w < md_w; ++w) {
        insertTriangle(getVTX(h, w), getVTX(h + 1, w), getVTX(h, w + 1));
        insertTriangle(getVTX(h + 1, w), getVTX(h + 1, w + 1),
                       getVTX(h, w + 1));
      }
    }
    trianglesObj = md_v.size();

    // todo: remove this conversion overread (use only vector<glm::vec3>)
    vector<GLfloat> vertexes;
    for (auto p : md_v) {
      vertexes.push_back(p.x);
      vertexes.push_back(p.y);
      vertexes.push_back(p.z);
    }
    vector<GLfloat> normals;
    for (auto n : md_n) {
      normals.push_back(n.x);
      normals.push_back(n.y);
      normals.push_back(n.z);
    }
    auto sz_vertexes = vertexes.size() * sizeof(GLfloat);
    auto sz_normals = normals.size() * sizeof(GLfloat);

    glGenVertexArrays(1, &objVBA);
    glBindVertexArray(objVBA);

    glGenBuffers(1, &objVBO);
    glBindBuffer(GL_ARRAY_BUFFER, objVBO);
    glBufferData(GL_ARRAY_BUFFER, sz_vertexes + sz_normals, NULL,
                 GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sz_vertexes, vertexes.data());
    glBufferSubData(GL_ARRAY_BUFFER, sz_vertexes, sz_normals, normals.data());

    GLint attrP = glGetAttribLocation(modelShader, "aPosition");
    glVertexAttribPointer(attrP, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrP);

    GLint attrN = glGetAttribLocation(modelShader, "aNormal");
    glVertexAttribPointer(attrN, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrN);

    cout << "model has " << trianglesObj / 3 << " triangles" << endl;
  }

  void displayObj() {
    glBindVertexArray(objVBA);

    // todo: manipulate object (rotate, translate, rescale)

    // position

    GLfloat bs = 2.0f;
    glm::mat4 ortho_box = glm::ortho(-bs, bs, -bs, bs, -bs, bs);

    GLfloat radius = 1.0f;
    GLfloat camY = sin(glfwGetTime()) * radius;
    GLfloat camZ = cos(glfwGetTime()) * radius;
    auto view = glm::lookAt(glm::vec3(0.0, camY, camZ),
                            glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 mvp = ortho_box * view;
    int loc = glGetUniformLocation(modelShader, "uMVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));

    // light

    // todo: texture

    int loc_ambient = glGetUniformLocation(modelShader, "Ambient");
    glUniform4f(loc_ambient, 0.5f, 0.0f, 0.0f, 1.0f);

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
