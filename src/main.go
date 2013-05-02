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
)

func init() {
	runtime.LockOSThread()
}

func main() {

	var (
		gameWidth  uint        = 800
		gameHeight uint        = 600
	)

	ticker := time.NewTicker(time.Second / 60)
	rand.Seed(time.Now().UnixNano())

	renderWindow := sf.NewRenderWindow(sf.VideoMode{gameWidth, gameHeight, 32}, "Rest it", sf.StyleDefault, nil)

	// Create the left paddle
/*	leftPaddle := sf.NewRectangleShape()
	leftPaddle.SetSize(sf.Vector2f{paddleSize.X - 3, paddleSize.Y - 3})
	leftPaddle.SetOutlineThickness(3)
	leftPaddle.SetOutlineColor(sf.ColorBlack())
	leftPaddle.SetFillColor(sf.Color{100, 100, 200, 255})
	leftPaddle.SetOrigin(sf.Vector2f{paddleSize.X / 2, paddleSize.Y / 2})
*/
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

			//renderWindow.Draw(block, nil)
			
			renderWindow.Draw(pauseMessage, nil)

			// Display things on screen
			renderWindow.Display()
		}
	}
}
