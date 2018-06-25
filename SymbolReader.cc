//
// Created by dimitrije on 6/24/18.
//

#include "SymbolReader.h"
#include <err.h>
#include <fcntl.h>
#include <gelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <set>
#include <cxxabi.h>
#include <iostream>
#include <cassert>

SymbolReader::SymbolReader(std::string &fileName) {

  // set the file name
  this->fileName = fileName;

  // true if it is loaded
  this->isLoaded = false;
}

SymbolReader::~SymbolReader() {

  // free the loaded resources
  if(isLoaded) {
    dwarfRes = dwarf_finish(dbg,&error);
    close(fd);
  }
}

bool SymbolReader::load() {


  // open the file
  fd = open(fileName.c_str(), O_RDONLY);
  if(fd < 0) {
    printf("Failure attempting to open %s\n", fileName.c_str());
    return false;
  }

  // init the dwarf processing
  dwarfRes = dwarf_init(fd, DW_DLC_READ, errhand, errarg, &dbg,&error);
  if(dwarfRes != DW_DLV_OK) {

    //close the file
    close(fd);

    // we failed to initialize the dwarf
    printf("Giving up, cannot do DWARF processing\n");
    return false;
  }

  // we managed to load this
  isLoaded = true;

  return true;
}

classInfo SymbolReader::getClassInformation(const std::type_info &typeInfo) {

  // the return
  classInfo ret;

  // grab the real name
  std::string realName;
  if(!realTypeName(typeInfo, realName)) {
    return ret;
  }

  return analyzeFile(realName);
}

bool SymbolReader::realTypeName(const std::type_info& typeInfo, std::string &typeName) {

  // get the mangled name
  std::string mangledName = typeInfo.name();

  // demangle it
  int status = 0;
  const char *realName = abi::__cxa_demangle(mangledName.c_str(), nullptr, nullptr, &status);

  // did we succeed
  if(status != 0) {
    return false;
  }

  // set the real name as output
  typeName.assign(realName);

  // we succeeded
  return true;
}

classInfo SymbolReader::analyzeFile(std::string &realName) {

  // the return stuff
  classInfo ret;

  Dwarf_Unsigned cu_header_length = 0;
  Dwarf_Half version_stamp = 0;
  Dwarf_Unsigned abbrev_offset = 0;
  Dwarf_Half address_size = 0;
  Dwarf_Unsigned next_cu_header = 0;
  Dwarf_Error error;
  int cu_number = 0;

  for (;; ++cu_number) {
    Dwarf_Die no_die = nullptr;
    Dwarf_Die cu_die = nullptr;
    int res = DW_DLV_ERROR;
    res = dwarf_next_cu_header(dbg, &cu_header_length,
                               &version_stamp, &abbrev_offset, &address_size,
                               &next_cu_header, &error);

    if (res == DW_DLV_ERROR) {
      printf("Error in dwarf_next_cu_header\n");
      exit(1);
    }

    if (res == DW_DLV_NO_ENTRY) {
      return ret;
    }

    /* The CU will have a single sibling, a cu_die. */
    res = dwarf_siblingof(dbg, no_die, &cu_die, &error);
    if (res == DW_DLV_ERROR) {
      printf("Error in dwarf_siblingof on CU die \n");
      exit(1);
    }

    if (res == DW_DLV_NO_ENTRY) {
      /* Impossible case. */
      printf("no entry! in dwarf_siblingof on CU die \n");
      exit(1);
    }

    getDieAndSiblings(dbg, cu_die, 0, realName, ret);

    dwarf_dealloc(dbg, cu_die, DW_DLA_DIE);
  }
}

void SymbolReader::getDieAndSiblings(Dwarf_Debug dbg, Dwarf_Die in_die, int in_level, std::string &realName, classInfo &ret) {

  int res = DW_DLV_ERROR;
  Dwarf_Die cur_die = in_die;
  Dwarf_Die sib_die = in_die;
  Dwarf_Die child = nullptr;
  Dwarf_Error error;

  /* Who am I? */
  //print_die_data(dbg, in_die, in_level);
  if(isClassSymbol(dbg, in_die, realName)) {
    extractClassInfo(dbg, in_die, ret);
  }

  /* First son, if any */
  res = dwarf_child(cur_die, &child, &error);

  // traverse tree depth first
  if (res == DW_DLV_OK) {
    getDieAndSiblings(dbg, child, in_level + 1, realName, ret); /* recur on the first son */
    sib_die = child;
    while (res == DW_DLV_OK) {
      cur_die = sib_die;
      res = dwarf_siblingof(dbg, cur_die, &sib_die, &error);
      getDieAndSiblings(dbg, sib_die, in_level + 1, realName, ret); /* recur others */
    };
  }
}

bool SymbolReader::isClassSymbol(Dwarf_Debug dbg, Dwarf_Die print_me, std::string &realName) {

  // the return value
  bool ret = false;

  // check if this entry is the symbol of the class
  char *name = nullptr;
  int gotName = !dwarf_diename(print_me, &name, &error);

  // grab the tag
  Dwarf_Half tag = 0;
  int gotTagName = !dwarf_tag(print_me, &tag, &error);

  // if it does have a name
  if(gotName && gotTagName) {

    // is this our guy
    ret = std::string(name) == realName && tag == DW_TAG_class_type;
  }

  // deallocate the name
  dwarf_dealloc(dbg, name, DW_DLA_STRING);

  // return the value
  return ret;
}

bool SymbolReader::isAttributeSymbol(Dwarf_Die print_me) {

  // grab the tag
  Dwarf_Half tag = 0;
  int gotTagName = !dwarf_tag(print_me, &tag, &error);

  if(gotTagName) {
    return tag == DW_TAG_member;
  }

  return false;
}

bool SymbolReader::isMethodSymbol(Dwarf_Die print_me) {

  // grab the tag
  Dwarf_Half tag = 0;
  int gotTagName = !dwarf_tag(print_me, &tag, &error);

  if(gotTagName) {
    return tag == DW_TAG_subprogram;
  }

  return false;
}

/// TODO do better error handling
void SymbolReader::parseAttribute(Dwarf_Die cur_die, classInfo &ret) {

  char *name = nullptr;
  char *typeName = nullptr;
  Dwarf_Attribute attr;
  Dwarf_Unsigned offset;
  Dwarf_Off typeOffset;
  Dwarf_Die typeDie;

  // grab the offset of the attribute
  int gotOffset = !dwarf_attr(cur_die, DW_AT_data_member_location, &attr, &error) &&
                  !dwarf_formudata(attr, &offset, &error);

  // this has to hold
  assert(gotOffset);

  // check if this entry is the symbol of the class
  int gotName = !dwarf_attr(cur_die, DW_AT_name, &attr, &error) &&
                !dwarf_diename(cur_die, &name, &error);

  // this has to hold
  assert(gotName);

  // grab the offset to the type from the attribute
  int gotTypeOffset = !dwarf_attr(cur_die, DW_AT_type, &attr, &error) && !dwarf_global_formref(attr, &typeOffset, &error);

  // this has to hold
  assert(gotTypeOffset);

  // grab the type die
  int gotTypeDie = !dwarf_offdie_b(dbg, typeOffset, 1, &typeDie, &error);

  // this has to hold
  assert(gotTypeDie);

  // grab the name of the type
  int gotTypeName = !dwarf_attr(typeDie, DW_AT_name, &attr, &error) &&
                    !dwarf_diename(typeDie, &typeName, &error);

  // this has to hold
  assert(gotTypeName);

  size_t typeSize = getTypeSize(typeDie);

  // set the extracted information
  attributeInfo atInfo{};
  atInfo.name.assign(name);
  atInfo.size = typeSize;
  atInfo.type.assign(typeName);
  atInfo.offset = offset;

  // add the attribute
  ret.attributes->push_back(atInfo);

  // free the memory
  dwarf_dealloc(dbg, name, DW_DLA_STRING);
  dwarf_dealloc(dbg, typeName, DW_DLA_STRING);
}

size_t SymbolReader::getTypeSize(Dwarf_Die cur_die) {

  Dwarf_Attribute attr;
  Dwarf_Unsigned size;
  Dwarf_Off typeOffset;
  Dwarf_Die typeDie;

  int gotByteSize = !dwarf_attr(cur_die, DW_AT_byte_size, &attr, &error) &&
                    !dwarf_formudata(attr, &size, &error);

  // did we manage to get the byte size
  if(gotByteSize) {
    return size;
  }

  // if not try to see if this is a typedef and try to follow
  int gotTypeOffset = !dwarf_attr(cur_die, DW_AT_type, &attr, &error) && !dwarf_global_formref(attr, &typeOffset, &error);

  // grab the type die
  int gotTypeDie = !dwarf_offdie_b(dbg, typeOffset, 1, &typeDie, &error);

  // make sure this never fails
  assert(gotTypeOffset && gotTypeDie);

  // recurse
  return getTypeSize(typeDie);
}

void SymbolReader::extractClassInfo(Dwarf_Debug dbg, Dwarf_Die in_die, classInfo &ret) {

  Dwarf_Die child = nullptr;
  Dwarf_Die sib_die = nullptr;
  Dwarf_Die cur_die = in_die;

  /* First son, if any */
  auto res = dwarf_child(cur_die, &child, &error);

  // traverse tree depth first
  if (res == DW_DLV_OK) {

    sib_die = child;
    while (res == DW_DLV_OK) {
      cur_die = sib_die;

      if(isAttributeSymbol(cur_die)){
        parseAttribute(cur_die, ret);
      }
      else if(isMethodSymbol(cur_die)) {
        //std::cout << "got methods" << std::endl;
      }

      res = dwarf_siblingof(dbg, cur_die, &sib_die, &error);
    };
  }
}