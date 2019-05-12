# AmiiSwap
Nintendo Switch GUI Amiibo Manager homebrew for emulation with [emuiibo](https://github.com/XorTroll/emuiibo) (nfp mitm)

## How to use
- Download and setup [emuiibo](https://github.com/XorTroll/emuiibo) on your Switch. Once all your bin files have been placed in the emuiibo folder and that you rebooted your console, you should see that they have been replaced with folders of the same name and files inside (including the amiibo.bin file), keep that in mind.
- Download [AmiiSwap](https://github.com/FuryBaguette/AmiiSwap/releases) and place the `switch` folder inside the zip file at the root of your sd card.
- The folder at `sdmc:/switch/AmiiSwap` contains a settings.txt file, it's an example file and you need to edit it to fit your amiibos and needs. There's a section below explaining how to set it up.
- Launch AmiiSwap, select a game/category you created in the settings file -> select the amiibo you want -> press A -> Confirm
- Enjoy

You can change amiibos whenever you want by just going back into AmiiSwap and selecting another amiibo.

## How to setup the settings file
The settings.txt should be place in `sdmc/switch/AmiiSwap`, the format is really simple.
This file allows you to "create" categories of amiibos, you can choose to organize them by anything (game name, category ect.). For the sake of this guide we'll use game names to organize amiibos

Here's an example settings file:
```
[Mario Kart 8]
Bowser
Captain Falcon
Donkey Kong
[Zelda BOTW]
20 Heart Wolf Link
Bokoblin
Guardian
```
And [here](https://raw.githubusercontent.com/FuryBaguette/AmiiSwap/master/Screenshots/FolderPlacement.png) is how the emuiibo folder is set

Your game names should be between brackets (you can set it to anything), example: [Super Smash Bros].
After that, create a new line for each amiibos (with their name) you have and want to put in that game. The amiibo name should be the name of the folders created by emuiibo.
You can add another game by simply doing these 2 steps again

## How to navigate
- Use the `dpad` or the `left stick` to navigate menus, `right stick` to navigate faster.
- Use `A` to select
- Use `B` to go back to the main menu

## Screenshot
Main menu showing the games:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/MainScreen.jpg)

Amiibo list for the selected game:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/AmiiboList.jpg)

Confirmation dialog:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/UseAmiibo.jpg)

## Credits
- [XorTroll](https://github.com/XorTroll/) - For [emuiibo](https://github.com/XorTroll/emuiibo)
- [AD2076](https://github.com/AD2076) - For some dev and ideas on AmiiSwap
- [spx01](https://github.com/spx01) - His fork of switch-nfp-mitm you can find [here](https://github.com/spx01/switch-nfp-mitm). I should also thank [averne](https://github.com/averne/) and [ogniK5377](https://github.com/ogniK5377). (Fork of fork of fork)
- [Subv](https://github.com/Subv) - For making [switch-nfp-mitm](https://github.com/Subv/switch-nfp-mitm)
