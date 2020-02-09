# Multithread-game on ncurses
This is a simple terminal game written in (<B>C</B>) using the (<B>ncurses</B>) library. This is my homework from university. All information about this project you can check in game_report.pdf.
# The (learning) goals of the project:
- Learn how to split a single-threaded program into multi-threaded.(+)
- Get to know the critical sections and learn how to handle them.(+)
- Learn how to create a recovery point, so that in case of incorrect operation of the system, roll back to working condition.(-)
- Create a disk image on which the game starts.(+)
# Game rules:
- Control a ship using arrows.
- Collision with asteroids takes your life.
- The game ends when the player runs out of energy or if the player pressed 'q'.
# Installing and running game
git clone https://github.com/volkov7/Multithread-game.git \
install:\
if you don't use MAC OS check 2 point of game_report.pdf.
```
make && make clean
```
running:
```
./game
```
You can also use the assembled image on which the game starts. Using qemu for instance, you can launch the game.iso image.\
Run command: qemu-system-x86_64 -cdrom /path/to/game.iso -m 1024
![Space game](https://github.com/volkov7/Multithread-game/raw/master/game.png)