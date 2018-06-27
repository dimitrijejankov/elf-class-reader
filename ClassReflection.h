//
// Created by dimitrije on 6/26/18.
//

#ifndef TESTINGCLASSSTUFF_CLASSREFLECTION_H
#define TESTINGCLASSSTUFF_CLASSREFLECTION_H

#include <memory>
#include <map>
#include "SymbolReader.h"
#include "MethodReflection.h"

class ClassReflection;
typedef std::shared_ptr<ClassReflection> ClassReflectionPtr;

class ClassReflection {
public:

  /**
   * Creates a new reflection class
   * @param clsInfo
   * @param sharedLibrary
   */
  ClassReflection(const classInfo &clsInfo, void *sharedLibrary);

  /**
   * Grab the method with the name
   * @param methodName - the method name
   * @return the reflected method
   */
  MethodReflectionPtr getMethod(std::string methodName);

private:

  /**
   * The class name with the full namespace specification
   */
  std::string className;

  /**
   * The attributes of the class
   */
  std::shared_ptr<std::vector<attributeInfo>> attributes;

  /**
   * Maps the name of the method to the
   */
  std::map<std::string, std::vector<methodInfo>> methods;

  /**
   * The shared library we loaded
   */
  void *sharedLibrary;
};

#endif //TESTINGCLASSSTUFF_CLASSREFLECTION_H
