// global
func makeCounter() // env 1 -> global created
{
    var i = 0;
    func count()   // env 2 -> env 1, needs to own env 1
    {
        i = i + 1;
        print(i);
    }
    return count;
}

var counter = makeCounter();
print(counter);

// counter();
