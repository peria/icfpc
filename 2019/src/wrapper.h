#pragma once

#include <memory>
#include <vector>

#include "action.h"
#include "point.h"

class Game;

class Wrapper {
 public:
  Wrapper(Game& game, const Point& p, int index, int birth_time);

  void takeAction(const ActionCommand* cmd);
  // Returns false if |time| needs access of unknown history.
  bool replayAction(int time);

  // Initializes everything other than history.
  void reset(const Point& init_pos);

  Game& game;
  Point pos;

  int getTime() const { return birth_time + history.size(); }

  std::string getCommand() const;

  // Relative position of manipulators from the wrapper.
  std::vector<Point> manipulators;
  std::vector<std::unique_ptr<const ActionCommand>> history;
  std::unique_ptr<ActionInfo> last_action;

  // Remained time durations for some boosters.
  int time_fast_wheel = 0;
  int time_drill = 0;

  const int index;

 private:
  // Process actual action
  void doAction(const ActionCommand& cmd);
  // Process tasks that need to be done before actions.
  void beforeAction();
  // Process tasks that need to be done after actions.
  void afterAction();
  void moveAndPaint(const Point& p);
  void useBoosterAction(Action a);

  const int birth_time;
};
