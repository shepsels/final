#include <cstdlib>
#include "SPImageProc.h"

# define EXT_MODE "entering extraction mode"

extern "C"{

	#include "SPPoint.h"
	#include "SPLogger.h"
	#include "SPConfig.h"
	#include "unistd.h"
	#include "mainAux.h"
    #include "SPBPriorityQueue.h"
	#include "query.h"
	#include "KDArray.h"
	#include "KDTree.h"
	#include "query.h"
	#include "SPBPriorityQueue.h"
}

using namespace sp;


int main( int argc, char *argv[] )  {

	// initializations
	int i, numOfFeats, queryNumOfFeats;
	char* defaultFile = "spcbir.config";
	char imgPath[MAX_LEN], loggerOutput[MAX_LEN], imgDir[MAX_LEN], imagePath[MAX_LEN];
    int bestImagesIndexes[MAX_LEN];
    imageCounter* imagesCnt;

	SPPoint* ptArr, queryFeats;
	SPConfig config;
	SP_CONFIG_MSG msg;
    SPBPQueue queue;

	//*** creating the config file ***//
	// there are 3 or more arguments, thus a filename is supplied
	if (argc > 2 && strcmp(argv[1], "-c") == 0)
	{
		//config file found in argv[2]
		config = spConfigCreate(argv[2], &msg);
	}
	else
	{
		// use default filename
		config = spConfigCreate(defaultFile, &msg);
	}

    // validate creation
    if (!config)
    {
        freeAndExit(config, NULL, NULL);
    }

    // initialize ImageProc
	sp::ImageProc imageProc(config);

	// get image directory
	spGetImageDir( imgDir, config);

	//create logger output file
	strcpy(loggerOutput, imgDir);
	strcat(loggerOutput, "logger_output.txt");

	// create the logger with the correct level
	SP_LOGGER_MSG loggerMSG = spLoggerCreate(loggerOutput, spGetLoggerLevel(config));

    // validate logger creation
	if(loggerMSG != SP_LOGGER_SUCCESS)
    {
        freeAndExit(config, NULL, NULL);
    }

    // logger creation info message
    spLoggerPrintInfo("logger created successfully");

    //*** extraction mode ***/

	if (spConfigIsExtractionMode(config, &msg))
	{
		//logger info
		spLoggerPrintInfo(EXT_MODE);

        // loop over images
		for (i = 0; i < spConfigGetNumOfImages(config, &msg); i++)
		{
			printf("%d\n", spConfigGetNumOfImages(config, &msg)); //todo delete
			imgPath[0] = '\0';	//todo
			spConfigGetImagePath(imgPath, config, i);
			printf("image path is: %s\n", imgPath); //todo delete

			// checking if the file doesn't exist
			if( access( imgPath, F_OK ) == -1 )
			{
                // logger error
                spLoggerPrintError("image doesn't exist", imgPath, "spConfigGetImagePath", __LINE__);

				freeAndExit(config, NULL, NULL);
			}

			// file exists, extract features
			ptArr = imageProc.getImageFeatures(imgPath, i, &numOfFeats);

			//save images to file
			if(!saveFeatsToFile(ptArr, numOfFeats, i, config))
            {
                // logger error
                spLoggerPrintError("couldn't create features file", imgPath, "saveFeatsToFile", __LINE__);
                freeAndExit(config, NULL, NULL);
            }
		}
		//logger info
		spLoggerPrintInfo("All features extracted successfully");
	}

	//******************************************//

	// non extraction mode
	SPPoint * allPoints;

	SPKDArray array;
	KDTreeNode tree;

	bool isExtracted = fromFilesToKDTree(config, array, tree, allPoints); //todo this is bool. validate later
	if(!isExtracted)
	{
		//todo freeandexit
		printf("ERROR!!");
	}


		/////////////////////////////////////////// query

	printf("Please enter an image path:\n");
	scanf("%s", imagePath);

    // memory allocation for the images counter array
    imagesCnt = (imageCounter *)malloc(config->spNumOfImages* sizeof(struct image_counter));

    queryFeats = imageProc.getImageFeatures(imagePath, -1, &queryNumOfFeats);

    queue = spBPQueueCreate(config->spKNN);

    for(i=0; i<queryNumOfFeats; i++)
    {
        spBPQueueClear(queue);
        kNearestNeighbors(tree, queue, queryFeats[i]);


    }


    // todo: here we have the indexes of the best images



    // minimal GUI
    if(config->spMinimalGUI)
    {
        // running over all images in queue
        while(spBPQueueSize(queue))
        {
            spBPQueuePeek(queue);
        }

        for(i=0; i < config->spKNN; i++)
        {
            //build path for every image //todo check if needed, or getting the paths
            imgPath[0] = '\0';
            spConfigGetImagePath(imgPath, config, bestImagesIndexes[i]);
            imageProc.showImage(imgPath);
        }
    }
    // non minimal GUI
    else
    {

    }


	return 0;
}

