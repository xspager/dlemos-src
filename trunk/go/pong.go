package main

/*
	Must use a image buffer to keep the background
*/

import (
	"fmt"
	"exp/draw"
	"exp/draw/x11"
	"image"
	"time"
	"os"
	"image/png"
)

func main() {
	w, e := x11.NewWindow()
	if e != nil { fmt.Printf(e.String()) }
	i := w.Screen()
	draw.Draw(i, i.Bounds(), image.NewColorImage(image.White), image.ZP)
	draw.Border(i, i.Bounds(), 10, image.NewColorImage(image.Black), image.ZP)
	f, _ := os.Open("ball.png", os.O_RDONLY, 0)
	ball, _ := png.Decode(f)

	for x := 100; x < i.Bounds().Max.X; x+=20 {
		draw.Draw(i, i.Bounds().Add(image.Point{100,x}), ball, image.ZP)
		w.FlushImage()
		time.Sleep(1e8)
	}

	e = w.Close()
	if e != nil { fmt.Printf(e.String()) }

}
