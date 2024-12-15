## bootloader
bootloader files in folder xtips_x7s/binaries

- xtips_x7s_vial.bin: default vendor bootloader
- dso138_boot20.bin: works with 64MB AMP chipset

## Flashing

- in rules.mk have EEPROM_DRIVER=transient 
- use dso138_boot20.bin to flash both halves

>`sudo st-flash --reset --format binary write ~/Downloads/dso138_boot20.bin 0x08000000`

only build miryoku_x7s keymap, default is not working

>`make clean xtips_x7s:miryoku_x7s`

after flashing dso138_boot20.bin, put boards in bootloader with W and O keys

>`qmk flash xtips_x7s_miryoku_x7s.bin`
