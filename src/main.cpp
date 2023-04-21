#include <iostream>

#include "window.h"

int main() {
  MyWindow window(1600, 900, 144);

  window.init();
  window.update();
  window.shutdown();
  return 0;
}