# include "unit_test_util.h"
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "../mainAux.h"


static bool lineToDoubleArrayTest() {
	fflush(NULL);
	printf("started test 1\n\n");
    char line[2048] = "144.378616$12.852055$-76.474480$87.260033$24.542471$16.175438$-30.460892$-87.560081$-59.111641$63.740837$-30.964336$-51.716667$-3.353472$47.239204$-99.560577$-56.206459$20.869055$24.432989$-3.072199$-10.529292$"; //todo change back
    fflush(NULL);
	double* arr;

	arr = (double*)malloc(20*sizeof(double));
    fflush(NULL);
	lineToDoubleArray (arr, 20, line, "$");
    fflush(NULL);
    printf("here");
    fflush(NULL);

    ASSERT_TRUE(arr[0] == 144.378616);
	ASSERT_TRUE(arr[1] == 12.852055);
	ASSERT_TRUE(arr[19] == -10.529292);

	free(arr);
    return true;
}

static bool extractFromFileTest() {
	printf("started test 1\n\n");
    char* path = "configFile1.txt"; //todo change back
    SP_CONFIG_MSG * msg;
    SPConfig conf_a;
    SPPoint** points;
    int i;

//    points = (SPPoint**)malloc(sizeof(SPPoint*));

    conf_a = spConfigCreate(path, msg);
    printf("num: %d", conf_a->spNumOfImages);


	int cnt = extractImagesFromFeats(conf_a, 100, points);
//
//	for (i=0; i<cnt; i++) {
//		spPointDestroy(*points[i]);
//		free(points[i]);
//	}
//	free(points);

	return true;
}


int main() {
	RUN_TEST(lineToDoubleArrayTest);
	RUN_TEST(extractFromFileTest);

}
