/**
 * Key Listener
 *   Allan Legemaate
 * Listens for keys JUST pressed or JUST released
 *   since the last tick
 * 16/05/2016
 **/

#ifndef KeyListener_H
#define KeyListener_H

#define KL_KEY_MAX 512

class KeyListener {
 public:
  KeyListener(){};
  virtual ~KeyListener(){};

  static void update();

  static bool keyPressed[KL_KEY_MAX];
  static bool keyReleased[KL_KEY_MAX];
  static bool keyDown[KL_KEY_MAX];
  static bool anyKeyPressed;

 private:
  static bool lastTicksKey[KL_KEY_MAX];
};

#endif  // KeyListener_H
