/*
 * mainAux.h
 *
 *  Created on: 5 בספט 2016
 *      Author: Paz
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

# include "SPConfig.h"
# include "KDTree.h"
# include "SPConfig.h"
# include "SPPoint.h"
# include "KDTree.h"
# include "KDArray.h"
void freeAndExit(SPConfig config, KDTreeNode tree, int* images);

bool saveFeatsToFile(SPPoint* imageInfo, int numberOfFeats, int index, SPConfig config);


#endif /* MAINAUX_H_ */
