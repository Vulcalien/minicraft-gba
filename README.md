# Minicraft for GBA

This is a demake of Minicraft by Markus Persson (aka Notch), a game made for
Ludum Dare 22 in two days.

My aim is to make a version that is as close as possible to the original.
Of course, the GBA has some hardware limitations but, since the game isn't too
complex, I could port it without too many problems.
When the limitations were impossible to overcome, I had to hack a few things,
change others, put limits and so on.

To improve the experience, I also added a pause menu and a way to save and load
the world.

## Differences due to Hardware limitations
|                | Original | GBA Demake |
| -------------- | -------- | ---------- |
| World size     | 128x128  | 112x112    |
| Entity limit   | ∞        | 255        |
| Chest limit    | ∞        | 32         |
| Inventory size | ∞        | 128        |

## Running
Download or build the ROM (`.gba` extension). Then open it with your GBA
emulator of choice. If you don't have one, I highly recommend mGBA.

If you have any trouble with the save files, try to manually set the save format
to `128 KB Flash ROM`.

## Building
To build the game, I use the `Makefile` present in the files.
You will need the gcc-arm-none-eabi compiler. I've not tested any other compiler
and, if you want to try, I cannot guarantee that the game will work or that the
performance will be as good (since it's optimized for the GCC compiler).

Run `make`: it should build without any configuration.

## License
The original game `Minicraft` was made by Markus Persson in 2011.
I do not own it, nor am I affiliated to it.

This demake of the game is released under the GNU General Public License, either
version 3 of the License or any later version.
