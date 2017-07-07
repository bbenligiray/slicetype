#ifndef ENUMS_H
#define ENUMS_H


int SCREEN_SIZE = 920;
int SCREEN_SIZE_X = SCREEN_SIZE;
int SCREEN_SIZE_Y = SCREEN_SIZE;
const int SCREEN_FPS = 60;
const float radi0 = SCREEN_SIZE / 7.f;
const float radi1 = SCREEN_SIZE / 3.f;
const float radi2 = SCREEN_SIZE / 2.f;

const double circleBoundary = 1.f;
const double lineWidth = 2.f;
const int slices = 40;
const int loops = 10;

const float charScale = (0.225 / 640) * SCREEN_SIZE;
const float predScale = (0.15 / 640) * SCREEN_SIZE;
const float textWidth = 3.f;
const float textSmallWidth = 2.f;
float selectTime;
float morphTime;

int selectCount;
int morphCount;

const float textSpacing = 2.f;

enum colorTypes { cornflowerBlue, paleOrange, orange, black, green, purple, coral, warmgrey, olivedrab, goldenrod, turquoise };
enum lateralMergePhases { firstPhase, secondPhase, thirdPhase };
enum kbState { noSelect, select1, select2, selectOuter, selectOtherKey, selectingOtherKey, secondClear };

colorTypes colorFront = turquoise;
colorTypes colorBack = paleOrange;
colorTypes colorS1 = orange;
colorTypes colorS2 = green;



//the first two goes in the middle ring, a total of 26 chars for each screen
static const char firstScreenChars[] = { 'e', 't',
'a', 'o', 'i', 'n', 's', 'r', 'h', 'l', 'd', 'c', 'u', 'm',
'f', 'p', 'g', 'w', 'y', 'b', 'v', 'k', 'x', 'j', 'z', 'q' };

static const char secondScreenChars[] = { '.', ',',
'\'', '?', '!', ':', ';', '"', '$', '&', '/', '-', '%', '#',
'(', ')', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };



#endif