#include "view/gl_func.h"

GLFunctions* GLFunctions::GetInstance() {
  static GLFunctions f;
  return &f;
}
