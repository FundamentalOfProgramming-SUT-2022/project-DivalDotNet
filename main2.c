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
//------------------------
//Arman things:
int ar_pos=-5;
int insAr=0,finAr=0,grepAr=0;
char arman_string[100000];
char insert_address[1000];
int insert_line;
int insert_character;
char find_text[1000];
int tree_arr=0;
char temp[1000];
char second_tree[1000];
int arm_tree=0;
int arm=-5;
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
    if(address[len-1]=='\n') remove_char(address,len-1);
    if(address[0]=='"') remove_char(address,0);

    int z=0;
    if(address[0]==' ')
        while(address[z]==' ')
            remove_char(address,z);

    len=strlen(address);
    int m=strlen(address)-1;
    if(address[len-1]==' ')
        while(address[m]==' ')
            {remove_char(address,m); m--;}



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




void remove_spaces (char *address)
{
    int len=strlen(address);
    int z=0;
    if(address[0]==' ')
        while(address[z]==' ')
            remove_char(address,z);

    len=strlen(address);
    int m=strlen(address)-1;
    if(address[len-1]==' ')
        while(address[m]==' ')
            {remove_char(address,m); m--;}

}

//------------------------------------------------------------------------------------------
//arman functions:
int is_it_arman (char *text)
{
    int len=strlen(text);
    int i=len-2;
    arm=-5;
    while(1)
    {
        if(text[i+1]=='D' && text[i]=='=')
        {
            arm=i;
            break;
        }
        if(i<0) break;
        i--;
    }
    insAr=0,finAr=0,grepAr=0;
    return arm;
}

void break_down_arman(char *text, int arm , char *second)
{
    int len=strlen(text);
    char order[1000]={'\0'};
    for(int i=0; i<1000; i++) second[i]='\0';
    for(int j=arm+3; j<len ; j++)
        second[j-3-arm]=text[j];
    //printf("this is the second part:%s*\ntwo whole:%s\n",second,text);
    for(int k=len-1; k>=arm; k--)
        remove_char(text,k);
    remove_spaces(second);
    //we have now separated orders into two parts and fixed them.

    for(int y=0; second[y]!=' '; y++)
        order[y]=second[y];

    insAr=0,finAr=0,grepAr=0;

    if(!strcmp(order,"insertstr"))
        insAr=1;
    else if (!strcmp(order,"find"))
        finAr=1;
    else if (!strcmp(order,"grep"))
        grepAr=1;

    //printf("this is the final second part:%s*\nthe fisrt part:%s\nins%d",second,text,insAr);
}

void do_arman(char *second)
{
    if(insAr==1)
    {
        for(int j=0; j<1000; j++) insert_address[j]='\0';
        insert_character=0;
        insert_line=0;
        int i=18;
        //printf("this is the second part:%s*\n",second);
        while(1)
        {
             if(second[i]=='-' && second[i+1]=='-' && second[i+2]=='p' && second[i+3]=='o' && second[i+4]=='s' && second[i-1]!=92)
             break;
             insert_address[i-18]=second[i];
             i++;
        }
        fix_the_string(second);
        int leng=strlen(second);
        insert_line=second[leng-3]-48;
        insert_character=second[leng-1]-48;
        fix_the_string(insert_address);
        fix_the_string(arman_string);
        insert(insert_address,arman_string,insert_line,insert_character);
        //printf("we are going to insert - the address is:%s\nthe content(=name) is:%s\nline and character are:%d:%d\n",insert_address,arman_string,insert_line,insert_character);
    }

    if(finAr==1)
    {
        for(int i=0; i<1000; i++) find_text[i]='\0';
        int i=4;
        while(second[i]!='\0')
        {
            find_text[i-4]=second[i];
            i++;
        }
        strcat(arman_string,find_text);
        fix_the_string(arman_string);
        char replace_message[1000]={'\0'};
        giome_count=0;
        if(arman_string[0]=='"') giome_count++;
        find(arman_string,0,replace_message);
        //printf("we are going to find:the whole text with out find --str is:%s\n",arman_string);
    }

    if(grepAr==1)
    {
        char c=0;
        int l=0;
        remove_spaces(second);
        if(second[5]=='-' && second[6]=='c') c=1;
        if(second[5]=='-' && second[6]=='l') l=1;
        int dash_file=0;
        while(1)
        {
            if(second[dash_file]=='-') break;
            dash_file++;
        }
        char text[1000]={'\0'};
        if(c==0 && l==0)
            for(int i=dash_file-1; second[i]!='\0'; i++)
                text[i+1-dash_file]=second[i];
        else
            for(int i=dash_file-1+3; second[i]!='\0'; i++)
                text[i-2-dash_file]=second[i];

        strcat(arman_string,text);
        remove_spaces(arman_string);
        giome_count=0;
        //printf("this is the text:%s*\n",text);
        if(text[0]=='"') giome_count++;
        //printf("we are going to do grep:the text without -? --str is:%s*\nthe giome count is:%d\nloption and coption are:%d,%d\n",arman_string,giome_count,l,c);
        grep(arman_string,giome_count,l,c);
    }
}
//------------------------------------------------------------------------------------------

void createfile()
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
        //------------------------------------------------------
        //extract the name out of this address:
        char name[1000]={'\0'};
        extract_name(address,name);
        //-------------------------------------
        chdir(family_roots);
        //-------------------------------------
        int len;
        char final_address[1000]={'\0'};
        char *temp;
        temp=strtok(address,"/");
        len=strlen(temp);
        if(temp[0]=='/') remove_char(temp,0);
        if(chdir(temp)!=0)
            mkdir(temp);
        strcat(final_address,temp);
        strcat(final_address,"/");
        while((temp=strtok(NULL,"/")!=NULL))
        {
            strcat(final_address,temp);
            strcat(final_address,"/");
            if(chdir(final_address)!=0)
                mkdir(final_address);
        }
        chdir(final_address);
        FILE *file;
        strcat(final_address,name);
        file=fopen(final_address,"w");
        fclose(file);
    }
}
//------------------------------------------------------------------------------------------
//void create_file()
//{
//    chdir(family_roots);
//
//    char check[1000]={'\0'};
//    scanf("%s",check);
//    if(strcmp(check,"--file")!=0)
//        {puts("Don't forget to write --file before name and address."); return;}
//    else
//    {
//        //getting input:
//        char address[1000],name[1000],real_address[1000],tmp[1000];
//        for(int ko=0; ko<1000; ko++)
//            {address[ko]==NULL;name[ko]=NULL;real_address[ko]=NULL;tmp[ko]=NULL;}
//
//        //for example, if the address is root/my/hello.txt, real_address will be root/my/ and name will be hello.txt
//        gets(address);
//
//        //cutting the address into name and path:
//        int j=0,namezone=0;
//        int length=strlen(address);
//
//        for(int i=length-1;address[i]!='/';i--)
//            namezone++;
//
//        for(int i=length-(namezone);i<1000;i++)
//        {
//            name[j]=address[i];
//            j++;
//        }
//
//        //name has been extracted. getting the address of the file:
//        for(int m=0; m<length-namezone; m++)
//            real_address[m]=address[m];
//
//        //now, we should check if the address exists. If not, we should create it.
//        //the real address should be broken into separate parts:
//
//        for(int i=0; i<1000; i++)
//        {
//            tmp[i]=real_address[i];
//            if(real_address[i]=='/' && i!=1)
//                {if(chdir(tmp)!=0) mkdir(tmp);printf("\n temp address:?%s-------did we move to it?%d\n",tmp,chdir(tmp));}
//        }
//
//        printf("%sdid we move to the address?:%d\n",real_address,chdir(real_address));
//        //now that the path is settled, we need to create the file:
//        chdir(real_address);
//        FILE *file;
//        if((file=fopen(address,"r"))!=NULL) {puts("It already exists"); fclose(file); return;}
//        file= fopen(address, "w");
//        fclose(file);
//
//    }
//
//}
//------------------------------------------------------------------------------------------
//void createfile()
//{
//    char *directory;
//    char *next_directory;
//    char *new_address;
//    int counter = 0;
//    int result;
//
//
//    scanf(" %[^ ]s", command_extension);
//    if(strcmp(command_extension, "--file") != 0) {
//        printf("Don't forget to add --file before name.");
//        return;
//    }
//
//    scanf("%[^\n]s", address);
//    // strcpy(cpy_address, address);
//
//    directory = strtok(address, "/");
//    directory = strtok(NULL, "/");
//    directory = strtok(NULL, "/");
//
//    // printf("%s\n", directory);
//    while(directory != NULL)
//    {
//        counter++;
//        next_directory = strtok(NULL, "/");
//        if(next_directory == NULL) {
//            if(!access(directory, F_OK)) {
//                printf("File already exist!\n");
//                return;
//            }
//            filepointer = fopen(directory, "a");
//            fclose(directory);
//            break;
//        }
//        result = mkdir(directory);
//        chdir(directory);
//
//        // mkdir(directory);
//        strcpy(directory, next_directory);
//    }
//    if(!result)
//    printf("File created successfully!\n");
//    // printf("count = %d\n", counter);
//
//    while(counter != 1)
//    {
//        chdir("..");
//        counter--;
//    }
//    return;
//}

//-----------------------------------------------------------------------------
void cat()
{
    chdir("D://vim project");
    char check[1000]={'\0'};
    scanf("%s",check);
    if(strcmp(check,"--file")!=0)
        {puts("Don't forget to write --file before name and address."); return;}
    else
    {

        char address[1000],name[1000],realaddress[1000];
        for(int ko=0; ko<1000; ko++)
            {address[ko]==NULL;name[ko]=NULL;realaddress[ko]=NULL;}
        scanf("%[^\n]s",address);

        //check for arman:
        remove_spaces(address);
        arm= is_it_arman(address);
        char second[1000]={'\0'};
        //printf("this is the arm:%d,\nthe text to be passed:%s***\n",arm,address);
        if(arm>=0) break_down_arman(address,arm,second);
        for(int i=0; i<100000; i++) arman_string[i]='\0';
        //----------------------------------------------

        fix_the_string(address);
        if(address[0]=='/') remove_char(address,0);
        FILE *file;
        file=fopen(address,"r");
        if(file==NULL) {puts("Hmmm... I couldn't find a file with that address.");return;}

        int jake=0;
        char c = fgetc(file);
        while (c != EOF)
        {
            if(arm<0) printf("%c",c);
            if(arm>=0) {arman_string[jake]=c; jake++;}
            c = fgetc(file);
        }
        //printf("this is arman:%s*\n",arman_string);
        if(arm>=0) do_arman(second);

        fclose(file);
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
    //printf("this should be the path of history:%s\n",history_address);
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
     FILE* file_tmp = fopen("D://vim project/backup/tmp.txt","w+");
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
        file_tmp = fopen("D://vim project/backup/tmp.txt","r");
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
    //check for arman:
    remove_spaces(text);
    arm= is_it_arman(text);
    char second[1000]={'\0'};
    //printf("this is the arm:%d,\nthe text to be passed:%s***\n",arm,text);
    if(arm>=0) break_down_arman(text,arm,second);
    for(int i=0; i<100000; i++) arman_string[i]='\0';
    //----------------------------------------------
    find(text,0,replace_message);
    if(arm>=0) do_arman(second);

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
       while(text[i+8]!=NULL)
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
            for(int r=strlen(address)-get_the_address; r<strlen(address); r++)
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

                    if(byword_positive==0 && have_replace==0)
                    {
                        if(arm<0) {printf("That thing you wanted... I found it! take a look at this character position: %d\n", position-1); return;}
                        if(arm>=0) arman_string[0]=position-1;
                    }
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
                         if(arm<0)
                         {
                            printf("That thing you wanted... I found it! take a look at this word position: %d\n", word_counter);
                            return;
                         }

                         if(arm>=0) arman_string[0]=word_counter;
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
        if(number_of_winners!=0)
        {
            if(arm<0) printf("Guess how many times this wicked thing was found in the file: %d\n",number_of_winners);
            if(arm>=0) arman_string[0]=number_of_winners;
        }
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

            if(arm<0) printf("%d ", word_counter);
            if(arm>=0) arman_string[o]=word_counter;
            }
            puts("\n");
        }
        if(byword_positive==0)
        {
            if(have_replace==0)
            {
                for(int j=0; j<nom; j++)
                {if(arm<0) printf("%d ", number_of_winners[j]);
                if(arm>=0) arman_string[j]=number_of_winners[j];}
                puts("\n");
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
                        if(byword_positive==0 && have_replace==0)
                            if (arm<0) {printf("That thing you wanted... I found it! take a look at this character position: %d\n", position-1); return;}
                            else if(arm>=0) arman_string[0]=position-1;
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
                            if(arm<0)
                            {printf("That thing you wanted... I found it! take a look at this word position: %d\n", word_counter);
                            return;}
                            if(arm>=0) arman_string[0]=word_counter;
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
void grep_input()
{
    chdir("D://vim project/root/");
    char check[1000];
    int coption=0, loption=0, action=0;
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

    //get the whole thing all together as one text:
    for(int ko=0; ko<1000; ko++)
        text[ko]==NULL;
    //get the info all together as a text:
    scanf(" %c",&text[0]);
    if(text[0]=='"') giome_count++;
    scanf("%[^\n]s", text+1);

    //check for arman:
    remove_spaces(text);
    arm= is_it_arman(text);
    char second[1000]={'\0'};
    //printf("this is the arm:%d,\nthe text to be passed:%s***\n",arm,address);
    if(arm>=0) break_down_arman(text,arm,second);
    for(int i=0; i<100000; i++) arman_string[i]='\0';
    //----------------------------------------------
    grep(text,giome_count,loption,coption);
    if(arm>=0)do_arman(second);

}

struct files
{
    char name[1000];
};


void grep (char *text, int giome_count, int loption, int coption)
{
    chdir("D://vim project/root/");
    //-----------------------------------------------------------------
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
    fix_the_string(common_message);
    //printf("this is the common message:%s",common_message);
    int file_counter=0;
    i=i+9;
    struct files f[1000];
    while(1)
    {
        if(text[i]==NULL) {break;}
        if(text[i]=='"')
        {
            i++;
            int j=0;
            while(1)
            {
                if(text[i]==NULL) break;
                if(text[i]=='"') {file_counter++; i+=2; break;}
                f[file_counter].name[j]=text[i];
                i++;
                j++;
            }
        }

        if(text[i]!='"')
        {
            int j=0;
            while(1)
            {
                if(text[i]==' ' || text[i]==NULL) {file_counter++; i++; break;}
                f[file_counter].name[j]=text[i];
                i++;
                j++;
            }
        }
    }
    for(int t=0; t<file_counter; t++) fix_the_string(f[t].name);
    char string[1000]; for(int d=0; d<1000; d++) string[d]=NULL;
    char string2[1000]; for(int d=0; d<1000; d++) string2[d]=NULL;
    int ansCounter=0;

    for(int i=0; i<file_counter; i++)
    {
        for(int l=0; l<1000; l++)
            string[l]=f[i].name[l];
        FILE *files;
        files = fopen(string, "r"); //printf("%s\n",string);
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

            if(flag==1 && coption==0 && loption==0) {if(arm<0) printf("%s: %s\n",string,string2); if(arm>=0){strcat(arman_string,string); strcat(arman_string,": ");strcat(arman_string,string2);strcat(arman_string,"\n");}}
            if(flag==1 && coption==1 && loption==0) {ansCounter++;}
            if(flag==1 && coption==0 && loption==1)
                {
                    if(arm<0) {printf("%s\n",string); fclose(files); break;}
                    if(arm>=0){strcpy(arman_string,string);}
                }

            fgets(string2, 1000, files);
        }

        for(int d=0; d<1000; d++) string2[d]=NULL;
        fclose(files);
    }
    if(coption==1)
    {
        if(arm<0)printf("%d\n",ansCounter);
        if(arm>0)arman_string[0]=ansCounter;
    }
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
    for(int o=0; o<1000; o++) temp[o]='\0';
    fixed_depth=0;
    scanf("%[^\n]s",temp);
    remove_spaces(temp);
    int i=0;
    while(temp[i]>47 && temp[i]<58)
    {
        fixed_depth+=temp[i];
        fixed_depth=fixed_depth*10;
        i++;
    }
    //check for arman:
    remove_spaces(temp);
    arm_tree=is_it_arman(temp);
    for(int o=0; o<1000; o++) second_tree[o]='\0';
    //printf("this is the arm_tree:%d,the text to be passed:%s***\n",arm_tree,temp);
    if(arm_tree>=0) break_down_arman(temp,arm_tree,second_tree);
    for(int i=0; i<100000; i++) arman_string[i]='\0';
    tree(0);
    //printf("%s",arman_string);
    if(arm_tree>=0) do_arman(second_tree);
}

//char tpath[1000]="D://vim project/root/";

void tree(int depth)
{
    //----------------------------------------------------------------------------------------------
    if(fixed_depth<-1)
    {puts("With all due respect, you're not making any sense, mate. Minimum depth is -1."); return;}
    if(depth==fixed_depth-1)
       {
           if(arm_tree<0) return;
       }

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
        if(dir->d_name[0] != '.')
        {
            if(arm_tree<0) printf("%s%s\n",branch,dir->d_name);
            if(arm_tree>=0)
            {
                //strcat(arman_string,branch);
                strcat(arman_string,dir->d_name);
                strcat(arman_string,"\n");
            }
        }
        //-----------------------------------------
        //we have printed the first layer. time to search deeper:

        if(S_ISDIR(filestat.st_mode) && dir->d_name[0] != '.' )
        {
            chdir(dir->d_name);
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

void auto_indent()
{
    chdir("D://vim project/root");
    //char address[1000],buffer[1000]; for(int i=0; i<1000; i++) {address[i]='\0'; buffer[i]='\0';}
    //should add a function to make sure the address is correct.
    int pre=0;
    int af=0;
    scanf("%[^\n]s",address);
    FILE *file;
    FILE *temp;
    file=fopen(address,"r+");
    temp=fopen("D://vim project/backup/indent.txt", "w");
    if(file==NULL) {puts("Your file could not be found. Try again later!"); return;}
    int line=0;
    char c;
    while((c=fgetc(file))!=EOF)
    if(c=='\n') line++;
    fseek(file , 0 , SEEK_SET);


    int indent=0;
    char buffer[1024]={'\0'};
    char buffer2[1024]={'\0'};
    int counter=1;

    while(counter<=line+1)
    {
        fgets(buffer , 1024 , file);
        for(int i=0; i<strlen(buffer); i++)
        {
            if((buffer[i]=='{' || buffer[i]=='}') && i!=0)
            {
                //printf("%d for the place %d/",(buffer[temp-1]==' '),temp-1);
                int bremoval=0;
                int temp=i;
                if(buffer[temp-1]==32)
                while(buffer[temp-1]==32)
                    {remove_char(buffer, temp-1); bremoval++; temp--;}

                temp=i+1-bremoval;
                if(buffer[temp]==32)
                while(buffer[temp]==32)
                    {remove_char(buffer, temp);}
            }
        }
        //حالا باید اینتر ها رو رعایت کنیم

        if(buffer[strlen(buffer)-1]=='\n') remove_char(buffer,strlen(buffer)-1);

        for(int i=0; i< strlen(buffer) ; i++)
        {

            if(buffer[i]=='{')
                {
                    //printf("here2");
                    indent++;
                    add_char(buffer, i+1 , '\n');
                    for(int l=0; l<indent*4; l++)
                    add_char(buffer, i+2 , ' ');
                    add_char(buffer , i , ' ');
                    i+=indent*4+2;
                }

            //printf("in the first line, this is what we start with to check }: %d\n",i);
            if(buffer[i]=='}')
                {
                    //printf("here");
                    indent--;
                    add_char(buffer, i+1 , '\n');
                    add_char(buffer, i , '\n');
                    for(int l=0; l<indent*4; l++)
                    add_char(buffer, i+1 , ' ');
                    for(int l=0; l<indent*4; l++)
                    add_char(buffer, i+7 , ' ');
                    i+=indent*4*2+2;
                }

            if(i==0 && buffer[i]!='{' && buffer[i]!='}')
                {for(int y=0; y<indent*4; y++)
                    add_char(buffer , 0 , ' ');
                 i+=4*indent;}
        }


        strcat(buffer2,buffer);
        //fputs(buffer,temp);

        for(int i=0; i<1024; i++) buffer[i]='\0';
        counter++;
    }


printf("%s",buffer2);




}




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
            grep_input();
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
            tree_arr=0;
            tree_input();
        }

        if(!strcmp(statement,"auto-indent"))
        {
            right_order=1;
            auto_indent();
        }

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










