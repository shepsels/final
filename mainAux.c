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
	//todo fails - check this!
	char *token;
	double tmp;

    int cnt=0;
    /* get the first token */
    token = strtok(line, delim);

    /* walk through other tokens */
    while(cnt < dim)
    {
    	tmp = strtod(token, NULL);
    	lineArray[cnt] = tmp;
        token = strtok(NULL, delim);
        cnt++;
    }
    return lineArray;
}

SPPoint* extractImagesFromFeats(SPConfig config, int* numOfFeatures)
{
    int i,j, imgIndex, featsPerImg, currSize=0, counterAllPoints=0, featuresPerImgMat[config->spNumOfImages], cnt2;
    char feats[6] = FEATS, imgPath[MAX_LEN], line[MAX_LEN];
    char *pointerToPoint;

    FILE *file;
    SPPoint** matrixOfPoints;
    SPPoint *points;

    // initial allocation
    points = (SPPoint*)malloc(sizeof(SPPoint));
    matrixOfPoints = (SPPoint**)malloc(config->spNumOfImages*sizeof(SPPoint*));

    // validate allocation
    if (!points || !matrixOfPoints)
    {
        //logger error
        spLoggerPrintError("Allocation Failure", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    // loop over all files and extract their features
    for (i=0; i < config->spNumOfImages; i++)
    {
    	// build the correct file path, with feats suffix
        spConfigGetImagePath(imgPath, config, i);

        // set the pointer to point to the beginning of the path
        pointerToPoint = imgPath;

        // move the pointer to the point before the suffix (three characters)
        pointerToPoint += (strlen(imgPath) -3);

        // copy feats suffix to the correct place in filePath
        memcpy(pointerToPoint, feats, strlen(feats)+1);

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

        // keep numbr of features per image in an array
        featuresPerImgMat[i] = featsPerImg;

        // allocating featsPerImg space for all features per image
        matrixOfPoints[i] = (SPPoint*)malloc(featsPerImg * sizeof(SPPoint));

        // validate allocation
        if (!matrixOfPoints[i])
        {
            //logger error
            spLoggerPrintError("Allocation Failure", __FILE__, __FUNCTION__, __LINE__);
            return false;
        }

        // running over the features file and reading
        cnt2 = 0;
        while(fgets(line, MAX_LEN, file))
        {
            //create data array with all the information from the whole line
            double *data = (double*)malloc(config->spPCADimension*sizeof(double));

            if (!data)
            {
                //logger error
                spLoggerPrintError("Allocation Failure", __FILE__, __FUNCTION__, __LINE__);
                return false;
            }
            // parse the line from the file
            lineToDoubleArray(data, config->spPCADimension, line, "$"); //todo change delimiter

            // create SPPoint with the data
            SPPoint linePoint = spPointCreate(data, config->spPCADimension, imgIndex);

            // save the pointer to the correct place in points array
            matrixOfPoints[i][cnt2] = linePoint;
            counterAllPoints++;
            cnt2++;
        }
        fclose(file);
    }

    // copying the 2d matrix to an all points 1d matrix
    points = (SPPoint*)malloc(counterAllPoints*sizeof(SPPoint));
    fflush(NULL);
    printf("counter for all: %d\n", counterAllPoints);
    fflush(NULL);
    cnt2 = 0;
    for(i=0; i<config->spNumOfImages; i++)
    {
    	for (j=0; j<featuresPerImgMat[i]; j++)
    	{
    		points[cnt2] = spPointCopy(matrixOfPoints[i][j]);
    		cnt2++;
//    		cnt2++;
    		free(matrixOfPoints[i][j]);
    	}
    	free(matrixOfPoints[i]);
    }
    free(matrixOfPoints);

    *numOfFeatures = counterAllPoints;
    return points;
}

bool fromFilesToKDTree(SPConfig config,SPKDArray array, KDTreeNode *tree, SPPoint* points)
{
    int numberOfPoints;

//    todo extract all features
    points = extractImagesFromFeats(config, &numberOfPoints);

    // initialize the array
    array = init(points, numberOfPoints); //todo change name and make sure that's the correct number
    //todo validate

    // initialize the tree
    *tree = kdTreeInit(array, config->spKDTreeSplitMethod, 0);
    //todo validate


    return true;
}
