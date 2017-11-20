# Very-Fast-Lights-Out-Solver
I attempted to optimize the naive technique in solving lights out (BFS). No algorithm improvements were done here. My goal was to use only implementation-level optimizations.

# Optimizations

1. Board is compressed into 1 dimension.

2. Board state is stored as an 32-bit integer

3. Toggling a button is done by XOR'ing the board with a hard-coded mask. 

4. Five toggles are done in one 128-bit XOR instruction by compressing five masks into a 128-bit integer. [SWAR](https://en.wikipedia.org/wiki/SWAR)

5. Loop unrolling is done together with (4) because five toggles are processed after a single XOR.

6. Bitset is used instead of bool/cbar array as it seemed to be faster in my benchmark. I think it's because random access is slow when the array is large. Indexing in the bitset uses only bitwise operators in place of division and modulo.

7. Queue is implemented via circular array. I predetermined the maximum size.

8. I store paths from any board to empty board by storing the toggles between the two states. Since there are only 25 toggles, I can use a byte (unsigned char) to store these.

9. Loop inversion.

# Performance
The BFS finished in 1.57-1.6 seconds on my i3 2nd gen machine with -Ofast flag enabled.

Update: Speed is now 1.05-1.15 seconds after removing some unecessary operations and loop inversion.
