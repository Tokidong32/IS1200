Welcome to the snake game produced by Viktor Danielsson and Tomas Ålund.

In order to make the game, open the make_files folder in the terminal and type the make command

Everything will automatically compile into a main.bin file.

To run on the DTEK-board use dtek-run main.bin and make sure that the jtag server is active.

The game is built around a breadboard for buttons so to play the game ensure that  breadboard has 4 buttons which are all connected 
to gpio. Pin gpio_[0],gpio_[1],gpio_[2] and gpio_[3] are connected to:
top button = gpio_[0], right button = gpio_[1], bottom button = gpio[2] and left button = gpio[3].

For visuals the game runs using the vga port so connect the dtek board to a compatible screen.