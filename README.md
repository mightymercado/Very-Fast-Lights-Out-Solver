# Very-Fast-Lights-Out-Solver
I attempted to optimize the naive technique in solving lights out (BFS).

#Optimizations

1. Board is compressed into 1 dimension.

2. Board state is stored as an integer

3. Toggling a button is done by XOR'ing the board with a hard-coded mask.

4. Two toggles are done in one XOR instruction via compressed 64-bit masks.

5. Loop unrolling is done together with (4) as it reduces XOR operations by a factor of 2.

6. Bitset is used instead of bool/cbar array as it seemed to be faster in my benchmark. I think it's because random access is slow when the array is large.

7. Queue is implemented via circular array. I predetermined the maximum size.

8. I store paths from any board to empty board by storing the toggles between the two states. Since there are only 25 toggles, I can use a byte (unsigned char) to store these.
