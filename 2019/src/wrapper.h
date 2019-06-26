#pragma once

#include <memory>
#include <vector>

#include "action.h"
#include "point.h"

class Game;

class Wrapper {
 public:
  Wrapper(Game& game, const Point& p, int index, int birth_time);

  void takeAction(const ActionCommand& cmd);
  void replayHistory(const ActionCommand& cmd);

  Game& game;
  Point pos;
  // Relative position of manipulators from the wrapper.
  std::vector<Point> manipulators;
  std::unique_ptr<ActionInfo> last_action;

  // Remained time durations for some boosters.
  int time_fast_wheel = 0;
  int time_drill = 0;

  const int index;
  const int birth_time;

 private:
  // Process actual action
  void doAction(const ActionCommand& cmd);
  // Process tasks that need to be done before actions.
  void beforeAction();
  // Process tasks that need to be done after actions.
  void afterAction();
  void moveAndPaint(const Point& p);
  void useBoosterAction(Action a);
};
