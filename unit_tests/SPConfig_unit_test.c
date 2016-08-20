# include "unit_test_util.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "../SPConfig.h"

static bool spConfigCreateTest() {
    char* path = "configFile1.txt";
    SP_CONFIG_MSG * msg;
    SPConfig conf_a;

    // creating the config file according to the example config file
    conf_a = spConfigCreate(path, msg);

    // testing its elements:
    if (conf_a) {
    	printf("ok\n");
    }
    ASSERT_TRUE(conf_a->spImagesDirectory == "./images/");
    // todo paz: continue checks
    return true;
}


int main() {
	RUN_TEST(spConfigCreateTest);
    return 0;
}
