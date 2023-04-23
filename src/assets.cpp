#include "assets.h"

Assets &Assets::get() {
  static Assets instance;
  return instance;
}