# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "SPConfig.h"
# include "SPPoint.h"

# define DELIMITER '#'

bool saveFeatsToFile(SPPoint* imageInfo, int numberOfFeats, int index, SPConfig config)
{
	char *pointerToPoint;
	char filePath[MAX_LEN];
	SP_CONFIG_MSG msg;
	int returnVal;

	if (!imageInfo || numberOfFeats < 1 || index < 0 || config == NULL)
	{
		return false;
	}

	// get image path
	msg = spConfigGetImagePath(filePath, config, index);

	if (msg != SP_CONFIG_SUCCESS)
	{
		// couldn't receive the filepath
		spLoggerPrintError("couldn't get filepath", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

	// received successfully. now creating the output file (changing filepath's end to ".feats")
	printf("filepath is: --%d\n", strlen(filePath));

	// set the pointer to point to the beginning of the path
	pointerToPoint = filePath;

	// move the pointer to the point before the suffix (three characters)
	pointerToPoint += (strlen(filePath) -3);

	// todo change to a better solution-------------
	*pointerToPoint = 'f';
	*(pointerToPoint+1) = 'e';
	*(pointerToPoint+2) = 'a';
	*(pointerToPoint+3) = 't';
	*(pointerToPoint+4) = 's';
	*(pointerToPoint+5) = '\0';
	//todo------------------------------------------ (use sprintf)

	// todo validate

	FILE* out = fopen(filePath, "w");
	// todo validate

	returnVal = fprintf(out, "%d$%d$", index, numberOfFeats);


	printf("new path: %s\n", filePath);
	fclose(out);
	return true;
}

