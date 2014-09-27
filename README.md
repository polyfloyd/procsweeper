ProcSweeper
===========

ProcSweeper is a minesweeper-like game. The difference being that when you hit
a mine, ProcSweeper will pick a random process from your computer and kill it.
By default, only processes running as the executing user will be selected.

![Screenshot](https://polyfloyd.net/data/file/dNcPHkDmj4gpxRT7iJ3fbQ/ProcSweeper%20Screenshot.png)

## Playing
Use the HJKL keys to move around. Use the X key to clear a tile. You can flag a
tile using the F key.

## Building
ProcSweeper requires ncurses5 and libprocps3.
```sh
sudo apt-get install libncurses5-dev libprocps3-dev
make
```

## Options
* -s | --system
  Pick any proces on the system. Requires root.

* -h | --hardcore
  Only kill PID 1 (init). Requires root.

* -k | --sigkill
  Use `SIGKILL` instead of `SIGINT`

* -p | --pussy
  ProcSweeper won't actually signal a process when you hit a mine.

## TODO
* Victory condition
* Adjustable minecount
* Adjustable fieldsize
* Show remaining mines

## License
MIT
