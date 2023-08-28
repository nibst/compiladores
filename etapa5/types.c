#include "types.h"

int isCompatibleDataType(DataType type1, DataType type2){
    //equal case
    if (type1 == type2)
        return 1;
    //compatible cases
    if (type1 == DATATYPE_INT)
        if (type2 == DATATYPE_CHAR)
            return 1;
    if (type2 == DATATYPE_INT)
        if (type1 == DATATYPE_CHAR)
            return 1;
    //not compatible
    return 0;
}
