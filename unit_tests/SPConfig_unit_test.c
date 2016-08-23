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
//    ASSERT_TRUE(strcmp(conf_a->spImagesDirectory, "./images/") == 0);
//    ASSERT_TRUE(strcmp(conf_a->spImagesPrefix, "img") == 0);
//    ASSERT_TRUE(strcmp(conf_a->spImagesSuffix, ".png") == 0);
//    ASSERT_TRUE(conf_a->spNumOfImages == 17);
//
//    // todo paz: continue checks
    return true;
}

static bool hasSpacesInVarTest() {
	char* a = "foo = bar";
	char* b = "foo= bar";
	char* c = "foo =bar";
	char* d = "fo o=bar";
	char* e = "fo o =bar";
	char* f = "fo o= bar";
	char* h = "fo o = bar";
	char* i = "foo=ba r";
	char* j = "foo =ba r";
	char* k = "foo= ba r";
	char* l = "foo = ba r";
	int z = 'a';

	ASSERT_FALSE(hasSpacesInVar(a));
	ASSERT_FALSE(hasSpacesInVar(b));
	ASSERT_FALSE(hasSpacesInVar(c));
	ASSERT_TRUE(hasSpacesInVar(d));
	ASSERT_TRUE(hasSpacesInVar(e));
	ASSERT_TRUE(hasSpacesInVar(f));
	ASSERT_TRUE(hasSpacesInVar(j));
	ASSERT_TRUE(hasSpacesInVar(h));
	ASSERT_TRUE(hasSpacesInVar(i));
	ASSERT_TRUE(hasSpacesInVar(j));
	ASSERT_TRUE(hasSpacesInVar(k));
	ASSERT_TRUE(hasSpacesInVar(l));

	return true;
}



int main() {
    printf("sdfsd");
	RUN_TEST(spConfigCreateTest);
    printf("sdfsd");


    RUN_TEST(hasSpacesInVarTest);
    return 0;
}
