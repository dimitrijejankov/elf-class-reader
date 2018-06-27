#include <iostream>
#include "Reflection.h"
#include "MyFancyClass.h"

int main(int argc, char **argv) {

  std::string fileName(argv[1]);

  // create a new reflection class and load the .so
  Reflection reflection;
  reflection.load(fileName);

  // reflect the class
  auto clsReflection = reflection.getClassReflection("myFancyNamespace::MyFancyClass");

  // grab the method
  auto fancyMethod = clsReflection->getMethod("fancyMethod");

  char dummyObj[1000];
  fancyMethod->putPointer(dummyObj, "MyFancyClass");
  fancyMethod->putChar('s');
  fancyMethod->putInt(999);

  // execute the constructor
  fancyMethod->execute();

  return 0;
}
