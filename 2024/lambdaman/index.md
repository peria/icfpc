Welcome to the Lambda-Man course.

It was the year 2014, and many members of our community worked hard to control Lambda-Man. Now, ten years later, this wonderful event is still memorized by holding a small Lambda-Man competition.

This course will teach you how to optimally control Lambda-Man to eat all pills. There is no fruit involved (neither low-hanging nor high-hanging), and even better: no ghosts! The input to each problem is a simple rectangular grid such as the following:

```
###.#...
...L..##
.#######
```

The grid contains exactly one `L` character, which is the starting position of Lambda-Man. There will be one or more `.` characters indicating the locations of pills to be eaten, and `#` characters are walls. The outside boundary of the grid is considered to consist of walls as well.

A solution should be a string of `U`, `R`, `D` and `L` characters (up, right, down, left, respectively) indicating the path to take. For example, a possible solution to the above example grid is the following path:
```
LLLDURRRUDRRURR
```
When Lambda-Man is instructed to move into a square containing a wall, nothing happens and the instruction is skipped. Your solution may consist of at most `1,000,000` characters.

The following levels are available:
* [lambdaman1]
* [lambdaman2]
* [lambdaman3]
* [lambdaman4]
* [lambdaman5]
* [lambdaman6]
* [lambdaman7]
* [lambdaman8]
* [lambdaman9]
* [lambdaman10]
* [lambdaman11]
* [lambdaman12]
* [lambdaman13]
* [lambdaman14]
* [lambdaman15]
* [lambdaman16]
* [lambdaman17]
* [lambdaman18]
* [lambdaman19]
* [lambdaman20]
* [lambdaman21]

To submit a solution, send an ICFP expression that evaluates to:

```
solve lambdamanX path
```

Your score is number of bytes that the ICFP expressions consists of (i.e. the size of the POST body), so a lower score is better.

