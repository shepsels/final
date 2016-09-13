# include "SPConfig.h"

int trimWhiteSpace(char *out, const char *str)
{
    int len = 0;

    if(str[0] == '\n' || str[0] == '\0')
    {
        out[0] = '\0';
        return 0;
    }

    // count
    while (str[len] != '\0')
    {
        len++;

    }
    len++;
    if(len == 1)
        return 0;

    const char *end;
    int out_size;

    // Trim leading space
    while(isspace(*str)) str++;

    // All spaces?
    if(*str == 0)
    {
        *out = 0;
        return 1;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    end++;

    // Set output size
    out_size = (end - str) < len-1 ? (end - str) : len-1;

    // Copy trimmed string
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
}

void removeSpaces(char* source)
{
    char* i = source;
    char* j = source;
    while(*j != 0)
    {
        *i = *j++;
        if(!isspace(*i))
            i++;
    }
    *i = 0;
}

bool hasSpaces(const char *s) {
    while (*s != '\0') {
        if (isspace(*s))
            return true;
        s++;
    }
    return false;
}

bool hasSpacesInVar(const char *s) {
    bool foundEqualSign = false;
    while (*s != '\0') {

        // check if there's a comment
        if(*s == '#')
        {
            return false;
        }
        if (isspace(*s) && !(*s == '\n')){
            //check if in format of "<a> = <b>"
            if((*(s+1) == '=') && (isspace(*(s+2))) && !foundEqualSign) {
                s+=2;
                foundEqualSign = true;
            }
                //check if in format of "<a> =<b>"
            else if((*(s+1) == '=') && !foundEqualSign) {
                s++;
                foundEqualSign = true;
            }
            else if((*(s-1) == '=') && !foundEqualSign) {
                foundEqualSign = true;
            }
            else {
                return true;
            }
        }
        s++;
    }
    return false;
}

void printInvalidMessage(char* filename, int lineNum)
{
    printf("File: %s\n", filename);
    printf("Line: %d\n", lineNum);
    printf("Message: Invalid configuration line\n");
}

void printConstraintNotMet(const char* filename, int lineNum)
{
    printf("File: %s\n", filename);
    printf("Line: %d\n", lineNum);
    printf("Message: Invalid value - constraint not met\n");
}

void printConstraintNotMetWithParam(const char* filename, int lineNum, char* parameter)
{
    printf("File: %s\n", filename);
    printf("Line: %d\n", lineNum);
    printf("Message: Parameter %s is not set\n", parameter);
}

void freeStructMemory(SPConfig spcnfg)
{
    //todo
    free(spcnfg);
//	printf("-------------not completely implemented---------------");
}

