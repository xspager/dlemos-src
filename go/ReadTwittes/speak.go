package speak

import (
	"exec"
)

func Say(text string){
	textToSpechURL := "http://translate.google.com.br/translate_tts?"
	params := "tl=pt&ie=UTF-8&q=" + text

	argv := []string{"","-q", textToSpechURL + params}

	exec.Run("/usr/bin/mpg123", argv, []string{}, ".", exec.PassThrough, exec.PassThrough, exec.PassThrough)
}
