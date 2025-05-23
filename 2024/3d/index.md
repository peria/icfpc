Welcome to the 3D course!

Back in the old days, when the Cult of the Bound Variable still lived on earth, programming languages were still quite primitive. For example, people wrote their programs in 2D, thereby ignoring the third dimension of time. Clearly the spacetime complexity of a program is 3-dimensional, and in this course you will learn to optimize the spacetime volume of your programs.

Below is a complete reference of the 3D language, which is a time-travel-oriented programming language. To keep the spacetime volume as small as possible, your program can use time warping to keep the time-axis small.

# Syntax

A 3D program is a text file that represents a 2D grid of cells. The rows are separated by newlines. Within each row, cells are represented by non-whitespace tokens separated by whitespace.

It is human-friendly but not mandatory to keep the columns aligned by adding extra whitespace between cells; each row is processed separately. Rows do not have to have the same length; short rows are assumed to be left-aligned: empty on the right.

## Available tokens

* `.` represents an empty cell
* an integer between `-99` and `99` (inclusive) represents that integer
* the following characters
  `<`, `>`, `^`, `v`, `+`, `-`, `*`, `/`, `%`, `@`, `=`, `#`, `S`, `A`, `B`
  represent operators.
  See [the operator reference](#operator-reference) for their meaning.

# Semantics

The board is an infinite 2D grid of cells. Each cell is either empty or contains a value. A value is either an arbitrarily sized integer or an operator.

Programs cannot contain arbitrarily sized integer literals. Integer literals can only range between `-99` and `99`, inclusive. This is a source-code-only restriction. At runtime, programs are free to compute with integers of arbitrary size.

## Basic 2D reduction

Time flows in discrete units, called ticks. The initial board is identical to the source code and its time coordinate is `t=1`. With each tick, we perform one round of reductions across the whole board simultaneously.

[The operator reference](#operator-reference) defines the reduction rules of all operators. Generally, all operators perform local rewriting on their surroundings. For example, the "move right" operator `>` rewrites `x > .` to `. > x`.

Operators are values, too, so `+ > .` reduces to `. > +`. This way, it's also possible to shuffle operators around.

Binary operators, like `+`, `*`, or `-`, rewrite like this:
```
. y .     .  .   .
x - .  ~> .  -  x-y
. . .     . x-y  .
```

Operators `A` and `B` are replaced with the input values (if any) after parsing. This mechanism is used to give different inputs to your program.

There is operator `S`, which you can overwrite to terminate the program and submit the answer. It is an error to submit multiple different values, submitting the same value simultaneously multiple times is fine.

Some reduction principles:

1. If the preconditions of an operator are not met, reduction simply does not take place. For example, if a binary operator
   has only one operand available, things stay as they are until the other operand arrives.

2. Outputs of operators overwrite the output cells.

   `1 > +` reduces to `. > 1`

3. Reading a value removes/consumes it.

4. Two operators can read from the same input cell at the same time. Both operators receive a copy of the input value before it's removed from the board.

   `. < 6 > .` reduces to `6 < . > 6`

5. Conflicting writes into the same cell like `3 > . < 3` or `3 > . < 4` are disallowed and will crash the simulation.

6. In every tick, all reads (and removals) happen before all the writes.

   `1 > 2 > .` reduces to `. > 1 > 2`

### Operator reference

In the diagrams below, the symbol `.` generally stands for an empty cell or a non-empty cell containing any value. We use the dot instead of metavariables for readability.

Arrows move values of any type (integers or operators).
```
. < x   ~>   x < .         x > .   ~>   . > x


  .            x             x            .
  ^     ~>     ^             v     ~>     v
  x            .             .            x
```

Binary arithmetic operators reduce only for integer arguments. They write their outputs both to the right and below at the same time.
```
. y .        .  .  .       . y .        .  .  .
x + .   ~>   .  + x+y      x * .   ~>   .  * x*y
. . .        . x+y .       . . .        . x*y .


. y .        .  .  .
x - .   ~>   .  - x-y
. . .        . x-y .
```

Operators `/` and `%` represent the quotient and remainder operations: operator `/` truncates the result towards zero;
`x%y` has the same sign as `x`.

```
. y .        .  .  .       . y .        .  .  .
x / .   ~>   .  / x/y      x % .   ~>   .  % x%y
. . .        . x/y .       . . .        . x%y .
```

Equality comparison reduces only if its two operands are equal. It works for both integers and operators.

```
. x .        . . .         . y .        . y .
x = .   ~>   . = x         x = .   ~>   x = .  (if x!=y)
. . .        . x .         . . .        . . .
```

Dually, the not-equal operator reduces only when the operands are not equal:
```
. x .        . x .         . y .        . . .
x # .   ~>   x # .         x # .   ~>   . # y  (if x!=y)
. . .        . . .         . . .        . x .
```

Operators `A` and `B` have no reduction rules defined. They may appear in the program code but they are replaced with the input values (if any) immediately after parsing.

Operator `S` ("submit") does not have any reduction rules defined, either. The program submits its results by overwriting operator `S` with the result value.

The time warp operator is described in [its own section](#time-warp).

## Scoring

Your score is the total spacetime volume of the program:
* maximal X coordinate ever used minus minimal X coordinate ever used + 1
* times (maximal Y coordinate ever used minus minimal Y coordinate ever used + 1)
* times (maximal T coordinate ever used minus minimal T coordinate ever used + 1)

In this definition, "ever used" ranges across the entire simulation
and across all time warps.

## Limits

After `1_000_000` ticks, the program is terminated without submitting a value, regardless of its current time coordinate.

## Time warp

You may be able to reduce the time complexity of your program, defined as the maximum time coordinate reached, using time travel. Time travel is triggered with the (quaternary) warp operator:

```
 .  v  .
dx  @ dy
 . dt  .
```

This rolls back the history of the board by `dt` time steps, writes value `v` into the cell with coordinates `(-dx, -dy)` relative to the `@` operator (note the negative signs!) on this past board, which means that the time coordinate of the target board is unchanged but the content is mutated. Then simulation then restarts from the modified point onward.

The minimal value of `dt` is `1`, which means stepping back one time step.

```
2 > . .        . > 2 .        2 > . .
. 2 @ 0   ~>   . 2 @ 0   ~>   2 2 @ 0
. . 1 .        . . 1 .        . . 1 .
```

### Time travel principles

1. Time is discrete and starts with `t=1` with the initial board.

2. Each tick, time `t` increases by 1, and the board is changed according to the action of all its operators.

3. The time warp operator rolls back time to any point in the past, up to and including `t=1` (the initial board).

4. Time warping with `dt=0` is not allowed.

4. After time warping to time `t`, the history before `t` is preserved but the future after `t` is discarded and its new version will be recomputed again.

5. If two different warp operators attempt to write different values into the same destination cell at the same destination time, the simulation will crash.

   Warping the same value into the same cell is fine, as is warping different values into different cells. (This is different from basic 2D reduction, which does not allow writing the same value into the same cell by two different operators.)

6. If two different warp operators attempt to travel to different times in the same tick, the simulation will crash.

7. As soon as the submit operator is overwritten, the entire simulation stops.

   A board can contain multiple submit operators but if more than one are overwritten at the same time, the simulation will crash.

8. If no operator on a board can reduce, the simulation terminates without submitting an answer.

# Example

As an example, the following program computes `A * B` by time-looping `B` times, adding `A` every time (for the sake of the example; of course there is also is the `*` operator):

```
. . . . 0 . . . .
. B > . = . . . .
. v 1 . . > . . .
. . - . . . + S .
. . . . . ^ . . .
. . v . . 0 > . .
. . . . . . A + .
. 1 @ 6 . . < . .
. . 3 . 0 @ 3 . .
. . . . . 3 . . .
```

When running this for `A = 3` and `B = 4`, this program has a spacetime volume of `320` (`vx * vy * vt = 8 * 10 * 4 = 320`). You can see the execution trace at [3d-example].

# Problems

The following problems are available:

* [3d1]
* [3d2]
* [3d3]
* [3d4]
* [3d5]
* [3d6]
* [3d7]
* [3d8]
* [3d9]
* [3d10]
* [3d11]
* [3d12]

To submit a solution, send an ICFP expression that evaluates to `solve 3dX` followed by your 3D program, e.g.:

```
solve 3dX
. . . . .
. . . . .
. . . . .
```

Your score is the sum of the spacetime complexity of your submission over the secret test cases, so a lower score is better.

# Testing

You can test your 3D programs by sending:

```
test 3d A B
. . . . .
. . . . .
. . . . .
```

where `A` and `B` must be integer values which are given to the corresponding inputs. Unlike the `solve` command, the `test` command has a tick limit of `4`.


