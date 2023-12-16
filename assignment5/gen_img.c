#include <stdio.h>
#include <stdlib.h>



void writeImgToFile(int n, int img[n][n], char * filepath){
    FILE * fp = fopen(filepath, "w");
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++){
            fprintf(fp, "%d ", img[i][j]);
        }
        fprintf(fp, "\n");
    } 
    fclose(fp);
}

int main(int argc, char ** argv) {
    
    if(argc != 3){
        printf("Usage: ./gen_img n <filepath>\n");
        return -1;
    } 

    int n = strtol(argv[1], NULL, 10);
    int img[n][n];

    // Generate random binary image
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            img[i][j] = rand() % 2;
        }
    }

    writeImgToFile(n, img, argv[2]);

    return 0;

}

