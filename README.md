# Multiplayer mod for BOFH: Servers Under Siege V1.5 (2003)
- Original release was made by Lasse Öörni and it is open source.
- The game can be downloaded from the Internet Archive [here](https://web.archive.org/web/20030625171044/http://www.student.oulu.fi:80/~loorni/software.htm)
- The game was originally developed using Öörni's own BME engine, but it got switched to SDL in June 2003. The original versions source code cannot be found anymore, so I used the SDL version instead.

## Multiplayer VERY VERY WIP
The multiplayer part is made with [ENet library](http://enet.bespin.org/). 
The mod consists of server and (at this moment) 2 clients, where one of the clients is a host. 
The first client to connect to the server gets assigned as the host.

### Multiplayer logic simplified

1. First client is the host.
2. The host spawns all the actors and items for itself.
3. The host compresses all the spawned data using zlib and sends it to the server.
4. Second client connects.
5. The client gets sent the data.
6. The client extracts the data.
7. The client spawns the actors using the data.


### What works?

- Player movement is synchronized
- Actor movevent is synchronized
- Enemy attacking logic

### What doesn't work?

- Any sort of item pickup
- Player health
- Remaining time
- Lots of other things for the client (elevators, bomb defusal...)
- Disconnections
- Player death
- Bullet shells (and other actors) don't spawn properly
- The game gets quite slow because of all the polling every frame

