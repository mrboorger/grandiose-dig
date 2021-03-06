#ifndef MODEL_MODEL_H_
#define MODEL_MODEL_H_

class Model {
 public:
  static Model* GetInstance();
 private:
  Model() = default;
};

#endif  // MODEL_MODEL_H_
