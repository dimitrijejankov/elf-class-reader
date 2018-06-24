//
// Created by dimitrije on 6/24/18.
//

#ifndef TESTINGCLASSSTUFF_SYMBOLREADER_H
#define TESTINGCLASSSTUFF_SYMBOLREADER_H

#include <libelf.h>
#include <gelf.h>
#include <string>
#include <set>

class SymbolReader {

 public:

  explicit SymbolReader(std::string &fileName);

  virtual ~SymbolReader();

  /**
   * Loads the file returns false if it fails or true if it can be loaded
   * @return true if it succeed false otherwise
   */
  bool load();

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
   * List of all sections in the elf file
   */
  std::set<std::string> sections;

  int fd;
  Elf *e;
  char *name, *p, pc[4 * sizeof(char)];
  Elf_Scn *scn;
  Elf_Data *data;
  GElf_Shdr shdr;
  size_t n, shstrndx, sz;

};

#endif //TESTINGCLASSSTUFF_SYMBOLREADER_H
