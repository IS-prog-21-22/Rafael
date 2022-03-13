#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <direct.h>

typedef struct {
    char ID[2];
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t  offset;
}BitmapFileHeader;

typedef struct {
    uint32_t info_header_size;
    int32_t width;
    int32_t height;
    uint16_t plane;
    uint16_t color_depth;
    uint32_t compression_method;
    uint32_t image_size;
    int32_t horizontal_resolution;
    int32_t vertical_resoluton;
    uint32_t number_of_colors;
    uint32_t number_of_important_colors;
}BitmapInfoHeader;

typedef struct {
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Reserved;
}ColorDefinition;


union EightPixel{
    unsigned char ep;
    struct{
        unsigned char b7: 1;
        unsigned char b6: 1;
        unsigned char b5: 1;
        unsigned char b4: 1;
        unsigned char b3: 1;
        unsigned char b2: 1;
        unsigned char b1: 1;
        unsigned char b0: 1;
    };
};

unsigned char getpixel (union EightPixel** PiData, int y, int x)
{
    switch (x%8)
    {
    case 0:
        return PiData[y][x/8].b0;
    case 1:
        return PiData[y][x/8].b1;
    case 2:
        return PiData[y][x/8].b2;
    case 3:
        return PiData[y][x/8].b3;
    case 4:
        return PiData[y][x/8].b4;
    case 5:
        return PiData[y][x/8].b5;
    case 6:
        return PiData[y][x/8].b6;
    case 7:
        return PiData[y][x/8].b7;
    }
}

void changepixel (union EightPixel** PiData, int y, int x)
{
    switch (x%8)
    {
    case 0:
        if ( PiData[y][x/8].b0 ==1 ) PiData[y][x/8].b0=0 ;
        else PiData[y][x/8].b0=1 ;
        break;
    case 1:
        if ( PiData[y][x/8].b1 ==1 ) PiData[y][x/8].b1=0 ;
        else PiData[y][x/8].b1=1 ;
        break;
    case 2:
        if ( PiData[y][x/8].b2 ==1 ) PiData[y][x/8].b2=0 ;
        else PiData[y][x/8].b2=1 ;
        break;
    case 3:
        if ( PiData[y][x/8].b3 ==1 ) PiData[y][x/8].b3=0 ;
        else PiData[y][x/8].b3=1 ;
        break;
    case 4:
        if ( PiData[y][x/8].b4 ==1 ) PiData[y][x/8].b4=0 ;
        else PiData[y][x/8].b4=1 ;
        break;
    case 5:
        if ( PiData[y][x/8].b5 ==1 ) PiData[y][x/8].b5=0 ;
        else PiData[y][x/8].b5=1 ;
        break;
    case 6:
        if ( PiData[y][x/8].b6 ==1 ) PiData[y][x/8].b6=0 ;
        else PiData[y][x/8].b6=1 ;
        break;
    case 7:
        if ( PiData[y][x/8].b7 ==1 ) PiData[y][x/8].b7=0 ;
        else PiData[y][x/8].b7=1 ;
        break;
    }
}

void upgradepixeldata(union EightPixel** ori, union EightPixel** cpy,  int h, int w)
{


    if (!(ori[0][0].b1+ori[1][0].b0+ori[1][0].b1<=1) && ori[0][0].b0==0 )
        cpy[0][0].b0=1;
    if ((ori[0][0].b1+ori[1][0].b0+ori[1][0].b1==0) && ori[0][0].b0==1 )
        cpy[0][0].b0=0;

    if (!(ori[h-1][0].b1 + ori[h-2][0].b0 + ori[h-2][0].b1<=1) && ori[h-1][0].b0==0 )
        cpy[h-1][0].b0=1;
    if ((ori[h-1][0].b1 + ori[h-2][0].b0 + ori[h-2][0].b1==0) && ori[h-1][0].b0==1 )
        cpy[0][0].b0=0;

    if ( !( getpixel(ori, 0, w-2) + getpixel(ori, 1, w-2) + getpixel(ori, 1, w-1)<=1 ) && getpixel(ori, 0, w-1)==0 )
        changepixel(cpy, 0, w-1);
    if ( ( getpixel(ori, 0, w-2) + getpixel(ori, 1, w-2) + getpixel(ori, 1, w-1)==0 ) && getpixel(ori, 0, w-1)==1 )
        changepixel(cpy, 0, w-1);

    if ( !( getpixel(ori, h-1, w-2) + getpixel(ori, h-2, w-2) + getpixel(ori, h-2, w-1)<=1 ) && getpixel(ori, h-1, w-1)==0 )
        changepixel(cpy, h-1, w-1);
    if ( ( getpixel(ori, h-1, w-2) + getpixel(ori, h-2, w-2) + getpixel(ori, h-2, w-1)==0 ) && getpixel(ori, h-1, w-1)==1 )
        changepixel(cpy, h-1, w-1);

    for (int i=1; i<h-1; i++){
        if (!(ori[i-1][0].b0+ori[i-1][0].b1+ori[i][0].b1+ori[i+1][0].b0+ori[i+1][0].b1==2 ||
              ori[i-1][0].b0+ori[i-1][0].b1+ori[i][0].b1+ori[i+1][0].b0+ori[i+1][0].b1==3) && ori[i][0].b0==0 )
            cpy[h-1][0].b0=1;
        if ( (ori[i-1][0].b0+ori[i-1][0].b1+ori[i][0].b1+ori[i+1][0].b0+ori[i+1][0].b1==2) && ori[i][0].b0==1 )
            cpy[0][0].b0=0;

        if ( !( getpixel(ori, i-1, w-1) + getpixel(ori, i-1, w-2) + getpixel(ori, i, w-2) + getpixel(ori, i+1, w-1) + getpixel(ori, i+1, w-2)==2 ||
                getpixel(ori, i-1, w-1) + getpixel(ori, i-1, w-2) + getpixel(ori, i, w-2) + getpixel(ori, i+1, w-1) + getpixel(ori, i+1, w-2)==3) && getpixel(ori, i, w-1)==0 )
            changepixel(cpy, i, w-1);
        if ( ( getpixel(ori, i-1, w-1) + getpixel(ori, i-1, w-2) + getpixel(ori, i, w-2) + getpixel(ori, i+1, w-1) + getpixel(ori, i+1, w-2)==2 ) && getpixel(ori, i, w-1)==1 )
            changepixel(cpy, i, w-1);
    }

    for (int j=1; j<w-1; j++){
    if ( !( getpixel(ori, 0, j-1) + getpixel(ori, 1, j-1) + getpixel(ori, 1, j) + getpixel(ori, 0, j+1) + getpixel(ori, 1, j+1)==2 ||
            getpixel(ori, 0, j-1) + getpixel(ori, 1, j-1) + getpixel(ori, 1, j) + getpixel(ori, 0, j+1) + getpixel(ori, 1, j+1)==3) && getpixel(ori, 0, j)==0 )
        changepixel(cpy, 0, j);
    if ( ( getpixel(ori, 0, j-1) + getpixel(ori, 1, j-1) + getpixel(ori, 1, j) + getpixel(ori, 0, j+1) + getpixel(ori, 1, j+1)==2 ) && getpixel(ori, 0, j)==1 )
        changepixel(cpy, 0, j);

    if ( !( getpixel(ori, h-1, j-1) + getpixel(ori, h-2, j-1) + getpixel(ori, h-2, j) + getpixel(ori, h-1, j+1) + getpixel(ori, h-2, j+1)==2 ||
            getpixel(ori, h-1, j-1) + getpixel(ori, h-2, j-1) + getpixel(ori, h-2, j) + getpixel(ori, h-1, j+1) + getpixel(ori, h-2, j+1)==3) && getpixel(ori, h-1, j)==0 )
        changepixel(cpy, h-1, j);
    if ( ( getpixel(ori, h-1, j-1) + getpixel(ori, h-2, j-1) + getpixel(ori, h-2, j) + getpixel(ori, h-1, j+1) + getpixel(ori, h-2, j+1)==2 ) && getpixel(ori, h-1, j)==1 )
        changepixel(cpy, h-1, j);
    }

    for (int i=1; i<h-1; i++) for (int j=1; j<w-1; j++){
    unsigned char s=getpixel(ori, i-1, j-1) + getpixel(ori, i-1, j) + getpixel(ori, i-1, j+1) + getpixel(ori, i, j-1) +
        getpixel(ori, i, j+1)+ getpixel(ori, i+1, j-1) + getpixel(ori, i+1, j) + getpixel(ori, i+1, j+1);
    if ( s!=5 && s!=6 && (getpixel(ori, i, j)==0) ) changepixel(cpy, i, j);
    if (  (s==5)  && (getpixel(ori, i, j)==1) ) changepixel(cpy, i, j);
    }
}

void write_new_file(FILE* fin, FILE* fout)
{
    fseek(fin, 0, 0);
    fseek(fout, 0, 0);

    BitmapFileHeader head;
    fread( head.ID, sizeof(char), 2, fin );
    head.ID[2]='\0';
    fwrite( head.ID, sizeof(char), 2, fout );
    if (strcmp(head.ID, "BM")) {
        fprintf(stderr, "Input file isn't .bmp\n");
        exit(1);
    }
    fread( &head.size, sizeof(int), 1, fin );
    fwrite( &head.size, sizeof(int), 1, fout );
    if (!head.size) {
        fprintf(stderr, "Input file is empty\n");
        exit(1);
    }
    fread( &head.reserved1, sizeof(int16_t), 1, fin);
    fwrite( &head.reserved1, sizeof(int16_t), 1, fout);
    fread( &head.reserved2, sizeof(int16_t), 1, fin);
    fwrite( &head.reserved2, sizeof(int16_t), 1, fout);
    fread( &head.offset, sizeof(int), 1, fin );
    fwrite( &head.offset, sizeof(int), 1, fout );

    BitmapInfoHeader infohead;
    fread( &infohead.info_header_size, sizeof(int), 1, fin);
    fwrite( &infohead.info_header_size, sizeof(int), 1, fout);
    fread( &infohead.width, sizeof(int), 1, fin);
    fwrite( &infohead.width, sizeof(int), 1, fout);
    fread( &infohead.height, sizeof(int), 1, fin);
    fwrite( &infohead.height, sizeof(int), 1, fout);
    fread( &infohead.plane, sizeof(int16_t), 1, fin);
    fwrite( &infohead.plane, sizeof(int16_t), 1, fout);
    fread( &infohead.color_depth, sizeof(int16_t), 1, fin);
    fwrite( &infohead.color_depth, sizeof(int16_t), 1, fout);
    if (infohead.color_depth != 1){
        fprintf(stderr, "Input file is not monochrome");
        exit(1);
    }

    fread( &infohead.compression_method, sizeof(int), 1, fin);
    fwrite( &infohead.compression_method, sizeof(int), 1, fout);
    fread( &infohead.image_size, sizeof(int), 1, fin);
    fwrite( &infohead.image_size, sizeof(int), 1, fout);
    fread( &infohead.horizontal_resolution, sizeof(int), 1, fin);
    fwrite( &infohead.horizontal_resolution, sizeof(int), 1, fout);
    fread( &infohead.vertical_resoluton, sizeof(int), 1, fin);
    fwrite( &infohead.vertical_resoluton, sizeof(int), 1, fout);
    fread( &infohead.number_of_colors, sizeof(int), 1, fin);
    fwrite( &infohead.number_of_colors, sizeof(int), 1, fout);
    fread( &infohead.number_of_important_colors, sizeof(int), 1, fin);
    fwrite( &infohead.number_of_important_colors, sizeof(int), 1, fout);

    ColorDefinition color_table[2];
    fread(color_table, sizeof(int32_t), 2, fin);
    fwrite(color_table, sizeof(int32_t), 2, fout);

    union EightPixel** PixelData;
    union EightPixel** cpy;
    PixelData=(union EightPixel**) malloc(sizeof(union EightPixel*) *infohead.height);
    cpy=(union EightPixel**) malloc(sizeof(union EightPixel*) *infohead.height);
    for (int i=0; i<infohead.height; i++){
        PixelData[i]=(union EightPixel*) malloc(sizeof(union EightPixel) * (( infohead.width+31)/32*4) );
        cpy[i]=(union EightPixel*) malloc(sizeof(union EightPixel) * (( infohead.width+31)/32*4) );
        fread(PixelData[i], sizeof(union EightPixel), (( infohead.width+31)/32*4), fin);
        for( int j=0; j<(infohead.width+31)/32*4; j++) cpy[i][j] = PixelData[i][j];
    }


    upgradepixeldata(PixelData, cpy, infohead.height, infohead.width);

    for (int i=0; i<infohead.height; i++){
        fwrite(cpy[i], sizeof(union EightPixel), (( infohead.width+31)/32*4), fout);
    }

    for (int i=0; i<infohead.height; i++){
        free(PixelData[i]);
        free(cpy[i]);
    }
    free(PixelData);
    free(cpy);

}

int main(int argc,char *argv[] )
{
    printf("\n");
    if (argc<5){ fprintf(stderr, "Too few arguments\n"); exit(0); }
    if (argc>9){ fprintf(stderr, "Too many arguments\n"); exit(0); }
    FILE *fin;
    char* dir;
    int max=20;
    int freq=1;

    for (int i=1; i<argc; i++){
        if ( !strcmp("--input", argv[i]) ){
                fin=fopen(argv[++i], "r+b");
                if (fin==NULL){ fprintf( stderr, "Can't open input file %s", argv[i] ); exit(1); }
        }
        if ( !strcmp("--output", argv[i] ) ){
            mkdir( argv[++i] );
            dir=argv[i];
        }
        if ( !strcmp("--max_iter", argv[i] ) ) max= atoi(argv[++i]);
        if ( !strcmp("--dump_freq", argv[i] ) ) freq= atoi(argv[++i]);
    }


    for (int i=0; i<max*freq; i+=freq){
        char* dirname=(char*) malloc(sizeof(char)*20);
        strcpy(dirname, dir);
        strcat(dirname, "\\");
        char* a=(char*) malloc(sizeof(char)*6);
        itoa(i, a, 10);
        strcat(dirname, a);
        free(a);
        strcat(dirname, ".bmp");
        FILE* fout=fopen(dirname, "w+b");
        write_new_file(fin, fout);
        fin=fopen(dirname, "r+b");
        free(dirname);
        fclose(fout);
    }

    fclose(fin);
    return 0;
}

/*Mycop





    fread( &head.ID, 2, 1, fin);
    fread( &head.size, sizeof(BitmapFileHeader)-4, 1, fin);
    fread( &infohead, sizeof(BitmapInfoHeader), 1, fin);
    printf( "%d\n", sizeof(BitmapFileHeader) );

*/
