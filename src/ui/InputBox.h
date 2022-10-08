#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <string>
#include "../lib/aar/aar.h"

class InputBox {
 public:
  InputBox();
  InputBox(int x,
           int y,
           int width,
           int height,
           const std::string& value = "",
           const std::string& type = "text");
  virtual ~InputBox();

  void Focus();

  std::string GetValue() const;
  void Update();
  void Draw();
  bool Hover() const;

 private:
  int x, y;
  int width, height;

  std::string text;
  std::string type;

  unsigned int text_iter;
  bool focused;
};

#endif  // INPUTBOX_H
