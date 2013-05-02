/*
*********************************************
*	GOSFML2
*	SFML Examples:	 Pong
*	Ported from C++ to Go
*********************************************
 */

package main

import (
	sf "bitbucket.org/krepa098/gosfml2"
//	"math"
	"math/rand"
	"runtime"
	"time"
	"fmt"
)

const (
	gridWidth = 10
	gridHeight = 20
	blockSize = 25
)


type tetrisGrid struct {
	tiles [gridWidth][gridHeight]bool
}

func init() {
	runtime.LockOSThread()
}

func main() {
	fmt.Println("Starting restit")

	grid := tetrisGrid{}
	grid.tiles[1][1] = true

	var (
		gameWidth  uint        = 800
		gameHeight uint        = 600
	)

	ticker := time.NewTicker(time.Second / 60)
	rand.Seed(time.Now().UnixNano())

	renderWindow := sf.NewRenderWindow(sf.VideoMode{gameWidth, gameHeight, 32}, "Rest it", sf.StyleDefault, nil)

	// Create the left paddle
	block := sf.NewRectangleShape()
	block.SetSize(sf.Vector2f{blockSize, blockSize})
	block.SetFillColor(sf.Color{100, 100, 200, 255})
	
	// Create the left paddle
	emptyBlock := sf.NewRectangleShape()
	emptyBlock.SetSize(sf.Vector2f{blockSize, blockSize})
	emptyBlock.SetFillColor(sf.Color{255,255,255,255})
	
	//block.SetOutlineThickness(3)
	//block.SetOutlineColor(sf.ColorBlack())
	
	
	
//	leftPaddle.SetOrigin(sf.Vector2f{paddleSize.X / 2, paddleSize.Y / 2})

	// Load the text font
	font, _ := sf.NewFontFromFile("resources/sansation.ttf")

	// Initialize the pause message
	pauseMessage := sf.NewText(font)
	pauseMessage.SetCharacterSize(40)
	pauseMessage.SetPosition(sf.Vector2f{170, 150})
	pauseMessage.SetColor(sf.ColorWhite())
	pauseMessage.SetString("Welcome to RESTIT!\ninstructions")

	for renderWindow.IsOpen() {
		select {
		case <-ticker.C:
			//poll events
			for event := renderWindow.PollEvent(); event != nil; event = renderWindow.PollEvent() {
				switch ev := event.(type) {
				case sf.EventKeyReleased:
					switch ev.Code {
					case sf.KeyEscape:
						renderWindow.Close()
					case sf.KeySpace:
						//do stuff
					}
				case sf.EventClosed:
					renderWindow.Close()
				}
			}
			//deltaTime := time.Second / 60

			// Move the player's paddle
			//if sf.KeyboardIsKeyPressed(sf.KeyUp) && leftPaddle.GetPosition().Y-paddleSize.Y/2 > 5 {
//			}

			// Clear the window
			renderWindow.Clear(sf.Color{140, 170, 255, 0})

			for column := range grid.tiles {
				for row := range grid.tiles[column] {
					if grid.tiles[column][row] {
						block.SetPosition(sf.Vector2f{float32(column*blockSize), float32(row*blockSize)})
						renderWindow.Draw(block, nil)
					} else {
						emptyBlock.SetPosition(sf.Vector2f{float32(column*blockSize), float32(row*blockSize)})
						renderWindow.Draw(emptyBlock, nil)
					}
				}
			}
			
			renderWindow.Draw(pauseMessage, nil)

			// Display things on screen
			renderWindow.Display()
		}
	}
}

