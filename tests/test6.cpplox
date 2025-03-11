var a = "global";
{
    func show_a() 
    {
        print(a);
    }
    show_a();
    var a = "block";
    show_a();
}

var i = 10;
print(i);

{
    print("one scope deep");
    var i = 15;
    print(i);
    {
        print("two scopes deep");
        var i = 20;
        print(i);
        {
            print("three scopes deep");
            i = 30;
            print(i);
        }
        print("two scopes deep (i was reassigned a new value)");
        print(i);
    }
}
