Image processor built in c++ that is used through the command line. The processor takes in image.tga files as input and 
outputs image.tga files. Some example input and output .tga files have been provided in the folders input and output.

The following methods can be called on the input images:

method name   |   needed variable(s)   |    purpose
- combine     |         2 images       |   combines input image's pixels' red values with 2nd image's green values and 3rd image's blue values
- screen      |          image         |   combines input image's color with 2nd image's color
- multiply    |          image         |   multiplies input image's color with 2nd image's color
- subtract    |          image         |   subtracts 2nd image's color from input image's color
- overlay     |          image         |   adds input image's color with 2nd image's color
- addred      |           int          |   adds [int] to red rgb values
- addgreen    |           int          |   adds [int] to green rgb values
- addblue     |           int          |   adds [int] to blue rgb values
- scalered    |           int          |   scales red rgb values up by [int]
- scalegreen  |           int          |   scales green rgb values up by [int]
- scaleblue   |           int          |   scales blue rgb values up by [int]
- onlyred     |           N/A          |   removes blue and green colors
- onlygreen   |           N/A          |   removes blue and red colors
- onlyblue    |           N/A          |   removes red and green colors
- flip        |           N/A          |   rotates input image 180 degrees


Instructions:
1.) clone repo

2.) navigate to repository in the command line

3.) Compile the program into an executable with GNU Compiler Collection with the following command:
    g++ --std=c++11 -o executablename.exe src/main.cpp
    
4.) run program with:
    ./executablename.exe output/result.tga input/firstimage.tga [method] [variable(s)] [method] [variable(s)] etc...
    
    -  result.tga     is the name of the resulting image.tga 
    -  firstimage.tga is the first image acted upon
    -  [method]       is one of the method names from the table above
    -  [variable(s)]  can be an integer, another image.tga, or 3 images based on the required inputs in the table above
    -  variables come directly after the cooresponding method
    -  (You can subsequently call as many methods at once as you want)

