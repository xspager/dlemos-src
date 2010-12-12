package main

import (
	"fmt"
)

type person struct {
	name string; age int
}

func main() {

	m := make(map[int]person)

	m[0] = person{"Daniel", 24}
	m[1] = person{"Helena", 20}

	for _, v := range m {
		fmt.Printf("%s %d anos\n", v.name, v.age)
	}
}
