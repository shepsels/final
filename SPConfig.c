# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "SPConfig.h"

# define DELIMITER "="
# define IM_DIR "spImagesDirectory"
# define MAX_LEN 1024



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
	config->spLoggerLevel = 3;
	strcpy(config->spLoggerFilename, "stdout");


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

        // reads until delimiter =
        sscanf(line, "%[^=]", var); //todo paz: change = to DELIMITER

        printf("var is --%s-- and cfLine is --%s--\n", var, cfLine);

        // checking there are no constrains violations



        // ---1--- var is spImagesDirectory
        if (strcmp(var,"spImagesDirectory ") == 0 || strcmp(var,"spImagesDirectory") == 0)
        {
        	// remove spaces and new line char from cfLine
        	removeSpaces(cfLine);
        	strtok(cfLine, "\n");

        	// copy cfLine to the correct attribute of config
            memcpy(config->spImagesDirectory, cfLine, strlen(cfLine));
        }

        // ---2--- var is spImagesPrefix
        else if (strcmp(var,"spImagesPrefix ") == 0 || strcmp(var,"spImagesPrefix") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spImagesPrefix, cfLine, strlen(cfLine));
		}

        // ---3--- var is spImagesSuffix
        else if (strcmp(var,"spImagesSuffix ") == 0 || strcmp(var,"spImagesSuffix") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spImagesSuffix, cfLine, strlen(cfLine));
		}

        // ---4--- var is spNumOfImages
        else if (strcmp(var,"spNumOfImages ") == 0 || strcmp(var,"spNumOfImages") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spNumOfImages = tempNum;
		}

        // ---5--- var is spPCADimension
        else if (strcmp(var,"spPCADimension ") == 0 || strcmp(var,"spPCADimension") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");
			tempNum = atoi(cfLine);

			// copy cfLine to the correct attribute of config
			config->spPCADimension = tempNum;
		}

        // ---6--- var is spPCAFilename
        else if (strcmp(var,"spPCAFilename ") == 0 || strcmp(var,"spPCAFilename") == 0)
		{
			// remove spaces and last line from cfLine
			removeSpaces(cfLine);
			strtok(cfLine, "\n");

			// copy cfLine to the correct attribute of config
			memcpy(config->spPCAFilename, cfLine, strlen(cfLine));
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
			config->spKDTreeSplitMethod = tempNum;
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
		}


    }


    return config;
}



///////////////////////////////// Help Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// todo paz: add functions to header file or to another file
size_t trimwhitespace(char *out, const char *str)
{
	int len = 0;

	// count
	while (str[len] != '\0')
	{
		len++;

	}
	if(len == 0)
	return 0;

	const char *end;
	size_t out_size;

	// Trim leading space
	while(isspace(*str)) str++;

	// All spaces?
	if(*str == 0)
	{
	*out = 0;
	return 1;
	}

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;
	end++;

	// Set output size
	out_size = (end - str) < len-1 ? (end - str) : len-1;

	// Copy trimmed string
	memcpy(out, str, out_size);
	out[out_size] = 0;

	return out_size;
}

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

bool hasSpaces(const char *s) {
  while (*s != '\0') {
    if (isspace(*s))
      return true;
    s++;
  }
  return false;
}

bool hasSpacesInVar(const char *s) {
	bool foundEqualSign = false;
	while (*s != '\0') {
    if (isspace(*s)){
    	//check if in format of "<a> = <b>"
    	if((*(s+1) == '=') && (isspace(*(s+2))) && !foundEqualSign) {
    		s+=2;
    		foundEqualSign = true;
    	}
    	//check if in format of "<a> =<b>"
    	else if((*(s+1) == '=') && !foundEqualSign) {
    		s++;
    		foundEqualSign = true;
    	}
    	else if((*(s-1) == '=') && !foundEqualSign) {
    		foundEqualSign = true;
    	}
    	else {
    		return true;
    	}
    }
    s++;
  }
  return false;
}

