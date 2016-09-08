# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "SPConfig.h"
# include "SPPoint.h"
# include "KDTree.h"
# include "KDArray.h"

# define DELIMITER '#'
# define FEATS "feats"

void freeAndExit(SPConfig config, KDTreeNode tree, int* images)
{
	// destroy logger
	spLoggerDestroy();

	//destroy SPConfig
	if (config)
	{
		spConfigDestroy(config);
	}

	// destroy tree
	if (tree)
	{
		kdTreeDestroy(tree);
	}
	free(images);
	exit(1);
}

bool saveFeatsToFile(SPPoint* imageInfo, int numberOfFeats, int index, SPConfig config)
{
	char *pointerToPoint;
	char *feats = FEATS;
	char filePath[MAX_LEN];
	int returnVal, i, j;

	SP_CONFIG_MSG msg;
	SPPoint curr;

	if (!imageInfo || numberOfFeats < 1 || index < 0 || config == NULL)
	{
		return false;
	}

	// get image path
	msg = spConfigGetImagePath(filePath, config, index);

	if (msg != SP_CONFIG_SUCCESS)
	{
		// couldn't receive the filepath
		spLoggerPrintError("Couldn't get filepath", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

	// received successfully. now creating the output file (changing filepath's end to ".feats")
	// set the pointer to point to the beginning of the path
	pointerToPoint = filePath;

	// move the pointer to the point before the suffix (three characters)
	pointerToPoint += (strlen(filePath) -3);

    // copy feats suffix to the correct place in filePath
    memcpy(pointerToPoint, feats, strlen(feats)+1);
    printf("new path is: %s\n", filePath); //todo delete

	FILE* out = fopen(filePath, "w");
	if (!out)
    {
        //logger error
        spLoggerPrintError("Output file doesn't exist", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

	// write to file the index and number of features, each in a new line
    if(fprintf(out, "%d\n%d\n", index, numberOfFeats)<0)
    {
    	// logger error
		spLoggerPrintError("Couldn't write to file", __FILE__, __FUNCTION__, __LINE__);
		return false;
    }

	for(i=0; i < numberOfFeats; i++)
		//todo document and make easier to read.also perform checks
	{
		curr = imageInfo[i];
		for(j=0; j<spPointGetDimension(curr); j++)
		{
			if(fprintf(out, "%f$", spPointGetAxisCoor(curr, j)) < 0)
			{
		    	// logger error
				spLoggerPrintError("Couldn't write to file", __FILE__, __FUNCTION__, __LINE__);
				return false;
			}
		}
		if(fprintf(out, "%s", "\n") < 0)
		{
	    	// logger error
			spLoggerPrintError("Couldn't write to file", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
	}
	fclose(out);
	return true;
}

//void printImages(char* queryPath, int* indexes)
//{
//    printf("Best candidates for - %s - are:\n", queryPath);
//    for (int i = 0; i < indexes)
//    {
//
//    }
//}

bool lineToDoubleArray (double* lineArray, int dim, char* line, char* delim)
{
	char *token;
	double tmp;

    //todo validate function
    int cnt=0;
    /* get the first token */
    token = strtok(line, delim);

    /* walk through other tokens */
    while(token)
    {
    	lineArray[cnt] = strtod(token, NULL);

//        printf("token: %s and lineArray[]: %s\n", token, lineArray[cnt]);
        token = strtok(NULL, delim);
        cnt++;
    }
    return lineArray;
}

bool extractImagesFromFeats(SPConfig config, int numberOfPoints, SPPoint* points)
{
    int i, imgIndex, featsPerImg, currSize=0;
    char *path = config->spImagesDirectory, *feats = FEATS;
    char *pointerToPoint;
    char imgPath[MAX_LEN], line[MAX_LEN];
    FILE *file;
    const char delim[2] = "$";
    char *token;

    // loop over all files and extract their features
    for (i=0; i < config->spNumOfImages; i++)
    {
        spConfigGetImagePath(imgPath, config, i);

        // set the pointer to point to the beginning of the path
        pointerToPoint = imgPath;

        // move the pointer to the point before the suffix (three characters)
        pointerToPoint += (strlen(imgPath) -3);

        // copy feats suffix to the correct place in filePath
        memcpy(pointerToPoint, feats, strlen(feats)+1);
        printf("new path is: %s\n", imgPath); //todo delete

        // open features file for reading
        file = fopen(imgPath, "r");
        if (!file)
        {
            //logger error
            spLoggerPrintError("Features file doesn't exist", __FILE__, __FUNCTION__, __LINE__);
            return false;
        }

        // read first time - the index
        fgets(line, MAX_LEN, file);
        imgIndex = atoi(line);

        // read second time - number of features
        fgets(line, MAX_LEN, file);
        featsPerImg = atoi(line);

        // re-allocate point array size by the number of features per image
        currSize += featsPerImg;
        points = (SPPoint*)realloc(points, currSize * sizeof(SPPoint*));

        printf("index is: %d, and number of feats is: %d\n", imgIndex, featsPerImg); //todo delete

        // running over the features file and reading
        while(fgets(line, MAX_LEN, file))
        {
            SPPoint dataLine;
        }
    }


    return true;
}

bool fromFilesToKDTree(SPConfig config,SPKDArray array, KDTreeNode tree, SPPoint* points)
{
    int numberOfPoints;

    //todo extract all features
//    points = extractImagesFromFeats(config, numberOfPoints, points);

    // initialize the array
//    array = init(allImages, allImagesSize); //todo change name and make sure that's the correct number
//    //todo validate
//
//    // initialize the tree
//    tree = kdTreeInit(array, config->spKDTreeSplitMethod, allImagesSize);
    //todo validate


    return true;
}
