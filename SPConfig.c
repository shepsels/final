# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "SPConfig.h"
# include "SPConfigAux.h"

# define DELIMITER "="
# define IM_DIR "spImagesDirectory"
# define MAX_LEN 1024


/////////////////////////////////////////////todo/////////////////////////////////////////////
// add messages to create
// check what if:      #zdfdfs


///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// Help Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\




  //////////////////////////							//////////////////////////
 ////////////////////////////////////////////////////////////////////////////////
/////////////////////////							  //////////////////////////


SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
    FILE* file;
    SPConfig config;
    char line[MAX_LEN], trimmedLine[MAX_LEN], var[MAX_LEN];
    int tempNum, lineNumber=0;
    bool isSet[4] = {false};	//todo paz: check if everything is set to false


//    if (msg == NULL)
//    {
//        return NULL;  //todo paz: check if it should be.
//    }

    // reading the filename
    file = fopen(filename, "r");
    if (file == NULL)
    {
        // cannot open the file
    	*msg = SP_CONFIG_CANNOT_OPEN_FILE;
        return NULL;
    }

    // creating the new configuration struct
    config = (SPConfig) malloc(sizeof(struct sp_config_t));

    // setting default values to variables
	config->spPCADimension = 20;
	strcpy(config->spPCAFilename, "pca.yml");
	config->spNumOfFeatures = 100;
	config->spExtractionMode = true;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spMinimalGUI = false;
	config->spLoggerLevel = 3;
	strcpy(config->spLoggerFilename, "stdout");


    // running over each line and setting its value to the correct struct element
    while (fgets(line, MAX_LEN, file))
    {
    	lineNumber++;
        // this is a comment line and shouldn't be parsed
    	//todo delete spaces

        // removes spaces before and after all other characters
//        removeSpaces(line); todo
        trimWhiteSpace(trimmedLine, line);
//        printf("%s\n",trimmedLine);

//        // the line is not a valid line
//        if(hasSpacesInVar(trimmedLine))
//        {
//        	printInvalidMessage(filename, lineNumber);
//        	freeStructMemory(config);
//        	exit(1);
//        }//todo

//        // after checking there are no spaces inside a variable, we can remove all spaces
//        removeSpaces(trimmedLine);//todo

        if (trimmedLine[0] == '#' || trimmedLine[0] == '\0' || trimmedLine[0] == '\n')
            // todo paz: maybe I should add here cond that validates there are no spaces before '#'
        {
            continue;
        }


        char *cfLine;

        // splitting the line into two parts, using '=' as delimiter
        cfLine = strstr((char*)trimmedLine, DELIMITER);
        cfLine = cfLine + strlen(DELIMITER);

        trimWhiteSpace(cfLine, cfLine);

        // reads until delimiter =
        sscanf(trimmedLine, "%[^=]", var); //todo paz: change = to DELIMITER
        trimWhiteSpace(var, var);


        // checking there are no constrains violations
        //todo


        // ---1--- var is spImagesDirectory
        if (strcmp(var,"spImagesDirectory ") == 0 || strcmp(var,"spImagesDirectory") == 0)
        {
        	// remove spaces and new line char from cfLine
        	removeSpaces(cfLine);
        	strtok(cfLine, "\n");

        	// check constraints
        	if (hasSpaces(cfLine))
        	{
        		printConstraintNotMet(filename, lineNumber);
            	freeStructMemory(config);
            	exit(1);
        	}

        	// copy cfLine to the correct attribute of config
            memcpy(config->spImagesDirectory, cfLine, strlen(cfLine));
            config->spImagesDirectory[strlen(cfLine)] = '\0';
            isSet[0] = true;
        }

        // ---2--- var is spImagesPrefix
        else if (strcmp(var,"spImagesPrefix ") == 0 || strcmp(var,"spImagesPrefix") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

        	// check constraints
        	if (hasSpaces(cfLine))
        	{
        		printConstraintNotMet(filename, lineNumber);
            	freeStructMemory(config);
            	exit(1);
        	}

			// copy cfLine to the correct attribute of config
			memcpy(config->spImagesPrefix, cfLine, strlen(cfLine));
			config->spImagesPrefix[strlen(cfLine)] = '\0';
			isSet[1] = true;
		}

        // ---3--- var is spImagesSuffix
        else if (strcmp(var,"spImagesSuffix ") == 0 || strcmp(var,"spImagesSuffix") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

        	// check constraints
        	if ((strcmp(cfLine, ".jpg") != 0)
        			&& (strcmp(cfLine, ".png") != 0)
					&& (strcmp(cfLine, ".bmp") != 0)
					&& (strcmp(cfLine, ".gif") != 0))
        	{
        		printConstraintNotMet(filename, lineNumber);
            	freeStructMemory(config);
            	exit(1);
        	}

			// copy cfLine to the correct attribute of config
			memcpy(config->spImagesSuffix, cfLine, strlen(cfLine));
			config->spImagesSuffix[strlen(cfLine)] = '\0';

			isSet[2] = true;
		}

        // ---4--- var is spNumOfImages
        else if (strcmp(var,"spNumOfImages ") == 0 || strcmp(var,"spNumOfImages") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

        	// check constraints
        	if (tempNum <= 0)
        	{
        		printConstraintNotMet(filename, lineNumber);
            	freeStructMemory(config);
            	exit(1);
        	}

			// copy cfLine to the correct attribute of config
			config->spNumOfImages = tempNum;
			isSet[3] = true;
		}

        // ---5--- var is spPCADimension
        else if (strcmp(var,"spPCADimension ") == 0 || strcmp(var,"spPCADimension") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// check constraints
        	if ((tempNum < 10) || (tempNum > 28))
        	{
        		printConstraintNotMet(filename, lineNumber);
            	freeStructMemory(config);
            	exit(1);
        	}

			// copy cfLine to the correct attribute of config
			config->spPCADimension = tempNum;
		}

        // ---6--- var is spPCAFilename
        else if (strcmp(var,"spPCAFilename ") == 0 || strcmp(var,"spPCAFilename") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

        	// check constraints
        	if (hasSpaces(cfLine))
        	{
        		printConstraintNotMet(filename, lineNumber);
            	freeStructMemory(config);
            	exit(1);
        	}

			// copy cfLine to the correct attribute of config
			memcpy(config->spPCAFilename, cfLine, strlen(cfLine));
			config->spPCAFilename[strlen(cfLine)] = '\0';

		}

        // ---7--- var is spNumOfFeatures
        else if (strcmp(var,"spNumOfFeatures ") == 0 || strcmp(var,"spNumOfFeatures") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spNumOfFeatures = tempNum;
		}

        // ---8--- var is spExtractionMode
        else if (strcmp(var,"spExtractionMode ") == 0 || strcmp(var,"spExtractionMode") == 0)
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
        else if (strcmp(var,"spNumOfSimilarImages ") == 0 || strcmp(var,"spNumOfSimilarImages") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spNumOfSimilarImages = tempNum;
		}

        // ---10--- var is spKDTreeSplitMethod
        else if (strcmp(var,"spKDTreeSplitMethod ") == 0 || strcmp(var,"spKDTreeSplitMethod") == 0)
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

        // ---11--- var is spKNN
        else if (strcmp(var,"spKNN ") == 0 || strcmp(var,"spKNN") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spKNN = tempNum;
		}

        // ---12--- var is spMinimalGUI
        else if (strcmp(var,"spMinimalGUI ") == 0 || strcmp(var,"spMinimalGUI") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// false is the default. if its false it should be changed
			if(strcmp("true", cfLine) == 0)
			{
				config->spMinimalGUI = false;
			}
		}

        // ---13--- var is spLoggerLevel
        else if (strcmp(var,"spLoggerLevel ") == 0 || strcmp(var,"spLoggerLevel") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spLoggerLevel = tempNum;
		}

        // ---14--- var is spLoggerFilename
        else if (strcmp(var,"spLoggerFilename ") == 0 || strcmp(var,"spLoggerFilename") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spLoggerFilename, cfLine, strlen(cfLine));
			config->spLoggerFilename[strlen(cfLine)] = '\0';

		}
    }
    // after reading the file, should check that all mandatory variables are initialized
	if (!isSet[0])
	{
		printConstraintNotMetWithParam(filename, lineNumber, "spImagesDirectory");
    	freeStructMemory(config);
    	exit(1);
	}
	if (!isSet[1])
	{
		printConstraintNotMetWithParam(filename, lineNumber, "spImagesPrefix");
    	freeStructMemory(config);
    	exit(1);
	}
	if (!isSet[2])
	{
		printConstraintNotMetWithParam(filename, lineNumber, "spImagesSuffix");
    	freeStructMemory(config);
    	exit(1);
	}
	if (!isSet[3])
	{
		printConstraintNotMetWithParam(filename, lineNumber, "spNumOfImages");
    	freeStructMemory(config);
    	exit(1);
	}

//	*msg = SP_CONFIG_SUCCESS; //todo fix
    return config;
}



bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (msg == NULL)
	{
		return NULL;
	}

	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	if (config->spExtractionMode)
	{
		*msg = SP_CONFIG_SUCCESS;
		return true;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return false;
	}
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (msg == NULL)
	{
		return NULL;
	}

	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	if (config->spMinimalGUI)
	{
		*msg = SP_CONFIG_SUCCESS;
		return true;
	}
	else
	{
		*msg = SP_CONFIG_SUCCESS;
		return false;
	}
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (msg == NULL)
	{
		return -1;
	}

	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	int num = config->spNumOfImages;
	*msg = SP_CONFIG_SUCCESS;
	return num;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (msg == NULL)
	{
		return -1;
	}

	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	int num = config->spNumOfFeatures;
	*msg = SP_CONFIG_SUCCESS;
	return num;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg)
{
	if (msg == NULL)
	{
		return -1;
	}

	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	int num = config->spPCADimension;
	*msg = SP_CONFIG_SUCCESS;
	return num;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index)
{
	if(imagePath == NULL || config == NULL)
	{
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (config->spNumOfImages < index)
	{
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	char num[MAX_LEN];

	strcpy(imagePath, config->spImagesDirectory);
	strcat(imagePath, config->spImagesPrefix);

	snprintf(num, 12,"%d",index); //todo check that its working
	strcat(imagePath, num);
	strcat(imagePath, config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config)
{
	if(pcaPath == NULL || config == NULL)
	{
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	char num[MAX_LEN];

	strcpy(pcaPath, config->spImagesDirectory);
	strcat(pcaPath, config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config)
{
	if(!config)
	{
		return;
	}
//	free(config->spImagesDirectory);
//	free(config->spImagesPrefix);
//	free(config->spImagesSuffix);
//	free(config->spLoggerFilename);
//	free(config->spPCAFilename);
	free(config);
	return;
}

void spGetImageDir(char* output, const SPConfig config) //todo refine
{
	if(config == NULL)
	{
		return;
	}

	strcpy(output, config->spImagesDirectory);
	return;
}

SP_LOGGER_LEVEL spGetLoggerLevel(const SPConfig config)
{
	if(config == NULL)
	{
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (config->spLoggerLevel == 1)
	{
		return SP_LOGGER_ERROR_LEVEL;
	}
	else if (config->spLoggerLevel == 2)
	{
		return SP_LOGGER_WARNING_ERROR_LEVEL;
	}
	else if (config->spLoggerLevel == 3)
	{
		return SP_LOGGER_INFO_WARNING_ERROR_LEVEL;
	}
	else
		//config->spLoggerLevel == 4
	{
		return SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
	}
}
