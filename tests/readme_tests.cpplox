// Dynamic typing
var i = 1;
i = "hello";
print(i);

// Arithmetic Operators
var sum = 10 + 20;
var product = 5 * 4;
print(sum);
print(product);

// Unary operators
var a = 5;
a++;
print(a);
var flag = true;
print(!flag);

// Binary Operators & Comparisons
print(10 > 5);
print("foo" == "bar");
print(null == null);

// Control Flow Statements
var x = 1;
if (x > 0) 
{
    print("x is positive");
}
else 
{
    print("x is non-positive");
}

i = 0;
while (i < 5) 
{
    print(i);
    i = i + 1;
}

for (var i = 0; i < 5; i = i + 1) 
{
    print(i);
}

// Block Scoping
{
    var temp = "inside block";
    print(temp);
}

// Loop Control
i = 10;
for (var i = 0; i < 5; ++i) 
{
    if (i == 2) 
    {
        continue;
    }

    if (i == 4) 
    {
        break;
    }
    print(i);
}

// Built in functions
print("This is the built in print!");
var get_str = input();
print(get_str);
var time = clock();
print("The clock time was: " + time);

// Classes
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

    static square(a)
    {
        return a * a;
    }
}

var math_class = math();
var result0 = math_class.add(5, 3);
var result1 = math_class.sub(5, 3);
print("Math class result0: " + result0);
print("Math class result1: " + result1);

// Static methods
var sqr = math_class.square(5);
print("math.square(5) results in: " + sqr);

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

var dough1 = doughnut();
var dough2 = boston_cream();

dough1.cook();
dough2.cook();

