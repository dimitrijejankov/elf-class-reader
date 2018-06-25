# Elf Class Reader

The Elf Class Reader uses libdwarf to extract the information about a particular class from a .so library via the debugging information. The reader can extract method names with types an parameters and individual attributes of the class including their names, offsets and types.

To compile the test do the following :

```bash
# compile the code
cmake .
make fancyClass
make testingClassStuff

# run the code
./testingClassStuff path-to-fancyClass.so
```

Output : 
```
Name of the attribute fancyAttribute1 offset from the start 0 type signed char the size 1
Name of the attribute fancyAttribute2 offset from the start 8 type long int the size 8
Name of the attribute fancyAttribute3 offset from the start 16 type long int the size 8
Name of the attribute fancyAttribute4 offset from the start 24 type long int* the size 8
Name of the attribute tmp offset from the start 32 type qwery1 the size 16
returns ( type : void, size : 1) | method name : MyFancyClass | parameters :
	( type : MyFancyClass*, size : 8)
	( type : bool, size : 1)
returns ( type : int, size : 8) | method name : fancyMethod | parameters :
	( type : MyFancyClass*, size : 8)
	( type : char, size : 1)
	( type : int, size : 4)
```

Example code can be seen in main.cc
