# Geo Programming Language

Geo is a simple, dynamically-typed programming language written in C++—inspired by Robert Nystrom's Crafting Interpreters. It is basically the book's version of lox, but with some extra features.  Geo uses a recursive descent parser to proceshttps://github.com/jbt/markdown-editors code.

Please note that the language is being actively worked on, and many things, including syntax are subject to change.

## Features

#### Dynamic Typing
Variables in Geo can hold values of any type and may change type over their lifetime.
```
var i = 1;
i = "hello";
```

#### Arithmetic Operations
Geo supports basic arithmetic operators (+, -, *, /, %).
```
var sum = 10 + 20;
var product = 5 * 4;
print(sum);
print(product);
```

#### Unary Operators
Geo featues unary operators like negation (-), logical NOT (!), and increment/decrement (++, --).
```
var a = 5;
a++;                // Postfix increment: increases a by 1
print(a);           // prints 6
var flag = true;
print(!flag);        // prints false
```
#### Binary Operators & Comparisons
Geo performs type-checking for binary operations and supports comparisons ( >, <, >=, <=, ==, and != ).
```
print 10 > 5;          // prints true
print "foo" == "bar";  // prints false
print(null == null);   // prints true
```
#### Ternary Operator
A compact conditional operator is available to choose between two expressions.
```
var a = 10;
var b = 20;
print (a > b) ? a : b;  // prints 20
```
#### Control Flow Statements
Geo supports control flows with if/else, while, and for loops.
If/Else Example:
```
if (x > 0) {
    print "x is positive";
} else {
    print "x is non-positive";
}

While Loop Example:

var i = 0;
while (i < 5) {
    print i;
    i = i + 1;
}

For Loop Example:

for (var i = 0; i < 5; i = i + 1) {
    print i;
}
```
#### Block Scoping
Blocks create new scopes, ensuring that variables declared inside do not leak to the outside.
```
{
    var temp = "inside block";
    print temp;
}
// temp is not available here
```
#### Loop Control
Use break to exit loops early and continue to skip to the next iteration.
Use continue to continue to the next iteration of the loop early.
```
for (var i = 0; i < 5; ++i) {
    if (i == 2) {
        continue;
    }

    if (i == 4) {
        break;
    }
    print i;
    // prints 0 1 3
}
```

## Installation

#### Dependencies
The cmake --preset command relies on the VCPKG_ROOT environment variable. You must have vcpkg installed on your system, and VCPKG_ROOT must be set to point to its install directory.

[vcpkg github](https://github.com/microsoft/vcpkg)

#### Steps
To build one of these targets, from the Geo root folder, run:

Clone the repository and compile the interpreter using your favorite C++ compiler. For example:
```
git clone https://github.com/JumiElazul/geo.git
cd geo
```

Geo uses a CMakePresets.json file with the following targets:
```
linux-debug
linux-release
windows-debug
windows-release
```

To build one of these targets, when in the Geo root folder, run:
```
cmake --preset <target>

// Example, to build debug for linux:
cmake --preset linux-debug
```

This will configure CMake and gather dependencies for Geo.  Then, to build:
```
cmake --build out/build/<target>

// Example for building debug for linux:
cmake --build out/build/linux-debug
```

## Usage

Run the interpreter from the command line and pass in a Geo source file:

./geo path/to/your/file.geo

Or, if you are developing, you can run the REPL:

./geo

## TODO

- [ ] Implement user-defined functions.
- [ ] Add support for arrays, dictionaries, and other collection types.
- [ ] Improve error messages across the lexer/parser/interpreter with more context and a stack trace.
- [ ] Develop a module/import system to allow code organization across multiple files.
- [ ] Investigate optimizations such as bytecode compilation or other parsing techniques.

## License

Distributed under the MIT License. See LICENSE for more information.
