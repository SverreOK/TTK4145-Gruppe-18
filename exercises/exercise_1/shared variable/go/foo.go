func incrementing() {
    //TODO: increment i 1000000 times
    for x := 0; x < 1000000; x++ {
        i++;
    }
}

func decrementing() {
    //TODO: decrement i 1000000 times
    for x:= 0; x < 1000000; x++ {
        i--;
    }
}

func main() {
    // What does GOMAXPROCS do? What happens if you set it to 1?
    runtime.GOMAXPROCS(2)    

    // TODO: Spawn both functions as goroutines
    go incrementing();
    go decrementing();

    // We have no direct way to wait for the completion of a goroutine (without additional synchronization of some sort)
    // We will do it properly with channels soon. For now: Sleep.