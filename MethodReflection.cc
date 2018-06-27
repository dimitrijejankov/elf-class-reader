//
// Created by dimitrije on 6/27/18.
//

#include <dlfcn.h>
#include "MethodReflection.h"


MethodReflection::MethodReflection(std::string methodName,
                                   const std::vector<methodInfo> &possibleMethods,
                                   void *sharedLibrary) : possibleMethods(possibleMethods),
                                                          sharedLibrary(sharedLibrary),
                                                          methodName(methodName){}

void MethodReflection::putPointer(void *address, std::string type) {

  // add the type
  stackTypes.emplace_back(type + "*");

  // convert the thing a char pointer
  auto addressValues = (char*) &address;

  // insert the thing to the stack
  stackBuffer.insert(stackBuffer.end(), addressValues, addressValues + sizeof(void*));
}

void MethodReflection::putInt(int value) {

  // add the type
  stackTypes.emplace_back("int");

  // convert the thing a char pointer
  auto addressValues = (char*) &value;

  // insert the thing to the stack
  stackBuffer.insert(stackBuffer.end(), addressValues, addressValues + sizeof(int));
}

void MethodReflection::putChar(char value) {

  // add the type
  stackTypes.emplace_back("char");

  // convert the thing a char pointer
  auto addressValues = (char*) &value;

  // insert the thing to the stack
  stackBuffer.insert(stackBuffer.end(), addressValues, addressValues + sizeof(char));
}

bool MethodReflection::execute() {

  // grab the method
  auto methodSymbol = possibleMethods.begin()->symbol;
  auto function = (void(*)(void*, int, char)) dlsym(this->sharedLibrary, methodSymbol.c_str());

  char buffer[1000];
  int64_t n = 1;

  //asm("subq $0xD, %rsp");

  //asm ("callq *%0"
  //     : "=r" (function));

  function(&buffer, 10, 'c');

  return true;
}
