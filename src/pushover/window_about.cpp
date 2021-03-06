#include "window.h"

#include "levelset.h"
#include "colors.h"
#include "screen.h"
#include "graphicsn.h"

class aboutWindow_c : public window_c {

  public:

    aboutWindow_c(surface_c & s, graphicsN_c & g, const levelsetList_c & ls);
    bool handleEvent(const SDL_Event & event);
};

aboutWindow_c::aboutWindow_c(surface_c & s, graphicsN_c & g, const levelsetList_c & ls) : window_c(2, 0, 16, 12, s, g) {

  clearInside();

  fontParams_s par;

  par.font = FNT_BIG;
  par.alignment = ALN_CENTER;
  par.color.r = TXT_COL_R; par.color.g = TXT_COL_G; par.color.b = TXT_COL_B;
  par.shadow = 2;
  par.box.x = gr.blockX()*(X()+1);
  par.box.y = gr.blockY()*(Y()+1);
  par.box.w = gr.blockX()*(W()-2);
  par.box.h = getFontHeight(FNT_BIG);

  {
    char title[200];
    //TRANSLATORS: the %s is a placeholder for the version
    snprintf(title, 200, _("Pushover - %s - About"), VERSION);
    surf.renderText(&par, title);
  }

  int ypos = gr.blockY()*(Y()+1) + getFontHeight(FNT_BIG);

  surf.fillRect(gr.blockX()*(X()+1)+1, ypos+1, gr.blockX()*(W()-2), 2, 0, 0, 0);
  surf.fillRect(gr.blockX()*(X()+1), ypos, gr.blockX()*(W()-2), 2, TXT_COL_R, TXT_COL_G, TXT_COL_B);
  ypos += 20;

  unsigned int lineH = getFontHeight(FNT_SMALL);  // height of one entry line

  par.font = FNT_SMALL;
  par.alignment = ALN_TEXT;
  par.box.x = gr.blockX()*(X()+1);
  par.box.y = ypos;
  par.box.w = gr.blockX()*(W()-2);
  par.box.h = lineH;

  int sk = rightToLeft() ? 0 : 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("Original Concept:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Chas Partington"); par.box.x -= sk; par.box.w += 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("Original Programming:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Dave Elcock, Helen Elcock, Keith Watterson"); par.box.x -= sk; par.box.w += 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("Original Graphics:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Bryan King, Barry Armstrong"); par.box.x -= sk; par.box.w += 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("Original Music & SFX:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Keith Tinman, Dean Evans, Johnathan Dunn"); par.box.x -= sk; par.box.w += 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("New Programming:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Andreas Röver, Volker Diels-Grabsch"); par.box.x -= sk; par.box.w += 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("New Music:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Roberto Lorenz"); par.box.x -= sk; par.box.w += 30;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("New Graphics:")); par.box.x += sk; par.box.w -= 30;
  par.shadow = 0; par.box.y += surf.renderText(&par, "Harald Radke"); par.box.x -= sk; par.box.w += 30;

  par.box.y += 5;

  par.shadow = 2; par.box.y += surf.renderText(&par, _("Levels:")); par.box.x += sk; par.box.w -= 30;

  for (size_t i = 0; i < ls.getLevelsetNames().size(); i++)
  {
    const std::string name = ls.getLevelsetNames()[i];
    par.shadow = 0;
    par.box.y += surf.renderText(&par, _(name) + _(": ") + collectAuthors(ls.getLevelset(name)));
  }
}


bool aboutWindow_c::handleEvent(const SDL_Event & event) {
  if (event.type == SDL_KEYDOWN &&
      event.key.keysym.sym == SDLK_ESCAPE)
  {
    done = true;
    return true;
  }

  return false;
}

window_c * getAboutWindow(surface_c & surf, graphicsN_c & gr, const levelsetList_c & ls) {
    return new aboutWindow_c(surf, gr, ls);
}

