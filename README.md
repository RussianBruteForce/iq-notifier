# IQ Notifier
IQ Notifier is fancy and flexible notification daemon.

## Spec
[Version 1.2](https://people.gnome.org/~mccann/docs/notification-spec/notification-spec-latest.html)

## Capabilities
- actions
- body
- body-markup
- icon-static
- persistence

## Screenshots
![0](/screenshots/0.png?raw=true)

![1](/screenshots/1.png?raw=true)

![2](/screenshots/2.png?raw=true)

## Features
### TitleToIcon
If icon not presented, IQ Notifier will compare title and app name; if its equals, IQ Notifier will try to find and set app icon.

### URL icons support
Icon can be simple link to image.

### ReplaceMinusToDash
IQ Notifier will replace all occurrences of `-` to `—`.

### BodyToTitleWhenTitleIsAppName
If icon not presented, IQ Notifier will compare title and app name; if its equals, IQ Notifier will move all text from body to title.

### All fields are optional
Unused parts of notifications will not shown. 

### Multiple actions
Buttons row at the bottom. :)

### Flexidble
You can configure most parts of IQ Notifier or rewrite some parts as you want. 

But currently no config/args support. Feel free to contribute.

# Compositing
To make opacity [of popups] works you need compositing. Try `compton -CG `, that should work.

# TODO
- Notifiaction history
- Config support
- Fit size of notification to content size
- Theming support
- HiDPI support
- Multiple monitor support
- More features
- ??????

Use `ag TODO` or your favorite IDE to find TODOs in code.

# Build deps
```bash
sudo apt install cmake qtbase5-dev qtdeclarative5-dev libqt5xdg-dev
```

# Build
To clone this repo with dependencies (GSL) use `--recursive` flag:
```bash
git clone --recursive git@github.com:RussianBruteForce/iq-notifier.git
mkdir iq-notifier/build; cd iq-notifier/build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Tested on ubuntu 16.04, GCC 5.4.0 and Clang 3.8.0.

# Config
Config path:`$XDG_CONFIG_HOME/iq-notifier/config` (`~/.config/iq-notifier/config`)

All 'modules' of IQ Notifier should be enabled in config file explicitly. To copy example config execute from repo root:
```bash
cp config.example ~/.config/iq-notifier/config
```

# Deb package
```bash
cpack
sudo apt update
sudo dpkg -i iq-notifier-0.1.0-amd64.deb
sudo apt installf -f
```

# Contributions
Feel free to make pull requests/fork this project. You can also contact me vie e-mail: [bruteforce@sigil.tk](mailto:bruteforce@sigil.tk)

You also need `clang-format` and `cpplint`.

# License
Look at COPYING file.
