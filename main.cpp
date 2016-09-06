#include <cstdlib>
#include "SPImageProc.h"

# define EXT_MODE "entering extraction mode"

extern "C"{

	#include "SPPoint.h"
	#include "SPLogger.h"
	#include "SPConfig.h"
	#include "unistd.h"
	#include "mainAux.h"
}

using namespace sp;


int main( int argc, char *argv[] )  {

	// initializations
	int i, numOfFeats;
	char* defaultFile = "spcbir.config";
	char imgPath[MAX_LEN], loggerOutput[MAX_LEN];
	char imgDir[MAX_LEN];

	SPPoint* ptArr;
	SPConfig config;
	SP_CONFIG_MSG msg;

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
		fflush(NULL);
	}

	sp::ImageProc imageProc(config);


	// get image directory
	spGetImageDir( imgDir, config);

	//create logger output file
	strcpy(loggerOutput, imgDir);
	strcat(loggerOutput, "logger_output.txt");

	// create the logger with the correct level
	SP_LOGGER_MSG loggerMSG = spLoggerCreate(loggerOutput, spGetLoggerLevel(config));

	//todo validate logger message


	//*** extraction mode ***/

	if (spConfigIsExtractionMode(config, &msg))
	{
		//logger info
		spLoggerPrintInfo(EXT_MODE);

		for (i = 0; i < spConfigGetNumOfImages(config, &msg); i++)
		{
			printf("%d\n", spConfigGetNumOfImages(config, &msg));
			imgPath[0] = '\0';	//todo
			spConfigGetImagePath(imgPath, config, i);
			printf("image path is: %s\n", imgPath);

			// checking if the file doesn't exist
			if( access( imgPath, F_OK ) == -1 )
			{
				free(config);

				// logger error
				spLoggerPrintError("image doesn't exist", imgPath, "spConfigGetImagePath", __LINE__);
				//todo: what else should do here? maybe message?
				exit(1);
			}

			// file exists, extract features
			//logger info
			spLoggerPrintInfo("file exists, saving features to file");
			ptArr = imageProc.getImageFeatures(imgPath, i, &numOfFeats);
			saveFeatsToFile(ptArr, numOfFeats, i, config);
		}
	}
//
//	//******************************************//
//
//	else
//	{
//		// non extraction mode
//		// todo
//	}
//

	return 0;
}

