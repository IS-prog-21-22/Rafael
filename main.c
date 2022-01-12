#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fin, *fout;
    fin=fopen("access_log_Jul95", "r");
    char *s;
    s=(char*) malloc( 500*sizeof(char) );
    int k=0, l=0;
    int dif;
    scanf("%d", &dif);
    while(!feof(fin)){
        fgets(s, 500, fin);
        l++;
    }
    fseek(fin, 0, 0);
    int j=0, *Local_times;
    Local_times=(int*) malloc(l*sizeof(int));
    for (fgets(s, 500, fin); !feof(fin); fgets(s, 500, fin) ){
        int day=0, hour=0, min=0, sec=0, status=0;
        int i=0;
        while (s[i]!='[')
            i++;
        i++;

        while (s[i]!='/'){
            day=(day*10)+s[i]-'0';
            i++;
        }
        i++;

        while (s[i]!=':')
            i++;
        i++;

        while (s[i]!=':'){
            hour=(hour*10)+s[i]-'0';
            i++;
        }
        i++;

        while (s[i]!=':'){
            min=(min*10)+s[i]-'0';
            i++;
        }
        i++;

        while (s[i]!=' '){
            sec=(sec*10)+s[i]-'0';
            i++;
        }
        i++;

        Local_times[j++]=day*86400 + hour * 3600 + min* 60 + sec;

        i=strlen(s)-1;
        while (s[i]!='"')
            i--;
        i++;
        i++;

        while (s[i]!=' '){
            status=(status*10)+s[i]-'0';
            i++;
        }
        i++;

        if (status>=500){
            printf("%s", s);
            k++;
        }
    }
    printf("%d\n", k);
    int max=0, max1, max2;
    for (int i=0, j=0; j<l-2; ){
        if (Local_times[j]-Local_times[i]<dif){
            if (j-(i-1)>max){
                max=j-(i-1);
                max1=Local_times[i];
                max2=Local_times[j];
            }
            else
                ;
            j++;
        }
        else
            i++;
    }

    printf("day %d hour %d min %d sec %d----", max1/86400, (max1%86400)/3600, (max1%3600)/60, max1%60);
    printf("day %d hour %d min %d sec %d", max2/86400, (max2%86400)/3600, (max2%3600)/60, max2%60);

    fclose(fin);
}
