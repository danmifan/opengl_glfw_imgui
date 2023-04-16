#include <iostream>

#include "window.h"

int main() {
  MyWindow window(1280, 720, 60);

  window.init();
  window.update();
  window.shutdown();
  return 0;
}