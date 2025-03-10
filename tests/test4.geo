var iter = 10;
 
for (var i = 0; i < iter; ++i)
{
    if (i == 2)
        continue;
 
    if (i == 8)
        break;

    print(i);
}

func sum_add(n)
{
    var sum = 0;
    while (n > 0)
    {
        sum = sum + n;
        --n;
    }

    return sum;
}

var sum = sum_add(10);
print(sum);
