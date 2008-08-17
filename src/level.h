#include <SDL.h>

class graphics_c;

class level_c {

  private:

    typedef struct levelEntry {
      unsigned short bg;
      unsigned char fg;
      unsigned char dominoType;
      unsigned char dominoState;
      char dominoDir;
      char dominoYOffset;
    } levelEntry;

    levelEntry level[13][20];

    char theme[10];

    /* 2 bitmasks containing a bit for each block saying if it changed
     * there is one array for the static background and one for the dynamic
     * foreground with the dominos and the ant, the clock, ...
     */
    uint32_t staticDirty[13];
    uint32_t dynamicDirty[13];

    /* this surface contains the background. It is only updated when necessary
     * the content it used to restore stuff behind the sprites
     */
    SDL_Surface * background;

  public:

    level_c(void);

    void load(const char * name);

    const char * getTheme(void) const { return theme; }

    unsigned short getBg(unsigned int x, unsigned int y) const { return level[y][x].bg; }
    unsigned char  getFg(unsigned int x, unsigned int y) const { return level[y][x].fg; }
    unsigned char  getDominoType(unsigned int x, unsigned int y) const { return level[y][x].dominoType; }
    unsigned char  getDominoState(unsigned int x, unsigned int y) const { return level[y][x].dominoState; }

    /* update the background where necessary */
    void updateBackground(graphics_c * gr);

    /* draw the changed stuff into the target surface */
    void drawDominos(SDL_Surface * target, graphics_c * gr);


};
