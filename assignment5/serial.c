#include <stdio.h>
#include <stdlib.h>

#define min(a,b) ((a)<(b))?(a):(b)
#define max(a,b) ((a)>(b))?(a):(b)

void printImg(int n, int img[n][n]){
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            printf("%d ", img[i][j]);
        }
        printf("\n");
    } 
}

// This is a serial implementation of the component labeling algorithm
int main(int argc, char ** argv) {
    
    if(argc != 2){
        printf("Usage: ./serial n\n");
        return -1;
    } 

    int n = strtol(argv[1], NULL, 10);
    int img[n][n];


    // Get Input
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            scanf("%d", &img[i][j]);
        }
        
    }
    

    // Labeling
    int label = 2;

    // We need at most n labels, but just to be safe I am using n*n
    int labelMap[n*n];


    // Initialize labelMap
    for (size_t i = 0; i < n*n; i++)
    {
        labelMap[i] = 0;
    }

    // First pass
    // We only need to check the top and left neighbors, 
    // if no match, assign new label

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
                
                // If the pixel is 0, skip
                if(img[i][j] == 0){
                    continue;
                }
    
                // Check top neighbor
                // We won't check the top neighbor if we are in the first row
                if(i > 0 && img[i-1][j] > 1){
                    img[i][j] = img[i-1][j];
                }
    
                // Check left neighbor
                // We won't check the left neighbor if we are in the first column
                if(j > 0 && img[i][j-1] > 1){
                    img[i][j] = img[i][j-1];
                }

                // Check if there is a conflict
                // Conflict does not happen in the first row or first column
                if(i>0 && j>0){
                    if(img[i-1][j] > 1 && img[i][j-1] > 1 && img[i-1][j] != img[i][j-1]){
                        // We have a conflict
                        // Because my current element and the top and left neighbors are not the same.
                        // We need to update the labelMap

                        int min_label = min(img[i-1][j], img[i][j-1]);
                        int max_label = max(img[i-1][j], img[i][j-1]);

                        // Update the labelMap with the minimum label
                        labelMap[max_label] = min_label;
                        img[i][j] = min_label;
                    }
                }

                // If no match, assign new label
                if(img[i][j] == 1){
                    img[i][j] = label;
                    labelMap[label] = label;
                    label++;
                }

        }
        
    }

    // second pass
    // Use the labelmap
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(img[i][j] > 1){
                int label = img[i][j];
                while(labelMap[label] != label){
                    label = labelMap[label];
                }
                labelMap[img[i][j]] = label;
                img[i][j] = label;

            }
        }
    }
    
    printImg(n, img);


    return 0;
}