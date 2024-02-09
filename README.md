# Minicraft for GBA

This is a demake of Minicraft by Markus Persson (aka Notch), a game made
for Ludum Dare 22 in two days.

My aim is to make a version that is as close as possible to the
original. Of course, the GBA has some hardware limitations but, since
the game isn't too complex, I could port it without too many problems.
When the limitations were impossible to overcome, I had to hack a few
things, change others, put limits and so on.

To improve the gameplay experience, I also added these features:
- a pause menu
- saving and loading the world
- a way to respawn, with or without keeping the inventory

## Differences due to Hardware limitations
|                | Original | GBA Demake |
| -------------- | :------: | :--------: |
| World size     | 128x128  | 112x112    |
| Entity limit   | ∞        | 255        |
| Chest limit    | ∞        | 32         |
| Inventory size | ∞        | 128        |

### Game Light
The light system is completely different, both visually and in how it
works.

The original Minicraft calculates the light for **each pixel**: that is,
for various reasons, impractical to do on the GBA.

So I had to find another way, and the best one seemed to use tiles,
because the GBA is very good at handling them. That gives light a
'blocky' look, but it seems acceptable.

## Running
Download or build the ROM (`.gba` extension). Then open it with your GBA
emulator of choice. If you don't have one, I highly recommend mGBA.

If you have any trouble with the save files, try to manually set the
save format to `128 KB Flash ROM`.

### Performance Overlay
By holding the `L` and `R` buttons down and then pressing `Select`, the
performance overlay is enabled.\
Four hexadecimal values and two decimal values are written at the top of
the screen:
```
FF <--- vcount after 'tick'     entity count ---> FF
FF <--- vcount after 'draw'     sprite count ---> FF

60 <--- tps (ticks per second)
60 <--- fps (frames per second)
```

## Building
To build the game, I use the `Makefile` present in the files.
You will need the gcc-arm-none-eabi compiler and Python 3.

First, run `make res` to convert the necessary resources. Then, run
`make` to build the ROM.

## License
The original game `Minicraft` was made by Markus Persson in 2011.
I do not own it, nor am I affiliated to it.

This demake of the game is released under the GNU General Public
License, either version 3 of the License or any later version.
