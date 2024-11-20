#include <stdio.h>

void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];  // To store allocation for each process
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;  // Initially no process is allocated
    }

    // Allocate processes
    for (int i = 0; i < n; i++) {
        int worstIdx = -1;  // To store the index of the worst block
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {  // Block can accommodate process
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;  // Update worstIdx to the largest fitting block
                }
            }
        }

        // If a suitable block is found
        if (worstIdx != -1) {
            allocation[i] = worstIdx;            // Allocate block to process
            blockSize[worstIdx] -= processSize[i];  // Reduce available memory in block
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

    worstFit(blockSize, m, processSize, n);

    return 0;
}
