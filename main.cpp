#include <sys/types.h> /* For open() */
#include <sys/stat.h>  /* For open() */
#include <fcntl.h>     /* For open() */
#include <stdlib.h>     /* For exit() */
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include "SymbolReader.h"
#include "MyFancyClass.h"

int main(int argc, char **argv) {

  std::string fileName(argv[1]);
  SymbolReader reader(fileName);

  reader.load();

  const std::type_info& s = typeid(MyFancyClass);
  auto x = reader.getClassInformation(s);

  for(auto &it : *x.attributes) {
    std::cout << "Name of the attribute " << it.name << " offset from the start " <<  it.offset << " type " << it.type << " the size " << it.size << std::endl;
  }

  return 0;
}
