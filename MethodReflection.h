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

  /**
   * Put a pointer of a particular type
   * @param address - the address
   * @param type - the type of the pointer
   */
  void putPointer(void *address, std::string type);

  /**
   * Put an integer value
   * @param value - the value
   */
  void putInt(int value);

  /**
   * The character value
   * @param value - the value
   */
  void putChar(char value);

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
   * The stack buffer
   */
  std::vector<int8_t> stackBuffer;

  /**
   * The list of types stored on the stack
   */
  std::vector<std::string> stackTypes;
};

#endif //TESTINGCLASSSTUFF_METHODREFLECTION_H
