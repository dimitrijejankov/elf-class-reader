//
// Created by dimitrije on 6/26/18.
//

#include <dlfcn.h>
#include "Reflection.h"

Reflection::Reflection() {
  this->isLoaded = false;
  this->sharedLibrary = nullptr;
}

bool Reflection::load(std::string fileName) {

  // load the debugging info
  auto didLoad = reader.load(fileName);

  // did we fail loading them
  if(!didLoad) {
    return false;
  }

  // load the .so
  this->sharedLibrary = dlopen(fileName.c_str(), RTLD_NOW | RTLD_GLOBAL);

  // did we succeed
  if(this->sharedLibrary != nullptr) {
    this->isLoaded = true;
  }

  // return the success or failure
  return this->isLoaded;
}

ClassReflectionPtr Reflection::getClassReflection(std::string className) {
  return std::make_shared<ClassReflection>(this->reader.getClassInformation(className), this->sharedLibrary);
}
 
ClassReflectionPtr Reflection::getClassReflection(const std::type_info &typeInfo) {
  return std::make_shared<ClassReflection>(this->reader.getClassInformation(typeInfo), this->sharedLibrary);
}
