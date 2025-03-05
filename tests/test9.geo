class my_class
{
    do_thing(a)
    {
        print(a);
    }
}

var i = my_class();
i.prop = "hello";
print(i);
i.do_thing(5);
print(i.prop);
