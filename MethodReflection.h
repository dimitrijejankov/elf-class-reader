//
// Created by dimitrije on 6/27/18.
//

#ifndef TESTINGCLASSSTUFF_METHODREFLECTION_H
#define TESTINGCLASSSTUFF_METHODREFLECTION_H

#include "SymbolReader.h"

class MethodReflection;
typedef std::shared_ptr<MethodReflection> MethodReflectionPtr;

class MethodReflection {

public:

  MethodReflection(std::string methodName, const std::vector<methodInfo> &possibleMethods, void *sharedLibrary);

  void putPointer(std::string type, void *value);

  void putInt(int value);

  bool execute();

 protected:

  /**
   * The possible methods
   */
  std::vector<methodInfo> possibleMethods;

  /**
   * The shared library we loaded
   */
  void *sharedLibrary;

  /**
   * The name of this method
   */
  std::string methodName;

  /**
   * The stack with all the attributes
   */
  std::vector<int8_t> stack;
};

#endif //TESTINGCLASSSTUFF_METHODREFLECTION_H
