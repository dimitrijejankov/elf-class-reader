//
// Created by dimitrije on 6/24/18.
//

#include <iostream>
#include "MyFancyClass.h"

namespace myFancyNamespace {

int OuterClass::MyFancyClass::fancyMethod(char x, int y) {
  return 8;
}

OuterClass::MyFancyClass::MyFancyClass(bool ss) {
  std::cout << ss << std::endl;
}

}