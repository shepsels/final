#include <cstdlib>

#include "SPImageProc.h"

extern "C"{

	#include "SPPoint.h"
	#include "SPLogger.h"
	#include "SPConfig.h"
	#include "unistd.h"
}

using namespace sp;


int main( int argc, char *argv[] )  {

	ImageProc imageProc(config);

	// initializations
	int i;
	char* defaultFile = "spcbir.config";
	char imgPath[MAX_LEN], loggerOutput[MAX_LEN], imgDir[MAX_LEN];

	SPPoint ptArr;
	SPConfig config;
	SP_CONFIG_MSG* msg;


	// there are 3 or more arguments, thus a filename is supplied
	if (argc > 2 && strcmp(argv[1], "-c") == 0)
	{
		//config file found in argv[2]
		config = spConfigCreate(argv[2], msg);
	}
	else
	{
		// use default filename
		config = spConfigCreate(defaultFile, msg);
	}

	// create path for logger output
	spGetImageDir(imgDir, config);
	strcat(loggerOutput, imgDir);
	strcat(loggerOutput, "logger_output.txt");
	SPLogger logger = spLoggerCreate(loggerOutput, config->spLoggerLevel);


	//************* extraction mode *************\\

	if (spConfigIsExtractionMode(config, msg))
	{
		for (i = 0; i < spConfigGetNumOfImages(config, msg); i++)
		{
			imgPath = {'\0'};
			spConfigGetImagePath(imgPath, config, i);

			// checking if the file doesn't exist
			if( access( imgPath, F_OK ) == -1 )
			{
				free(config);
				//todo: what else should do here? maybe message?
				exit(1);
			}

			// file exists
			ptArr = getImageFeatures(imgPath, i, spConfigGetNumOfFeatures(config, msg));
		}

	}

	//******************************************\\

	else
	{
		// non extraction mode
		// todo
	}


	return 0;
}

