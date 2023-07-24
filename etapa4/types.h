#ifndef TYPES_HEADER
#define TYPES_HEADER

typedef enum data_type
{
    DATATYPE_NA,      // Typing not applicable, used for nodes containing commands
    DATATYPE_INT,    
    DATATYPE_REAL,   
    DATATYPE_BOOL,   
    DATATYPE_CHAR,   
    DATATYPE_STRING, 
    DATATYPE_TBA    // Unknown type, "To Be Assigned"

} DataType;

int isCompatibleDataType(DataType type1, DataType type2);

#endif /*TYPES_HEADER*/