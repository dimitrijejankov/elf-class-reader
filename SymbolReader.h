//
// Created by dimitrije on 6/24/18.
//

#ifndef TESTINGCLASSSTUFF_SYMBOLREADER_H
#define TESTINGCLASSSTUFF_SYMBOLREADER_H

#include <libelf.h>
#include <gelf.h>
#include <string>
#include <set>
#include <libdwarf/dwarf.h>
#include <libdwarf/libdwarf.h>
#include <vector>
#include <memory>

struct attributeInfo {

  /**
   * The name of the attribute
   */
  std::string name;

  /**
   * The size of the attribute
   */
  size_t size;

  /**
   * The offset of the attribute
   */
  size_t offset;

  /**
   * The type of the attribute
   */
  std::string type;
};

struct classInfo {

  /**
   * Define the default constructor
   */
  classInfo() {
    attributes = std::make_shared<std::vector<attributeInfo>>();
  }

  /**
   * The attributes of the class
   */
  std::shared_ptr<std::vector<attributeInfo>> attributes;

};

class SymbolReader {

 public:

  explicit SymbolReader(std::string &fileName);

  virtual ~SymbolReader();

  /**
   * Loads the file returns false if it fails or true if it can be loaded
   * @return true if it succeed false otherwise
   */
  bool load();

  /**
   *
   * @param typeInfo
   * @return
   */
  classInfo getClassInformation(const std::type_info &typeInfo);

 private:

  /**
   * Is this loaded
   */
  bool isLoaded;

  /**
   * The name of file we are opening
   */
  std::string fileName;

  /**
   * The file table index of the file we need to load
   */
  int fd = -1;

  /**
   * The dwarf resource
   */
  int dwarfRes = DW_DLV_ERROR;

  Dwarf_Debug dbg = nullptr;
  Dwarf_Error error;
  Dwarf_Handler errhand = nullptr;
  Dwarf_Ptr errarg = nullptr;

  bool realTypeName(const std::type_info& typeInfo, std::string &typeName);

  classInfo analyzeFile(std::string &realName);

  void getDieAndSiblings(Dwarf_Debug dbg, Dwarf_Die in_die, int in_level, std::string &realName, classInfo &ret);

  bool isClassSymbol(Dwarf_Debug dbg, Dwarf_Die print_me, std::string &realName);

  bool isAttributeSymbol(Dwarf_Die print_me);

  bool isMethodSymbol(Dwarf_Die print_me);

  void parseAttribute(Dwarf_Die cur_die, classInfo &ret);

  std::pair<std::string, size_t> getType(Dwarf_Die cur_die, std::string &previousName, bool isPointer);

  void print_die_data(Dwarf_Debug dbg, Dwarf_Die print_me, int level);

  void extractClassInfo(Dwarf_Debug dbg, Dwarf_Die in_die, classInfo &ret);
  void parseMethod(Dwarf_Die curDie, classInfo &info);
};

#endif //TESTINGCLASSSTUFF_SYMBOLREADER_H
