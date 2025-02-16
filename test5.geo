func func1()
{
    var i = 0;
    func inc()
    {
        i = i + 1;
    }
    return inc;
}

var a = func1();
print(a);
print(a());
print(a());
print(a());
print(a());
