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

void MethodReflection::putPointer(std::string type, void *value) {

}

void MethodReflection::putInt(int value) {

}

bool MethodReflection::execute() {

  // grab the method
  auto methodSymbol = possibleMethods.begin()->symbol;
  auto function = (void(*)()) dlsym(this->sharedLibrary, methodSymbol.c_str());

  char buffer[1000];
  int64_t n = 13;

  asm ("subq %0, %%rsp \n\t" : "+r" (n)::"rax");
  function();

  return true;
}
