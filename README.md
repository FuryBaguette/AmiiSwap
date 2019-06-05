# AmiiSwap
Nintendo Switch GUI Amiibo Manager homebrew for emulation with [emuiibo](https://github.com/XorTroll/emuiibo) (nfp mitm)

## How to setup
- Download and setup [emuiibo](https://github.com/XorTroll/emuiibo) on your Switch. Once all your bin files have been placed in the emuiibo folder and that you rebooted your console, you should see that they have been replaced with folders of the same name and files inside (including the amiibo.bin file).
- Download [AmiiSwap](https://github.com/FuryBaguette/AmiiSwap/releases) and place the `switch` folder inside the zip file at the root of your sd card.
- Launch AmiiSwap, you'll now see a menu with multiple options

### Menus
- **Amiibos**: This menu is used to manage and use amiibos
- **Emuiibo**: Allows you to change emuiibo's state
- **Images**: Find, place, delete images used for amiibo icons
- **Selected amiibo**: Shows the currently activated amiibo and some info
- **User Manual**: Basicly this readme inside AmiiSwap
- **About**: Information about AmiiSwap

## Adding Categories and Managing Amiibos
- Go to the `Amiibos` menu in the main menu of AmiiSwap
- Press `X` to add a new category -> Enter a name
- Press `Y` to manage selected category's amiibos
- When managing amiibos, simply click on amiibos to add/remove them from the category

## How to use
- Go to the `Amiibos` menu in the main menu of AmiiSwap
- Select a category then a amiibo, from here press `A` to use the amiibo or press `X` to toggle Random UUID
- If you want to use emuiibo combos for emulation, quit here and use emuiibo normaly. **OR**
- Go to the `Emuiibo` menu in the main menu of AmiiSwap
- Select `Enable` to have emulation activated all the time, `Enable once` to emulate only once, `Disable` to use original combo system

## How to navigate
- Use the `dpad` or the `left stick` to navigate menus, `right stick` to navigate faster.
- Use `A` to select
- Use `B` to go back
- Use `X` to add a new game in the `Amiibos menu`
- Use `Y` to manage amiibos of a selected category

## Support
- Use the github issues to report problems/bugs **OR**
- Join the [discord server](https://discord.gg/ap6yfR2) for support, news/announcements before anyone, be a tester or just talk.

## Screenshots
Main menu:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/MainScreen.jpg)

Games list:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/GameList.jpg)

Amiibo list for the selected game:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/AmiiboList.jpg)

Confirmation dialog:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/UseAmiibo.jpg)

Emuiibo toggles:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/EmuiiboToggle.jpg)

Images setup menu:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/ImagesMenu.jpg)

Currently active amiibo:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/ActiveAmiibo.jpg)

About screen:
![](https://github.com/FuryBaguette/AmiiSwap/blob/master/Screenshots/AboutScreen.jpg)

## Credits
- [FuryBaguette](https://github.com/FuryBaguette) - Author and developper
- [AD2076](https://github.com/AD2076) - Developper
- [XorTroll](https://github.com/XorTroll/) - For [emuiibo](https://github.com/XorTroll/emuiibo)
- [spx01](https://github.com/spx01) - His fork of switch-nfp-mitm you can find [here](https://github.com/spx01/switch-nfp-mitm). I should also thank [averne](https://github.com/averne/) and [ogniK5377](https://github.com/ogniK5377). (Fork of fork of fork)
- [Subv](https://github.com/Subv) - For making [switch-nfp-mitm](https://github.com/Subv/switch-nfp-mitm)
