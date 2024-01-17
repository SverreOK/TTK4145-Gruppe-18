// Use `go run foo.go` to run your program

package main

import (
    ."fmt"
    "runtime"
)

var i = 0
var ch_complete = 0

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

func incrementingCh(incrementing_ch chan string) {
    for x:=0; x < 1000000; x++ {
        incrementing_ch <- "1"
    }
    incrementing_ch <- "finished"
}

func decrementingCh(decrementing_ch chan string) {
    for x:=0; x < 400000; x++ {
        decrementing_ch <- "1"
    }
    decrementing_ch <- "finished"
}
/*
    Checks for "finished" first, if not finished var i is either incremented or decremented.
    
    You could also make use of a third channel to send a done messages through and have that as an own case
*/
func server(incrementing_ch, decrementing_ch chan string) {
    for {
        select {
        case msg := <-incrementing_ch:
            if msg == "finished" {
                ch_complete++
                if ch_complete == 2 {
                    return
                }
            } else {
                i++
            }
        case msg := <-decrementing_ch:
            if msg == "finished" {
                ch_complete++
                if ch_complete == 2 {
                    return
                }
            } else {
                i--
            }
        }
    }
}

func main() {
    // What does GOMAXPROCS do? What happens if you set it to 1?
    /* 
        Setting it to 1 makes the magic number always equal to 0.
        The reason for this is that the max amount of CPUs used now are 1, which means there are no race conditions.
    */
    runtime.GOMAXPROCS(2)

    incrementing_channel := make(chan string)
    decrementing_channel := make(chan string)

    go incrementingCh(incrementing_channel)
    go decrementingCh(decrementing_channel)
    
    server(incrementing_channel, decrementing_channel)

    close(incrementing_channel)
    close(decrementing_channel)


    Println("The magic number is:", i)
}
