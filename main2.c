#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MAX 10000
#include <dirent.h>
int fixed_depth;
#define family_roots "D:/vim project"
//find and replace things:
char text[1000];
char replace_message[1000];
int have_replace=0;
int giome_count=0;
//------------------------
FILE *filepointer;
char command[MAX];
char command_extension[MAX];
char* address[MAX];
char clipboard[1000];
void cat();
void createfile();
void FindCommand();
int right_order=0;
int aposition;
int bposition;
char **hidden;
int counter=0;

//void make_hidden_names(char *address, char *name)
//{
//        for(int j=0; j<1000; j++) name[j]='\0';
//        int len=strlen(address);
//        int i=len-1,counter=0;
//        while(1)
//        {
//            if(address[i]=='/') break;
//            counter++;
//            i--;
//        }
//        for(int j=len-counter; j<len; j++)
//            name[j+counter-len]=address[j];
//        //convert to history file:
//         add_char(name,0,'h');
//        //for(int y=0; y<strlen(name); y++) {hidden[counter][y]=name[y]; printf("%d\n",hidden[counter][y]);}
//        //printf("this is:%s\n",hidden+counter*strlen(name));
//        //counter++;
//}

void extract_name(char *address, char *nameOutOfFile)
{
    for(int i=0; i<1000; i++) nameOutOfFile[i]='\0';
    int len = strlen(address);
    int i;
    for(i=len-1; address[i]!='/'; i--) {}
    i++;
    int temp=i;
    for(;i<len;i++)
        nameOutOfFile[i-temp]=address[i];
}


void remove_char(char *s, int pos)
{
  int length = strlen(s);
  for (int i = pos; i < length; i++)
    s[i] = s[i + 1];
}




void add_char(char *a, int pos, char c)
{
    int len = strlen(a);
    for(int i = len-1; i >= pos ; i--)
        a[i+1]=a[i];
    a[pos]=c;
}




void fix_the_string(char *address)
{
    int len=strlen(address);
    if(address[len-1]==' ' || address[len-1]=='\n') remove_char(address,len-1);
    if(address[0]=='"' || address[0]==' ') remove_char(address,0);
    int w=1;
    while(address[w]!='\0')
        {if(address[w]=='"' && address[w-1]!=92)
        remove_char(address,w); w++;}

    //converting \*s to *s and \"s to "s:
    int max_go= strlen(address)-1;
    for(int i=1; i<strlen(address); i++)
    {
        if(address[i-1]==92 && (address[i]=='*' || address[i]=='"'))
        {
            remove_char(address,i-1);
            max_go--;
        }
        if(i>=max_go) break;
    }
    //converting \n s to enters:
    max_go= strlen(address)-1;
    if(address[0]==92 && address[1]=='n') {remove_char(address,0); address[0]='\n';}
    for(int i=2; i<strlen(address); i++)
    {
        if(address[i-1]==92 && address[i]=='n' && address[i-2]!=92)
        {
            remove_char(address,i-1);
            address[i-1]='\n';
            max_go--;
        }
        if(i>=max_go) break;
    }
    //converting \\ns to \n:
    max_go= strlen(address)-1;
    for(int i=2; i<strlen(address); i++)
    {
        if(address[i-1]==92 && address[i]=='n' && address[i-2]==92)
        {
            remove_char(address,i-2);
            max_go--;
        }
        if(i>=max_go) break;
    }
    if(address[strlen(address)-1]==' ')  remove_char(address,strlen(address)-1);
}







void createfile()
{
    char *directory;
    char *next_directory;
    char *new_address;
    int counter = 0;
    int result;


    scanf(" %[^ ]s", command_extension);
    if(strcmp(command_extension, "--file") != 0) {
        printf("Invalid Command!\nSimply type <--help> for more information.");
        return;
    }

    scanf("%[^\n]s", address);
    // strcpy(cpy_address, address);

    directory = strtok(address, "/");
    directory = strtok(NULL, "/");
    directory = strtok(NULL, "/");

    // printf("%s\n", directory);
    while(directory != NULL)
    {
        counter++;
        next_directory = strtok(NULL, "/");
        if(next_directory == NULL) {
            if(!access(directory, F_OK)) {
                printf("File already exist!\n");
                return;
            }
            filepointer = fopen(directory, "a");
            fclose(directory);
            break;
        }
        result = mkdir(directory);
        chdir(directory);

        // mkdir(directory);
        strcpy(directory, next_directory);
    }
    if(!result)
    printf("File created successfully!\n");
    // printf("count = %d\n", counter);

    while(counter != 1)
    {
        chdir("..");
        counter--;
    }
    return;
}

//-----------------------------------------------------------------------------
void cat()
{
     char check[1000]={'\0'};
    scanf("%s",check);
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {

        char address[1000],name[1000],realaddress[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;name[ko]=NULL;realaddress[ko]=NULL;}
        scanf("%s",address);
        if(address[0]=='/') remove_char(address,0);
        int length=strlen(address);

        int j=0,namezone=0;
        for(int i=length-1;address[i]!='/';i--)
            namezone++;
        for(int i=length-1-(namezone-1);i<1001;i++)
        {
            name[j]=address[i];
            j++;}

        for(int m=0; m<length-1-namezone; m++)
            realaddress[m]=address[m];
        realaddress[length-namezone-1]='/';

    chdir(realaddress);
    char str;
    int *ptr = fopen(name , "r");
    if(ptr != NULL) {
        str = fgetc(ptr);
        while(str != EOF) {
            printf("%c", str);
            str = getc(ptr);

        }
        printf("\n");
    }else {
        printf("No Such File!\n");
        return;
    }
    fclose(ptr);
    }
}
//-------------------------------------------------------------------------------------

void insert_input()
{
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {

        char address[1000],name[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;name[ko]=NULL;}

        //taking input after --file
        scanf("%[^-]s",address);
        if(address[0]=='/') remove_char(address,0);
        for(int j=0; j<6; j++) getchar();
        scanf("%[^-]s",name);
        for(int j=0; j<6; j++) getchar();
        char rubbish;
        int line,character;
        scanf("%d%c%d",&line,&rubbish,&character);
        insert(address,name,line,character);
    }
}


//--------------------------------------------------------------
void insert(char *address,char *name, int line, int character)
{
    chdir(family_roots);
    //-----------------------------------
    //correcting the address of the file.
    fix_the_string(address);
    if(address[0]=='/') remove_char(address,0);
    //-----------------------------------
    //correcting the content=name
    fix_the_string(name);
    //-----------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //printf("the address is:%s\ncontent is:%s\nline and char:%d,%d\n",address,name,line,character);
    //saving the current file:
    char nameOutOfFile[1000]={'\0'};
    extract_name(address,nameOutOfFile);
    FILE * file;
    FILE * history;
    file= fopen(address,"r");
    if(file==NULL) {printf("Well, I didn't find any file with THAT address...\n"); return;}

    char history_address[1000]={'\0'};
    strcpy(history_address,"D://vim project/backup/");
    strcat(history_address,nameOutOfFile);
    history=fopen(history_address,"w+");
    printf("this should be the path of history:%s\n",history_address);
    if(history==NULL) puts("history could not be openend.");

    char c = fgetc(file);
    while (c != EOF)
    {
        fputc(c, history);
        c = fgetc(file);
    }
    fclose(history);
    fclose(file);
    chdir(family_roots);
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    file=fopen(address,"r+");
    if(file==NULL) puts("could not be opened.");
    //count the lines and decide if there are enough:
    int lines=0;
    char ch;
    for (ch = getc(file); ch != EOF; ch = getc(file))
        if (ch == '\n') lines++;

    if(line>lines && lines!=0) {printf("Not enough lines!\n"); return;}
    //---------------------------------------------------------
    //count the characters and decide if there are enough of them in the asked line:
    fseek(file , 0 , SEEK_SET);
        int line_location=1;
        char cop;
        while(line_location<line)
        {
            cop=fgetc(file);
            if(cop=='\n') line_location++;
        }
        cop=fgetc(file);
        int char_location=0;
        while(cop!='\n' && cop!=EOF && cop!='\0')
        {
            cop=fgetc(file);
            char_location++;
        }
        if(line_location==lines) char_location--;
        if(char_location<character) {printf("Not enough characters in the line\n"); return 0;}
    //--------------------------------------------------------
    char string[1000];
    for(int u=0; u<1000; u++)
        string[u]=NULL;
    fseek(file, 0 , SEEK_SET);
    int noc=0;
    int nol=1;
    while(nol<line)
    {
        fgets(string, 1000, file);
        noc=noc+strlen(string);
        nol++;
        for(int u=0; u<1000; u++)
        string[u]=NULL;
    }

    noc+=character;
    //if(line!=1) noc = noc + 2 - line;

    //count the number of characters.
    fseek(file , 0 , SEEK_SET);
    int total = 0;
    char cameron ;
    while ((cameron = fgetc(file)) != EOF )
    total++;

    //printf("\nWAS THE SEEK SUCCESFULL:%d\n*ATTENSION*:size of the NOC now is %d\n",fseek(file, noc, SEEK_SET),noc);

    char temp[1000];
     for(int l=0; l<1000; l++)
        temp[l]=NULL;
    fseek(file, noc+line-1 , SEEK_SET);

    c=fgetc(file);
    for(int i=0; c!=EOF; i++)
    {
        temp[i]=c;
        c=fgetc(file);
    }


    //printf("\nbefore anything else, this is the name:*%s*and this is the temp:*%s*",name,temp);
    if(temp[0]=='\n' && character==0) remove_char(temp,0);
      strcat(name,temp);
    //printf("\nis name modified?%s\n",name);


    fseek(file, noc+line-1 , SEEK_SET);
    fputs(name,file);
    fseek(file , 0 , SEEK_SET);
    fclose(file);
    //-------------------------------------------
     file = fopen(address,"r");
     FILE* file_tmp = fopen("tmp.txt","w+");
     int n=1;
     char cameron2;
      while((cameron2=getc(file))!=EOF)
    {
        fputc(cameron2,file_tmp);
        n++ ;
    }

        fclose(file);
        fclose(file_tmp);
        file = fopen(address,"w");
        file_tmp = fopen("tmp.txt","r");
        c = getc(file_tmp);
        while(c != EOF){
            putc(c,file);
            c = getc(file_tmp);
        }
        fclose(file);
        fclose(file_tmp);
        puts("Your say has been saved in the file. You have my word!");
        //------------------------------------------------
        //------------------------------------------------
        //------------------------------------------------
        //------------------------------------------------
        //In the end, we make a backup file for double undo.
        FILE * backup;
        file= fopen(address,"r");
        backup= fopen("D://vim project/backup/double_undo.txt", "w+");
        if(backup==NULL) puts("backup could not be openend.");

        c = fgetc(file);
        while (c != EOF)
        {
            fputc(c, backup);
            c = fgetc(file);
        }
        fclose(backup);
        fclose(file);
}

//--------------------------------------------------------------

void removestr_input()
{
    chdir(family_roots);
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {
        int line, character ,size;
        char direction;
        char rubbish;
        char address[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;}

        //taking input after --file
        scanf("%[^-]s",address);
        if(address[0]=='/') remove_char(address,0);
        for(int j=0; j<6; j++) getchar();
        scanf("%d%c%d",&line,&rubbish,&character);
        for(int jo=0; jo<7; jo++) getchar();
        scanf("%d %c%c",&size,&rubbish,&direction);
        removestr(address,line,character,size,direction);
    }
}



void removestr(char *address, int line, int character, int size, char direction)
{
    chdir(family_roots);
    fix_the_string(address);
    if(address[0]=='/') remove_char(address,0);
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //saving the current file:
    char nameOutOfFile[1000]={'\0'};
    extract_name(address,nameOutOfFile);
    FILE * file;
    FILE * history;
    file= fopen(address,"r");
    if(file==NULL) {printf("Well, I didn't find any file with THAT address...\n"); return;}

    char history_address[1000]={'\0'};
    strcpy(history_address,"D://vim project/backup/");
    strcat(history_address,nameOutOfFile);
    history=fopen(history_address,"w+");
    //printf("this should be the path of history:%s\n",history_address);
    if(history==NULL) puts("history could not be opened.");

    char c = fgetc(file);
    while (c != EOF)
    {
        fputc(c, history);
        c = fgetc(file);
    }
    fclose(history);
    fclose(file);
    chdir(family_roots);
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    file= fopen(address,"r+");
    int linenom=1;
    int i=0;
    //count the lines and decide if there are enough:
    fseek(file, 0 , SEEK_SET);
    int lines=1;
    int ch;
    while(!feof(file))
    {
        ch = fgetc(file);
        if(ch == '\n')
        {
            lines++;
        }
    }

        if(line>lines && lines!=0) {printf("Not enough lines!\n"); return;}
    //---------------------------------------------------------
    //count the characters and decide if there are enough of them in the asked line:
    fseek(file , 0 , SEEK_SET);
        int line_location=1;
        char cop;
        while(line_location<line)
        {
            cop=fgetc(file);
            if(cop=='\n') line_location++;
        }
        cop=fgetc(file);
        int char_location=0;
        while(cop!='\n' && cop!=EOF && cop!='\0')
        {
            cop=fgetc(file);
            char_location++;
        }
        if(line_location==lines) char_location--;
        if(char_location<character) {printf("Not enough characters in the line\n"); return 0;}
    //--------------------------------------------------------

    char string[1000];
    for(int u=0; u<1000; u++)
        string[u]=NULL;
    fseek(file, 0 , SEEK_SET);

    int noc=0;
    int nol=1;
    while(nol<line)
    {
        fgets(string, 1000, file);
        noc=noc+strlen(string);
        nol++;
        for(int u=0; u<1000; u++)
        string[u]=NULL;
    }

    noc+=character;
    if(direction=='b') {noc=noc-size;}
    //count the number of characters.
    fseek(file , 0 , SEEK_SET);
    int total = 0;
    char cameron ;
    while ((cameron = fgetc(file)) != EOF )
    total++;

    //printf("\nWAS THE SEEK SUCCESFULL:%d\n*ATTENSION*:size of the NOC now is %d\n",fseek(file, noc, SEEK_SET),noc);
    fseek(file , 0 , SEEK_SET);
    FILE* file_tmp = fopen("tmp.txt","w+");
    for(int i=0; i<noc; i++)
    {
        c=fgetc(file);
        fputc(c,file_tmp);
    }


    char temp[1000];
     for(int l=0; l<1000; l++)
        temp[l]=NULL;
    fseek(file, noc+line-1+size , SEEK_SET);
    temp[0]=fgetc(file);
    int order=1;

   while(1) {
      temp[order] = fgetc(file);
      if(feof(file))
         break ;
      order++;
      }
    //printf("\nbefore anything else, this is the name:*%s*and this is the temp:*%s*",name,temp);
    if(temp[0]=='\n' && character==0) remove_char(temp,0);

    fputs(temp,file_tmp);
    fclose(file);
    fclose(file_tmp);
    //-------------------------------------------
     file = fopen(address,"w");
     file_tmp=fopen("tmp.txt","r+");
        c = getc(file_tmp);
        while(c != EOF){
            putc(c,file);
            c = getc(file_tmp);
        }
        fclose(file);
        fclose(file_tmp);
        puts("Successful removal.");
        //------------------------------------------------
        //------------------------------------------------
        //------------------------------------------------
        //In the end, we make a backup file for double undo.
        FILE * backup;
        file= fopen(address,"r");
        backup= fopen("D://vim project/backup/double_undo.txt", "w+");
        if(backup==NULL) puts("backup could not be openend.");

        c = fgetc(file);
        while (c != EOF)
        {
            fputc(c, backup);
            c = fgetc(file);
        }
        fclose(backup);
        fclose(file);

}
//------------------------------------------------------------------------
void cutstr()
{
    chdir(family_roots);
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {
        int line, character ,size;
        char direction;
        char rubbish;
        char address[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;}

        //taking input after --file
        scanf("%[^-]s",address);
        if(address[0]=='/') remove_char(address,0);
        for(int j=0; j<6; j++) getchar();
        scanf("%d%c%d",&line,&rubbish,&character);
        for(int jo=0; jo<7; jo++) getchar();
        scanf("%d %c%c",&size,&rubbish,&direction);
        //printf("address is:%s\nline:%d\ncharacter:%d\nsize:%d\ndirection:%c\n",address,line,character,size,direction);
        copystr(address,line,character,size,direction);
        removestr(address,line,character,size,direction);
    }
}

//--------------------------------------------------------------------
void copystr_input()
{
    chdir(family_roots);
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {
        int line, character ,size;
        char direction;
        char rubbish;
        char address[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;}

        //taking input after --file
        scanf("%[^-]s",address);
        if(address[0]=='/') remove_char(address,0);
        for(int j=0; j<6; j++) getchar();
        scanf("%d%c%d",&line,&rubbish,&character);
        for(int jo=0; jo<7; jo++) getchar();
        scanf("%d %c%c",&size,&rubbish,&direction);
        copystr(address,line,character,size,direction);
    }
}



void copystr(char *address,int line,int character,int size,char direction)
{
        //correcting the address of the file.
        int cor=0;
        while(address[cor]!=NULL)
        {
            if(address[cor]=='"') {remove_char(address,cor);cor--;}
            cor++;
        }

        FILE *file;
        file= fopen(address,"r+");
        if(file==NULL) {puts("Well, I didn't find any file with THAT address..."); return;}


        //count the lines and decide if there are enough:
        fseek(file, 0 , SEEK_SET);
        int lines=1;
        int ch;
        while(!feof(file))
        {
            ch = fgetc(file);
            if(ch == '\n')
            {
                lines++;
            }
        }

        if(line>lines && lines!=0) {printf("Not enough lines!\n"); return;}
       //---------------------------------------------------------
       //count the characters and decide if there are enough of them in the asked line:
       fseek(file , 0 , SEEK_SET);
        int line_location=1;
        char cop;
        while(line_location<line)
        {
            cop=fgetc(file);
            if(cop=='\n') line_location++;
        }
        cop=fgetc(file);
        int char_location=0;
        while(cop!='\n' && cop!=EOF && cop!='\0')
        {
            cop=fgetc(file);
            char_location++;
        }
        if(line_location==lines) char_location--;
        if(char_location<character) {printf("Not enough characters in the line\n"); return 0;}
        //--------------------------------------------------------
        //now we should count the characters from the beginning of the file, and move to the position.
        fseek(file, 0 , SEEK_SET);
        char string[1000]; for(int u=0; u<1000; u++) string[u]=NULL;

        int noc=0;
        int nol=1;
        while(nol<line)
        {
            fgets(string, 1000, file);
            noc=noc+strlen(string);
            nol++;
            for(int u=0; u<1000; u++)
            string[u]=NULL;
        }
        if(nol!=1) noc++;
        noc+=character;

        fseek(file, noc , SEEK_SET);

        //now that we found our position, we should copy the characters into clipboard.

        for(int jake=0; jake<1000; jake++) clipboard[jake]=NULL;

        if(direction=='f')
        {
            for(int i=0; i<size; i++)
            {
                clipboard[i] = fgetc(file);
            }
        }

        if(direction=='b')
        {
            noc--;
            fseek(file, noc-size+1 , SEEK_SET);
            for(int i=0; i<size; i++)
            {
                clipboard[i] = fgetc(file);
            }
        }

        fclose(file);
        if(clipboard[0]=='\n' && character!=0) remove_char(clipboard,0);
        if(clipboard[strlen(clipboard)-1]=='\n') remove_char(clipboard,strlen(clipboard)-1);
        FILE *clippy;
        clippy=fopen("D:/vim project/clipboard.txt","w");
        fputs(clipboard,clippy);
        fclose(clippy);
        //printf("\nmighty clip:%s*\n",clipboard);
}
//-------------------------------------------------------------------------

void pastestr()
{

    chdir(family_roots);
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {
        int line, character ,size;
        char rubbish;
        char address[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;}

        //taking input after --file
        scanf("%[^-]s",address);
        if(address[0]=='/') remove_char(address,0);
        for(int j=0; j<6; j++) getchar();
        scanf("%d%c%d",&line,&rubbish,&character);
        //printf("address is:%s\nline:%d\ncharacter:%d\n",address,line,character);
        insert(address,clipboard,line,character);
    }
}

//        //correcting the address of the file.
//        int cor=0;
//        while(address[cor]!=NULL)
//        {
//            if(address[cor]=='"') {remove_char(address,cor);cor--;}
//            cor++;
//        }
//
//
//        FILE *file;
//        file= fopen(address,"r");
//        if(file==NULL) {puts("Well, I didn't find any file with THAT address..."); return;}
//        FILE *backup;
//
//        FILE *clippy;
//        clippy=fopen("D:/vim project/clipboard.txt","r+");
//
//        //count the lines and decide if there are enough:
//        fseek(file, 0 , SEEK_SET);
//        int lines=1;
//        int ch;
//        while(!feof(file))
//        {
//            ch = fgetc(file);
//            if(ch == '\n')
//            {
//                lines++;
//            }
//        }
//
//        if(line>lines) {printf("Not enough lines!\n"); return;}
//       //---------------------------------------------------------
//       //count the characters and decide if there are enough of them in the asked line:
//       fseek(file , 0 , SEEK_SET);
//        int line_location=1;
//        char cop;
//        while(line_location<line)
//        {
//            cop=fgetc(file);
//            if(cop=='\n') line_location++;
//        }
//        cop=fgetc(file);
//        int char_location=0;
//        while(cop!='\n' && cop!=EOF && cop!='\0')
//        {
//            cop=fgetc(file);
//            char_location++;
//        }
//        if(line_location==lines) char_location--;
//        if(char_location<character) {printf("Not enough characters in the line\n"); return 0;}
//        //--------------------------------------------------------
//
//        //now we should count the characters from the beginning of the file, and move to the position.
//        fseek(file, 0 , SEEK_SET);
//        char string[1000]; for(int u=0; u<1000; u++) string[u]=NULL;
//
//        int noc=0;
//        int nol=1;
//        while(nol<line)
//        {
//            fgets(string, 1000, file);
//            //printf("lines to calculate noc:%s**%d*",string,strlen(string));
//            noc=noc+strlen(string);
//            nol++;
//            for(int u=0; u<1000; u++)
//            string[u]=NULL;
//        }
//        noc+=character;
//        if(line>1) noc++;
//        fseek(file, noc , SEEK_SET);
//        printf("%d",noc);
//        //now that we found our position, it's time to delete characters.
//
//        FILE *temp;
//        temp= fopen("tmp.txt","w+");
//        fseek(file , 0 , SEEK_SET);
//        char c;
//        //printf("\nclipboard is:%s\n",clipboard);
//            for(int n=0; n<noc; n++)
//            {
//             c=fgetc(file);
//             fputc(c,temp);
//            }
//
//
//            int len=strlen(clipboard);
//            for(int num=0; num<len; num++)
//            {
//                fputc(clipboard[num],temp);
//            }
//
//
//
//
////        int counter=0;
////            c=fgetc(clippy);
////            while(!feof(clippy))
////                  {
////                      fputc(c,temp);
////                      counter++;
////                      c=fgetc(clippy);
////                  }
//
//        //fclose(temp);
//        fclose(clippy);
//        fclose(backup);
////
////            FILE *temp2;
////            file=fopen(address,"r");
////            temp2=fopen("D://vim project/temp2.txt","w+");
//            printf("success?:%d",fseek(file , noc+2 , SEEK_SET ));
//        char d;
//        d=fgetc(file);
//        while(d!=EOF)
//        {
//            fputc(d,temp);
//            d=fgetc(file);
//        }
//
//        fclose(temp);
//        fclose(file);
//
//
//        file = fopen(address,"w+");
//        temp = fopen("D://vim project/tmp.txt","r+");
//        c = getc(temp);
//        while(c != EOF){
//            putc(c,file);
//            c = getc(temp);
//        }
//        fclose(temp);
////        temp2 = fopen("D://vim project/temp2.txt","r+");
////        c = getc(temp2);
////        while(c != EOF){
////            putc(c,file);
////            c = getc(temp2);
////        }
//        fclose(file);
//        //fclose(temp2);
//    }

//}

//-------------------------------------------------------------------------
void replace_input ()
{
    chdir("D://vim project");
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--str1")!=0)
        {puts("Don't forget to write --str1 before the text."); return;}
    //-----------------------------------------------------------------
    //get the whole thing all together as one text:
    for(int ko=0; ko<1000; ko++)
        {text[ko]==NULL; replace_message[ko]=NULL;}
    giome_count=0;
    //get the info all together as a text:
    scanf(" %c",&text[0]);
    if(text[0]=='"') giome_count++;
    scanf("%[^\n]s", text+1);
    //printf("this is the whole thing:%s",text);
    //now we break down the text into separate parts:

    char realmessage[1000],wanted[1000];
    for(int jake=0; jake<1000; jake++) {wanted[jake]=NULL;realmessage[jake]=NULL;}

    if(giome_count!=0)
    {
       realmessage[0]=text[0];
       int i=1;
       while(1)
       {
           if(text[i]=='"' && text[i-1]!=92) break;
           realmessage[i]= text[i];
           i++;
       }
       int j=0;
       remove_char(realmessage,0);
       //printf("that is the real message:%s\n",realmessage);
       while(text[i+8]!=NULL)
       {
           wanted[j]=text[i+9];
           //if(text[i+9]=='"' && text[i+8]!=92) {remove_char(wanted,j);remove_char(text,i+9);i--;j--;}
           i++;
           j++;
       }
       //printf("that is the wanted, and name of the file:%s\n",wanted);

    }
    if(giome_count==0)
    {
        realmessage[0]=text[0];

        //printf("\nthis is the whole text:%s\n",text);
        int i=0;
        while(1)
        {
            if(text[i]!=92 && text[i+1]=='-' && text[i+2]=='-' && text[i+3]=='s' && text[i+4]=='t' && text[i+5]=='r' && text[i+6]=='2') break;
            realmessage[i]=text[i];
            i++;
        }
        //printf("\nthis is the real message:%sspace\n",realmessage);
        int jo=0;
        for(int j=0; j<strlen(text); j++)
        {
            wanted[jo]=text[i+8];
            i++;
            jo++;
        }

    }
    replace(realmessage,wanted);
}
//------------------------------------------------------------------------------------------------
void replace (char *realmessage, char *wanted)
{
//    int max_go;
//    //we got the full text for find function. now let's clean the replace message:
//    //converting \"s to "s:
//    max_go= strlen(realmessage)-1;
//    for(int i=1; i<strlen(realmessage); i++)
//    {
//        if(realmessage[i-1]==92 && (realmessage[i]=='"'))
//        {
//            remove_char(realmessage,i-1);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \n s to enters:
//    max_go= strlen(realmessage)-1;
//    if(realmessage[0]==92 && realmessage[1]=='n') {remove_char(realmessage,0); realmessage[0]='\n';}
//    for(int i=2; i<strlen(realmessage); i++)
//    {
//        if(realmessage[i-1]==92 && realmessage[i]=='n' && realmessage[i-2]!=92)
//        {
//            remove_char(realmessage,i-1);
//            realmessage[i-1]='\n';
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \\ns to \n:
//    max_go= strlen(realmessage)-1;
//    for(int i=2; i<strlen(realmessage); i++)
//    {
//        if(realmessage[i-1]==92 && realmessage[i]=='n' && realmessage[i-2]==92)
//        {
//            remove_char(realmessage,i-2);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //printf("this is the message to replace the old pale:%s\n",realmessage);
//    //real message has been fixed. time to pass it to find:
    find(wanted, 1 , realmessage);
}

//--------------------------------------------------------------------
void find_input()
{

    chdir("D://vim project");
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--str")!=0)
        {puts("Don't forget to write --str before the text."); return;}
    //-----------------------------------------------------------------

    for(int ko=0; ko<1000; ko++)
            {text[ko]==NULL; replace_message[ko]=NULL;}
    giome_count=0;
    //get the info all together as a text:
    scanf(" %c",&text[0]);
    if(text[0]=='"') giome_count++;
    scanf("%[^\n]s", text+1);
    find(text,0,replace_message);
}
//------------------------------------------------------------------------


void find (char *text, int have_replace, char *replace_message)
{

    char address[1000],realmessage[1000];
    for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;realmessage[ko]=NULL;}

    if(giome_count!=0)
    {
       realmessage[0]=text[0];
       //printf("that is the text given to find:%s\n",text);
       int i=1;
       while(1)
       {
           if(text[i]=='"' && text[i-1]!=92) break;
           realmessage[i]= text[i];
           i++;
       }

       int j=0;
       remove_char(realmessage,0);
       //printf("that is the message to be find:%s\n",realmessage);
       while(text[i+8]!=NULL)//به نظرم خوب ادرس رو نمی گیره
       {
           address[j]=text[i+9];
           if(text[i+9]=='"' && text[i+8]!=92) {remove_char(address,j);remove_char(text,i+9);i--;j--;}
           i++;
           j++;
       }
       //printf("that is the address, and name of the file:%s\n",address);

    }

    if(giome_count==0)
    {
        realmessage[0]=text[0];

        //printf("\nthis is the whole text:%s\n",text);
        int i=0;
        while(1)
        {
            if(text[i]!=92 && text[i+1]=='-' && text[i+2]=='-' && text[i+3]=='f' && text[i+4]=='i' && text[i+5]=='l' && text[i+6]=='e') break;
            realmessage[i]=text[i];
            i++;
        }
        //printf("\nthis is the real message:%sspace\n",realmessage);
        int jo=0;
        for(int j=0; j<strlen(text)-strlen(realmessage)+8; j++)
        {
            address[jo]=text[i+8];
            i++;
            jo++;
        }

    }


    printf("%this is text:%s\n",text);
    //-------------------------------------------------
    //determining whether we have a wild card or not:
    int WildCard=0;
    int pre_wild=0;
    int after_wild=0;
    if(realmessage[0]=='*') {pre_wild=1; remove_char(realmessage,0); WildCard++;}
    if(realmessage[strlen(realmessage)-1]=='*' && realmessage[strlen(realmessage)-2]!=92)
        {after_wild==1; remove_char(realmessage,strlen(realmessage)-1); WildCard++;}
    for(int u=1; u<strlen(realmessage); u++)
    {
        if(realmessage[u]=='*' && realmessage[u-1]!=92)
        {
            WildCard++;
            remove_char(realmessage,u);
        }
    }


    //-------------------------------------------------
    //main quotation marks  are removed.
    fix_the_string(address);
    if(address[0]=='/') remove_char(address,0);
    fix_the_string(realmessage);
    fix_the_string(replace_message);
    //printf("this is the address of the file:%s\nthe message to be found in file:%s\nand the replacement message:%s\n",address,realmessage,replace_message);
    //time to correct the given text:
    //converting \*s to *s and \"s to "s:
//    int max_go= strlen(realmessage)-1;
//    for(int i=1; i<strlen(realmessage); i++)
//    {
//        if(realmessage[i-1]==92 && (realmessage[i]=='*' || realmessage[i]=='"'))
//        {
//            remove_char(realmessage,i-1);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \n s to enters:
//    max_go= strlen(realmessage)-1;
//    if(realmessage[0]==92 && realmessage[1]=='n') {remove_char(realmessage,0); realmessage[0]='\n';}
//    for(int i=2; i<strlen(realmessage); i++)
//    {
//        if(realmessage[i-1]==92 && realmessage[i]=='n' && realmessage[i-2]!=92)
//        {
//            remove_char(realmessage,i-1);
//            realmessage[i-1]='\n';
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \\ns to \n:
//    max_go= strlen(realmessage)-1;
//    for(int i=2; i<strlen(realmessage); i++)
//    {
//        if(realmessage[i-1]==92 && realmessage[i]=='n' && realmessage[i-2]==92)
//        {
//            remove_char(realmessage,i-2);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //-------------------------------------------------
//    //do for the replacement message:
//    max_go= strlen(replace_message)-1;
//    for(int i=1; i<strlen(replace_message); i++)
//    {
//        if(replace_message[i-1]==92 && (replace_message[i]=='*' || replace_message[i]=='"'))
//        {
//            remove_char(replace_message,i-1);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \n s to enters:
//    max_go= strlen(replace_message)-1;
//    if(replace_message[0]==92 && replace_message[1]=='n') {remove_char(replace_message,0); replace_message[0]='\n';}
//    for(int i=2; i<strlen(replace_message); i++)
//    {
//        if(replace_message[i-1]==92 && replace_message[i]=='n' && replace_message[i-2]!=92)
//        {
//            remove_char(replace_message,i-1);
//            replace_message[i-1]='\n';
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \\ns to \n:
//    max_go= strlen(replace_message)-1;
//    for(int i=2; i<strlen(replace_message); i++)
//    {
//        if(replace_message[i-1]==92 && replace_message[i]=='n' && replace_message[i-2]==92)
//        {
//            remove_char(replace_message,i-2);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //-------------------------------------------------
//    //do the same for the given address:
//    char c;
//    if(address[0]=='"') remove_char(address,0);
//    int w=1;
//    while(address[w]!='\0')
//        {if(address[w]=='"' && address[w-1]!=92)
//        remove_char(address,w); w++;}
//
//    //converting \*s to *s and \"s to "s:
//    max_go= strlen(address)-1;
//    for(int i=1; i<strlen(address); i++)
//    {
//        if(address[i-1]==92 && (address[i]=='*' || address[i]=='"'))
//        {
//            remove_char(address,i-1);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \n s to enters:
//    max_go= strlen(address)-1;
//    if(address[0]==92 && address[1]=='n') {remove_char(address,0); address[0]='\n';}
//    for(int i=2; i<strlen(address); i++)
//    {
//        if(address[i-1]==92 && address[i]=='n' && address[i-2]!=92)
//        {
//            remove_char(address,i-1);
//            address[i-1]='\n';
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
//    //converting \\ns to \n:
//    max_go= strlen(address)-1;
//    for(int i=2; i<strlen(address); i++)
//    {
//        if(address[i-1]==92 && address[i]=='n' && address[i-2]==92)
//        {
//            remove_char(address,i-2);
//            max_go--;
//        }
//        if(i>=max_go) break;
//    }
    //printf("%this is the whole line without --str:%s\n",text);
    //printf("%this is the whole address:%s\n",address);
    //printf("%this is the whole search content:%s\n",realmessage);
    //now we should get the attributes:
    int atnum=0;
    int count_positive=0;
    int at_positive=0;
    int byword_positive=0;
    int all_positive=0;
    int get_the_address=0;

    for(int jake=strlen(text)-1; ; jake--)
    {
        if(text[jake]=='-' && text[jake-1]=='-') break;
        if(text[jake-5]=='-' && text[jake-4]=='c' && text[jake-3]=='o' && text[jake-2]=='u' && text[jake-1]=='n' && text[jake-0]=='t') {count_positive=1; get_the_address=strlen(text)-1-jake+6;}
        if(text[jake-6]=='-' && text[jake-5]=='b' && text[jake-4]=='y' && text[jake-3]=='w' && text[jake-2]=='o' && text[jake-1]=='r' && text[jake-0]=='d') {byword_positive=1; get_the_address=strlen(text)-1-jake+7;}
        if(text[jake-4]=='-' && text[jake-3]=='a' && text[jake-2]=='t' && text[jake-1]==' ') {at_positive=1; atnum=text[jake-0]-'0'; get_the_address=strlen(text)-1-jake+5;}
        if(text[jake-3]=='-' && text[jake-2]=='a' && text[jake-1]=='l' && text[jake-0]=='l') {all_positive=1; get_the_address=strlen(text)-1-jake+4;}
    }


    //fix the address once and for all:
        if(count_positive!=0 || all_positive!=0 || at_positive!=0 || byword_positive!=0)
            for(int r=strlen(address)-get_the_address-1; r<strlen(address); r++)
                address[r]='\0';
        if(address[0]=='/') remove_char(address,0);

    //dealing with interfering attributes:
    if(at_positive==1 && all_positive==1) {puts("Mission impossible: doing 'at' and 'all' at the same time is beyond my capabilities.");return;}
    if(at_positive==1 && count_positive==1) {puts("Mission impossible: doing 'at' and 'count' at the same time is beyond my capabilities.");return;}
    if(count_positive==1 && all_positive==1) {puts("Mission impossible: doing 'count' and 'all' at the same time is beyond my capabilities.");return;}
    if(have_replace==1 && (count_positive==1 || byword_positive==1)) {puts("Mission impossible: doing the wanted attribute and replace function at the same time is beyond my capabilities.");return;}

    if(realmessage[0]=='"') remove_char(realmessage, 0 );
    if(realmessage[strlen(realmessage)-1]=='"') remove_char(realmessage, strlen(realmessage)-1);

    printf("\n\nthe message you are trying to give is:%s\nand the address of your file is:%s\ntext remaining:%s\nattributes:\nall:%d\nat:%d\natnum:%d\ncount:%d\nbyword:%d\nand the get the address was:%d\n",realmessage,address,text,all_positive,at_positive,atnum,count_positive,byword_positive,get_the_address);
    printf("\n------------------------att:%d\n",all_positive);

    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //saving the current file:
    char nameOutOfFile[1000]={'\0'};
    extract_name(address,nameOutOfFile);
    FILE * file;
    FILE * history;
    file= fopen(address,"r");

    char history_address[1000]={'\0'};
    strcpy(history_address,"D://vim project/backup/");
    strcat(history_address,nameOutOfFile);
    history=fopen(history_address,"w+");
    //printf("this should be the path of history:%s\n",history_address);
    if(history==NULL) puts("history could not be opened.");

    char c = fgetc(file);
    while (c != EOF)
    {
        fputc(c, history);
        c = fgetc(file);
    }
    fclose(history);
    fclose(file);
    chdir(family_roots);
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------
    //------------------------------------------------

    //now we SEARCH:
    int ckecker=0;
    int position=0;
    int counter=1;

    //swap messages:
    char temp[1000];
    if(have_replace==1) for(int i=0; i<1000; i++) {temp[i]=realmessage[i]; realmessage[i]=replace_message[i];replace_message[i]=temp[i];}


    file=fopen(address,"r+");
    //printf("your address is:%s\nthe message to be found is:%s\nand the message to replace the other one is:%s\n",address,realmessage,replace_message);
    if(file==NULL) {puts("It is not in my power to open up a file that doesn't even exist!"); return;}
    fseek(file, 0 , SEEK_SET);
    c = fgetc(file);

    //---------------------------------------------------------
    //covers byword, simple form, and simple wild card:
    if(count_positive==0 && at_positive==0 && all_positive==0)
    {
        int word_counter=0;
        while(!feof(file))
        {
            if(c==realmessage[0])
            {
                ckecker=1;
                position=counter;
                for(int i=0 ; i<strlen(realmessage) ; i++)
                {
                    if(c!=realmessage[i] || feof(file))
                    {
                        ckecker=0; break;
                    }
                    c =fgetc(file);
                    counter++;
                }
                if(ckecker==1)
                {
                     if(WildCard==1)
                    {
                        if(after_wild==1) {if(realmessage[position-1+strlen(realmessage)]==' ' || realmessage[position-1+strlen(realmessage)]=='\n' || realmessage[position-1+strlen(realmessage)]==EOF) continue;}
                        if(pre_wild==1)
                        {
                            if(realmessage[position-2]==' ' || realmessage[position-2]=='\n') continue;
                            while(realmessage[position-1]!=' ' || realmessage[position-1]!='\n' || realmessage[position-1]!=NULL)
                            position-=1;
                        }
                    }

                    if(byword_positive==0 && have_replace==0) {printf("That thing you wanted... I found it! take a look at this character position: %d\n", position-1); return;}

                    if(byword_positive==0 && have_replace==1)
                    {
                        int line=1;
                        fseek(file , 0 , SEEK_SET);
                        for(int i=0; i<position-1; i++) if(c=fgetc(file)=='\n') line++;
                        fseek(file , position+line-2 , SEEK_SET);
                        //printf("the position to start replacing is:%d\nthe looking for text:%s\n",position-1,realmessage);
                        fputs(replace_message,file);
                        fclose(file);
                        puts("Replace done successfully");
                        break;
                    }
                    if(byword_positive==1)
                    {
                        fseek(file , 0 , SEEK_SET);
                        for(int i=0; i<=position; i++)
                        {
                            c=fgetc(file);
                            if(c==' ' || c=='\n') word_counter++;
                        }
                        printf("That thing you wanted... I found it! take a look at this word position: %d\n", word_counter);
                        return;
                    }
                }
            }
            else
            {
                c = fgetc(file) ;
                counter++ ;
            }
        }
        printf("There is no such thing in here mate.");
    }
    //----------------------------------------------------------------------------------------
    //covers byword and count.
    if(count_positive==1 && at_positive==0 && all_positive==0)
    {
        int security=1;
        int number_of_winners=0;
        while(!feof(file))
        {
            if(c==realmessage[0])
            {
                ckecker=1;
                position=counter;
                for(int i=0 ; i<strlen(realmessage) ; i++)
                {
                    if(c!=realmessage[i] || feof(file))
                    {
                        ckecker=0; break;
                    }
                    c =fgetc(file);
                    counter++;
                }
                if(ckecker==1)
                {
                    security=1;
                    if(after_wild==1) {if(realmessage[position-1+strlen(realmessage)]==' ' || realmessage[position-1+strlen(realmessage)]=='\0' || realmessage[position-1+strlen(realmessage)]==EOF) security=0;}
                    if(pre_wild==1) {if(realmessage[position-2]==' ' || realmessage[position-2]=='\n') security=0;}
                    if(security==1) number_of_winners++;
                }
            }
            else
            {
                c = fgetc(file) ;
                counter++ ;
            }
        }
        if(number_of_winners==0) printf("There is no such thing in here mate.");
        if(number_of_winners!=0) printf("Guess how many times this wicked thing was found in the file: %d",number_of_winners);

    }
    //----------------------------------------------------------------------------------------
    //covers all and byword and simple wild cards.
    if(at_positive==0 && all_positive==1)
    {
        int correct=1;
        int security=1;
        int word_counter=0;
        int number_of_winners[1000];
        for(int jake=0; jake<1000; jake++)
            number_of_winners[jake]=NULL;
        int nom=0;
        while(!feof(file))
        {
            if(c==realmessage[0])
            {
                ckecker=1;
                position=counter;
                for(int i=0 ; i<strlen(realmessage) ; i++)
                {
                    if(c!=realmessage[i] || feof(file))
                    {
                        ckecker=0; break;
                    }
                    c =fgetc(file);
                    counter++;
                    //printf("\nthis is c eachtime: %c\n", c);
                }

                if(ckecker==1)
                {
                    security=1;
                    if(after_wild==1) {if(realmessage[position-1+strlen(realmessage)]==' ' || realmessage[position-1+strlen(realmessage)]=='\0' || realmessage[position-1+strlen(realmessage)]==EOF) security=0;}
                    if(pre_wild==1) {if(realmessage[position-2]==' ' || realmessage[position-2]=='\n') security=0;}
                    if(security==1){number_of_winners[nom]=position-1; nom++;}
                }
            }
            else
            {
                c = fgetc(file) ;
                counter++ ;
            }
        }
        if(have_replace==0) puts("Numbers (if there are any) are:");
        if(byword_positive==1)
        {

            for(int o=0; o<nom; o++)
            {
                fseek(file , 0 , SEEK_SET);
                word_counter=0;
                for(int i=0; i<number_of_winners[o]; i++)
                {
                    c=fgetc(file);
                    if(c==' ' || c=='\n') word_counter++;

                }

            printf("%d ", word_counter);

            }
        }
        if(byword_positive==0)
        {
            if(have_replace==0)
            {
                for(int j=0; j<nom; j++)
                printf("%d ", number_of_winners[j]);
            }
            if(have_replace==1)
            {
                for(int j=0; j<nom; j++)
                {
                    int line=1;
                    fseek(file , 0 , SEEK_SET);
                    for(int i=0; i<number_of_winners[j]; i++) if(c=fgetc(file)=='\n') line++;
                    fseek(file , number_of_winners[j]+line-1 , SEEK_SET);
                    if(fputs(replace_message,file)!=0) correct=0;
                }
            }
        }
        if(have_replace==1) {correct==1?puts("Replace done successfully"):puts("No match found");}
        //printf("\nthe replace messae was:%s", replace_message);
        fclose(file);


    }
    //----------------------------------------------------------------------------------------
    //covers byword and at.
     if(at_positive==1)
    {
        int security=1;
        int word_counter=0;
        int at_times=0;
        while(!feof(file))
        {
            if(c==realmessage[0])
            {
                ckecker=1;
                position=counter;
                for(int i=0 ; i<strlen(realmessage) ; i++)
                {
                    if(c!=realmessage[i] || feof(file))
                    {
                        ckecker=0; break;
                    }
                    c =fgetc(file);
                    counter++;
                }
                if(ckecker==1)
                {
                    security=1;
                    if(after_wild==1) {if(realmessage[position-1+strlen(realmessage)]==' ' || realmessage[position-1+strlen(realmessage)]=='\0' || realmessage[position-1+strlen(realmessage)]==EOF) security=0;}
                    if(pre_wild==1) {if(realmessage[position-2]==' ' || realmessage[position-2]=='\n') security=0;}
                    if(security==1) at_times++;

                    if(at_times==atnum)
                    {
                        if(byword_positive==0 && have_replace==0) {printf("That thing you wanted... I found it! take a look at this character position: %d\n", position-1); return;}
                        if(byword_positive==0 && have_replace==1)
                        {
                            int line=1;
                            fseek(file , 0 , SEEK_SET);
                            for(int i=0; i<position-1; i++) if(c=fgetc(file)=='\n') line++;
                            fseek(file , position+line-2 , SEEK_SET);
                            fputs(replace_message,file);
                            fclose(file);
                            puts("Replace done successfully");
                            break;
                        }
                        if(byword_positive==1)
                        {
                            fseek(file , 0 , SEEK_SET);
                            for(int i=0; i<=position; i++)
                            {
                                c=fgetc(file);
                                if(c==' ' || c=='\n') word_counter++;
                            }
                            printf("That thing you wanted... I found it! take a look at this word position: %d\n", word_counter);
                            return;
                        }
                    }
                }
            }
            else
            {
                c = fgetc(file);
                counter++;
            }
        }
        printf("-1");
    }
        //------------------------------------------------
        //------------------------------------------------
        //------------------------------------------------
        //In the end, we make a backup file for double undo.
        FILE * backup;
        file= fopen(address,"r");
        backup= fopen("D://vim project/backup/double_undo.txt", "w+");
        if(backup==NULL) puts("backup could not be openend.");

        c = fgetc(file);
        while (c != EOF)
        {
            fputc(c, backup);
            c = fgetc(file);
        }
        fclose(backup);
        fclose(file);

}

//--------------------------------------------------------------------1
int my_own_damn_strstr (char *a, char *b)
{
    int gotOne=0;
    int fail;
    for(int k=0; a[k+strlen(b)-1]!=NULL; k++)
    {
       fail=0;
       for(int i=0; i<strlen(b); i++)
        if(a[i+k]!=b[i]) fail=1;
       if(fail==0) return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------------------
void grep ()
{
    chdir("D://vim project/root/");
    int coption=0, loption=0, action=0;
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"-c")==0)
        coption=1;
    if(strcmp(check,"-l")==0)
        loption=1;
    if(strcmp(check,"--str")==0)
        action=1;
    if (action==0) for(int q=0; q<6; q++) getchar();
    //-----------------------------------------------------------------
     //get the whole thing all together as one text:
    for(int ko=0; ko<1000; ko++)
        text[ko]==NULL;
    if(text[0]=='"') remove_char(text,0);
    //get the info all together as a text:
    scanf(" %c",&text[0]);
    if(text[0]=='"') giome_count++;
    scanf("%[^\n]s", text+1);
    //printf("this is the whole thing:%s",text);

    //now we break down the text into separate parts:
    char common_message[1000];
    for(int jake=0; jake<1000; jake++)
        common_message[jake]=NULL;
    common_message[0]=text[0];
    int i=1;
    while(1)
    {
        if(text[i]!=92&&text[i+1]=='-'&&text[i+2]=='-'&&text[i+3]=='f'&&text[i+4]=='i'&&text[i+5]=='l'&&text[i+6]=='e'&&text[i+7]=='s')
            break;
        common_message[i]=text[i];
        i++;
    }
   // printf("this is the common message:%s",common_message);
    //extracting names:
    char names[1000][1000];
    for(int o=0; o<1000; o++)
        for(int y=0; y<1000; y++)
            names[o][y]=NULL;
    int file_counter=0;
    i=i+9;
 //printf("size%d  ",i);
    if(text[i]=='"')
    {
        while(1)
        {
            if(text[i]=='"' && text[i-1]!=92) {i+=2; file_counter++; break;}
            names[file_counter][i];
            i++;
        }
    }

    int jake=0;
    //printf("size%d  ",i);
    while(1)
    {
        if( i>=strlen(text) || text[i]==NULL ) break;
        if(text[i]=='"')
        {
            jake=0;
            while(1)
            {
                if((text[i]=='"' && text[i-1]!=92) || text[i]==NULL) {i+=2; file_counter++; break;}
                names[file_counter][jake]=text[i];
                //printf(" %c numer of i:%d\n", names[file_counter][jake], jake);
                i++;
            }
        }
        if(text[i]!='"')
        {
            jake=0;
            while(1)
            {
                if(text[i]==' ' || text[i]==NULL) {i++; file_counter++; break;}
                names[file_counter][jake]=text[i];
                //printf(" %c %d %d\n", names[file_counter][jake], file_counter, jake);
                i++;
                jake++;
            }
            jake=0;
        }
    }
    //printf("\nfile counter is:%d\n",file_counter);

    char string[1000]; for(int d=0; d<1000; d++) string[d]=NULL;
    char string2[1000]; for(int d=0; d<1000; d++) string2[d]=NULL;
    int ansCounter=0;

    for(int i=0; i<file_counter; i++)
    {
        for(int l=0; l<1000; l++)
            string[l]=names[i][l];
        //printf("this string is:%s\nthis is the first name:%c\n",string,names[i][0]);

        FILE *files;
        files = fopen(string, "r");
         if(files==NULL) {puts("error opening files"); return;}
        //--------------------------------------
        //count the number of lines in the file:
        char c;
        int flag=0;
        int nol=1;
        for (c = getc(files); c != EOF; c = getc(files))
        if (c == '\n')
            nol = nol + 1;
        //--------------------------------------
        fseek(files , 0 , SEEK_SET);
        fgets(string2, 1000 , files);
        for(int op=0; op<nol; op++)
        {
            flag=0;
            //printf("---------------------the line:%s---and the message:%s--\n",string2,common_message);
            if(my_own_damn_strstr(string2,common_message)==1) flag=1;
            if(string2[strlen(string2)-1]=='\n') {remove_char(string2,strlen(string2)-1);}

            if(flag==1 && coption==0 && loption==0) {printf("%s: %s\n",string,string2);}
            if(flag==1 && coption==1 && loption==0) {ansCounter++;}
            if(flag==1 && coption==0 && loption==1) {printf("%s\n",string); fclose(files); break;}

            fgets(string2, 1000, files);
        }

        for(int d=0; d<1000; d++) string2[d]=NULL;
        fclose(files);
    }
    if(coption==1) printf("%d",ansCounter);
}
//--------------------------------------------------------------------

int wordy(char *a, char*b)
{
    int word=0;
    int ia=0, ib=0;
    char temp;
    char *bmax, *amin;
    if(strlen(a)> strlen(b))
    {
       bmax=a; amin=b;
    }
    else
    {
        bmax=b; amin=a;
    }

    while(amin[ia]!='\0')
    {
        if(amin[ia]!=bmax[ib])
        {
            word++;
            aposition=ia;
            bposition=ib;
            while(amin[ia]!=' ' && amin[ia]!='\0') ia++;
            while(bmax[ib]!=' ' && bmax[ib]!='\0') ib++;
        }
        if(amin[ia]!='\0')
        ia++;
        ib++;
    }

    if(b[ib]=='\0') return word==1?1:0;
    else for(int ko=ib; ko < strlen(bmax); ko++)
    {
        if(bmax[ko]==' ') word++;
    }

    if (word==1) return 1;
    else return 0;
}

//--------------------------------------------------------------------

void compare()
{
    chdir("D://vim project");
    int giome_count=0;
    char file1[1000];
    char file2[1000];
    for(int i=0; i<1000; i++)
        {text[i]='\0'; file1[i]='\0'; file2[i]='\0';}
    scanf("%[^\n]s",text);
    if(text[0]=='"') giome_count=1;
    file1[0]=text[0];

    //getting file names and addresses:
    int file1end;
    int i=1;
    while(1)
    {
        if(text[i]=='"' && text[i-1]!=92) giome_count++;
        if(text[i]==' ' && giome_count%2==0) break;
        file1[i]=text[i];
        i++;
    }
    int k=0;
    giome_count=0;
    if(text[i]=='"') giome_count=1;
    file2[i]=text[i];
    i++;
    while(1)
    {
        if(text[i]=='"' && text[i-1]!=92) giome_count++;
        if(text[i]=='\0') break;
        file2[k]=text[i];
        i++;
        k++;
    }


    int j=1;
    if (file1[0]=='"') remove_char(file1 , 0);
    if (file2[0]=='"') remove_char(file2 , 0);
    if (file2[strlen(file2)-1]=='"') remove_char(file2 , strlen(file2)-1 );

    while(file1[j]!='\0')
    {
        if(file1[j]=='"' && file1[j-1]!=92)
            remove_char(file1 , j);
        if(file1[j]=='"' && file1[j-1]==92)
            remove_char(file1 , j-1);
        j++;
    }
    j=1;
    while(file2[j]!='\0')
    {
        if(file2[j]=='"' && file2[j-1]!=92)
            remove_char(file2 , j);
        if(file2[j]=='"' && file2[j-1]==92)
            remove_char(file2 , j-1);
        j++;
    }

    //printf("address of ile 1:%s\nand 2:%s\n",file1, file2);
    if(file1[0]=='/') remove_char(file1 , 0);
    if(file2[0]=='/') remove_char(file2 , 0);

    FILE *fi1;
    FILE *fi2;
    fi1=fopen(file1,"r");
    fi2=fopen(file2,"r");
    int lineNumber=0;
    int tline1=0;
    int tline2=0;
    int dead1=0;
    int dead2=0;
    int deathPos=0;
    char li1[1000];
    char li2[1000];


    for(int jake=0; jake<1000; jake++)
    {
        li1[jake]='\0';
        li2[jake]='\0';
    }

    if(fi1==NULL) {puts("Error opening the first file"); return;}
    if(fi2==NULL) {puts("Error opening the second file"); return;}
    //count number of lines for each file:
    while(1)
    {
        fgets(li1 , 1000 , fi1);
        if(li1[0]=='\0') {break;}
        tline1++;
        for(int h=0; h<1000; h++) li1[h]='\0';
    }
    while(1)
    {
        fgets(li2 , 1000 , fi2);
        if(li2[0]=='\0') {break;}
        tline2++;
        for(int h=0; h<1000; h++) li2[h]='\0';
    }
    for(int h=0; h<1000; h++) {li1[h]='\0'; li2[h]='\0';}
    fseek(fi1 , 0 , SEEK_SET);
    fseek(fi2 , 0 , SEEK_SET);
    //------------------------------------
    while (1)
    {

        fgets(li1 , 1000 , fi1);
        fgets(li2 , 1000 , fi2);
        lineNumber++;
        if(li1[0]=='\0') dead1=1;
        if(li2[0]=='\0') dead2=1;

        if(dead1==0 && dead2==0 && wordy(li1,li2)==1)
        {
            printf("============ #%d ============\n", lineNumber);
            aposition--; bposition--;
            //fix both positions to the first of the words.
            if(li1[aposition-1]!=' ' && aposition!=0)
                {while(li1[aposition-1]!=' ' && aposition!=0)
                    aposition--;
                }

            if(li2[bposition-1]!=' ' && bposition!=0)
                while(li2[bposition-1]!=' ' && bposition!=0)
                    bposition--;
            //---------------------------------------------
            int i;
            int endaposition=aposition;
            while(li1[endaposition]!=' ' && li1[endaposition]!='\0') endaposition++;
            int endbposition=bposition;
            while(li2[endbposition]!=' ' && li2[endbposition]!='\0') endbposition++;

            for(int i=0; i<strlen(li1); i++)
            {
                if(i==aposition)
                printf(">>");

                if(i==endaposition-1  && endaposition==strlen(li1))
                printf("<<");

                if(i==endaposition && endaposition!=strlen(li1))
                printf("<<");

                printf("%c",li1[i]);
            }


            for(int i=0; i<strlen(li2); i++)
            {
                if(i==bposition)
                printf(">>");

                if(i==endbposition-1 && endbposition==strlen(li2))
                printf("<<");

                 if(i==endbposition && endbposition!=strlen(li2))
                printf("<<");

                printf("%c",li2[i]);
            }
       }

        if(dead1==0 && dead2==0 && wordy(li1 , li2)==0)
            if(strcmp(li1, li2)!=0)
               {if(lineNumber!=tline1) {printf("============ #%d ============\n%s%s",lineNumber,li1,li2);}
                if(lineNumber==tline1) {printf("============ #%d ============\n%s\n%s",lineNumber,li1,li2);}}

        if(dead1==1 && dead2==1) return;
        if(dead1==1 && dead2==0)
        {
            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>>\n",lineNumber,tline2);

                while(li2[0]!='\0')
                {
                printf("%s",li2);
                for(int jake=0; jake<1000; jake++) li2[jake]='\0';
                fgets(li2 , 1000 , fi2);

                }
        }

        if(dead1==0 && dead2==1)
        {
            printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<",lineNumber,tline1);
            while(li1[0]!='\0')
                {
                printf("%s",li1);
                for(int jake=0; jake<1000; jake++) li1[jake]='\0';
                fgets(li1 , 1000 , fi1);
                }
        }

        for(int jake=0; jake<1000; jake++)
        {
            li1[jake]='\0'; li2[jake]='\0';
        }
    }
    puts("\n");
    fclose(fi1);
    fclose(fi2);
}


//--------------------------------------------------------------------
void tree_input()
{
    chdir("D://vim project/root");
    scanf("%d", &fixed_depth);
    tree(0);
}

char tpath[1000]="D://vim project/root/";

void tree(int depth)
{

    if(fixed_depth<-1)
    {puts("With all due respect, you're not making any sense, mate. Minimum depth is -1."); return;}

    if(depth==fixed_depth)
        return;

    struct dirent *dir;
    struct stat filestat;
    DIR *fol;
    fol = opendir(".");

    if(fol==NULL)
    {puts("Folder cannot be found. sorry."); return;}

    while(dir=readdir(fol))
    {
        char *branch = (char*) calloc(1000, sizeof(char));
        if(fixed_depth>depth)
        {
            if(depth!=0)strcat(branch, "|");
            for(int i=0; i<depth; i++)
                strcat(branch, "_____");
        }

        stat(dir->d_name,&filestat);
        if(dir->d_name[0] != '.' && S_ISDIR(filestat.st_mode))
        {
            printf("%s%s\n",branch,dir->d_name);
        }
         else if(dir->d_name[0] != '.')
        {
           // printf("%s*\n",dir->d_name);
           //strcat(tpath, dir->d_name);
        //chdir("D://vim project/root");

        strcat(tpath, dir->d_name); printf("this is how path look like:%s*****",tpath);
        if(GetFileAttributesA(tpath)!=FILE_ATTRIBUTE_HIDDEN)
        printf("%s%s\n",branch,dir->d_name);
        for(int i=0; i<1000; i++) tpath[i]='\0';
        strcpy(tpath,"D://vim project/root/");


        }


        //for(int g=0; g<1000; g++) root[g]='\0';
        //-----------------------------------------
        //we have printed the first layer. time to search deeper:


        if(S_ISDIR(filestat.st_mode) && dir->d_name[0] != '.' )
        {
            chdir(dir->d_name);
            //strcat(tpath,dir->d_name);
            //strcat(tpath, "/");
            tree(depth+1);
        }
    }
    closedir(fol);
}

//--------------------------------------------------------------------

void undo()
{
    chdir(family_roots);
    char check[1000];
    for(int jake=0; jake<1000; jake++) check[jake]=NULL;
    scanf("%s",check);
    getchar();
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address.");}
    else
    {
        char address[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;}

        //taking address after --file
        scanf("%[^\n]s",address);
        if(address[0]=='/') remove_char(address,0);
        char c;
        if(address[0]=='"') remove_char(address,0);
        int w=1;

        fix_the_string(address);
        if(address[0]=='/') remove_char(address,0);
        //------------------------------------------------------
        //extract the name out of this address:
        char name[1000]={'\0'};
        extract_name(address,name);
        //-------------------------------------
        chdir(family_roots);
        //-------------------------------------
        //from history to file:
        FILE *file;
        file=fopen(address,"w+");
        if(file==NULL) {puts("Well, I didn't find any file with THAT address...");}

        char history_address[]="D://vim project/backup/";
        strcat(history_address,name);
        FILE *history=fopen(history_address,"r+");

        char buffer[1000]={'\0'};
        fgets(buffer, 1000 ,history);
        while(!feof(history))
        {
            fputs(buffer,file);
            for(int i=0; i<1000; i++) buffer[i]='\0';
            fgets(buffer, 1000 , history);
        }
        fclose(file);
        fclose(history);
        //-------------------------------------------------------------------
        //copy backup into the hidden file:
        history=fopen(history_address,"w+");
        if(history==NULL) {puts("Well, I didn't find any history with THAT address..."); fclose(history); return;}
        FILE *backup=fopen("D://vim project/backup/double_undo.txt","r");
        if(backup==NULL) {fclose(history); fclose(backup); return;}

        for(int i=0; i<1000; i++) buffer[i]='\0';
        fgets(buffer, 1000 ,backup);
        while(!feof(backup))
        {
            fputs(buffer,history);
            for(int i=0; i<1000; i++) buffer[i]='\0';
            fgets(buffer, 1000 , backup);
        }
        fclose(history);
        fclose(backup);
        puts("Done. But please be careful my child, the choices you make in life cannot be easily reversed.");
    }
}

//--------------------------------------------------------------------
//اتو ایندنت زده شد ولی قسمت امتیازیش باگ داره. درستش کن در اتو ایندنت سیمپل
//--------------------------------------------------------------------




int main ()
{
    char statement[1000];
    while(1)
    {
        right_order=0;
        chdir(family_roots);

        for(int j=0; j<1000; j++)
        {statement[j]=NULL;}

        scanf("%s",statement);

        if (!strcmp(statement,"createfile"))
        {
            right_order=1;
            createfile();
        }


        if(!strcmp(statement,"cat"))
        {
            right_order=1;
            cat();
        }

        if(!strcmp(statement,"pastestr"))
        {
            right_order=1;
            pastestr();
        }


         if(!strcmp(statement,"insertstr"))
        {
            right_order=1;
            insert_input();
        }

        if(!strcmp(statement,"copystr"))
        {
            right_order=1;
            copystr_input();
        }


        if(!strcmp(statement,"cutstr"))
        {
            right_order=1;
            cutstr();
        }

        if(!strcmp(statement,"removestr"))
        {
            right_order=1;
            removestr_input();
        }

        if(!strcmp(statement,"find"))
        {
            right_order=1;
            find_input();
        }

        if(!strcmp(statement,"replace"))
        {
            right_order=1;
            replace_input();
        }

        if(!strcmp(statement,"undo"))
        {
            right_order=1;
            undo();
        }

        if(!strcmp(statement,"grep"))
        {
            right_order=1;
            grep();
        }

        if(!strcmp(statement,"compare"))
        {
            right_order=1;
            getchar();
            compare();
        }

        if(!strcmp(statement,"tree"))
        {
            right_order=1;
            tree_input();
        }

//        if(!strcmp(statement,"auto-indent"))
//        {
//            right_order=1;
//            auto_indent();
//        }

        if(!strcmp(statement,"exit"))
        {
            right_order=1;
            puts("Till we meet again, Nick!");
            break;
        }


        else if(right_order==0)
        {
            while(1)
            {
                char a;
                a=getchar();
                if (a=='\n') {puts("Invalid command, your majesty!");break;}
            }
        }


    }
}
//--------------------------------------------------------------------










