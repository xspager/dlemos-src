package main

import (
	"os"
	"fmt"
)

func main() {

	env := os.Environ()

	for _, e := range env {
		fmt.Printf("%s\n", e)
	}
}
