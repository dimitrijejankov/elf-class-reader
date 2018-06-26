//
// Created by dimitrije on 6/24/18.
//

#ifndef TESTINGCLASSSTUFF_MYFANCYCLASS_H
#define TESTINGCLASSSTUFF_MYFANCYCLASS_H

class qwery1 {
  size_t x;
  size_t y;
};

class MyFancyClass {

public:

  explicit MyFancyClass(bool ss);

  int8_t fancyAttribute1;
  int64_t fancyAttribute2;
  int64_t fancyAttribute3;
  int64_t** fancyAttribute4;
  qwery1 tmp;

  int fancyMethod(char x, int y);

};

#endif //TESTINGCLASSSTUFF_MYFANCYCLASS_H
