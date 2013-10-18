#include <stdio.h>
#include <string.h>
#include <sqlite3.h>


#include "db.h"

static int CallBackExecStatement(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int CreateDatabase(char* Path){

    if((NULL == Path) || ((strlen(Path)) < 0)){
        return DB_INVALID_NAME_ERROR;
    }

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(Path, &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return DB_OPEN_ERROR;
    }else{
       fprintf(stderr, "Opened database successfully\n");
    }
   sqlite3_close(db);
   return DB_OPEN_SUCCESS;

}

int ExecuteStatement(char* Database, char* Statement, int (*CallBack)(void*, int, char**, char**)){

    if((NULL == Database) || ((strlen(Database)) < 0)){
        return DB_INVALID_NAME_ERROR;
    }

    if((NULL == Statement) || ((strlen(Statement)) < 0)){
        return DB_INVALID_STATEMENT_ERROR;
    }

    if(NULL == CallBack){
        return DB_INVALID_STATEMENT_ERROR;
    }

    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    const char* data = "Callback function called";

    /* Open database */
    rc = sqlite3_open(Database, &db);
    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return DB_OPEN_ERROR;
    }else{
       fprintf(stdout, "Opened database successfully\n");
    }

    /* Execute SQL statement */
    rc = sqlite3_exec(db, Statement, CallBack, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return DB_CREATE_STATEMENT_ERROR;
    }else{
       fprintf(stdout, "Statement executed successfully\n");
    }
    sqlite3_close(db);
    return DB_CREATE_STATEMENT_SUCCESS;

}

int main(){

    if(CreateDatabase("rajnikantha.db") < 0){
        printf("\nCouldn't create database");
        return 0;
    }

    puts("----------************-----------\n");

    char* CreateStatement = "CREATE TABLE hero(NAME , ROLE);";
    if(ExecuteStatement("rajnikantha.db", CreateStatement, &CallBackExecStatement) < 0){
        printf("\nCouln't create table");
        return 0;
    }

    puts("----------************-----------\n");

    char* InsertStatement = "INSERT INTO hero (NAME,ROLE) VALUES('Salman Khan','Muscle man');"  \
                            "INSERT INTO hero (NAME,ROLE) VALUES('Aamir Khan','Chocolate hero');";
    if(ExecuteStatement("rajnikantha.db", InsertStatement, &CallBackExecStatement) < 0){
        printf("\nCouln't insert values");
        return 0;
    }

    puts("----------************-----------\n");

    char* SelectStatment = "SELECT * FROM hero";
    if(ExecuteStatement("rajnikantha.db", SelectStatment, &CallBackExecStatement) < 0){
        printf("\nCouln't select values");
        return 0;
    }

    puts("----------************-----------\n");

    char* UpdateStatement = "UPDATE hero set ROLE = 'Director' where NAME='Aamir Khan';";
    if(ExecuteStatement("rajnikantha.db", UpdateStatement, &CallBackExecStatement) < 0){
        printf("\nCouln't update values");
        return 0;
    }

    puts("----------************-----------\n");

    if(ExecuteStatement("rajnikantha.db", SelectStatment, &CallBackExecStatement) < 0){
        printf("\nCouln't select values");
        return 0;
    }

    puts("----------************-----------\n");

    InsertStatement = "INSERT INTO hero (NAME,ROLE) VALUES('Amitabh Bachchan','Ever green hero');";
    if(ExecuteStatement("rajnikantha.db", InsertStatement, &CallBackExecStatement) < 0){
        printf("\nCouln't insert values");
        return 0;
    }

    puts("----------************-----------\n");

    char* DeleteStatement = "DELETE FROM hero WHERE NAME='Salman Khan';";
    if(ExecuteStatement("rajnikantha.db", DeleteStatement, &CallBackExecStatement) < 0){
        printf("\nCouln't delete  values");
        return 0;
    }

    puts("----------************-----------\n");

    if(ExecuteStatement("rajnikantha.db", SelectStatment, &CallBackExecStatement) < 0){
        printf("\nCouln't select values");
        return 0;
    }

    return 0;

}
