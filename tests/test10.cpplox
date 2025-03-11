class cake
{
    taste()
    {
        var adjective = "delicious";
        print("The " + this.flavor + " cake is " + adjective + "!");
    }
}

var my_cake = cake();
var my_cake2 = cake();
my_cake.flavor = "German chocolate";
my_cake2.flavor = "Ice cream";

print("my_cake.taste():");
my_cake.taste();
print("my_cake2.taste():");
my_cake2.taste();

print("my_cake.taste = my_cake2.taste");
var temp = my_cake.taste;
my_cake.taste = my_cake2.taste;
my_cake2.taste = temp;
print("my_cake.taste()");
my_cake.taste();
print("my_cake2.taste()");
my_cake2.taste();

print("Constructing test_init classes");

class test
{
    init()
    {
        print("Calling initializer method on test_init class");
        return;
    }
}

var cls = test();

cls.prop = "hello static method";

class math
{
    static square(a)
    {
        return a * a;
    }
}

var num = 5;
var result = math.square(num);
print("Result of " + num + " squared is " + result);

class circle
{
    init(radius)
    {
        print("initializing class circle");
        this.radius = radius;
    }
}

var cir = circle(1.5);
print(cir.radius);
