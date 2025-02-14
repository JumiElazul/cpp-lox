func count(a)
{
    if (a == 1)
    {
        print(1);
    }
    else
    {
        print(a);
        count(a - 1);
    }
}

func my_func(a)
{
    print(a);
}

func outer()
{
    print("outer");
    func inner()
    {
        print("inner");
    }
    inner();
    print("outer");
}

count(5);
my_func("hello");
outer();

{
    var b = 1;
    print(b);
}

print(b);

