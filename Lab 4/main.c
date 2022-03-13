#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union Flags{
    char fl;
};

typedef struct {
    char file_identifier [4] ;
    char version [2];
    union Flags flags;
    int size;
}Id3v2Header;


typedef struct {
    char ID [5] ;
    int size;
    char flags [2] ;
}FrameHeader;

typedef struct {
    FrameHeader head;
    char text_encoding;
    char *text;
}TextFrames;



// Вспомогательные функции
void printbin (char x)
{
     static int i=8;
     if (i>0){
        i--;
        printbin(x/2);
        i++;
        printf ("%d", x%2);
        if (i==8) printf("\n");
     }
}


void fprint28oct(FILE *f, int x)
{
     static int i=4;
     if (i>0){
        i--;
        fprint28oct(f, x/128);
        i++;// ??? Почему эта строка необходима?
        char c=x%128;
        fprintf (f, "%c", c);

     }
}


int get_size_frame(FILE *f, char * nameframe)
{
    Id3v2Header head;

    for (int i=0; i<3; i++) head.file_identifier[i] = fgetc(f) ;
    head.file_identifier[3]='\0';


    for (int i=0; i<2; i++)  head.version[i] = fgetc(f);

    head.flags.fl=fgetc(f);

    head.size=0;
    for (int i=0; i<4; i++){
        char c=fgetc(f);
        head.size=head.size*128+c;
    }


    char s[5];
    for (int i=0; i<4; i++) s[i]=fgetc(f);
    s[4]='\0';


    for (int i=4; i<head.size; i++){
        if ( !strcmp(s, nameframe)  )
         {
                TextFrames frame;
                strcpy ( frame.head.ID, nameframe );


                frame.head.size=0;
                for (int i=0; i<4; i++){
                    char c=fgetc(f);
                    frame.head.size=frame.head.size*128+c;
                }
                fseek( f, 0, 0);
                return frame.head.size;
         }
        s[0]=s[1];
        s[1]=s[2];
        s[2]=s[3];
        s[3]=fgetc(f);
    }


    fseek( f, 0, 0);
    return 0;
}


// Основные функции
void show (FILE *f)
{
    printf("---------------------------------------\n");
    Id3v2Header head;

    for (int i=0; i<3; i++) head.file_identifier[i] = fgetc(f) ;
    head.file_identifier[3]='\0';
    printf("File indentifier |%s\n", head.file_identifier);


    printf("Version          |");
    for (int i=0; i<2; i++){
        head.version[i] = fgetc(f);
        printf("%02d ", head.version[i] );
    }
    printf("\n");


    printf("Flags            |");
    head.flags.fl=fgetc(f);
    printbin (head.flags.fl);

    printf("Size             |");
    head.size=0;
    for (int i=0; i<4; i++){
        char c=fgetc(f);
        head.size=head.size*128+c;
    }
    printf("%d\n", head.size);
    printf("---------------------------------------\n");

    char s[5];
    for (int i=0; i<4; i++) s[i]=fgetc(f);
    s[4]='\0';

    for (int i=4; i<head.size; i++){
//MALOVATO
        if (!strcmp(s, "TALB") || !strcmp(s, "TBPM") || !strcmp(s, "TCOM")
         || !strcmp(s, "TCON") || !strcmp(s, "TCOP") || !strcmp(s, "TDAT")
         || !strcmp(s, "TDLY") || !strcmp(s, "TENC") || !strcmp(s, "TEXT")
         || !strcmp(s, "TFLT") || !strcmp(s, "TIME") || !strcmp(s, "TIT1")
         || !strcmp(s, "TIT2") || !strcmp(s, "TIT3") || !strcmp(s, "TKEY")
         || !strcmp(s, "TLAN") || !strcmp(s, "TLEN") || !strcmp(s, "TMED")
         || !strcmp(s, "TOAL") || !strcmp(s, "TOFN") || !strcmp(s, "TOLY")
         || !strcmp(s, "TOPE") || !strcmp(s, "TORY") || !strcmp(s, "TOWN")
         || !strcmp(s, "TPE1") || !strcmp(s, "TPE2") || !strcmp(s, "TPE3")
         || !strcmp(s, "TRCK") || !strcmp(s, "TRDA") || !strcmp(s, "TRSN")
         || !strcmp(s, "TRSO") || !strcmp(s, "TSIZ") || !strcmp(s, "TSRC")
         || !strcmp(s, "TSSE") || !strcmp(s, "TYER") || !strcmp(s, "TXXX")
        || !strcmp(s, "TPUB"))
         {
                TextFrames frame;
                strcpy ( frame.head.ID, s );
                printf( "Frame indentifier|%s\n", frame.head.ID );

                printf("Size             |");
                //fread(&frhead, sizeof(int), 1, f);
                frame.head.size=0;
                for (int i=0; i<4; i++){
                    char c=fgetc(f);
                    frame.head.size=frame.head.size*128+c;
                }
                printf( "%d\n", frame.head.size );

                printf("Flags            |");
                frame.head.flags[0]=fgetc(f);
                frame.head.flags[1]=fgetc(f);
                printbin (frame.head.flags[0]);
                printf("Flags            |");
                printbin (frame.head.flags[1]);
                i+=6;
                if (!frame.head.size) break;

                frame.text_encoding=fgetc(f);
                //printf( "Text encoding    |%d\n", frame.text_encoding );

                frame.text= (char *) malloc( frame.head.size *sizeof(char) );// -1 за encoding +1 за '/0'
                for (int j=0; j<frame.head.size-1; j++, i++)
                    frame.text[j]=fgetc(f);
                frame.text [ frame.head.size-1 ]='\0';
                printf ("Information      |%s\n", frame.text );
                printf("---------------------------------------\n");
                free(frame.text);
         }

        s[0]=s[1];
        s[1]=s[2];
        s[2]=s[3];
        s[3]=fgetc(f);
    }
    printf("\n");
    fseek( f, 0, 0 );
}


void get ( FILE *f, char * nameframe)
{
    printf("---------------------------------------\n");
    Id3v2Header head;

    for (int i=0; i<3; i++) head.file_identifier[i] = fgetc(f) ;
    head.file_identifier[3]='\0';


    for (int i=0; i<2; i++)  head.version[i] = fgetc(f);

    head.flags.fl=fgetc(f);

    head.size=0;
    for (int i=0; i<4; i++){
        char c=fgetc(f);
        head.size=head.size*128+c;
    }


    char s[5];
    for (int i=0; i<4; i++) s[i]=fgetc(f);
    s[4]='\0';


    char flag=0;
    for (int i=4; i<head.size; i++){
        if ( !strcmp(s, nameframe)  )
         {
                flag=1;
                TextFrames frame;
                strcpy ( frame.head.ID, nameframe );
                printf( "Frame indentifier|%s\n", frame.head.ID );


                printf("Size             |");
                frame.head.size=0;
                for (int i=0; i<4; i++){
                    char c=fgetc(f);
                    frame.head.size=frame.head.size*128+c;
                }
                printf( "%d\n", frame.head.size );

                printf("Flags            |");
                frame.head.flags[0]=fgetc(f);
                frame.head.flags[1]=fgetc(f);
                printbin (frame.head.flags[0]);
                printf("Flags            |");
                printbin (frame.head.flags[1]);
                i+=6;
                if (!frame.head.size) break;

                frame.text_encoding=fgetc(f);
                //printf( "Text encoding    |%d\n", frame.text_encoding );

                frame.text= (char *) malloc( frame.head.size *sizeof(char) );// -1 за encoding +1 за '/0'
                for (int j=0; j<frame.head.size-1; j++, i++)
                    frame.text[j]=fgetc(f);
                frame.text [ frame.head.size-1 ]='\0';
                printf ("Information      |%s\n", frame.text );
                free(frame.text);
         }

        s[0]=s[1];
        s[1]=s[2];
        s[2]=s[3];
        s[3]=fgetc(f);
    }
    if ( !flag ) printf ("File doesn't have this frame\n");
    printf("---------------------------------------\n");
    printf("\n");
    fseek( f, 0, 0 );
}


void set ( FILE *f, char * nameframe, char* value )
{
    FILE *tmp=fopen("tmp.mp3", "w+b");

    Id3v2Header head;
    TextFrames frame;

    frame.head.size=get_size_frame(f, nameframe);

    for (int i=0; i<3; i++) head.file_identifier[i] = fgetc(f) ;
    for (int i=0; i<3; i++) fprintf(tmp, "%c", head.file_identifier[i] );
    head.file_identifier[3]='\0';


    for (int i=0; i<2; i++)  head.version[i] = fgetc(f);
    for (int i=0; i<2; i++)  fprintf(tmp, "%c", head.version[i] );


    head.flags.fl=fgetc(f);
    fprintf(tmp, "%c", head.flags.fl );


    head.size=0;
    for (int i=0; i<4; i++){
        char c=fgetc(f);
        head.size=head.size*128+c;
    }

    if (frame.head.size){
        fprint28oct( tmp, head.size-frame.head.size+strlen(value)+1 );

        char s[5];
        s[4]='\0';
        for (int i=0; i<4; i++) s[i]=fgetc(f);
        for (int i=0; i<4; i++) fprintf( tmp, "%c", s[i] );

        while ( strcmp(s, nameframe) ){
            s[0]=s[1];
            s[1]=s[2];
            s[2]=s[3];
            s[3]=fgetc(f);
            fprintf( tmp, "%c", s[3]);
        }


        fprint28oct( tmp, strlen(value)+1 );

        frame.head.size=0;
        for (int i=0; i<4; i++){
            char c=fgetc(f);
            frame.head.size=frame.head.size*128+c;
        }

        for (int i=0; i<2; i++) frame.head.flags[i]=fgetc(f);
        for (int i=0; i<2; i++) fprintf( tmp, "%c", frame.head.flags[i] );

        frame.text_encoding=fgetc(f);
        fprintf( tmp, "%c", frame.text_encoding );

        frame.text = (char*) malloc( frame.head.size*sizeof(char) );
        for (int j=0; j<frame.head.size-1; j++) frame.text[j]=fgetc(f);
        frame.text [ frame.head.size-1 ]='\0';
        fprintf( tmp, "%s", value);
    }

    else{
        fprint28oct( tmp, head.size-+strlen(value)+11 );
        while( head.size-- ) fprintf(tmp, "%c", fgetc(f) );
        fprintf( tmp, "%s", nameframe);
        fprint28oct(tmp, strlen(value)+1 );
        fprintf( tmp, "%c%c%c", 0, 0, 0);
        fprintf( tmp, "%s", value);

    }
    while (!feof(f)) fprintf(tmp, "%c", fgetc(f) );
        //fprintf(tmp, "%c", fgetc(f) );

    fseek( tmp, 0, 0 );
    fseek( f, 0, 0 );
    while (!feof(tmp)) fprintf(f, "%c", fgetc(tmp) );
    fclose (tmp);
    //printf("1\n");
}


int main(int argc, char *argv[])
{
    printf( "\n" );
    while (*argv[1]!='=') argv[1]++;
    argv[1]++;
    FILE *f = fopen(argv[1], "r+b");

    if (!strncmp(argv[2], "--set", 5)){
        while ( *argv[2] != '=' ) argv[2]++;
        while ( *argv[3] != '=' ) argv[3]++;
        set( f, ++argv[2] , ++argv[3] );
    }
    if (!strcmp(argv[2], "--show")){
        show(f);
    }
    if (!strncmp(argv[2], "--get", 5)){
        while ( *argv[2] != '=' ) argv[2]++;
        get( f, ++argv[2] );
    }
    fclose(f);
}


/* Mycop

*/
