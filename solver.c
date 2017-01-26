
/*
    Compile with -Ofast flag.
    Kendrick timothy b. mercado
    this builds the bfs tree of lights out with efficient data structures and low-level optimiations
*/
#include <stdio.h>
#include <time.h>
typedef unsigned int uint;
typedef unsigned long long u64;

#define N 5
#define STATES (1 << (N * N))

// unpacked toggle masks
uint mask[] = {
    35, 71, 142, 284, 536, 1121, 2274, 4548, 9096, 17168, 35872, 72768, 145536, 291072, 549376,
    1147904, 2328576, 4657152, 9314304, 17580032, 3178496, 7405568, 14811136, 29622272, 25690112
};
                
// packed toggle masks
u64 cmask[13] = {
    150323855431,
    609885356316,
    2302102471777,
    9766755635652,
    39067022541584,
    154069066914880,
    625072360681728,
    2359551954355200,
    10001157770907648,
    40004631082582016,
    13651536377872384,
    63613344766230528,
    110338190870577152
};

unsigned char prv[STATES];
int size = 0;
int start = 0;
#define lim 2019953

// Queue via Circular array
int q[lim];
int pop() {
    size--;
    return q[start++ % lim];
}
void push(int x) {
    q[(start + size++) % lim] = x;
}

// in my benchmark, bitset tends to be faster than char/int array.
u64 visited[1<<19] = {0};
#define visit(i) visited[i >> 6] |= 1 << (i & 63)
#define test(i) ((visited[i >> 6] & (1 << (i & 63))) != 0)

void bfs() {
    push(0);
    visit(0);
    while (size != 0) {
        u64 u = pop();
        for (uint i = 0; i < 13; ++i) {
            // loop unrolling x2 + packed 64-bit XOR
            u64 x = (u | (u << 32)) ^ cmask[i];
            int a = x >> 32;
            int b = x;
            if (!test(a)) { visit(a); prv[a] = i << 1; push(a); }
            if (!test(b)) { visit(b); prv[b] = (i << 1) + 1; push(b); }
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
    clock_t start = clock();
    bfs();
    printf("%.2f\n", (double) (clock() - start) / CLOCKS_PER_SEC);
    uint board = open();
    render(board);
    printf("\n");
    while (board != 0) {
        board ^= mask[prv[board]];
        render(board);
        printf("\n");
    }
}