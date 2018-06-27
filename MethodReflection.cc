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
//
//  int *a;
//  int *b;
//  int tmp0, tmp1;
//  int x, y;
//
//  __asm__ volatile (
//  "sub (%[a]), %esp\n\t" /* x = (*a) */
//  "%=:\n\t"
//
//  : [x] "=&r" (tmp0), [y] "=&r" (tmp1)
//  : [a] "r" (a), [b] "r" (b) : "memory" /* "cc" */ );

  return true;
}
