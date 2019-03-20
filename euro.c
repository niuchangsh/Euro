#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Games{
    char group[15];
    char team_1[15];
    char team_2[15];
    int goals_1;
    int goals_2;
} Game[200];


struct Teams{
    char group_name[15];
    char team_name[15];
    int goals_f;
    int goals_a;
    int goals_d;
    int win;
    int lose;
    int draw;
    int point;
} Team[32];


int main(int argc, char *argv[]){
    int n;
    int i = 0;
    int j = 0;
    char str[15];
    int len = 0;/*the valid length of Game[], that is to say when i<len, Game[i] ！= NULL*/
    for(n=1;n<argc;n++){
        FILE *fp = fopen(argv[n], "r");
        if(fp != NULL){
            while(fscanf(fp, "%s", str) != EOF){ /*读入txt文件并存入game中*/
                switch (j % 5){
                    case 0: strcpy(Game[i].group,str); break;
                    case 1: strcpy(Game[i].team_1,str); break;
                    case 2: Game[i].goals_1 = atoi(str); break;
                    case 3: Game[i].goals_2 = atoi(str); break;
                    case 4: strcpy(Game[i].team_2,str); i++;break;
                    default: printf("input error\n");
                }
                j++;
            }
        }
        len = i;
        fclose(fp);
    }
    /*When a team is at home*/
    /*Initialization of Team[0]*/
    strcpy(Team[0].group_name,Game[0].group);
    strcpy(Team[0].team_name,Game[0].team_1);
    Team[0].goals_f = 0;
    Team[0].goals_a = 0;
    Team[0].win = 0;
    Team[0].lose = 0;
    Team[0].draw = 0;


    int k;
    int judge;
    j = 0;
    for(i=0; i<len; i++){
        judge = 1;
        for(k=0; k<=j; k++){
            if(strcmp(Game[i].team_1,Team[k].team_name) == 0){
                judge = 0;
                Team[k].goals_f = Team[k].goals_f + Game[i].goals_1;
                Team[k].goals_a = Team[k].goals_a + Game[i].goals_2;
                if(Game[i].goals_1 > Game[i].goals_2){
                    Team[k].win ++;
                }
                else if(Game[i].goals_1 < Game[i].goals_2){
                    Team[k].lose ++;
                }
                else{
                    Team[k].draw ++;
                }
                break;
            }
        }
        if(judge == 1){
            j ++;
            strcpy(Team[j].group_name , Game[i].group);
            strcpy(Team[j].team_name , Game[i].team_1);
            Team[j].goals_f = Game[i].goals_1;
            Team[j].goals_a = Game[i].goals_2;
            Team[j].win = 0;
            Team[j].lose = 0;
            Team[j].draw = 0;
            if(Game[i].goals_1 > Game[i].goals_2){
                Team[j].win ++;
            }
            else if(Game[i].goals_1 < Game[i].goals_2){
                Team[j].lose ++;
            }
            else{
                Team[j].draw ++;
            }
        }
    }


    /*When a team is away*/
    for(i=0; i<len; i++){
        for(k=0; k<32; k++){
            if(strcmp(Game[i].team_2,Team[k].team_name) == 0){
                Team[k].goals_f = Team[k].goals_f + Game[i].goals_2;
                Team[k].goals_a = Team[k].goals_a + Game[i].goals_1;
                if(Game[i].goals_1 < Game[i].goals_2){
                    Team[k].win ++;
                }
                else if(Game[i].goals_1 > Game[i].goals_2){
                    Team[k].lose ++;
                }
                else{
                    Team[k].draw ++;
                }
                break;
            }
        }
    }


    /*calculate Point and GoalD*/
    for(k=0; k<32; k++){
        Team[k].goals_d = Team[k].goals_f - Team[k].goals_a;
        Team[k].point = Team[k].win * 3 + Team[k].draw;
    }

    /*Sort the teams*/
    /*Creat a function to convert two teams' indexes*/
    int fun_convert(int num_a, int num_b){
        struct Teams tmp;
        tmp = Team[num_b];
        Team[num_b] = Team[num_a];
        Team[num_a] = tmp;
    }

    /*Bubble Sort*/
    int num_a = 0;
    int num_b;
    for(i=0; i<32; i++){
        num_b = 32;
        for(k=0; k<32; k++){
            num_b --;
            if(num_a < num_b){
                if(Team[num_a].point < Team[num_b].point){
                    fun_convert(num_a,num_b);
                }
                else if(Team[num_a].point == Team[num_b].point){
                    if(Team[num_a].goals_d < Team[num_b].goals_d){
                        fun_convert(num_a,num_b);
                    }
                    else if(Team[num_a].goals_d == Team[num_b].goals_d){
                        if(Team[num_a].goals_f < Team[num_b].goals_f){
                            fun_convert(num_a,num_b);
                        }
                        else if(Team[num_a].goals_f == Team[num_b].goals_f){
                            if(strcmp(Team[num_a].team_name,Team[num_b].team_name) > 0){
                                fun_convert(num_a,num_b);
                            }
                        }
                    }
                }
            }
        }
        num_a ++;
    }

    /*Output*/
    /*char group[8]={'A','B','C','D','E','F','G','H'};*/
    char group[10]="ABCDEFGH";
    for(i=0;i<8;i++){
        printf("Group%c\n",group[i]);
        printf("Team\tW\tD\tL\tPoint\tGoalF\tGoalA\tGoalD\n");
        for(k=0; k<32; k++){
            if(Team[k].group_name[5] == group[i]){
                printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",Team[k].team_name,Team[k].win,Team[k].draw,Team[k].lose,Team[k].point,Team[k].goals_f,Team[k].goals_a,Team[k].goals_d );
            }
        }
    }
    return 0;
}