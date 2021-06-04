#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#include "cJSON.h"
#include <unistd.h>
#include <stdlib.h>

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define BLUE "\e[0;35m"
#define RESET "\e[0m"


//***************************************************function***********************************************************
int initialize();
char *send_data(char *);
void register_menu();
void sign_up();
void log_in();
void twitter();
void timeline();
void search();
void tweet_profile();
void personal_area();
void log_out();
void send_tweet();
void refresh();
void set_bio();
void tweet_like();
void tweet_comment();
void change_pass();
void tweet_delete();

//**********************************************************************************************************************

struct sockaddr_in server;
int port=12345;
char token[1000]="aliz";
int sleep_time=2;
char show_comment[100][5000];

int main() {
    int can_initialize=initialize();

    if(can_initialize==0) {
        register_menu();
    }
    //log_out(0);
    return 0;
}


//*********************************************network function*********************************************************

int initialize(){
    WSADATA wsadate;

    int wsaStartUp = WSAStartup(MAKEWORD(2,2), &wsadate);
    if(wsaStartUp!=0){
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    return 0;
}

char *send_data (char *data){

    char *buffer= (char*)malloc(100000);
    memset(buffer,0,100000);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == INVALID_SOCKET){
        return buffer;
    }

    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    if(can_connect !=0 ){
        return buffer;
    }

    send(client_socket, data, 100000, 0);
    recv(client_socket, buffer,100000 ,0);
    closesocket(client_socket);
    return buffer;
}

//****************************************************register**********************************************************

void register_menu() {
    int key;
    system("CLS");
    printf(BLUE"******Register menu******\n\n"RESET);
    printf("1.Sign up\n");
    printf("2.Log in\n");
    printf("enter your choice:");
    scanf("%d", &key);
    if (key == 1)
        sign_up();
    if(key==2)
        log_in();
}

void sign_up() {
    char username[1000], pass[1000];
    system("CLS");
    printf(BLUE"******sign up******\n\n"RESET);
    int v = 0;
    while (v==0) {

        char massage[1000] = "signup \0";
        printf("Username:\n");
        scanf("%s", username);
        printf("Password:\n");
        scanf("%s", pass);
        strcat(massage, username);
        strcat(massage, ", ");
        strcat(massage, pass);
        strcat(massage, "\n");
        char *feedback = send_data(massage);
        if(feedback[0]=='E'){
            printf(RED "\ncommand is wrong!\n" RESET );
        }
        if (feedback[9] == 'S') {
            printf( GREEN "\nYour account has been created\n"
                   "back to register menu...\n" RESET);
            sleep(sleep_time);
            v=1;
        }

        if(feedback[9]=='E' && feedback[15]=='e') {
            system("CLS");
            printf( RED "\nyour username is already taken\n"
                   "enter new username\n\n" RESET);
        }
    }
    register_menu();
}

void log_in(){
    char username[1000], pass[1000];
    system("CLS");
    printf(BLUE"******log in******\n\n"RESET);
    int v = 0;
    while (v==0) {

        char massage[1000] = "login \0";
        printf("Username:\n");
        scanf("%s", username);
        printf("Password:\n");
        scanf("%s", pass);
        strcat(massage, username);
        strcat(massage, ", ");
        strcat(massage, pass);
        strcat(massage, "\n");

        char *feedback = send_data(massage);
        if (feedback[9] == 'E' && feedback[15] == 'e'){
            system("CLS");
            printf(RED "\nthis username is not exist\n"
                   "please enter correct information\n\n"RESET);
        }
        if(feedback[0]=='E') {
            printf(RED"\ncommand is wrong!\n"RESET);
        }
        if(feedback[9]=='E' && feedback[15]=='o'){
            system("CLS");
            printf(RED"\nYou are already logged in\n"
                   "please enter correct information\n\n"RESET);
        }
        if (feedback[9] == 'T') {
            cJSON * root = cJSON_Parse(feedback);
            cJSON * mass = cJSON_GetObjectItem(root , "message");
            char * string=cJSON_Print(mass);
            strcpy(token,string);
            char string2[1000]="";
            for(int i=0;i<strlen(token)-2;i++){
                string2[i]=token[i+1];
            }
            strcpy(token,string2);

            printf(GREEN"\nYou have been logged in successfully\n"
                   "go to main menu..."RESET);
            sleep(sleep_time);
            v=1;
        }

        if(feedback[9]=='E' && feedback[15]=='w') {
            system("CLS");
            printf(RED"\nyour username or password is wrong\n"
                   "please enter correct information\n\n"RESET);
        }
    }
    twitter();
}

//******************************************************main menu*******************************************************

void twitter(){
    int v;
    system("CLS");
    printf(BLUE"******main menu******\n\n"RESET);
    printf("1.Timeline\n");
    printf("2.Search\n");
    printf("3.Tweet profile\n");
    printf("4.Personal area\n");
    printf("5.Log out\n\n");
    printf("enter your choice:");
    scanf("%d",&v);
    if(v==1)
        timeline();
    if(v==2)
        search();
    if(v==3)
        tweet_profile();
    if(v==4)
        personal_area();
    if(v==5){
        log_out();
    }
}

void timeline(){
    int v;
    system("CLS");
    printf("******Time line******\n\n");
    printf("1.send tweet\n");
    printf("2.refresh\n");
    printf("3.back\n\n");
    printf("enter your choice:");
    scanf("%d",&v);
    if(v==1)
        send_tweet();
    if(v==2)
        refresh();
    if(v==3)
        twitter();
}

void search(){
    char user[1000]="\0";
    system("CLS");
    printf("enter username: ");
    scanf("%s",user);
    char massage[1000] = "search \0";
    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, user);
    strcat(massage,"\n");
    char *feedback = send_data(massage);
    if(feedback[9]=='E') {
        printf(RED"\n\ntoken is not exist\n"
               "back to main menu...\n"RESET);
        sleep(sleep_time);
    }
    else {
        const cJSON *alltweets = NULL;
        cJSON *root = cJSON_Parse(feedback);
        cJSON *mas = cJSON_GetObjectItem(root, "message");
        cJSON *username = cJSON_GetObjectItem(mas, "username");
        cJSON *bio = cJSON_GetObjectItem(mas, "bio");
        cJSON *follower = cJSON_GetObjectItem(mas, "numberOfFollowers");
        cJSON *following = cJSON_GetObjectItem(mas, "numberOfFollowings");
        cJSON *follow = cJSON_GetObjectItem(mas, "followStatus");
        printf("username: %s\n", username->valuestring);
        printf("bio: %s\n", bio->valuestring);
        printf("followers: %d  ", follower->valueint);
        printf("following: %d\n", following->valueint);
        char *v = cJSON_Print(follow);
        if (v[1] == 'F')
            printf("unfollow\n");
        else
            printf("follow");

        printf("\n");
        cJSON *tweets = cJSON_GetObjectItem(mas, "allTweets");
        int i;
        cJSON_ArrayForEach(alltweets, tweets) {
            cJSON *content = cJSON_GetObjectItem(alltweets, "content");
            cJSON *comments = cJSON_GetObjectItem(alltweets, "comments");
            cJSON *likes = cJSON_GetObjectItem(alltweets, "likes");
            i = 0;
            const cJSON *allcomments = NULL;
            cJSON_ArrayForEach(allcomments, comments) {
                cJSON *comm = cJSON_GetObjectItem(allcomments, "comments");
                i++;
            }
            printf(BLUE"----------------------------------------------------------------------------\n\n"RESET);
            printf("tweet: %s\n", content->valuestring);
            printf("likes: %d  comments: %d\n", likes->valueint, i);
        }
        printf("\n");
        int key;
        if (v[1] == 'F')
            printf("1.unfollow\n");
        else
            printf("1.follow\n");
        printf("2.back\nenter your choice:");
        scanf("%d", &key);
        if (key == 1) {
            char c;
            if (v[1] == 'F') {
                char massage2[1000] = "unfollow \0";
                strcat(massage2, token);
                strcat(massage2, ", ");
                strcat(massage2, user);
                strcat(massage2, "\n");
                char *res = send_data(massage2);
                if (res[9] == 'E')
                    printf(RED"\nsomting is wrong!\n"
                           "you can not unfollow this user!\n"RESET);
                else
                    printf(GREEN"\nyou unfollow this user\npress \"enter\" to return to menu"RESET);
                scanf("%*c%c", &c);
            } else {
                char massage2[1000] = "follow \0";
                strcat(massage2, token);
                strcat(massage2, ", ");
                strcat(massage2, user);
                strcat(massage2, "\n");
                char *res = send_data(massage2);
                if (res[9] == 'E')
                    printf(RED"\nsomting is wrong!\n"
                           "you can not follow this user!\n"RESET);
                else
                    printf(GREEN"\nyou follow this user\npress \"enter\" to return to menu"RESET);
                scanf("%*c%c", &c);
            }
        }
    }
    twitter();

}

void tweet_profile(){
    int v=0,key,i;
    system("CLS");
    char massage[1000] = "profile \0";
    strcat(massage, token);
    strcat(massage,"\n");
    char *feedback = send_data(massage);
    printf("1");
    if(feedback[0]=='E'){
        v=1;
        printf(RED"\ncommand is wrong!\n"RESET);
    }
    if(feedback[9]=='E'){
        v=1;
        printf(RED"\ntoken is not exist\n"RESET);
    }
    else if(v!=1){
        const cJSON *alltweets = NULL;
        cJSON *root = cJSON_Parse(feedback);
        cJSON *mas = cJSON_GetObjectItem(root, "message");
        cJSON *username = cJSON_GetObjectItem(mas, "username");
        cJSON *bio = cJSON_GetObjectItem(mas, "bio");
        cJSON *follower = cJSON_GetObjectItem(mas, "numberOfFollowers");
        cJSON *following = cJSON_GetObjectItem(mas, "numberOfFollowings");

        printf("username: %s\n", username->valuestring);
        printf("bio: %s\n", bio->valuestring);
        printf("followers: %d  ", follower->valueint);
        printf("following: %d\n", following->valueint);
        printf("\n");
        cJSON *tweets = cJSON_GetObjectItem(mas, "allTweets");


        cJSON_ArrayForEach(alltweets, tweets) {
            cJSON *id = cJSON_GetObjectItem(alltweets, "id");
            cJSON *content = cJSON_GetObjectItem(alltweets, "content");
            cJSON *comments = cJSON_GetObjectItem(alltweets, "comments");
            cJSON *likes = cJSON_GetObjectItem(alltweets, "likes");
            i = 0;
            const cJSON *allcomments = NULL;
            cJSON_ArrayForEach(allcomments, comments) {
                cJSON *comm = cJSON_GetObjectItem(allcomments, "comments");
                i++;
            }

            printf(BLUE"------------------------------------------------------------------------------------------\n\n"RESET);
            printf("tweet id: %d\n",id->valueint);
            printf("tweet: %s\n\n", content->valuestring);
            printf("likes: %d  comments: %d\n", likes->valueint, i);
        }
        printf("\n");
        printf("1.delete tweet\n");
        printf("2.back\n");
        printf("\nenter your choice:");
        scanf("%d",&key);
        if(key==2)
            twitter();
        if(key==1){
            tweet_delete();
        }
    }
}

void personal_area(){
    int v;
    system("CLS");
    printf(BLUE"******Personal area******\n\n"RESET);
    printf("1.set bio\n");
    printf("2.change password\n");
    printf("3.back\n");
    printf("enter your choice:");
    scanf("%d",&v);
    if(v==1)
        set_bio();
    if(v==2)
        change_pass();
    if(v==3)
        twitter();
}

void log_out(){
    char massage[1000] = "logout \0";

    strcat(massage, token);
    strcat(massage, "\n");
    char *res=send_data(massage);
    if(res[0]=='E'){
        printf(RED"\ncommand is wrong!\n"RESET);
    }
    if(res[9]=='E'){

        printf(RED"\ntoken is not exist\n"RESET);
    }
    if(res[9]=='S'){
        printf(GREEN"\nYou have been successfully logged out!\n"
               "back to register menu...\n"RESET);
        sleep(sleep_time);
        register_menu();
    }
}

//**********************************************************************************************************************

void send_tweet(){
    char tweet_l[1000];
    system("CLS");
    printf(BLUE"******send tweet******\n\n"RESET);
    printf("enter your tweet:\n");
    scanf("%*c%[^\n]s",tweet_l);
    int v=0;
    char massage[1000] = "send tweet \0";

    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, tweet_l);
    strcat(massage,"\n");

    char* res=send_data(massage);
    if(res[0]=='E'){
        v=1;
        printf(RED"\ncommand is wrong!\n"RESET);
    }
    if(res[9]=='E') {
        v=1;
        printf(RED"\ntoken is not exist\n"RESET);
    }else if(v!=1) {
        printf(GREEN"\nTweet sent\n"
               "back to timeline...\n"RESET);
    }
    sleep(sleep_time);
    timeline();
}

void set_bio(){
    system("CLS");
    int v=0;
    char bios[1000]="";
    printf(BLUE"******set bio******\n\n"RESET);
    printf("enter bio:\n");
    scanf("%*c%[^\n]s",bios);
    char massage[1000] = "set bio \0";
    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, bios);
    strcat(massage,"\n");
    char *feedback = send_data(massage);
    if(feedback[0]=='E'){
        v=1;
        printf(RED"\ncommand is wrong!\n"RESET);
    }
    if(feedback[9]=='E') {
        v=1;
        printf(GREEN"\nYour bio is already set\n"
               "You can't change it\n"RESET);
    }else if(v!=1) {
        printf(GREEN"\nYour bio was set successfully\n"RESET);
    }
    sleep(sleep_time);
    personal_area();
}

void change_pass(){
    char current_pass[1000],new_pass[1000];
    system("CLS");
    printf(BLUE"******change password******\n\n"RESET);
    printf("current passwor:\n");
    scanf("%s",current_pass);
    printf("new password:\n");
    scanf("%s",new_pass);
    char massage[1000] = "change password \0";
    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, current_pass);
    strcat(massage, ", ");
    strcat(massage, new_pass);
    strcat(massage,"\n");
    char *feedback = send_data(massage);
    if(feedback[0]=='E')
        printf(RED"\ncommand is wrong!\n"RESET);

    if(feedback[9]=='E' && feedback[15]=='p')
        printf(RED"\nthat password is incorrect\n"
               "Your password has not been changed\n"RESET);

    if(feedback[9]=='E' && feedback[15]=='t')
        printf(RED"\ntoken is not exist!\n"RESET);
    if(feedback[9]=='E' && feedback[15]=='s')
        printf(RED"\nNew password length longer than 16 characters!\n"RESET);
    if(feedback[9]=='S')
        printf(GREEN"\nYour password was changed successfully\n"RESET);
    sleep(sleep_time);

    personal_area();
}

void refresh(){
    printf(BLUE"********timeline********\n\n"RESET);
    char massage[1000] = "refresh \0";
    strcat(massage, token);
    strcat(massage,"\n");
    char *feedback = send_data(massage);
    if(feedback[9]=='E' && feedback[15]=='t')
        printf(RED"\ntoken is not exist!\n"RESET);
    if(feedback[0]=='E')
        printf(RED"\ncommand is wrong!\n"RESET);
    else{
        const cJSON *alltweets = NULL;
        cJSON *root = cJSON_Parse(feedback);
        cJSON *mess = cJSON_GetObjectItem(root, "message");

        int t, an;
        cJSON_ArrayForEach(alltweets, mess) {
            cJSON *id = cJSON_GetObjectItem(alltweets, "id");
            cJSON *author = cJSON_GetObjectItem(alltweets, "author");
            cJSON *content = cJSON_GetObjectItem(alltweets, "content");
            cJSON *comments = cJSON_GetObjectItem(alltweets, "comments");
            cJSON *likes = cJSON_GetObjectItem(alltweets, "likes");
            //char * co= (char*)malloc(1048576*32);
            t = 0;
            char *co = cJSON_Print(comments);
            for (int j = 0; j <= strlen(co); j++) {
                if (co[j] == '\"')
                    t++;
            }
            an = (int) id->valueint;
            strcpy(show_comment[an], co);
            printf(BLUE"------------------------------------------------------------------------------------------\n\n"RESET);
            printf("user: %s    Tweet ID: %d\n", author->valuestring, id->valueint);
            printf("%s\n\n", content->valuestring);
            printf("likes: %d  comments: %d\n\n", likes->valueint, t / 4);
        }
        int key;
        printf(BLUE"\n****** menu ******\n\n"RESET);
        printf("1.like\n");
        printf("2.leave a comment\n");
        printf("3.show comments\n");
        printf("4.back\n\n");
        printf("enter your choice:");
        scanf("%d", &key);

        if (key == 1)
            tweet_like();
        if (key == 2)
            tweet_comment();
        if (key == 3) {
            int tweet_id;
            printf(BLUE"******show comments******\n"RESET);
            printf("\nenter tweet ID:");
            scanf("%d", &tweet_id);
            printf("\ncomments for this tweet:\n");
            for (int x = 1; x < strlen(show_comment[tweet_id]) - 1; x++) {
                printf("%c", show_comment[tweet_id][x]);
            }
            printf("\n");
            refresh();
        }
        if (key == 4)
            timeline();
    }
}

void tweet_like(){
    char id[100];
    printf(BLUE"\n******like tweet******\n\n"RESET);
    printf("\nenter tweet ID:");
    scanf("%s",id);
    char massage[1000] = "like \0";
    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, id);
    strcat(massage,"\n");
    //printf("%s",massage);
    char* res=send_data(massage);
    if(res[0]=='E'){
        printf(RED"command is wrong!\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='t'){
        printf(RED"\ntoken is not exist\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='e'){
        printf(RED"\nThere are no tweets with this ID!\n"RESET);
    }
    if(res[9]=='E' && res[15]=='r'){
        printf(RED"\nYou have already liked this tweet!\n\n"RESET);
    }
    if(res[9]=='S') {
        printf(GREEN"\ntweet liked!\n\n"RESET);
    }
    sleep(sleep_time);

    refresh();
}

void tweet_comment(){
    char id[100];
    printf(BLUE"******comment tweet******\n\n"RESET);
    printf("\nenter tweet ID:");
    scanf("%s",id);
    char massage[1000] = "comment \0";
    char comment_txt[1000];
    printf("enter comment:\n");
    scanf("%*c%[^\n]s",comment_txt);
    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, id);
    strcat(massage, ", ");
    strcat(massage, comment_txt);
    strcat(massage,"\n");
    char*res=send_data(massage);
    if(res[0]=='E'){
        printf(RED"\ncommand is wrong!\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='t'){
        printf(RED"\ntoken is not exist!\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='e'){
        printf(RED"\nThere are no tweets with this ID!\n\n"RESET);
    }
    if(res[9]=='S') {
        printf(GREEN"\ncomment sent!\n\n"RESET);
    }
    sleep(sleep_time);
    refresh();
}

void tweet_delete(){
    char id[10];
    printf(BLUE"\n******delete tweet******\n\n"RESET);
    printf("\nenter tweet ID:");
    scanf("%s",id);
    char massage[1000] = "delete \0";
    strcat(massage, token);
    strcat(massage, ", ");
    strcat(massage, id);
    strcat(massage,"\n");
    char* res=send_data(massage);
    if(res[0]=='E'){
        printf(RED"\ncommand is wrong!\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='t'){
        printf(RED"\ntoken is not exist!\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='e'){
        printf(RED"\nThere are no tweets with this ID!\n\n"RESET);
    }
    if(res[9]=='E' && res[15]=='u'){
        printf(RED"\nThis tweet is not for you\n"
               "you can not delet this tweet!\n"RESET);
    }
    if(res[9]=='S') {
        printf(GREEN"\ntweet deleted!\n"
               "update profile...\n"RESET);
    }
    sleep(sleep_time);
    tweet_profile();
}
