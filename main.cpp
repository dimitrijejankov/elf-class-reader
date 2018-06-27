#include <iostream>
#include "SymbolReader.h"
#include "MyFancyClass.h"

int main(int argc, char **argv) {

  std::string fileName(argv[1]);
  SymbolReader reader(fileName);

  reader.load();

  //const std::type_info& s = typeid(myFancyNamespace::MyFancyClass);
  std::string s = "myFancyNamespace::OuterClass::MyFancyClass";
  auto clInfo = reader.getClassInformation(s);

  for(auto &it : *clInfo.attributes) {
    std::cout << "Name of the attribute " << it.name << " offset from the start " <<  it.offset << " type " << it.type << " the size " << it.size << std::endl;
  }

  for(auto &it : *clInfo.methods) {
    std::cout << "returns ( type : " << it.returnType.name << ", size : " << it.returnType.size << ")" << " | method name : " << it.name << " | parameters :" << std::endl;

    for(auto &jt : it.parameters) {
      std::cout << "\t( type : " << jt.name  << ", size : " << jt.size << ")" << std::endl;
    }
  }

  return 0;
}
