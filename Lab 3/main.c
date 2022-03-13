#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Get_diference (char* s1, char* s2)
{
    int year1=0, day1=0, hour1=0, min1=0, sec1=0;
    char* month1=(char *) malloc( 3*sizeof(char));

    while (*s1!='[') s1++;
    s1++;

    while (*s1!='/'){
        day1=(day1*10)+*s1-'0';
        s1++;
    }
    s1++;

    strncpy(month1, s1, 3);
    if ( !strcmp(month1,"Jan") )
            ;
    else if ( !strcmp(month1,"Feb") )
            day1+=31;
    else if ( !strcmp(month1,"Mar") )
            day1+=59;
    else if ( !strcmp(month1,"Apr") )
            day1+=90;
    else if ( !strcmp(month1,"May") )
            day1+=120;
    else if ( !strcmp(month1,"Jun") )
            day1+=151;
    else if ( !strcmp(month1,"Jul") )
            day1+=181;
    else if ( !strcmp(month1,"Aug") )
            day1+=212;
    else if ( !strcmp(month1,"Sep") )
            day1+=243;
    else if ( !strcmp(month1,"Oct") )
            day1+=273;
    else if ( !strcmp(month1,"Nov") )
            day1+=304;
    else if ( !strcmp(month1,"Dec") )
            day1+=334;;
    s1+=4;

    while (*s1!=':'){
        year1=year1*10+*s1-'0';
        s1++;
    }
    s1++;

    while (*s1!=':'){
        hour1=(hour1*10)+*s1-'0';
        s1++;
    }
    s1++;

    while (*s1!=':'){
        min1=(min1*10)+*s1-'0';
        s1++;
    }
    s1++;

    while (*s1!=' '){
        sec1=(sec1*10)+*s1-'0';
        s1++;
    }
    //printf("%c",*s);

    int year2=0, day2=0, hour2=0, min2=0, sec2=0;
    char* month2=(char *) malloc( 3*sizeof(char));;

    while (*s2!='[') s2++;
    s2++;

    while (*s2!='/'){
        day2=(day2*10)+*s2-'0';
        s2++;
    }
    s2++;

    strncpy(month2, s2, 3);
    if ( !strcmp(month2,"Jan") )
            ;
    else if ( !strcmp(month2,"Feb") )
            day2+=31;
    else if ( !strcmp(month2,"Mar") )
            day2+=59;
    else if ( !strcmp(month2,"Apr") )
            day2+=90;
    else if ( !strcmp(month2,"May") )
            day2+=120;
    else if ( !strcmp(month2,"Jun") )
            day2+=151;
    else if ( !strcmp(month2,"Jul") )
            day2+=181;
    else if ( !strcmp(month2,"Aug") )
            day2+=212;
    else if ( !strcmp(month2,"Sep") )
            day2+=243;
    else if ( !strcmp(month2,"Oct") )
            day2+=273;
    else if ( !strcmp(month2,"Nov") )
            day2+=304;
    else if ( !strcmp(month2,"Dec") )
            day2+=334;;
    s2+=4;

    while (*s2!=':'){
        year2=year2*10+*s2-'0';
        s2++;
    }
    s2++;

    while (*s2!=':'){
        hour2=(hour2*10)+*s2-'0';
        s2++;
    }
    s2++;

    while (*s2!=':'){
        min2=(min2*10)+*s2-'0';
        s2++;
    }
    s2++;

    while (*s2!=' '){
        sec2=(sec2*10)+*s2-'0';
        s2++;
    }

    return (year2-year1)*31536000 + (day2-day1)*86400 + (hour2-hour1)*3600 + (min2-min1)*60 +sec2-sec1 ;
}

char* Gets_time(char* s)
{
    while (*s!='[') s++;
    s++;
    int i=0;
    while ( *(s+i)!=' ') i++;
    *(s+i)='\0';
    return s;
}

int main()
{
    FILE *fin1, *fin2;
    fin1=fopen("access_log_Jul95", "r");
    fin2=fopen("access_log_Jul95", "r");

    int k=0;// k - кол-во запросов с ошибкой

    char *s=(char*) malloc( 1000*sizeof(char) );

    for (fgets(s, 1000, fin1); !feof(fin1); fgets(s, 1000, fin1) ){
        int status=0;
        int i=strlen(s)-1; // i для разбора строки

        while (s[i]!='"') i--;

        i+=2;

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
    fseek(fin1, 0, 0);
    free(s);

    char *s1=(char*) malloc( 1000*sizeof(char) );
    fgets(s1, 1000, fin1);
    char *s2=(char*) malloc( 1000*sizeof(char) );
    fgets(s2, 1000, fin2);

    printf("Enter seconds: ");
    int dif;
    scanf("%d", &dif);

    int max=0;
    char *max1=(char *) malloc( 1000*sizeof(char));
    char *max2=(char *) malloc( 1000*sizeof(char));
    for ( int i=0, j=0; !feof(fin2); ){
        if (  Get_diference(s1, s2) <= dif){
            if (j-(i-1)>max){
                max=j-(i-1);
                strcpy(max1, s1);
                strcpy(max2, s2);
                //printf("%s---%s\n", max1, max2);
            }
            else{
            fgets(s2, 1000, fin2);
            j++;
            }
        }
        else{
            fgets(s1, 1000, fin1);
            i++;
        }
    }
    printf("%s\n%s", Gets_time(max1), Gets_time(max2) );

    free(s1);
    free(s2);
    free(max1);
    free(max2);
    fclose(fin1);
    fclose(fin2);
}
/*Mycop
    ;
*/
