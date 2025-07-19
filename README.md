# Seki

In honor of exploring the beautiful Sequoia and King's Canyon front and backcountry, this Internet "browser" is a Sierra Nevada themed app (Find The Easter Eggs Too)!

## Building Raylib

First build raylib. Go to lib/raylib/src/ and run

```
make PLATFORM=PLATFORM_DESKTOP
```

depending on your preferred platform.

## Build

Navigate to the base directory and simply compile using

```
make -f desktop.mk
```

Then run by

```
./build/seki
```
