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
        sscanf(line, "%s", var);

        // var is spImagesDirectory
        if (var == IM_DIR)
        {

            memcpy(config->spImagesDirectory, cfLine, strlen(cfLine));
        }
    }
    return config;
}
