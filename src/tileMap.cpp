#include "TileMap.h"

#include <sstream>
#include <fstream>

#include "globals.h"
#include "utility/tools.h"

// Constructor
TileMap::TileMap (std::string file) {
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  frame_timer.Start();

  if (file != "")
    load (file);
}

// Destructor
TileMap::~TileMap() { }

// Get frame
int TileMap::getFrame() {
  return int(frame_timer.GetElapsedTime<milliseconds>() / 100) % 8;
}

bool TileMap::load (std::string file) {
  //Change size
  std::ifstream read ((file + ".txt").c_str());

  if (read.fail())
    return false;

  width = height = x = y = 0;
  int data;

  while (read >> data) {
    if (height == 0)
      width++;
    if (read.peek() == '\n')
      height++;
  }
  read.close();

  //Setup Map
  mapTiles.clear();
  mapTilesBack.clear();

  read.open ((file + ".txt").c_str());
  for (int t = 0; t < height; t++) {
    for (int i = 0; i < width; i++) {
      read >> data;
      mapTiles.push_back (tile (data, i * 64, t * 64));
    }
  }
  read.close();

  read.open ((file + "_back.txt").c_str());
  for (int t = 0; t < height; t++) {
    for (int i = 0; i < width; i++) {
      read >> data;
      mapTilesBack.push_back (tile (data, i * 64, t * 64));
    }
  }
  read.close();

  return true;
}

// Save individual layer
void TileMap::save_layer (std::string file, std::vector<tile> *layer) {
  int i = 0;
  std::ofstream of(file.c_str());

  for (auto &t: *layer) {
    i++;
    of << t.getType();
    if (i == width) {
      of << std::endl;
      i = 0;
    }
    else {
      of << " ";
    }
  }
  of.close();
}

// Save file
void TileMap::save (std::string file) {
  save_layer (file, &mapTiles);
  save_layer (file.insert(file.size() - 4, "_back"), &mapTilesBack);
}

// Get tile at
tile* TileMap::get_tile_at (int s_x, int s_y, int layer) {
  std::vector<tile> *ttm = (layer == 1) ? &mapTiles : &mapTilesBack;
  for (auto &t: *ttm) {
    if (collisionAny (s_x + x, s_x + x, t.getX(), t.getX() + 64,
                      s_y + y, s_y + y, t.getY(), t.getY() + 64)) {
      return &t;
    }
  }
  return nullptr;
}

// Find tile type
tile* TileMap::find_tile_type (int type, int layer) {
  std::vector<tile> *ttm = (layer == 1) ? &mapTiles : &mapTilesBack;
  for (auto &t: *ttm) {
    if (t.getType() == type) {
      return &t;
    }
  }
  return nullptr;
}

// Get tile at
std::vector<tile*> TileMap::get_tiles_in_range (int x_1, int x_2, int y_1, int y_2) {
  std::vector<tile*> ranged_map;
  for (auto &t : mapTiles) {
    if (collisionAny (x_1, x_2, t.getX(), t.getX() + 64,
                      y_1, y_2, t.getY(), t.getY() + 64)) {
      ranged_map.push_back(&t);
    }
  }
  return ranged_map;
}

// Draw a layer
void TileMap::draw_layer (BITMAP *buffer, std::vector<tile> *layer, int x, int y) {
  for (auto &t: *layer) {
    if ((t.getX() >= x - t.getWidth() ) && (t.getX() < x + SCREEN_W) &&
        (t.getY() >= y - t.getHeight()) && (t.getY() < y + SCREEN_H)) {
      t.draw_tile (buffer, x, y, getFrame());
    }
  }
}

//Draw tile map
void TileMap::draw (BITMAP *buffer) {
  draw (buffer, x, y);
}

// Draw at position
void TileMap::draw (BITMAP *buffer, int x, int y) {
  draw_layer (buffer, &mapTilesBack, x, y);
  draw_layer (buffer, &mapTiles, x, y);
}
