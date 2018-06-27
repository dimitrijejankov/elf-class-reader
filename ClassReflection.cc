//
// Created by dimitrije on 6/26/18.
//

#include "ClassReflection.h"

ClassReflection::ClassReflection(const classInfo &clsInfo, void *sharedLibrary) : sharedLibrary(sharedLibrary),
                                                                                  className(clsInfo.className),
                                                                                  attributes(clsInfo.attributes) {

    // go through each method
    for(auto &method : *clsInfo.methods) {

      // if we don't have the vector add it
      this->methods.try_emplace(method.name, std::vector<methodInfo>());

      // add the methods
      this->methods[method.name].push_back(method);
    }
}

MethodReflectionPtr ClassReflection::getMethod(std::string methodName) {

  if(this->methods.find(methodName) == this->methods.end()) {
    return nullptr;
  }

  return std::make_shared<MethodReflection>(methodName, methods[methodName], this->sharedLibrary);
}
