func my_func(a)
{
    print(a);
}

func factorial(n)
{
    if (n == 1)
        print(1);

    print(factorial(n - 1));
}

factorial(5);
