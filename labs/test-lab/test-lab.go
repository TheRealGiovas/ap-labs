package main

import (
	"fmt"
	"os"
)

func main() {
  
  var names = ""
  
  for i := 1; i < len(os.Args); i++ {
    names += os.Args[i] + " "
  }
  
	fmt.Println("Welcome to the jungle", names)
}
