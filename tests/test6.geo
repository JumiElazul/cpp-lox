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
