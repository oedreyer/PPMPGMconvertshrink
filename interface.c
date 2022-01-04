#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _pixel {
    int red;
    int green;
    int blue;
} Pixel;

typedef struct _imagePPM {
    char magic[3]; 
    int numRows; 
    int numCols; 
    int maxVal; 
    Pixel **pixels; 
} ImagePPM;

typedef struct _imagePGM {
    char magic[3];
    int numRows;
    int numCols; 
    int maxVal; 
    int **pixels;
} ImagePGM;

ImagePPM *readPPM(char *filename);
int writePPM(ImagePPM *pImagePPM, char *filename);
void freePPM(ImagePPM *pImagePPM);

ImagePGM *readPGM(char *filename);
int writePGM(ImagePGM *pImagePGM, char *filename);
void freePGM(ImagePGM *pImagePGM);

ImagePGM *convertToPGM(ImagePPM *pImagePPM);
ImagePGM *shrinkPGM(ImagePGM *pImagePGM);
ImagePPM *shrinkPPM(ImagePPM *pImagePPM);

ImagePPM *readPPM(char *filename)
{
    FILE *inputFile = fopen(filename, "r");
    if(!inputFile) return NULL;
    ImagePPM *newImagePPM = (ImagePPM *)malloc(sizeof(ImagePPM));
    fscanf(inputFile, "%s%d%d%d", newImagePPM->magic, &(newImagePPM->numCols), &(newImagePPM->numRows),  &(newImagePPM->maxVal));
    newImagePPM->pixels = (Pixel**)malloc(sizeof(Pixel*) * newImagePPM->numRows);
    for (int i = 0; i < newImagePPM->numRows; i++){
        newImagePPM->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * newImagePPM->numCols);
    }
    for(int row = 0; row < newImagePPM->numRows; row++){
        for(int col = 0; col < newImagePPM->numCols; col++){
            fscanf(inputFile, "%d%d%d", &(newImagePPM->pixels[row][col].red), &(newImagePPM->pixels[row][col].green), &(newImagePPM->pixels[row][col].blue));
        }
    }
    fclose(inputFile);
    return newImagePPM;
}
int writePPM(ImagePPM *pImagePPM, char *filename)
{
    FILE *outputFile = fopen(filename, "w");
    if(!outputFile) return 0;
    fprintf(outputFile, "%s\n%d %d\n%d\n", pImagePPM->magic, pImagePPM->numCols, pImagePPM->numRows, pImagePPM->maxVal);
    for(int row = 0; row < pImagePPM->numRows; row++){
        for(int col = 0; col < pImagePPM->numCols; col++){
            fprintf(outputFile, " %3d %3d %3d \t", pImagePPM->pixels[row][col].red, pImagePPM->pixels[row][col].green, pImagePPM->pixels[row][col].blue);
        }
        fprintf(outputFile, "\n");
    }
    fclose(outputFile);
    return 1;
}
void freePPM(ImagePPM *pImagePPM)
{
    for (int i = 0; i < pImagePPM->numRows; i++){
        free(pImagePPM->pixels[i]);
    }
    free(pImagePPM->pixels);
    free(pImagePPM);
    return;
}
ImagePGM *readPGM(char *filename)
{
    FILE *inputFile = fopen(filename, "r");
    if(!inputFile){
        return NULL;
    }
    ImagePGM *newImagePGM = (ImagePGM *)malloc(sizeof(ImagePGM));
    fscanf(inputFile, "%s%d%d%d", newImagePGM->magic, &(newImagePGM->numCols), &(newImagePGM->numRows),  &(newImagePGM->maxVal));
    newImagePGM->pixels = (int**)malloc(sizeof(int*) * newImagePGM->numRows);
    for (int i = 0; i < newImagePGM->numRows; i++){
        newImagePGM->pixels[i] = (int*)malloc(sizeof(int) * newImagePGM->numCols);
    }
    for(int row = 0; row < newImagePGM->numRows; row++){
        for(int col = 0; col < newImagePGM->numCols; col++){
            fscanf(inputFile, "%d", &(newImagePGM->pixels[row][col]));
        }
    }
    fclose(inputFile);
    return newImagePGM;
}
int writePGM(ImagePGM *pImagePGM, char *filename)
{
    FILE *outputFile = fopen(filename, "w");
    if(!outputFile) return 0;
    fprintf(outputFile, "%s\n%d %d\n%d\n", pImagePGM->magic, pImagePGM->numCols, pImagePGM->numRows, pImagePGM->maxVal);
    for(int row = 0; row < pImagePGM->numRows; row++){
        for(int col = 0; col < pImagePGM->numCols; col++){
            fprintf(outputFile, " %3d", pImagePGM->pixels[row][col]);
        }
        fprintf(outputFile, "\n");
    }
    fclose(outputFile);
    return 1;
}
void freePGM(ImagePGM *pImagePGM)
{
    for (int i = 0; i < pImagePGM->numRows; i++){
        free(pImagePGM->pixels[i]);
    }
    free(pImagePGM->pixels);
    free(pImagePGM);
    return;
}
ImagePGM *convertToPGM(ImagePPM *pImagePPM)
{
    ImagePGM *newImagePGM = (ImagePGM *)malloc(sizeof(ImagePGM));
    strcpy(newImagePGM->magic,"P2\0");
    newImagePGM->numRows = pImagePPM->numRows;
    newImagePGM->numCols = pImagePPM->numCols;
    newImagePGM->maxVal = pImagePPM->maxVal;
    newImagePGM->pixels = (int**)malloc(sizeof(int*) * newImagePGM->numRows);
    for (int i = 0; i < newImagePGM->numRows; i++){
        newImagePGM->pixels[i] = (int*)malloc(sizeof(int) * newImagePGM->numCols);
    }
    for(int row = 0; row < newImagePGM->numRows; row++){
        for(int col = 0; col < newImagePGM->numCols; col++){
            newImagePGM->pixels[row][col] = (((pImagePPM->pixels[row][col].red) + 
            (pImagePPM->pixels[row][col].green) + (pImagePPM->pixels[row][col].blue))/3);
        }
    }
    return newImagePGM;
}
ImagePGM *shrinkPGM(ImagePGM *pImagePGM)
{
    int rowIndex = 0, colIndex = 0;
    ImagePGM *newImagePGM = (ImagePGM*)malloc(sizeof(ImagePGM));
    strcpy(newImagePGM->magic, "P2\0");
    newImagePGM->maxVal = pImagePGM->maxVal;
    newImagePGM->numRows = pImagePGM->numRows / 2;
    newImagePGM->numCols = pImagePGM->numCols / 2;
    newImagePGM->pixels = (int**)malloc(sizeof(int*) * newImagePGM->numRows);
    for (int i = 0; i < newImagePGM->numRows; i++){
        newImagePGM->pixels[i] = (int*)malloc(sizeof(int) * newImagePGM->numCols);
    }
    for (int row = 0; row < newImagePGM->numRows * 2; row = row + 2){
        for (int col = 0; col < newImagePGM->numCols * 2; col = col + 2){
            newImagePGM->pixels[rowIndex % (newImagePGM->numRows)][colIndex % (newImagePGM->numCols)] = (((pImagePGM->pixels[row][col])+(pImagePGM->pixels[row+1][col])+(pImagePGM->pixels[row][col+1])+(pImagePGM->pixels[row+1][col+1]))/4);
            colIndex++;
        }   
        rowIndex++;
    }
    return newImagePGM;
}


int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Usage: %s copy-ppm input.ppm output.ppm\n", argv[0]);
		printf("       %s copy-pgm input.pgm output.pgm\n", argv[0]);
		printf("       %s grayscale input.ppm output.pgm\n", argv[0]);
		printf("       %s shrink-pgm input.pgm output.pgm\n", argv[0]);
		printf("       %s shrink-ppm input.ppm output.ppm\n", argv[0]);
		return 1;
	}

	char *command = argv[1];
	char *inputFilename = argv[2];
	char *outputFilename = argv[3];

	if (strcmp(command, "copy-ppm") == 0)
	{
		ImagePPM *pImagePPM = readPPM(inputFilename);
		if (pImagePPM == NULL)
		{
			printf("Unable to read the PPM file: %s\n", inputFilename);
			return 2;
		}
		int success = writePPM(pImagePPM, outputFilename);
		if (!success)
		{
			printf("Unable to write to the file: %s\n", outputFilename);
			freePPM(pImagePPM);
			return 3;
		}
		freePPM(pImagePPM);
	}
	else if (strcmp(command, "copy-pgm") == 0)
	{
		ImagePGM *pImagePGM = readPGM(inputFilename);
		if (pImagePGM == NULL)
		{
			printf("Unable to read the PGM file: %s\n", inputFilename);
			return 4;
		}
		int success = writePGM(pImagePGM, outputFilename);
		if (!success)
		{
			printf("Unable to write to the file: %s\n", outputFilename);
			freePGM(pImagePGM);
			return 5;
		}
		freePGM(pImagePGM);
	}
	else if (strcmp(command, "grayscale") == 0)
	{
		ImagePPM *pImagePPM = readPPM(inputFilename);
		if (pImagePPM == NULL)
		{
			printf("Unable to read the PPM file: %s\n", inputFilename);
			return 6;
		}
		ImagePGM *pImagePGM = convertToPGM(pImagePPM);
		int success = writePGM(pImagePGM, outputFilename);
		if (!success)
		{
			printf("Unable to write to the file: %s\n", outputFilename);
			freePPM(pImagePPM);
			freePGM(pImagePGM);
			return 7;
		}
		freePPM(pImagePPM);
		freePGM(pImagePGM);
	}
	else if (strcmp(command, "shrink-pgm") == 0)
	{
		ImagePGM *pOrig = readPGM(inputFilename);
		if (pOrig == NULL)
		{
			printf("Unable to read the PGM file: %s\n", inputFilename);
			return 8;
		}
		ImagePGM *pShrink = shrinkPGM(pOrig);
		int success = writePGM(pShrink, outputFilename);
		if (!success)
		{
			printf("Unable to write to the file: %s\n", outputFilename);
			freePGM(pOrig);
			freePGM(pShrink);
			return 9;
		}
		freePGM(pOrig);
		freePGM(pShrink);
	}
	else
	{
	   printf("Unrecognized command\n");
	   return 10;
	}

	return 0;
}