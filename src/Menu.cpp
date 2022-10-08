#include "Menu.h"

#include "utility/KeyListener.h"
#include "utility/MouseListener.h"

// Create menu
void Menu::init() {
  auto screenSize = aar::display::getLogicalSize();

  // Load images
  menu = aar::load::bitmap("assets/images/gui/menu.png");
  menuselect = aar::load::bitmap("assets/images/gui/menuSelector.png");
  help = aar::load::bitmap("assets/images/gui/help.png");
  cursor = aar::load::bitmap("assets/images/gui/cursor1.png");
  levelSelectNumber =
      aar::load::bitmap("assets/images/gui/levelSelectNumber.png");
  copyright = aar::load::bitmap("assets/images/gui/copyright.png");
  credits = aar::load::bitmap("assets/images/gui/credits.png");

  // Load sound
  click = aar::load::sample("assets/sounds/click.wav");
  intro = aar::load::sample("assets/sounds/intro.wav");
  music = aar::load::sampleOgg("assets/sounds/music/MiniJim.ogg");

  // Sets Font
  menuFont = aar::load::font("assets/fonts/ariblk.ttf", 24);

  // Create map for live background
  levelOn = 0;
  tile_map = new TileMap();
  change_level(0);
  next_state = -1;

  // Lighting
  // Build a color lookup table for lighting effects
  // get_palette(pal);
  // create_light_table(&light_table, pal, 0, 0, 0, nullptr);
  darkness = aar::load::bitmap("assets/images/darkness.png");
  darkness_old = aar::load::bitmap("assets/images/darkness.png");
  spotlight = aar::load::bitmap("assets/images/spotlight.png");

  // Buttons
  buttons[BUTTON_START] = Button(60, 630);
  buttons[BUTTON_START_MP] = Button(60, 690);
  buttons[BUTTON_EDIT] = Button(60, 750);
  buttons[BUTTON_HELP] = Button(60, 810);
  buttons[BUTTON_EXIT] = Button(60, 870);
  buttons[BUTTON_LEFT] = Button(screenSize.x - 180, 80);
  buttons[BUTTON_RIGHT] = Button(screenSize.x - 80, 80);

  buttons[BUTTON_START].SetImages("assets/images/gui/button_start.png",
                                  "assets/images/gui/button_start_hover.png");
  buttons[BUTTON_START_MP].SetImages(
      "assets/images/gui/button_start_mp.png",
      "assets/images/gui/button_start_mp_hover.png");
  buttons[BUTTON_EDIT].SetImages("assets/images/gui/button_edit.png",
                                 "assets/images/gui/button_edit_hover.png");
  buttons[BUTTON_HELP].SetImages("assets/images/gui/button_help.png",
                                 "assets/images/gui/button_help_hover.png");
  buttons[BUTTON_EXIT].SetImages("assets/images/gui/button_quit.png",
                                 "assets/images/gui/button_quit_hover.png");
  buttons[BUTTON_LEFT].SetImages("assets/images/gui/button_left.png",
                                 "assets/images/gui/button_left_hover.png");
  buttons[BUTTON_RIGHT].SetImages("assets/images/gui/button_right.png",
                                  "assets/images/gui/button_right_hover.png");

  buttons[BUTTON_START].SetOnClick([this]() {
    single_player = true;
    next_state = StateEngine::STATE_GAME;
  });

  buttons[BUTTON_START_MP].SetOnClick([this]() {
    single_player = false;
    next_state = StateEngine::STATE_GAME;
  });

  buttons[BUTTON_EDIT].SetOnClick(
      [this]() { next_state = StateEngine::STATE_EDIT; });

  buttons[BUTTON_EXIT].SetOnClick(
      [this]() { next_state = StateEngine::STATE_EXIT; });

  buttons[BUTTON_LEFT].SetOnClick([this]() { change_level(-1); });

  buttons[BUTTON_RIGHT].SetOnClick([this]() { change_level(1); });

  // Variables
  aar::sound::play(music, 255, 128, 1);
  aar::sound::play(intro);
}

Menu::~Menu() {
  // Destory Bitmaps
  aar::load::destroyTexture(levelSelectNumber);
  aar::load::destroyTexture(cursor);
  aar::load::destroyTexture(menuselect);
  aar::load::destroyTexture(menu);
  aar::load::destroyTexture(help);
  aar::load::destroyTexture(copyright);
  aar::load::destroyTexture(credits);

  // Destory Samples
  aar::load::destroySample(click);
  aar::load::destroySample(intro);
  aar::load::destroySample(music);

  // Destory background
  delete tile_map;
}

void Menu::change_level(int level) {
  auto screenSize = aar::display::getLogicalSize();

  levelOn = (levelOn + level) < 0 ? 4 : (levelOn + level) % 5;

  tile_map->load("assets/data/level_" + std::to_string(levelOn + 1));

  scroll_x = random(screenSize.x, tile_map->getWidth() - screenSize.x);
  scroll_dir_x = random(0, 1) ? -3 : 3;
  scroll_y = random(screenSize.y, tile_map->getHeight() - screenSize.y);
  scroll_dir_y = random(0, 1) ? -3 : 3;

  aar::sound::play(click);

  cam = Camera(screenSize.x, screenSize.y, tile_map->getWidth(),
               tile_map->getHeight());
  cam.SetSpeed(5);
}

void Menu::update(StateEngine& engine) {
  auto screenSize = aar::display::getLogicalSize();

  // Move around live background
  if (scroll_x + screenSize.x / 2 >= tile_map->getWidth() ||
      scroll_x <= screenSize.x / 2)
    scroll_dir_x *= -1;

  if (scroll_y + screenSize.y / 2 >= tile_map->getHeight() ||
      scroll_y <= screenSize.y / 2)
    scroll_dir_y *= -1;

  scroll_x += scroll_dir_x;
  scroll_y += scroll_dir_y;

  cam.Follow(scroll_x, scroll_y);

  // State change
  if (next_state != -1)
    setNextState(engine, next_state);

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++)
    buttons[i].Update();
}

void Menu::draw() {
  auto screenSize = aar::display::getLogicalSize();

  // Draw background to screen
  aar::draw::primRectFill(0, 0, screenSize.x, screenSize.y,
                          aar::util::makeColor(255, 255, 255, 255));

  // Draw live background
  tile_map->draw(cam.GetX(), cam.GetY(), screenSize.x, screenSize.y);

  // Lighting
  if (tile_map->hasLighting()) {
    // // set_alpha_blender();
    // aar::draw::sprite(darkness, darkness_old, 0, 0);

    // // Get map area
    // std::vector<Tile*> ranged_map = tile_map->get_tiles_in_range(
    //     cam.GetX() - spotlight->w, cam.GetX() + cam.GetWidth() +
    //     spotlight->w, cam.GetY() - spotlight->h, cam.GetY() + cam.GetHeight()
    //     + spotlight->w);

    // for (auto t : ranged_map) {
    //   if (t->containsAttribute(light)) {
    //     // aar::draw::stretchSprite(
    //     //     darkness, spotlight,
    //     //     t->getX() - cam.GetX() + t->getWidth() / 2 - t->getWidth() *
    //     3,
    //     //     t->getY() - cam.GetY() + t->getHeight() / 2 - t->getHeight() *
    //     3,
    //     //     t->getWidth() * 6, t->getHeight() * 6);
    //   }
    // }

    aar::draw::sprite(darkness, 0, 0);
  }

  // Overlay
  aar::draw::sprite(credits, 0, 0);
  aar::draw::sprite(menu, 40, 480);

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++)
    buttons[i].Draw();

  // Level selection
  aar::draw::sprite(levelSelectNumber, screenSize.x - 160, 80);
  aar::draw::text(menuFont, std::to_string(levelOn + 1), screenSize.x - 120, 80,
                  aar::util::makeColor(0, 0, 0));

  // Cursor
  aar::draw::sprite(cursor, MouseListener::x, MouseListener::y);

  // Help menu
  if (buttons[BUTTON_HELP].Hover()) {
    aar::draw::sprite(help, 0, 0);
  }

  aar::draw::sprite(copyright, screenSize.x - 350, screenSize.y - 40);

  aar::draw::sprite(cursor, MouseListener::x, MouseListener::y);
}
