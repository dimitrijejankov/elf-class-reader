//
// Created by dimitrije on 6/26/18.
//

#ifndef TESTINGCLASSSTUFF_REFLECTION_H
#define TESTINGCLASSSTUFF_REFLECTION_H

#include "SymbolReader.h"
#include "ClassReflection.h"

class Reflection {
public:

  /**
   * The constructor for the reflection
   */
  explicit Reflection();

  /**
   * Loads the specified file
   * @param fileName - the path to the .so library
   * @return true if we can load the library
   */
  bool load(std::string fileName);

  /**
   *
   * @param className
   * @return
   */
  ClassReflectionPtr getClassReflection(std::string className);

  /**
   *
   * @param typeInfo
   * @return
   */
  ClassReflectionPtr getClassReflection(const std::type_info &typeInfo);

protected:

  /**
   * This thing reads the symbols
   */
  SymbolReader reader;

  /**
   * The shared library we loaded
   */
  void *sharedLibrary;

  /**
   * Is the reflection loaded
   */
  bool isLoaded;

  /**
   * The file name of the .so library
   */
   std::string fileName;
};

#endif //TESTINGCLASSSTUFF_REFLECTION_H
