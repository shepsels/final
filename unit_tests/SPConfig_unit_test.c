# include "unit_test_util.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "../SPConfig.h"

static bool spConfigCreateTest_1() {
	printf("started test 1\n\n");
    char* path = "configFile3_valid.txt"; //todo change back
    SP_CONFIG_MSG * msg;
    SPConfig conf_a;


    conf_a = spConfigCreate(path, msg);

    // validating

    ASSERT_TRUE(strcmp(conf_a->spImagesDirectory, "./images/") == 0);
    ASSERT_TRUE(strcmp(conf_a->spImagesPrefix, "img") == 0);
    ASSERT_TRUE(strcmp(conf_a->spImagesSuffix, ".png") == 0);
    ASSERT_TRUE(conf_a->spNumOfImages == 17);
    ASSERT_TRUE(conf_a->spPCADimension == 20);	//default
    ASSERT_TRUE(strcmp(conf_a->spPCAFilename, "pca.yml") == 0);
    ASSERT_TRUE(conf_a->spNumOfFeatures == 100);
    ASSERT_TRUE(conf_a->spExtractionMode == true);
    ASSERT_TRUE(conf_a->spMinimalGUI == false);	//default
    ASSERT_TRUE(conf_a->spNumOfSimilarImages == 5);
    ASSERT_TRUE(strcmp(conf_a->spLoggerFilename, "stdout") == 0);

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
	char* m = "spImagesDirectory=./images/\n";
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
	ASSERT_FALSE(hasSpacesInVar(m));

	return true;
}



int main() {
    SPConfig conf_a;
    SP_CONFIG_MSG * msg;

    conf_a = spConfigCreate("configFile3_valid.txt", msg);
//    printf("%d\n", conf_a->spKNN);


	RUN_TEST(spConfigCreateTest_1);


    RUN_TEST(hasSpacesInVarTest);
    return 0;
}
