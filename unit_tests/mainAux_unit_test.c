//# include "unit_test_util.h"
//# include <stdbool.h>
//# include <stdio.h>
//# include <stdlib.h>
//# include <string.h>
//# include "../mainAux.h"
//# include "../KDArray.h"
//# include "../KDTree.h"
//
//static bool lineToDoubleArrayTest() {
////	fflush(NULL);
//	printf("started test 1\n\n");
//    char line[2048] = "144.378616$12.852055$-76.474480$87.260033$24.542471$16.175438$-30.460892$-87.560081$-59.111641$63.740837$-30.964336$-51.716667$-3.353472$47.239204$-99.560577$-56.206459$20.869055$24.432989$-3.072199$-10.529292$"; //todo change back
////    fflush(NULL);
//	double* arr;
//
//	arr = (double*)malloc(20*sizeof(double));
////    fflush(NULL);
//	lineToDoubleArray (arr, 20, line, "$");
////    fflush(NULL);
//    printf("here");
////    fflush(NULL);
//
//    ASSERT_TRUE(arr[0] == 144.378616);
//	ASSERT_TRUE(arr[1] == 12.852055);
//	ASSERT_TRUE(arr[19] == -10.529292);
//
//	free(arr);
//    return true;
//}
//
//static bool extractFromFileTest() {
//	printf("started test 1\n\n");
//    char* path = "spcbir.config"; //todo change back
//    SP_CONFIG_MSG * msg;
//    SPConfig conf_a;
//    int i;
//
//
//    conf_a = spConfigCreate(path, msg);
//    printf("num: %d", conf_a->spNumOfImages);
//
//int cnt=0;
//
//	SPPoint* points = extractImagesFromFeats(conf_a, &cnt);
//	printf("24, 0 is: %f\n", points[24]->data[0]);
//	printf("sizeof array is %d\n",cnt);
//
//	ASSERT_TRUE(points[0]->data[0] == 231.330978);
//	ASSERT_TRUE(points[0]->data[4] == 40.230812);
//	ASSERT_TRUE(points[24]->data[0] == -141.818588);
//	ASSERT_TRUE(points[24]->data[4] == 84.121841);
//	return true;
//}
//
//static bool fromFilesToTreeTest() {
//	printf("started test 1\n\n");
//    char* path = "spcbir.config"; //todo change back
//    SP_CONFIG_MSG * msg;
//    SPConfig conf_a;
//    SPPoint* points;
//
//    SPKDArray array;
//    KDTreeNode tree;
//    int i;
//
//
//    conf_a = spConfigCreate(path, msg);
//    printf("num: %d\n", conf_a->spNumOfImages);
//
//
//	fromFilesToKDTree(conf_a ,array, tree, points);
//
//	ASSERT_TRUE(array);
//	ASSERT_TRUE(tree);
//
//
//
//	return true;
//}
//
//
//int main() {
////	RUN_TEST(lineToDoubleArrayTest);
////	RUN_TEST(extractFromFileTest);
//	RUN_TEST(fromFilesToTreeTest);
//	exit(0);
//
//}
