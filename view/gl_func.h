#ifndef VIEW_GL_FUNC_H_
#define VIEW_GL_FUNC_H_

#include <QOpenGLContext>
#include <QOpenGLFunctions>

class GLFunctions : public QOpenGLFunctions {
 public:
  static GLFunctions* GetInstance();

 private:
  GLFunctions() = default;
};

#endif  // VIEW_GL_FUNC_H_
