
#ifndef SPCONFIGAUX_H_
#define SPCONFIGAUX_H_


int trimWhiteSpace(char *out, const char *str);


void removeSpaces(char* source);


bool hasSpaces(const char *s);


bool hasSpacesInVar(const char *s);


void printInvalidMessage(char* filename, int lineNum);


void printConstraintNotMet(const char* filename, int lineNum);


void printConstraintNotMetWithParam(const char* filename, int lineNum, char* parameter);


void freeStructMemory(SPConfig spcnfg);



#endif /* SPCONFIGAUX_H_ */
