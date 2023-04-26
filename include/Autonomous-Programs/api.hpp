/** @file Autonomous-Programs/api.hpp
  * @brief Autonomous Programs
  *
  * We use the following following programs
  * 
  * 1. Solo AWP (8 discs max)
  * 2. Left Side Support (7 discs max)
  * 3. Right Side Support (7 discs max)
  * 4. Skills Autonomous (270 point potential)
  * 
  * @author Logan Daugherty
*/

// Autonomous Selector Options
// These names are what display on the controller screen when checking the right slot
enum autoTitles{ SKILLS=1, SKILLS_2=2, AWP=3, AWP_2=4, LEFT_SIDE=5, LEFT_SIDE_AUTO_STACK=6, RIGHT_SIDE=7, RIGHT_SIDE_AUTO_STACK=8};
extern bool isRightSet;

void Awp(); // Solo AWP
void Awp_Auto_Stack(); // Solo AWP + 3 Stack on Auto Line
void Left_Side(); // Left Side Support
void Left_Side_Auto_Stack(); // Left Side Support + 3 Stack on Auto Line
void Right_Side(); // Right Side Support
void Right_Side_Auto_Stack(); // Right Side Support + 3 Stack on Auto Line
void Skills(); // Skills Autonomous
