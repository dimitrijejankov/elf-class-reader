#include <iostream>
#include "Reflection.h"
#include "MyFancyClass.h"

int main(int argc, char **argv) {

  std::string fileName(argv[1]);

  // create a new reflection class and load the .so
  Reflection reflection;
  reflection.load(fileName);

  // reflect the class
  auto clsReflection = reflection.getClassReflection("myFancyNamespace::OuterClass::MyFancyClass");

  return 0;
}
