
/*
    Compile with -Ofast flag.
    Kendrick timothy b. mercado
    this builds the bfs tree of lights out with efficient data structures and low-level optimiations
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
typedef unsigned int uint;
typedef unsigned long long u64;
typedef unsigned __int128 u128;

#define N 5
#define STATES (1 << (N * N))

// unpacked toggle masks
uint mask[] = {
    35, 71, 142, 284, 536, 1121, 2274, 4548, 9096, 17168, 35872, 72768, 145536, 291072, 549376,
    1147904, 2328576, 4657152, 9314304, 17580032, 3178496, 7405568, 14811136, 29622272, 25690112
};

// packed toggle masks
u128 cmask[5] = {
    443677736902923512 * (u128) (1e14)
    +139394679320,

    142103640876514133 * (u128) (1e16)
    +6035471552168720,

    454731650804845227 * (u128) (1e17)
    +53135089669399040,

    145514128257550472 * (u128) (1e19)
    +8100322869420769280,

    402922264199749179 * (u128) (1e19)
    +4093243870648729600
};

unsigned char prv[STATES];
int size = 0;
int start = 0;
#define lim 2097152

// Queue via Circular array
int q[lim];
int pop() {
    size--;
    return q[start++ & (lim-1)];
}
void push(int x) {
    q[(start + size++) & (lim-1)] = x;
}

// in my benchmark, bitset tends to be faster than char/int array.
unsigned char visited[1<<22] = {0};
#define visit(i) visited[i >> 3] |= 1 << (i & 7)
#define test(i) ((visited[i >> 3] & (1 << (i & 7))) != 0)

void bfs() {
    int i, j;
    push(0);
    visit(0);
    while (size != 0) {
        u128 u = pop();
        u = u | u << 25 | u << 50 | u << 75 | u << 100;
        for (i = 4; i >= 0; --i) {
            // loop unrolling x5 + packed 128-bit XOR
            u128 x = u ^ cmask[i];
            int a = x >> 100;
            int b = (x >> 75)  & 0x1FFFFFF;
            int c = (x >> 50)  & 0x1FFFFFF;
            int d = (x >> 25)  & 0x1FFFFFF;
            int e = x & 0x1FFFFFF;
            j = i * 5;
            if (!test(a)) { visit(a); prv[a] = j  ; push(a); }
            if (!test(b)) { visit(b); prv[b] = j+1; push(b); }
            if (!test(c)) { visit(c); prv[c] = j+2; push(c); }
            if (!test(d)) { visit(d); prv[d] = j+3; push(d); }
            if (!test(e)) { visit(e); prv[e] = j+4; push(e); }
        }
    }
}

void render(uint board) {
    for (uint i = 0; i < N; ++i) {
        for (uint j = 0; j < N; ++j) {
            printf("%d", (board & (1 << ((i * N) + j))) != 0);
        }
        printf("\n");
    }
}

uint open() {
    FILE *f = fopen("lightsout.in", "r");
    uint board = 0;
    for (uint i = 0; i < N * N; ++i) {
        uint c = fgetc(f);
        if (c!='0'&&c!='1') {
            i--; continue;
        }
        board |= (c-'0') * (1 << i);
    }
    fclose(f);
    return board;
}

int main() {
    memset(prv, 25, sizeof prv);
    clock_t start = clock();
    bfs();
    printf("%.2f\n", (double) (clock() - start) / CLOCKS_PER_SEC);
    uint board = open();
    render(board);
    printf("\n");
    while (board != 0) {
        if (prv[board] == 25) {
            printf("Impossible\n");
            break;
        }
        board ^= mask[prv[board]];
        render(board);
        printf("\n");
    }
}
