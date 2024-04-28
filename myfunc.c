#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STR_SIZE 20000

typedef struct card {
    int32_t id;
    char * name;
    char * attribute;
    char * race;
    int8_t star;
    char * monster_tag;
    char * cross_over;
    char * vrpair;
    char * skill;
    char * team_skill;
    int8_t max_level;
    int8_t max_skill;
    int8_t max_refine;
    char * version;
    char * board;
    char * spcial_image;
} sCard ;

typedef struct skill {
    char* name;
    char* type;
    char* charge;
    int32_t num;
    char* description;
    char* tag;
} sSkill;

int32_t create_card(sCard *card);
int32_t free_card(sCard *card);
int32_t parse_str(char* str,sCard* card);
int32_t find_pairwise(char* str,char left, char right);
int32_t clean_str(char * str);
int32_t put_data(sCard *card, char * tmpstr, int8_t col_counter);
int32_t write_file(FILE* file, sCard *card);
int32_t write_skill(FILE* file,sCard* card);
int32_t parse_skill(sSkill* skill,char* skill_str);
int32_t create_skill(sSkill* skill);

int main(){
    char * str = calloc(MAX_STR_SIZE,sizeof(char));
    
    if( str==NULL ){
        printf("calloc failed \n");
        return 0;
    }

    FILE *file = fopen("dataset.csv", "r");
    FILE *new_file = fopen("dataset_new.csv","w");
    FILE *skill_file = fopen("./skill/new_skill.csv","w");

    if (file == NULL) {
        printf("無法打開檔案\n");
        return 1;
    }
    if(new_file == NULL){
        printf("無法打開檔案\n");
        return 1;
    }
    if(skill_file == NULL){
        printf("無法打開檔案 skill\n");
        return 1;
    }

    size_t line_number = 0;

    fprintf(new_file, "id,name,attribute,race,star,monster_tag,cross_over,vrpair,skill,teamskill,maxlevel,maxrefine,version,board,specialimage\n");
    fprintf(skill_file, "name,type,charge,num,description,tag\n");

    while( fgets(str, MAX_STR_SIZE, file)) {
        // if(line_number >= 2778-1 && line_number <= 2778-1 ){
            printf("%s \n", str);
            printf("strlen = %ld \n",strlen(str));
            sCard card;
            if( create_card(&card) == -1 ){
                printf("func create_card failed \n");
                return -1;
            }
            parse_str(str,&card);

            write_file(new_file,&card);

            write_skill(skill_file,&card);

            free_card(&card);
        // }
        line_number ++;
    }

    // strncpy(str,"[{""name"":""穿透人心的惑力"",""type"":""normal"",""charge"":""CD"",""num"":8,""description"":""     I. 敵方互相或自我進行 1 次攻擊，持續 1 回合     1 回合內     II. 全隊無視「攻前盾」     "",""tag"":[""魅惑敵方"",""無視攻前盾"",""敵身狀態""]},{""name"":""泯滅原則的絕色"",""type"":""normal"",""charge"":""CD"",""num"":8,""description"":""     1 回合內     I. 符石首批 1 粒     ⇒ 即可發動消除     II. 全隊無視「反首消符石盾」     "",""tag"":[""一消"",""無視反首消盾""]}]",sizeof(str)/sizeof(char));

    printf("length = %ld \n",strlen(str));

    // clean_str(str);
    
    fclose(file);
    fclose(new_file);

    return 0;
}


int32_t write_skill(FILE* file,sCard* card){
    char* output_str = calloc(10020,sizeof(char));
    char* skill_str = calloc(5000,sizeof(char));
    int32_t left=0,right=0;
    while(card->skill[left] != '\0' && card->skill[right] != '\0'){
        if(card->skill[right]=='{'){
            right += find_pairwise(card->skill+right,'{','}');
        }
        if(card->skill[right] == ',' || card->skill[right] == ']'){
            strncpy(skill_str,card->skill+left,right-left);
            skill_str[right-left] = '\0';

            sSkill skill;
            if(create_skill(&skill)!=0){
                printf("記憶體分配錯誤 \n");
                return -1;
            }

            parse_skill(&skill,skill_str);

            char* tmp = calloc(5,sizeof(char));
            strncat(output_str,skill.name,100);
            strncat(output_str,",",5);
            strncat(output_str,skill.type,100);
            strncat(output_str,",",5);
            strncat(output_str,skill.charge,100);
            strncat(output_str,",",5);
            sprintf(tmp,"%d",skill.num);
            strncat(output_str,tmp,10);
            strncat(output_str,",",5);
            strncat(output_str,skill.description,5000);
            strncat(output_str,",",5);
            strncat(output_str,skill.tag,5000);
            strncat(output_str,"\n",5);
            left = right+1;
        }
        
        right ++;
    }
    fprintf(file,"%s",output_str);
    return 0;
}

int32_t parse_skill(sSkill* skill,char* skill_str){
    printf("----- skill start -----\n");
    printf("skill= %s\n",skill_str);
    int32_t counter = 0;
    int32_t left=0,right=0;
    while(skill_str[counter]!='\0'){
        if( strncmp(skill_str+counter,"name\":",strlen("name\":"))==0 ){
            counter += strlen("name\":")+1;
            left = counter;
            right = counter;
            for(;skill_str[right]!=',';right++)

            strncpy(skill->name,skill_str+left,right-left);
            printf("skill->name= %s \n",skill->name);
        }else if(strncmp(skill_str+counter,"type\":",strlen("type\":"))==0){
            counter += strlen("type\":")+1;
            left = counter;
            right = counter;
            for(;skill_str[right]!=',';right++)

            strncpy(skill->type,skill_str+left,right-left);
            printf("skill->type= %s \n",skill->type);
        }else if(strncmp(skill_str+counter,"charge\":",strlen("charge\":"))==0){
            counter += strlen("charge\":")+1;
            left = counter;
            right = counter;
            for(;skill_str[right]!=',';right++)

            strncpy(skill->charge,skill_str+left,right-left);
            printf("skill->charge= %s \n",skill->charge);
        }else if(strncmp(skill_str+counter,"num\":",strlen("num\":"))==0){
            counter += strlen("num\":");
            left = counter;
            right = counter;
            for(;skill_str[right]!=',';right++){}

            char* tmp = calloc(10,sizeof(char));
            strncpy(tmp,skill_str+left,right-left);
            skill->num = atoi(tmp);

            printf("skill->num= %d \n",skill->num);
        }else if(strncmp(skill_str+counter,"description\":",strlen("description\":"))==0){
            counter += strlen("description\":")+1;
            left = counter;
            right = counter;
            for(;skill_str[right]!=',';right++)

            strncpy(skill->description,skill_str+left,right-left);
            printf("skill->desgription= %s \n",skill->description);
        }else if(strncmp(skill_str+counter,"tag\":",strlen("tag\":"))==0){
            counter += strlen("tag\":");
            left = counter;
            right = counter;
            // printf("| %s |\n",skill_str+right);
            right += find_pairwise(skill_str+right,'[',']');

            strncpy(skill->tag,skill_str+left,(right+1)-left);
            printf("skill->tag= %s \n",skill->tag);
        }else {
            counter ++;
        }
    }

    printf("------ skill end ------\n");
    return 0;
}

int32_t create_skill(sSkill* skill){
    skill->name = calloc(50,sizeof(char));
    skill->type = calloc(50,sizeof(char));
    skill->charge = calloc(50,sizeof(char));
    skill->description = calloc(4096,sizeof(char));
    skill->tag = calloc(512,sizeof(char));
    if(skill->name==NULL || skill->type==NULL || skill->charge==NULL || skill->description==NULL || skill->tag==NULL){
        printf("記憶體分配錯誤 \n");
        return -1;
    }
    return 0;
}

int32_t create_card(sCard *card) {
    // 為每個 char* 欄位分配記憶體
    card->name = (char*)calloc(100, sizeof(char));
    card->attribute = (char*)calloc(50, sizeof(char));
    card->race = (char*)calloc(50, sizeof(char));
    card->monster_tag = (char*)calloc(100, sizeof(char));
    card->cross_over = (char*)calloc(100, sizeof(char));
    card->vrpair = (char*)calloc(100, sizeof(char));
    card->skill = (char*)calloc(20000, sizeof(char));
    card->team_skill = (char*)calloc(20000, sizeof(char));
    card->version = (char*)calloc(100, sizeof(char));
    card->board = (char*)calloc(100, sizeof(char));
    card->spcial_image = (char*)calloc(100, sizeof(char));

    // 檢查記憶體是否成功分配
    if (card->name == NULL || card->attribute == NULL || card->race == NULL ||
        card->monster_tag == NULL || card->cross_over == NULL || card->vrpair == NULL ||
        card->skill == NULL || card->team_skill == NULL || card->version == NULL ||
        card->board == NULL || card->spcial_image == NULL) {
        printf("記憶體分配失敗\n");
        return -1;
    }

    return 0;
}

int32_t write_file(FILE* file, sCard *card) {
    // 檢查檔案和卡片是否為 NULL
    if (file == NULL || card == NULL) {
        printf("無效的參數\n");
        return -1;
    }

    // 將卡片的內容寫入到檔案中
    fprintf(file, "%d,%s,%s,%s,%d,%s,%s,%s,%s,%s,%d,%d,%d,%s,%s,%s\n",
            card->id, card->name, card->attribute, card->race, card->star,
            card->monster_tag, card->cross_over, card->vrpair, card->skill,
            card->team_skill, card->max_level, card->max_skill, card->max_refine,
            card->version, card->board, card->spcial_image);

    return 0;
}

int32_t free_card(sCard *card) {
    // 釋放每個 char* 欄位的記憶體
    free(card->name);
    free(card->attribute);
    free(card->race);
    free(card->monster_tag);
    free(card->cross_over);
    free(card->vrpair);
    free(card->skill);
    free(card->team_skill);
    free(card->version);
    free(card->board);
    free(card->spcial_image);
    return 0;
}

int32_t parse_str(char* str, sCard* card){
    
    printf("----- parse start ----- \n");

    char * tmpstr = calloc(20000,sizeof(char));

    int32_t left = 0;
    int32_t right = 0;
    int8_t col_counter = 0;
    while( str[left] != '\0' && str[right] != '\0'){
        
        if( str[right]== '[' ){
            right += find_pairwise(str+right,'[',']');
            printf("right change to %d [ \n",right);
        }

        if( str[right]== '{' ){
            right = left + find_pairwise(str+right,'{','}');
            printf("right change to %d { \n",right);
        }
        

        if( str[right] == ','){
            strncpy(tmpstr,str+left,right-left);
            tmpstr[right-left] = '\0';
            printf("len = %d left = %d right = %d \n",right-left,left,right);
            printf("parse = %s \n\n",tmpstr);
            left = right+1;

            if(put_data(card,tmpstr,col_counter)==-1){
                printf("put data failed \n");
            }else {
                col_counter ++;
            }
        }

        right ++;
    }

    printf("flag1 \n");
    free(tmpstr);
    printf("flag2 \n");
    // free_card(&card);

    printf("------ parse end ------ \n");

    return 0;
}

int32_t put_data(sCard *card, char * tmpstr, int8_t col_counter){
    if(col_counter == 0){
        card->id = atoi(tmpstr);
    }else if(col_counter == 1){
        clean_str(tmpstr);
        strncpy(card->name,tmpstr,100);
    }else if(col_counter == 2){
        clean_str(tmpstr);
        strncpy(card->attribute,tmpstr,50);
    }else if(col_counter == 3){
        clean_str(tmpstr);
        strncpy(card->race,tmpstr,50);
    }else if(col_counter == 4){
        card->star = atoi(tmpstr);
    }else if(col_counter == 5){
        clean_str(tmpstr);
        strncpy(card->monster_tag,tmpstr,50);
    }else if(col_counter == 6){
        clean_str(tmpstr);
        strncpy(card->cross_over,tmpstr,100);
    }else if(col_counter == 7){
        clean_str(tmpstr);
        strncpy(card->vrpair,tmpstr,100);
    }else if(col_counter == 8){
        clean_str(tmpstr);
        strncpy(card->skill,tmpstr,20000);
    }else if(col_counter == 9){
        clean_str(tmpstr);
        strncpy(card->team_skill,tmpstr,20000);
    }else if(col_counter == 10){
        card->max_level = atoi(tmpstr);
    }else if( col_counter == 11){
        card->max_skill = atoi(tmpstr);
    }else if( col_counter == 12){
        card->max_refine = atoi(tmpstr);
    }else if( col_counter == 13){ 
        clean_str(tmpstr);
        strncpy(card->version,tmpstr,100);
    }else if( col_counter == 14){
        clean_str(tmpstr);
        strncpy(card->board,tmpstr,100);
    }else if( col_counter == 15){
        clean_str(tmpstr);
        strncpy(card->spcial_image,tmpstr,100);
    }else {
        printf("col_counter exceed %d\n",col_counter);
        return -1;
    }

    return 0;
}

int32_t find_pairwise(char* str,char left, char right){
    int64_t counter = 1;

    if(str[counter-1]!=left){
        printf("sth not the same \n");
        return -1;
    }   

    int32_t flag = 1; 

    while( str[counter] != '\0' ){
        // the same 
        if( str[counter] == left ){
            flag++;
        }
        if( str[counter] == right ){
            flag--;
        }
        // end 
        if(flag==0){
            return counter;
        }
        counter++;
    }

    return -1;
}

int32_t clean_str(char* str){
    int32_t counter = 0;
    int32_t shift = 0;
    while( str[counter+shift]  != '\0' ){
        if(str[counter+shift] == '"'){
            shift++;
        }
        str[counter] = str[counter+shift];
        counter++;
    }
    printf("%d \n",shift);
    return 0;
}

/*
"[
  {
   ""name"":""穿透人心的惑力"",
   ""type"":""normal"",
   ""charge"":""CD"",
   ""num"":8,
   ""description"":""     I. 敵方互相或自我進行 1 次攻擊，持續 1 回合     1 回合內     II. 全隊無視「攻前盾」     "",
   ""tag"":[""魅惑敵方"",""無視攻前盾"",""敵身狀態""]
  },
  {
   ""name"":""泯滅原則的絕色"",
   ""type"":""normal"",
   ""charge"":""CD"",
   ""num"":8,
   ""description"":""     1 回合內     I. 符石首批 1 粒     ⇒ 即可發動消除     II. 全隊無視「反首消符石盾」     "",
   ""tag"":[""一消"",""無視反首消盾""]
  }
]"
*/
