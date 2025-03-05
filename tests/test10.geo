class cake
{
    taste()
    {
        var adjective = "delicious";
        print("The " + this.flavor + " cake is " + adjective + "!");
    }
}

var my_cake = cake();
my_cake.flavor = "German chocolate";
my_cake.taste();

//print(this);

func notAMethod()
{
    print(this);
}

notAMethod();
