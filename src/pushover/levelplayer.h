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

#ifndef __LEVEL_PLAYER_H__
#define __LEVEL_PLAYER_H__

#include "leveldata.h"

class levelPlayer_c : public levelData_c {

  private:

    // requested states for the 2 doors
    bool openDoorExit;
    bool openDoorEntry;

    // returns true, if there are still counter stoppers
    // of num or bigger (num = DominoTypeCounter1, 2 or 3) that are not falling
    bool CounterStopper(DominoType num);

    // calls the different states of the dominoes
    void callStateFunction(int type, int state, int x, int y);

    void DominoCrash(int x, int y, int type, int extra);

    void DTA(int num, int x, int y);
    void DTA_1(int x, int y);
    void DTA_2(int x, int y);
    void DTA_3(int x, int y);
    void DTA_4(int x, int y);
    void DTA_E(int x, int y);
    void DTA_I(int x, int y);
    void DTA_J(int x, int y);
    void DTA_K(int x, int y);
    void DTA_F(int x, int y);
    void DTA_C(int x, int y);
    void DTA_5(int x, int y);
    void DTA_G(int x, int y);
    void DTA_6(int x, int y);
    void DTA_L(int x, int y);
    void DTA_7(int x, int y);
    void DTA_M(int x, int y);
    void DTA_P(int x, int y);
    void DTA_8(int x, int y);
    void DTA_9(int x, int y);
    void DTA_N(int x, int y);
    void DTA_A(int x, int y);
    void DTA_H(int x, int y);
    void DTA_O(int x, int y);

    // a counter used to see, if nothing happens within the level
    int inactive;

    bool triggerFalln;

  public:

    levelPlayer_c(void) : inactive(0) {}

    void load(const textsections_c & sections, const std::string & userString);

    /* opens and closes doors */
    void performDoors(void);
    // allows you to open and close the 2 doors
    void openEntryDoor(bool open) { openDoorEntry = open; }
    void openExitDoor(bool open) { openDoorExit = open; }

    void performDominoes(void);

    DominoType pickUpDomino(int x, int y);  // removes the domino from that position and returns the domino type
    void putDownDomino(int x, int y, DominoType domino, bool pushin);
    void fallingDomino(int x, int y);

    bool pushDomino(int x, int y, int dir);

    bool levelInactive(void) { return inactive > 3; }
    bool levelLongInactive(void) { return inactive > 36; }

    // returns true, when the trigger is not standing but also not flat on the ground
    bool triggerNotFlat(void) const;



    bool dominoesFalln(void) const; // returns true when all dominoes are falln far enough
    bool dominoesStanding(void) const; // returns true, when there is at least one domino still standing
    bool rubblePile(void) const;  // there is at least one pile of rubbish on the board
    bool triggerIsFalln(void) const;
};

#endif

