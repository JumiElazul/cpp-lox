class my_class
{
    do_thing(a)
    {
        print(a);
    }

    do_thing2(b)
    {
        print(b);
    }
}

var i = my_class();
i.do_thing("a");
var i2 = i.do_thing2;
i2("b");
