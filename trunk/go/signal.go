package main
import (
  "fmt"
  "time"
  "os"
  "os/signal"
  "syscall"
)
 
func main() {
  start := time.Nanoseconds()
 
  go func() {
    for {
      s := <-signal.Incoming // all types of signals are received on this channel
      if (s.(signal.UnixSignal) == syscall.SIGINT) {
        fmt.Printf("\nRan for %f seconds.\n", float64(time.Nanoseconds() - start) / 1e9)
        os.Exit(0)
      }
    }
  }()
 
  for n := 1; ; n++ {
    fmt.Println(n)
    time.Sleep(5e8) // 0.5 seconds
  }
}
