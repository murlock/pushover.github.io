/* Pushover
 *
 * Pushover is the legal property of its developers, whose
 * names are listed in the AUTHORS file, which is included
 * within the source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA
 */

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <SDL.h>
#include <string>


/* this is the display class, there is only one instance for this class and it contains
 * the complete screen
 * the background is always a level.
 * In front of that can be stacked windows
 */

class graphicsN_c;
class pngLoader_c;

/* the 3 available fonts, they are a fixed size and are supposed to contain
 * all the required letters
 */
enum {
  FNT_SMALL,
  FNT_NORMAL,
  FNT_BIG
};

enum {
  ALN_TEXT,
  ALN_CENTER,
  ALN_TEXT_CENTER
};


typedef struct {
  int font = 0;
  SDL_Rect box;
  int alignment = ALN_TEXT;
  SDL_Color color = {0, 0, 0, 0};
  uint8_t shadow = 0;
} fontParams_s;


void initText(void);
void deinitText(void);
unsigned int getFontHeight(unsigned int font);
unsigned int getTextWidth(unsigned int font, const std::string & t);
unsigned int getTextHeight(const fontParams_s * par, const std::string & t);

bool rightToLeft(void);


// 2dimensional bitfield of 20x13 bits
class bitfield_c {

  private:

    // the dirty blocks
    uint32_t dynamicDirty[25];

  public:

    void markDirty(int x, int y) { if (x >= 0 && x < 20 && y >= 0 && y < 25) dynamicDirty[y] |= (1 << x); }
    bool isDirty(int x, int y) const {
      if (x >= 0 && x < 20 && y >= 0 && y < 25)
        return (dynamicDirty[y] & (1 << x)) != 0;
      else
        return false;
    }
    void clearDirty(void);
    void markAllDirty(void);
};

class surface_c {

  protected:

    SDL_Surface * video;

  public:

    SDL_Surface * getIdentical(void) const;

    surface_c(void) : video(0) {}
    surface_c(uint16_t x, uint16_t y, bool alpha = true);
    surface_c(SDL_Surface * c) : video(c) {}
    ~surface_c(void);

    // blit the complete surface s so that the lower left corner of x is at x, y
    // and nothing is drawn above the clip line
    void blit(const surface_c & s, int x, int y, int clip = 0);
    void blitBlock(const surface_c & s, int x, int y);
    void copy(const surface_c & src, int x, int y, int w, int h, int dx = 0, int dy = 0);
    void fillRect(int x, int y, int w, int h, int r, int g, int b, int a = SDL_ALPHA_OPAQUE);

    uint16_t getX(void) const { if (!video) return 0; else return video->w; }
    uint16_t getY(void) const { if (!video) return 0; else return video->h; }

    // render a given UFT-8 encoded text to the surface v into the given box
    // the text is automatically broken into lines and newlines within the
    // text are considered to be new paragraphs
    // depending on the alignment mode the text is either:
    //   put as paragraph text starting with the upper corner, no block alignment, left or right
    //   alignment depending on language
    //   centered within the box
    //
    // the text will not leave the assign box, if it is too big it will be clipped
    // the given text will be put through gettext for translation before it is displayed
    // the function behaves a bit like printf in that you can format values into the
    // string with additional parameters
    //
    // return value is the number of lines that were output
    unsigned int renderText(const fontParams_s * par, const std::string & t);


    // apply the gradient within the given area
    void gradient(int x, int y, int w, int h);

    friend class pngLoader_c;
};

class screen_c : public surface_c {

  private:

    int animationState;
    uint16_t blockX, blockY;

  public:

    // constructor, does nothing for the time being
    screen_c(const graphicsN_c & gr);
    ~screen_c(void);

    void flipComplete(void);  // flips the complete screen, not looking at the dirty blocks
    void flipDirty(const bitfield_c & dirty);     // updates only the dirty blocks
    bool flipAnimate(void);   // updates only the dirty blocks, but does that step by step resultin in an blending effetc return true, when done

    void toggleFullscreen(void);
};

// used for internationalization to mark strings to translate
const std::string _(const std::string & x);
const char * _(const char *x);


#endif

