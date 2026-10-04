#include <stdio.h>
#include <string.h>
#include <io.h>

char activeDatabase[60] = "";

struct Column
{
    char name[50];
    char type[20];
};

struct Table
{
    char name[50];
    int columnCount;
    struct Column columns[10];
};

struct Record
{
    char values[10][100];
};

struct RecordSet
{
    struct Record records[100];
    int count;
};

// for automatic id
int getNextId()
{
    FILE *file;
    int id;
    int tempId;
    char tempName[50];

    id = 1;

    if (strlen(activeDatabase) == 0)
    {
        return id;
    }

    file = fopen(activeDatabase, "r");

    if (file != NULL)
    {
        while (fscanf(file, "%d,%49s", &tempId, tempName) == 2)
        {
            id++;
        }

        fclose(file);
    }

    return id;
}

// select function decleration
void retrieveRecords();

// add function decleration
void appendRecord();

// build function decleration
void buildDatabase();

// inspect function decleration
void inspectDatabases();

// inspect function decleration
void accessDatabase();

// savetable function decleration
void saveTableSchema();

// loadtable function decleration
void loadTableSchema();

// buildTable function decleration
void buildTable();

// inspectTable function decleration
void inspectTable();

// main function
int main()
{

    char command[50];

    printf("VORIX Database Engine\n");
    printf("If you want to exit, type TERMINATE.\n\n");

    while (1)
    {
        printf("VORIX> ");
        scanf("%49s", command);

        if (strcmp(command, "BUILD") == 0 || strcmp(command, "build") == 0)
        {
            buildDatabase();
        }
        else if (strcmp(command, "TABLE") == 0 || strcmp(command, "table") == 0)
        {
            buildTable();
        }
        else if (strcmp(command, "DESCRIBE") == 0 || strcmp(command, "describe") == 0)
        {
            inspectTable();
        }
        else if (strcmp(command, "INSPECT") == 0 || strcmp(command, "inspect") == 0)
        {
            inspectDatabases();
        }
        else if (strcmp(command, "ACCESS") == 0 || strcmp(command, "access") == 0)
        {
            accessDatabase();
        }
        else if (strcmp(command, "APPEND") == 0 || strcmp(command, "append") == 0)
        {
            appendRecord();
        }
        else if (strcmp(command, "RETRIEVE") == 0 || strcmp(command, "retrieve") == 0)
        {
            retrieveRecords();
        }
        else if (strcmp(command, "TERMINATE") == 0 || strcmp(command, "terminate") == 0)
        {
            printf("VORIX shutting down...\n");
            printf("VORIX shutdown\n");
            break;
        }
        else
        {
            printf("Unknown command.\n");
        }
    }

    return 0;
}

// table box function defination
void printTableHeader(struct Table *table)
{
    printf("+");

    for (int i = 0; i < table->columnCount; i++)
    {
        printf("------------------+");
    }

    printf("\n|");

    for (int i = 0; i < table->columnCount; i++)
    {
        printf(" %-16s |", table->columns[i].name);
    }

    printf("\n+");

    for (int i = 0; i < table->columnCount; i++)
    {
        printf("------------------+");
    }

    printf("\n");
}

// Build function defination
void buildDatabase()
{
    FILE *file;
    char databaseName[50];
    char fileName[60];

    printf("Enter database name: ");
    scanf("%49s", databaseName);

    sprintf(fileName, "%s.vrx", databaseName);

    file = fopen(fileName, "r");

    if (file != NULL)
    {
        fclose(file);

        printf("Database '%s' already exists.\n", databaseName);
        return;
    }

    file = fopen(fileName, "w");

    if (file == NULL)
    {
        printf("Database create nahi ho saka.\n");
        return;
    }

    fclose(file);

    printf("Database '%s' created successfully.\n", databaseName);
}

// inspect function decleration
void inspectDatabases()
{
    struct _finddata_t fileInfo;
    intptr_t handle;

    handle = _findfirst("*.vrx", &fileInfo);

    if (handle == -1)
    {
        printf("No databases found.\n");
        return;
    }

    printf("\nAVAILABLE DATABASES\n");
    printf("-------------------\n");

    do
    {
        printf("%s\n", fileInfo.name);
    } while (_findnext(handle, &fileInfo) == 0);

    _findclose(handle);
}

// access function defination

void accessDatabase()
{
    FILE *file;
    char databaseName[50];
    char fileName[60];

    printf("Enter database name: ");
    scanf("%49s", databaseName);

    sprintf(fileName, "%s.vrx", databaseName);

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Database '%s' does not exist.\n", databaseName);
        return;
    }

    fclose(file);

    strcpy(activeDatabase, fileName);

    printf("Database '%s' is now active.\n", databaseName);
}

// savetable function defination
void saveTableSchema(struct Table *table)
{
    FILE *file;
    char fileName[60];

    sprintf(fileName, "%s_%s.tbl",
            activeDatabase,
            table->name);

    file = fopen(fileName, "w");

    if (file == NULL)
    {
        printf("Table schema save nahi ho saka.\n");
        return;
    }

    fprintf(file, "TABLE|%s\n", table->name);
    fprintf(file, "COLUMNS|%d\n", table->columnCount);

    for (int i = 0; i < table->columnCount; i++)
    {
        fprintf(file,
                "COLUMN|%s|%s\n",
                table->columns[i].name,
                table->columns[i].type);
    }

    fclose(file);
}

// column function declaration
void calculateColumnWidths(
    struct Table *table,
    struct RecordSet *set,
    int widths[])
{
    for (int i = 0; i < table->columnCount; i++)
    {
        widths[i] = strlen(table->columns[i].name);
    }

    for (int r = 0; r < set->count; r++)
    {
        for (int c = 0; c < table->columnCount; c++)
        {
            int length = strlen(set->records[r].values[c]);

            if (length > widths[c])
            {
                widths[c] = length;
            }
        }
    }
}

// Borderprint function defination
void printBorder(struct Table *table, int widths[])
{
    printf("+");

    for (int i = 0; i < table->columnCount; i++)
    {
        for (int j = 0; j < widths[i] + 2; j++)
        {
            printf("-");
        }

        printf("+");
    }

    printf("\n");
}

// record print function defination
void printRecords(
    struct Table *table,
    struct RecordSet *set,
    int widths[])
{
    for (int r = 0; r < set->count; r++)
    {
        printf("|");

        for (int c = 0; c < table->columnCount; c++)
        {
            printf(" %-*s |",
                   widths[c],
                   set->records[r].values[c]);
        }

        printf("\n");
    }
}

// inspectTable function defination
void inspectTable()
{
    struct Table table;
    char tableName[50];

    if (strlen(activeDatabase) == 0)
    {
        printf("No database is active.\n");
        printf("Use ACCESS first.\n");
        return;
    }

    printf("Enter table name: ");
    scanf("%49s", tableName);

    loadTableSchema(&table, tableName);

    if (table.columnCount == 0)
    {
        printf("Table not found or has no columns.\n");
        return;
    }

    printf("\nTABLE: %s\n", table.name);
    printf("-----------------------------\n");

    printf("COLUMNS: %d\n\n", table.columnCount);

    for (int i = 0; i < table.columnCount; i++)
    {
        printf("%d. %-15s %s\n",
               i + 1,
               table.columns[i].name,
               table.columns[i].type);
    }
}

// loadtable function defination
void loadTableSchema(struct Table *table, char tableName[])
{
    FILE *file;
    char fileName[60];
    char line[100];

    sprintf(fileName, "%s_%s.tbl",
            activeDatabase,
            tableName);

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Table schema nahi mila.\n");
        return;
    }

    table->columnCount = 0;

    strcpy(table->name, tableName);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "COLUMN|", 7) == 0)
        {
            sscanf(line,
                   "COLUMN|%49[^|]|%19[^\n]",
                   table->columns[table->columnCount].name,
                   table->columns[table->columnCount].type);

            table->columnCount++;
        }
    }

    fclose(file);
}

// headerprint function declaration
void printHeader(struct Table *table, int widths[])
{
    printf("|");

    for (int i = 0; i < table->columnCount; i++)
    {
        printf(" %-*s |",
               widths[i],
               table->columns[i].name);
    }

    printf("\n");
}

// buildtable function
void buildTable()
{
    struct Table table;

    if (strlen(activeDatabase) == 0)
    {
        printf("No database is active.\n");
        printf("Use ACCESS first.\n");
        return;
    }

    printf("Enter table name: ");
    scanf("%49s", table.name);

    printf("Enter number of columns: ");
    scanf("%d", &table.columnCount);

    if (table.columnCount < 1 || table.columnCount > 10)
    {
        printf("Column count must be between 1 and 10.\n");
        return;
    }

    for (int i = 0; i < table.columnCount; i++)
    {
        printf("\nColumn %d name: ", i + 1);
        scanf("%49s", table.columns[i].name);

        printf("Column %d type: ", i + 1);
        scanf("%19s", table.columns[i].type);
    }

    saveTableSchema(&table);

    printf("\nTable '%s' created successfully.\n", table.name);
}

// check int function defination
int isInteger(char value[])
{
    if (value[0] == '\0')
    {
        return 0;
    }

    int i = 0;

    if (value[0] == '-' || value[0] == '+')
    {
        i = 1;
    }

    if (value[i] == '\0')
    {
        return 0;
    }

    for (; value[i] != '\0'; i++)
    {
        if (value[i] < '0' || value[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}

// idexist check function defination
int idExists(char tableName[], char id[])
{
    FILE *file;
    char fileName[60];
    char line[1000];

    sprintf(fileName, "%s_%s.data",
            activeDatabase,
            tableName);

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file))
    {
        char storedId[100];

        int i = 0;

        while (line[i] != '|' &&
               line[i] != '\0' &&
               line[i] != '\n')
        {
            storedId[i] = line[i];
            i++;
        }

        storedId[i] = '\0';

        if (strcmp(storedId, id) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    return 0;
}

// add function  defination
void appendRecord()
{
    FILE *file;
    struct Record record;
    struct Table table;
    char tableName[50];
    char fileName[60];

    if (strlen(activeDatabase) == 0)
    {
        printf("No database is active.\n");
        printf("Use ACCESS first.\n");
        return;
    }

    printf("Enter table name : ");
    scanf("%49s", tableName);

    loadTableSchema(&table, tableName);

    if (table.columnCount == 0)
    {
        printf("Table not found.\n");
        return;
    }

    for (int i = 0; i < table.columnCount; i++)
    {
        while (1)
        {
            printf("Enter %s: ", table.columns[i].name);

            scanf("%99s", record.values[i]);

            if (strcmp(table.columns[i].type, "INTEGER") == 0 || strcmp(table.columns[i].type, "integer ") == 0 || strcmp(table.columns[i].type, "INT") == 0 || strcmp(table.columns[i].type, "int") == 0)
            {
                if (!isInteger(record.values[i]))
                {
                    printf("Invalid value. %s must be INTEGER.\n",
                           table.columns[i].name);
                    continue;
                }
            }

            if (strcmp(table.columns[i].name, "id") == 0)
            {
                if (idExists(tableName, record.values[i]))
                {
                    printf("Error: ID %s already exists.\n",
                           record.values[i]);
                    continue;
                }
            }

            break;
        }
    }

    sprintf(fileName, "%s_%s.data", activeDatabase, tableName);

    file = fopen(fileName, "a");

    if (file == NULL)
    {
        printf("Table data file can't be open.\n");
        return;
    }

    for (int i = 0; i < table.columnCount; i++)
    {
        fprintf(file, "%s", record.values[i]);

        if (i < table.columnCount - 1)
        {
            fprintf(file, "|");
        }
    }

    fprintf(file, "\n");

    fclose(file);

    printf("Record successfully addes.\n");
}

// select function defination
void retrieveRecords()
{
    FILE *file;

    struct Table table;
    struct RecordSet set;

    char tableName[50];
    char fileName[60];

    int widths[10];

    if (strlen(activeDatabase) == 0)
    {
        printf("No database is active.\n");
        printf("Use ACCESS first.\n");
        return;
    }

    printf("Enter table name: ");
    scanf("%49s", tableName);

    loadTableSchema(&table, tableName);

    if (table.columnCount == 0)
    {
        printf("Table not found.\n");
        return;
    }

    sprintf(fileName, "%s_%s.data",
            activeDatabase,
            tableName);

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("No records found.\n");
        return;
    }

    set.count = 0;

    while (set.count < 100 &&
           fscanf(file,
                  "%99[^|]|",
                  set.records[set.count].values[0]) == 1)
    {
        for (int i = 1; i < table.columnCount; i++)
        {
            if (i < table.columnCount - 1)
            {
                fscanf(file,
                       "%99[^|]",
                       set.records[set.count].values[i]);

                fgetc(file);
            }
            else
            {
                fscanf(file,
                       "%99[^\n]",
                       set.records[set.count].values[i]);

                fgetc(file);
            }
        }

        fgetc(file);

        set.count++;
    }

    fclose(file);

    if (set.count == 0)
    {
        printf("No records found.\n");
        return;
    }

    calculateColumnWidths(&table, &set, widths);

    printf("\n");

    printBorder(&table, widths);

    printHeader(&table, widths);

    printBorder(&table, widths);

    printRecords(&table, &set, widths);

    printBorder(&table, widths);
}