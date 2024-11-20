#include <stdio.h>

void bestFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];  // To store allocation for each process
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;  // Initially no process is allocated
    }

    // Allocate processes
    for (int i = 0; i < n; i++) {
        int bestIdx = -1;  // To store the index of the best block
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {  // Block can accommodate process
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx]) {
                    bestIdx = j;  // Update bestIdx to the smaller fitting block
                }
            }
        }

        // If a suitable block is found
        if (bestIdx != -1) {
            allocation[i] = bestIdx;            // Allocate block to process
            blockSize[bestIdx] -= processSize[i];  // Reduce available memory in block
        }
    }

    // Display allocation
    printf("\nProcess No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1) {
            printf("%d\n", allocation[i] + 1);
        } else {
            printf("Not Allocated\n");
        }
    }
}

int main() {
    int blockSize[] = {100, 500, 200, 300, 600};
    int processSize[] = {212, 417, 112, 426};
    int m = sizeof(blockSize) / sizeof(blockSize[0]);
    int n = sizeof(processSize) / sizeof(processSize[0]);

    bestFit(blockSize, m, processSize, n);

    return 0;
}
