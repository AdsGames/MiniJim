#ifndef EDITOR_H
#define EDITOR_H

#include "State.h"

#include "./lib/aar/aar.h"

#include "Camera.h"
#include "Tile.h"
#include "TileMap.h"
#include "ui/Button.h"
#include "ui/InputBox.h"

class Editor : public State {
 public:
  virtual ~Editor();

  virtual void init(aar::Window* window);
  virtual void update(StateEngine& engine) override;
  virtual void draw(aar::Renderer* buffer) override;

 private:
  void SaveClicked();
  void OpenClicked();

  int layer;
  int draw_layer;

  enum editor_states { SAVE, OPEN, CREATE, EDIT };

  char editor_state;

  TileMap* tile_map;
  Tile* pallette_tile;
  aar::Texture* cursor;

  aar::Font* editorFont;

  // Text input
  Button btn_save, btn_open, btn_new, btn_close;
  InputBox ib_save, ib_open, ib_width, ib_height;

  void Close();
  void Save();
  void Open();
  void New();
  void Edit();

  Camera cam;
};

#endif  // EDITOR_H