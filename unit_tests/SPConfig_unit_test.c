# include "unit_test_util.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "../SPConfig.h"

static bool spConfigCreateTest() {
	printf("started");
    char* path = "configFile1.txt"; //todo change back
    SP_CONFIG_MSG * msg;
    SPConfig conf_a;

    // creating the config file according to the example config file
    conf_a = spConfigCreate(path, msg);

//    printf("%s%s%s%s%s%d", "should be: ", "./images/", " in struct: ", conf_a->spImagesDirectory,
//	  " diff: ", strcmp(conf_a->spImagesDirectory, "./images/"));
//    printf("%s%s%s", "./images/", "\n and this is:", conf_a->spImagesDirectory);


    // testing its elements:
    ASSERT_TRUE(strcmp(conf_a->spImagesDirectory, "./images/") == 0);
//    ASSERT_TRUE(strcmp(conf_a->spImagesPrefix, "img") == 0);
//    ASSERT_TRUE(strcmp(conf_a->spImagesSuffix, ".png") == 0);
//    ASSERT_TRUE(conf_a->spNumOfImages == 17);
//
//    // todo paz: continue checks
    return true;
}


int main() {
    printf("sdfsd");
	RUN_TEST(spConfigCreateTest);
    printf("sdfsd");
    return 0;
}
