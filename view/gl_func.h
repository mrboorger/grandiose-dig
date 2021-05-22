#ifndef VIEW_GL_FUNC_H_
#define VIEW_GL_FUNC_H_

#include <QOpenGLContext>
#include <QOpenGLFunctions>

class GLFunctions : public QOpenGLFunctions {
 public:
  GLFunctions(const GLFunctions&) = delete;
  GLFunctions(GLFunctions&&) = delete;
  ~GLFunctions() = default;
  GLFunctions& operator=(const GLFunctions&) = delete;
  GLFunctions& operator=(GLFunctions&&) = delete;

  static GLFunctions* GetInstance();

 private:
  GLFunctions() = default;
};

#endif  // VIEW_GL_FUNC_H_
