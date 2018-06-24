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

SymbolReader::SymbolReader(std::string &fileName) {

  // set the file name
  this->fileName = fileName;

  // true if it is loaded
  this->isLoaded = false;
}

SymbolReader::~SymbolReader() {

  if(isLoaded) {
    (void) elf_end(e);
    (void) close(fd);
  }
}

bool SymbolReader::load() {

  // init the elf library
  if (elf_version(EV_CURRENT) == EV_NONE) {
    printf("ELF library initialization failed: %s", elf_errmsg(-1));
    return false;
  }

  // open the file
  if ((fd = open(fileName.c_str(), O_RDONLY, 0)) < 0)  {
    printf("open %s failed", fileName.c_str());
    return false;
  }

  // begin reading the elf
  if ((e = elf_begin(fd, ELF_C_READ, nullptr)) == nullptr) {
    printf("elf_begin() failed: %s.", elf_errmsg(-1));

    // close the file
    (void) close(fd);

    // we failed :(
    return false;
  }

  // check if this is an elf object
  if (elf_kind(e) != ELF_K_ELF) {
    printf("%s is not an ELF object.", fileName.c_str());
    return false;
  }

  // get the index with all the section names
  if (elf_getshdrstrndx(e, &shstrndx) != 0) {
    printf("elf_getshdrstrndx() failed: %s.", elf_errmsg(-1));
  }

  // read in all the section
  scn = nullptr;
  while ((scn = elf_nextscn(e, scn)) != nullptr) {

    if (gelf_getshdr(scn, &shdr) != &shdr) {
      printf("getshdr() failed: %s.", elf_errmsg(-1));
      return false;
    }

    if ((name = elf_strptr(e, shstrndx, shdr.sh_name)) == nullptr) {
      printf("elf_strptr() failed: %s.", elf_errmsg(-1));
      return false;
    }

    // insert all the sections
    sections.insert(name);
  }

  // we managed to load this elf file and the sections
  isLoaded = true;
  return true;
}

