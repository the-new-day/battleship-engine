# Battleship engine

Backend for a bot capable of playing Battleship: to create a full-fledged game, it is sufficient to implement the client-side and ensure interaction with the backend.

## Building
To build, you need a compiler with C++23 support. If you are using CMake, you can build it with the following command:
```bash
cmake -B ./build & cmake --build ./build
```

## Main logic
Before starting the game, you need to create it. To do this, you need to select a game mode with the bot. There are 2 game modes: *master* and *slave*. In the first case, the bot chooses the field size and the number of ships (but always chooses the standard ones: 10x10, system 1x4 + 2x3 + 3x2 + 4x1). If *slave* is selected, you can (and have to) specify the field size and the number of ships of (cannot be 0).

### Gameplay
After setting up the game (field, ships), you can start the game. If you want to start a new game with the same settings, you can stop the game and start it again.

The gameplay is very simple: you ask the bot to name the coordinates of its next shot, and then you report the result. If you want to shoot yourself, you also provide the shot coordinates, and the bot returns the result (miss/hit/kill).

The program does not monitor compliance with the game rules (in particular, the order of shots), this function is left to the player or the client.

### Strategies
You can choose the strategy in which the bot will play. There are 3 strategies in total: *ordered*, *parity*, and *probability*. You can also choose the *custom* strategy, which allows the bot to decide for itself (in fact, the strategy will then depend on the field size).

#### Ordered
This is the simplest strategy. The bot will simply shoot at all cells in order, starting from the top left and ending with the bottom right.

*Hunting strategy* - a strategy in which, if the bot manages to hit the opponent, it will try to sink the entire ship. Also, in the hunting strategy, guaranteed empty cells are remembered to avoid shooting at them.

The next two are such.

#### Parity
This is a hunting strategy that in the basic mode (not in "hunt") differs from ordered only in that it shoots at cells with a step of 2. This increases the chance of quickly hitting large ships.

#### Probability
A strategy based on the expectation of a ship appearing in a specific cell. After each shot, the expectations are recalculated, and the most probable cell is selected. This is the "smartest" strategy.

## Interaction
In the basic version, interaction is carried out through standard input-output. However, the game logic does not depend on the specific client implementation, so it is possible to add any other type of interaction.

Through the stdin/stdout, interaction is carried out using the following commands:
| Command                      | Response       | Description |
| -------                      | -----          | --------                                    |
| exit                         |  ok            |   Program terminates                    |
| create [master, slave]       |  ok            |   Create a game in master or slave mode respectively       |
| start                        |  ok            |   Start the game                    |
| stop                         |  ok            |   Stop the current game       |
| set width  N                 |  ok/failed     |   Set the field width (N positive, fits in uint64_t)       |
| get width                    |  N             |   Get the field width      |
| set height N                 |  ok/failed     |   Set the field height (N positive, fits in uint64_t)        |
| get height                   |  N             |   Get the field height      |
| set count [1, 2, 3, 4]  N    |  ok/failed     |   Set the number of ships of a certain type (N positive, fits in uint64_t)       |
| get count [1, 2, 3, 4]       |  N             |   Get the number of ships of a certain type   |
| set strategy [ordered, custom, parity, probability]|  ok            |   Choose a strategy for the game        |
| shot X Y                     |  miss/hit/kill |   Shoot at the bot's ships at coordinates (X, Y) (X, Y positive, fit in uint64_t)  | 
| shot                         |  X Y           |   Return the coordinates of the bot's next shot, in response two numbers separated by a space (X,Y positive, fit in uint64_t)     |
| set result [miss, hit, kill] |  ok            |   Set the result of the bot's last shot       |
| finished                     |  yes/no        |   Is the current game finished       |
| win                          |  yes/no        |   Did the bot win       |
| lose                         |  yes/no        |   Did the bot lose       |
| dump PATH                    |  ok            |   Save the field size and the current arrangement of the bot's ships to a file        |
| load PATH                    |  ok            |   Load the field size and the arrangement of ships from a file      |
| ping                         |  pong          |   Test command                          |

## Saving the Field
You can load your own field for the bot or save the current arrangement of ships to a file to resume the game. Use the appropriate commands for this.

Loading the field automatically sets the configuration and sets the bot's role to master, so without stopping the game, it cannot be changed.

File format:
```
10 10
1 h 3 0
1 h 8 1
1 h 2 3
1 h 9 4
2 h 0 5
2 h 0 7
4 h 6 7
2 h 1 9
3 v 3 5
3 v 7 3
```
The first line: the length and width of the field.
The remaining lines: the size of the ship, the type of placement (h - horizontal, v - vertical), x and y coordinates of the top-left point of the ship (from the top-left corner).

## Notes
During the writing of this project, I was not familiar with many features of modern C++, such as smart pointers. So in some places, it could have been written better. But not now.

This is an assignment in the 1st semester of the "Basics of programming" course (ITMO SE).

In our course, a Battleship tournament was held among all students. I took [13th place out of 57](https://docs.google.com/spreadsheets/d/1cczS6qZeuPLAA_t2Tx2bU086r6r0xfqchPo1ZK2ZJc4/edit?gid=2095339941#gid=2095339941).
