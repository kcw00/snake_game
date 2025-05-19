# 🐍 Snake Game (built from scratch in 2022)

A modern remake of the classic **Snake** game using **C++**, **SDL2**, and **SDL_ttf**.

---
### 📷 Live Demo

![ScreenRecording2025-05-18at19 34 46-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/351f54ad-b05c-4988-979d-25d9a0c5ffa1)

---
### ✨ Features
- Real-time snake movement logic
- Dynamic score rendering with SDL_ttf
- Game over and win screens with restart flow
- Clean, retro-inspired visuals

---
### ⚙️ Set up (macOS Apple Silicon)

#### ✅ Prerequisites
Make sure you have the following dependencies installed via [Homebrew](https://brew.sh/)

```bash
brew install sdl2 sdl2_ttf
```

1. Clone this repo
```bash
git clone https://github.com/kcw00/snake_game.git
cd snake_game
```
2. Compile the project
```bash
clang++ -std=c++17 main.cpp -o snake_game \
  -I/opt/homebrew/include/SDL2 \
  -L/opt/homebrew/lib \
  -lSDL2 -lSDL2_ttf
```
3. Run the game
```bash
./snake_game
```

---
### 🕹 Controls

| Key    | Action             |
|--------|--------------------|
| `Arrow Keys`    | Move the snake     |
| `Return`    | Restart after game over/win   |
| `ESC`  | Quit the game      |


---
### 🏆 Objective
- Eat the food (cyan blocks) to grow longer.
- Avoid hitting your own tail or the screen border.
- Reach maximum tail length to win the game!

