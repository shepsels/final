# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "SPConfig.h"

# define DELIMITER "="
# define IM_DIR "spImagesDirectory"
# define MAX_LEN 1024


//helper function to remove spaces
void removeSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
    FILE* file;
    SPConfig config;
    char line[MAX_LEN], var[MAX_LEN];
    int tempNum;

    if (msg == NULL)
    {
        return NULL;  //todo paz: check if it should be.
    }

    // reading the filename
    file = fopen(filename, "r");
    if (file == NULL)
    {
        // cannot open the file
        return SP_CONFIG_CANNOT_OPEN_FILE;
    }

    // creating the new configuration struct
    config = (SPConfig) malloc(sizeof(struct sp_config_t));

    // setting default values to variables
	config->spPCADimension = 20;
	strcpy(config->spPCAFilename, "pca.yml"); 	//todo check if need eol symbol
	config->spNumOfFeatures = 100;
	config->spExtractionMode = true;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spMinimalGUI = false;


    // running over each line and setting its value to the correct struct element
    while (fgets(line, MAX_LEN, file) != NULL)
    {
        // this is a comment line and shouldn't be parsed
        if (line[0] == '#')
            // todo paz: maybe I should add here cond that validates there are no spaces before '#'
        {
            continue;
        }

        char *cfLine;
        cfLine = strstr((char*)line, DELIMITER);
        cfLine = cfLine + strlen(DELIMITER);

        // reads first word in line
        sscanf(line, "%[^=]", var);
        printf("%s%s%s%s%s", "var is: ", var, "-- and cfLine is: ", cfLine, "\n");

        // ---1--- var is spImagesDirectory
        printf("-----------%s--------\n", var);
        if (strcmp(var,"spImagesDirectory ") == 0 || strcmp(var,"spImagesDirectory") == 0)
        {
        	// remove spaces and last line from cfLine
        	removeSpaces(cfLine);
        	strtok(cfLine, "\n");


        	// copy cfLine to the correct attribute of config
            memcpy(config->spImagesDirectory, cfLine, strlen(cfLine));
        }

        // ---2--- var is spImagesPrefix
		if (strcmp(var,"spImagesPrefix ") == 0 || strcmp(var,"spImagesPrefix") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spImagesPrefix, cfLine, strlen(cfLine));
		}

        // ---3--- var is spImagesSuffix
		if (strcmp(var,"spImagesSuffix ") == 0 || strcmp(var,"spImagesSuffix") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spImagesSuffix, cfLine, strlen(cfLine));
		}

        // ---4--- var is spNumOfImages
		if (strcmp(var,"spNumOfImages ") == 0 || strcmp(var,"spNumOfImages") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spNumOfImages = tempNum;
		}

        // ---5--- var is spPCADimension
		if (strcmp(var,"spPCADimension ") == 0 || strcmp(var,"spPCADimension") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spPCADimension = tempNum;
		}

        // ---6--- var is spPCAFilename
		if (strcmp(var,"spPCAFilename ") == 0 || strcmp(var,"spPCAFilename") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spPCAFilename, cfLine, strlen(cfLine));
		}

        // ---7--- var is spNumOfFeatures
		if (strcmp(var,"spNumOfFeatures ") == 0 || strcmp(var,"spNumOfFeatures") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spNumOfFeatures = tempNum;
		}

        // ---8--- var is spExtractionMode
		if (strcmp(var,"spExtractionMode ") == 0 || strcmp(var,"spExtractionMode") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// true is the default. if its false it should be changed
			if(strcmp("false", cfLine) == 0)
			{
				config->spExtractionMode = false;
			}
		}

        // ---9--- var is spNumOfSimilarImages
		if (strcmp(var,"spNumOfSimilarImages ") == 0 || strcmp(var,"spNumOfSimilarImages") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spKDTreeSplitMethod = tempNum;
		}

        // ---10--- var is spKDTreeSplitMethod
		if (strcmp(var,"spKDTreeSplitMethod ") == 0 || strcmp(var,"spKDTreeSplitMethod") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// set the correct enum value to config
		    if(strcmp(cfLine, "RANDOM") == 0)
		    {
				  config->spKDTreeSplitMethod = RANDOM;
		    }
		    if(strcmp(cfLine, "MAX_SPREAD") == 0)
		    {
				  config->spKDTreeSplitMethod = MAX_SPREAD;
		    }
		    if(strcmp(cfLine, "INCREMENTAL") == 0)
		    {
				  config->spKDTreeSplitMethod = INCREMENTAL;
		    }
		}


    }


    return config;
}
