#ifndef VIEW_GL_FUNC_H_
#define VIEW_GL_FUNC_H_

#include <QOpenGLContext>
#include <QOpenGLFunctions>

class GLFunctions : public QOpenGLFunctions {
 public:
  GLFunctions(const GLFunctions&) = default;
  GLFunctions(GLFunctions&&) = default;
  ~GLFunctions() = default;
  GLFunctions& operator=(const GLFunctions&) = default;
  GLFunctions& operator=(GLFunctions&&) = default;

  static GLFunctions* GetInstance();

 private:
  GLFunctions() = default;
};

#endif  // VIEW_GL_FUNC_H_
