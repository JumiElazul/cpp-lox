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

count(5);
