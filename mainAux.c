# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "SPConfig.h"
# include "SPPoint.h"
# include "KDTree.h"

# define DELIMITER '#'

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
	char *feats = "feats";
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

void printImages(char* queryPath, int* indexes)
{
    printf("Best candidates for - %s - are:\n", queryPath);
    for (int i = 0; i < indexes)
    {

    }
}

