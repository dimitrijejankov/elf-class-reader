//
// Created by dimitrije on 6/26/18.
//

#ifndef TESTINGCLASSSTUFF_CLASSREFLECTION_H
#define TESTINGCLASSSTUFF_CLASSREFLECTION_H

#include <memory>
#include "SymbolReader.h"

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

 private:

  /**
   * The info about the reflected class (methods and attributes)
   */
  classInfo clsInfo;

  /**
   * The shared library we loaded
   */
  void *sharedLibrary;
};

#endif //TESTINGCLASSSTUFF_CLASSREFLECTION_H
