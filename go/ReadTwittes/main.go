package main

import (
	"flag"
	"./speak"
)

func main(){
	var text = flag.String("text", "", "Text")
	flag.Parse()

	speak.Say(*text)
}
