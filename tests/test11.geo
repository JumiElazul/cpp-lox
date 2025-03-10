class doughnut
{
    cook()
    {
        print("Fry until golden brown");
    }
}

class boston_cream < doughnut
{
    
}

class chocolate_doughnut
{
    cook()
    {
        print("Drizzle in delicious chocolate");
    }
}

var i = boston_cream();
var i2 = chocolate_doughnut();

i.cook();
i2.cook();
