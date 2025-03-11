# cpp-lox

cpp-lox is a dynamically-typed programming language written in C++, translated from java to cpp while following along with Robert Nystrom's Crafting Interpreters. 

## Features

#### Dynamic Typing
Variables in cpp-lox can hold values of any type and may change type over their lifetime.
```
var i = 1;
i = "hello";
```

#### Arithmetic Operations
cpp-lox supports basic arithmetic operators (+, -, *, /, %).
```
var sum = 10 + 20;
var product = 5 * 4;
print(sum);
print(product);
```

#### Unary Operators
cpp-lox featues unary operators like negation (-), logical NOT (!), and increment/decrement (++, --).
```
var a = 5;
a++;                // Postfix increment: increases a by 1
print(a);           // prints 6
var flag = true;
print(!flag);       // prints false
```
#### Binary Operators & Comparisons
cpp-lox performs type-checking for binary operations and supports comparisons ( >, <, >=, <=, ==, and != ).
```
print(10 > 5);          // prints true
print("foo" == "bar");  // prints false
print(null == null);    // prints true
```
#### Control Flow Statements
cpp-lox supports control flows with if/else, while, and for loops.
```
// if/else example:
if (x > 0) {
    print("x is positive");
} else {
    print("x is non-positive");
}

// while loop example:
var i = 0;
while (i < 5) {
    print(i);
    i = i + 1;
}

// for loop example:
for (var i = 0; i < 5; i = i + 1) {
    print(i);
}
```
#### Block Scoping
Blocks create new scopes, ensuring that variables declared inside do not leak to the outside.
```
{
    var temp = "inside block";
    print(temp);
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
    print(i);
    // prints 0 1 3
}
```

#### Built in functions
Currently there are not many built in functions, but more will be added as the language is developed.
```
print("Hello, World!");  // prints "Hello, World!"
var my_str = input();    // waits for user input and stores it in my_str
clock();                 // prints the current system time since epoch in seconds

```

#### Classes
cpp-lox supports classes.
```
class math
{
    add(a, b)
    {
        return a + b;
    }

    sub(a, b)
    {
        return a - b;
    }
}

var i = math();
var result0 = i.add(5, 3);
var result1 = i.sub(5, 3);
print(result0);  // prints 8
print(result1);  // prints 2
```

They can also have static methods, where an instance is not required.
```
class math
{
    static square(a)
    {
        return a * a;
    }
}

var sqr = math.square(5);
print(sqr);  // prints 25
```

Single inheritance is supported as well!
```
class doughnut
{
    cook()
    {
        print("Fry until golden brown");
    }
}

class boston_cream < doughnut
{
    cook()
    {
        super.cook();
        print("Pipe full of custard and coat with chocolate");
    }
}

doughnut().cook();     // prints "Fry until golden brown";
boston_cream().cook(); // prints "Fry until golden brown"
                                 "Pipe full of custard and coat with chocolate"
``````


## Installation

#### Dependencies
The cmake --preset command relies on the VCPKG_ROOT environment variable. You must have vcpkg installed on your system, and VCPKG_ROOT must be set to point to its install directory.

[vcpkg github](https://github.com/microsoft/vcpkg)

#### Steps
Clone the repository and compile the interpreter using your favorite C++ compiler. For example:
```
git clone https://github.com/JumiElazul/cpp-lox.git
cd cpp-lox
```

cpp-lox uses a CMakePresets.json file with the following targets:
```
linux-debug
linux-release
windows-debug
windows-release
```

To build one of these targets, when in the cpp-lox root folder, run:
```
cmake --preset <target>

// Example, to build debug for linux:
cmake --preset linux-debug
```

This will configure CMake and gather dependencies for cpp-lox.  Then, to build:
```
cmake --build out/build/<target>

// Example for building debug for linux:
cmake --build out/build/linux-debug
```

## Usage

Run the interpreter from the command line and pass in a cpp-lox source file:

```
./cpp-lox path/to/your/file.cpplox
```

Or, if you are developing, you can run the REPL:

```
./cpp-lox
```

## TODO

- [ ] Implement user-defined functions.
- [ ] Vastly improve testing.
- [ ] Add support for arrays, dictionaries, and other collection types.
- [ ] Improve error messages across the lexer/parser/interpreter with more context and a stack trace.
- [ ] Develop a module/import system to allow code organization across multiple files.
- [ ] Investigate optimizations such as bytecode compilation or other parsing techniques.

## License

Distributed under the MIT License. See LICENSE for more information.
