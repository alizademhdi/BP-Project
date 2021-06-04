#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cJSON.h"
#include <windows.h>

#define SIZE 100000
#define PORT 12345
#define BIGDATA 1000000

char filesourse_user_id[] = "Resources\\Users\\ID\\";
char filesourse_user_tweetid[] = "Resources\\Users\\TweetID\\";
char filesourse_user_bio[] = "Resources\\Users\\Bio\\";
char filesourse_user_following[] = "Resources\\Users\\Following\\";
char filesourse_user_followers[] = "Resources\\Users\\Followers\\";
char filesourse_user_viewed[] = "Resources\\Users\\Viewed\\";
char filesourse_user_online[]="Resources\\Online\\";

char filesourse_tweet_content[] = "Resources\\Tweets\\Content\\";
char filesourse_tweet_like[] = "Resources\\Tweets\\Likes\\";
char filesourse_tweet_comment[] = "Resources\\Tweets\\Comments\\";
char filesourse_token[]="Resources\\Token\\";




int initialize(int);
int fileexists(char*);
void start(int);
char* send_data(char*);
char* signup(char *);
char* login(char*);
char* send_tweet(char*);
char* search_token(char*);
char* like_tweet(char*);
char* comment_tweet(char*);
char* change_password(char*);
char* set_bio(char*);
char* follow(char*);
char* unfollow(char*);
char* randtoken();
char* tweet_profile(char*);
char* search(char*);
char* logout(char*);
char* delete(char*);
char* refresh(char*);


int main(){

    mkdir(".\\Resources");
    mkdir(".\\Resources\\Users");mkdir(".\\Resources\\Tweets");
    mkdir(".\\Resources\\Users\\ID");mkdir(".\\Resources\\Users\\TweetID");mkdir(".\\Resources\\Users\\Bio");
    mkdir(".\\Resources\\Users\\Following");mkdir(".\\Resources\\Users\\Followers");
    mkdir(".\\Resources\\Tweets\\Content");mkdir(".\\Resources\\Tweets\\Comments");mkdir(".\\Resources\\Tweets\\Likes");
    mkdir(".\\Resources\\Token");mkdir(".\\Resources\\Online");
    mkdir(".\\Resources\\Users\\Viewed");

    int server_socket = initialize(PORT);
    if (server_socket != INVALID_SOCKET){
        start(server_socket);
    }

    return 0;
}

int initialize(int port){
    WSADATA wsadata;
    int wsa_res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (wsa_res != 0){
        printf("Failed to run wsadata");
        return INVALID_SOCKET;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Could not create socket");
        return INVALID_SOCKET;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_res == SOCKET_ERROR){
        printf("Bind Faild");
        return INVALID_SOCKET;
    }

    listen(server_socket, SOMAXCONN);

    return server_socket;

}

void start(int server_socket) {
    int len = sizeof(struct sockaddr_in);

    int client_socket;
    struct sockaddr_in client_addr;


    while((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len))
          != INVALID_SOCKET) {
        printf("\nClient connected\n");
        char *buffer;
        buffer = (char *)malloc(SIZE * sizeof(char));
        memset(buffer, 0, SIZE);
        recv(client_socket, buffer, SIZE-1, 0);
        char *response;
        response = (char *)malloc(SIZE * sizeof(char));
        char *data=send_data(buffer);
        strcpy(response, data);
        send(client_socket, response, 99999, 0);
        printf("server message: %s\n",data);
        closesocket(client_socket);
        free(buffer);
        free(response);

    }

}
//****************************************************main****************************************************
char* send_data(char *recv){
    int i=0;
    int v=0;
    char s[500]="\0";
    while(*(recv+i)!='\0'){
        s[i]=*(recv+i);
        i++;
    }
    char type[5][200];
    sscanf(s,"%s",type[0]);
    printf("request: %s\n",type[0]);
    char* send=(char*)malloc(SIZE*sizeof(char));

    char *ptr = strtok(s, " ");
    i=0;
    while(ptr != NULL)
    {
        strcpy(type[i],ptr);
        ptr= strtok(NULL, " ");
        i++;
    }


    if(!strcmp(type[0],"signup")) {//sign up
        if(i==3) {
            send = signup(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"login")){//login
        if(i==3) {
            send = login(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"send") && !strcmp(type[1],"tweet")){//send tweet
        if(i>=4) {
            send = send_tweet(recv);
            v = 1;
        }
    }

    if(!strcmp(type[0],"like")){//like
        if(i==3) {
            send = like_tweet(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"comment")){//comment
        if(i>=4) {
            send = comment_tweet(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"change") && !strcmp(type[1],"password")){//comment
        if(i==5) {
            send = change_password(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"set") && !strcmp(type[1],"bio")){//comment
        if(i>=4) {
            send = set_bio(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"follow")){//comment
        if(i==3) {
            send = follow(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"unfollow")){//comment
        if(i==3) {
            send = unfollow(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"profile")){//profile
        if(i==2) {
            send = tweet_profile(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"search")){//profile
        if(i==3) {
            send = search(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"logout")) {//login
        if (i == 2){
            send = logout(recv);
        v = 1;
        }
    }
    if(!strcmp(type[0],"delete")){//delete
        if(i==3) {
            send = delete(recv);
            v = 1;
        }
    }
    if(!strcmp(type[0],"refresh")){//delete
        if(i==2) {
            send = refresh(recv);
            v = 1;
        }
    }
    if(v==0){
        send="Error command";
    }
    return send;
}
//******************************************************************************************************************
int fileexists(char * filename){
    FILE *file;
    if (file=fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}
char* search_token(char* string){
    char tokenaddres[200]="\0",id1[100]="\0",*id;
    strcpy(tokenaddres,filesourse_token);
    strcat(tokenaddres,string);
    strcat(tokenaddres,".txt");
    if(fileexists(tokenaddres)){
        FILE *fptr;

        fptr=fopen(tokenaddres,"r");
        fscanf(fptr,"%s",id1);
        fclose(fptr);
        id=id1;
        return id;
    }
    return "Error";

}

char *randtoken() {
    static int mySeed = 25011984;
    int length=32;
    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t stringLen = strlen(string);
    char *randomString = NULL;
    srand(time(NULL) * length + ++mySeed);
    if (length < 1) {
        length = 1;
    }
    randomString = malloc(sizeof(char) * (length + 1));
    if (randomString) {
        short key;
        for (int n = 0; n < length; n++) {
            key = rand() % stringLen;
            randomString[n] = string[key];
        }
        randomString[length] = '\0';
        return randomString;
    } else {
        printf("No memory");
        exit(1);
    }
}
//******************************************************************************************************************
char* signup(char* string){
    char *final;
    char sign[10]="\0",username1[50]="\0",username[50]="\0",password[100]="\0";
    sscanf(string,"%s %s %s",sign,username1,password);
    int i=0;
    while(username1[i]!=','){
        username[i]=username1[i];
        i++;
    }
    char userfile[200];
    strcpy(userfile,filesourse_user_id);
    strcat(userfile,username);
    strcat(userfile,".txt");
    if(!fileexists(userfile)){
        FILE *fptr;

        //password
        fptr=fopen(userfile,"w");
        fprintf(fptr,"%s",password);
        fclose(fptr);

        //bio
        strcpy(userfile,filesourse_user_bio);
        strcat(userfile,username);
        strcat(userfile,".txt");
        fptr=fopen(userfile,"w");
        fprintf(fptr,"!&^bio\n");
        fclose(fptr);

        //folloing
        strcpy(userfile,filesourse_user_following);
        strcat(userfile,username);
        strcat(userfile,".txt");
        fptr=fopen(userfile,"w");
        fprintf(fptr,"folloing");
        fclose(fptr);

        //follower
        strcpy(userfile,filesourse_user_followers);
        strcat(userfile,username);
        strcat(userfile,".txt");
        fptr=fopen(userfile,"w");
        fprintf(fptr,"follower");
        fclose(fptr);

        //tweetid
        strcpy(userfile,filesourse_user_tweetid);
        strcat(userfile,username);
        strcat(userfile,".txt");
        fptr=fopen(userfile,"w");
        fprintf(fptr,"ID");
        fclose(fptr);

        //viewed
        strcpy(userfile,filesourse_user_viewed);
        strcat(userfile,username);
        strcat(userfile,".txt");
        fptr=fopen(userfile,"w");
        fprintf(fptr,"ID");
        fclose(fptr);

        final="{\"type\":\"Successful\"}";
    }
    else{
        final="{\"type\":\"Error exist\"}";
    }
    return final;
}

char* login(char* string){
    char final[200]="";char* final1="ab";
    FILE *fptr;
    char login[10]="\0",username1[50]="\0",username[50]="\0",password[100];
    sscanf(string,"%s %s %s",login,username1,password);
    int i=0;
    while(username1[i]!=','){
        username[i]=username1[i];
        i++;
    }
    char userfile[200];
    char token[40];
    char tokenaddres[200];


    strcpy(userfile,filesourse_user_id);
    strcat(userfile,username);
    strcat(userfile,".txt");

    if(fileexists(userfile)){

        fptr=fopen(userfile,"r");
        char pass[50];
        fscanf(fptr,"%s",pass);
        fclose(fptr);
        if(!strcmp(password,pass)) {

            strcpy(userfile,filesourse_user_online);
            strcat(userfile,username);
            strcat(userfile,".txt");
            if(fileexists(userfile)){
                strcpy(final,"{\"type\":\"Error online\"}");
                final1=final;
                return final1;
            }

            strcpy(userfile,filesourse_user_id);
            strcat(userfile,username);
            strcat(userfile,".txt");

            //*************creat token**************
            strcpy(token,randtoken());
            strcpy(tokenaddres,filesourse_token);
            strcat(tokenaddres,token);
            strcat(tokenaddres,".txt");
            fptr=fopen(tokenaddres,"w");
            fprintf(fptr,"%s",username);
            fclose(fptr);
            //***************creat token****************

            //creat user online
            strcpy(userfile,filesourse_user_online);
            strcat(userfile,username);
            strcat(userfile,".txt");

            fptr=fopen(userfile,"w");
            fclose(fptr);

            strcpy(final , "{\"type\":\"Token\",\"message\":\"");
            strcat(final,token);
            strcat(final, "\"}");

            final1=final;
            return final1;

        }else{
            strcpy(final,"{\"type\":\"Error wrong password\"}");
            final1=final;
            return final1;
        }

    }
    else {
        strcpy(final, "{\"type\":\"Error exist\"}");
        final1=final;
    }
    return final1;
}

char* send_tweet(char* string){
    char tok1[40]="\0",tok[40]="\0",tweet[200]="\0",type[20]="\0",*id;
    sscanf(string,"%s %*s %s %[^\n]s",type,tok1,tweet);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id,"Error")){
        return "{\"type\":\"Error\"}";
    }

    char userfile[200];
    i = 1;
    char idtw[10];
    sprintf(idtw,"%d",i);
    strcpy(userfile, filesourse_tweet_content);
    strcat(userfile, idtw);
    strcat(userfile, ".txt");
    while (fileexists(userfile)) {
        i++;
        sprintf(idtw,"%d",i);
        strcpy(userfile, filesourse_tweet_content);
        strcat(userfile, idtw);
        strcat(userfile, ".txt");
    }

    FILE *fptr;

    fptr=fopen(userfile,"w");
    fprintf(fptr,"%s\n%s",id_str,tweet);
    fclose(fptr);

    strcpy(userfile, filesourse_tweet_like);

    strcat(userfile, idtw);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"w");
    fprintf(fptr,"likes");
    fclose(fptr);

    //tweet comment

    strcpy(userfile, filesourse_tweet_comment);
    strcat(userfile, idtw);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"w");
    fprintf(fptr,"comments");
    fclose(fptr);

    //tweet if for user file

    strcpy(userfile, filesourse_user_tweetid);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"a");
    fputs("\n",fptr);
    fputs(idtw,fptr);
    fclose(fptr);
    return "{\"type\":\"Successful\"}";

}

char* like_tweet(char* string){
    char tok1[40]="\0",tok[40]="\0",type[20]="\0",tweet_id[10]="\0",*id;
    sscanf(string,"%s %s %s",type,tok1,tweet_id);

    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);

    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }
    FILE *fptr;
    char userfile[150];
    strcpy(userfile, filesourse_tweet_like);
    strcat(userfile, tweet_id);
    strcat(userfile, ".txt");

    if(!fileexists(userfile)){
        return "{\"type\":\"Error exist\"}";
    }
    fptr=fopen(userfile,"r");
    char u[40];
    fscanf(fptr,"%s",u);
    while(!feof(fptr)){
        fscanf(fptr,"%s",u);
        if(!strcmp(u,id_str))
            return "{\"type\":\"Error repeat\"}";
    }
    fclose(fptr);
    fptr=fopen(userfile,"a");
    fputs("\n",fptr);
    fputs(id_str,fptr);

    fclose(fptr);
    return "{\"type\":\"Successful\"}";
}

char* comment_tweet(char* string){
    char tok1[40]="\0",tok[40]="\0",tweet[200]="\0",type[20]="\0",tweet_id1[10]="\0",tweet_id[10]="\0",*id;
    sscanf(string,"%s %s %s %[^\n]s",type,tok1,tweet_id1,tweet);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    i=0;
    while(tweet_id1[i]!=','){
        tweet_id[i]=tweet_id1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id,"Error")){
        return "{\"type\":\"Error token\"}";
    }
    FILE *fptr;
    char tweetfile[150];
    strcpy(tweetfile, filesourse_tweet_comment);
    strcat(tweetfile, tweet_id);
    strcat(tweetfile, ".txt");
    if(!fileexists(tweetfile)){
        return "{\"type\":\"Error exist\"}";
    }
    fptr=fopen(tweetfile,"a");
    fputs("\n",fptr);
    fputs(id_str,fptr);
    fputs("\n",fptr);
    fputs(tweet,fptr);

    fclose(fptr);
    return "{\"type\":\"Successful\"}";
}
char* change_password(char* string){
    char tok1[40]="\0",tok[40]="\0",type[20]="\0",current_pass1[40]="\0",current_pass[40]="\0",new_pass[50]="\0",*id;
    sscanf(string,"%s %*s %s %s %s",type,tok1,current_pass1,new_pass);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }

    i=0;
    while(current_pass1[i]!=','){
        current_pass[i]=current_pass1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id,"Error")){
        return "{\"type\":\"Error token\"}";
    }

    FILE *fptr;
    char userfile[150];
    strcpy(userfile, filesourse_user_id);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    if(!fileexists(userfile)){
        return "{\"type\":\"Error exist\"}";
    }

    fptr=fopen(userfile,"r");
    char passold[100];
    fscanf(fptr,"%s",passold);
    fclose(fptr);

    if(strcmp(current_pass,passold)){
        return "{\"type\":\"Error password\"}";
    }
    if(strlen(new_pass)>16)
        return "{\"type\":\"Error size\"}";
    fptr=fopen(userfile,"w");
    fprintf(fptr,"%s",new_pass);
    fclose(fptr);
    return "{\"type\":\"Successful\"}";
}
char* set_bio(char* string){
    char tok1[40]="\0",tok[40]="\0",type[20],bio[100]="\0",*id;
    sscanf(string,"%s %*s %s %[^\n]s",type,tok1,bio);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }
    FILE *fptr;
    char userfile[150];
    strcpy(userfile, filesourse_user_bio);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    char bioold[100]="";
    fscanf(fptr,"%s",bioold);

    fclose(fptr);
    if(strcmp(bioold,"!&^bio"))
        return "{\"type\":\"Error bio\"}";
    fptr=fopen(userfile,"w");
    fprintf(fptr,"%s",bio);
    fclose(fptr);
    return "{\"type\":\"Successful\"}";
}
char* follow(char* string){
    char tok1[40]="\0",tok[40]="\0",type[20]="\0",username[100]="\0",*id;

    sscanf(string,"%s %s %s",type,tok1,username);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }
    FILE *fptr;
    char userfile[150];

    strcpy(userfile, filesourse_user_following);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    fptr=fopen(userfile,"a");
    fputs("\n",fptr);
    fputs(username,fptr);
    fclose(fptr);

    strcpy(userfile, filesourse_user_followers);
    strcat(userfile, username);
    strcat(userfile, ".txt");

    fptr=fopen(userfile,"a");
    fputs("\n",fptr);
    fputs(id_str,fptr);
    fclose(fptr);

    return "{\"type\":\"Successful\"}";
}
char* unfollow(char* string){
    char tok1[40]="\0",tok[40]="\0",type[20]="\0",username[100]="\0",*id;
    sscanf(string,"%s %s %s",type,tok1,username);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;

    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }
    FILE *fptr;
    char userfile[150];
    strcpy(userfile, filesourse_user_following);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    int t=0;
    char alluser[1000][40];
    fscanf(fptr,"%s",alluser[t]);
    while(!feof(fptr)){
        fscanf(fptr,"%s",alluser[t]);
        t++;
    }
    fclose(fptr);
    int v=0;
    for(i=0;i<t;i++){
        if(!strcmp(alluser[i],username)) {
            strcpy(alluser[i], "!^");
            v=1;
        }
    }
    if(v==0){
        return "{\"type\":\"Error username\"}";
    }
    fptr=fopen(userfile,"w");
    for(i=0;i<t-1;i++){
        if(strcmp(alluser[i],"!^"))
            fprintf(fptr,"\n%s",alluser[i]);
    }
    fclose(fptr);

    strcpy(userfile, filesourse_user_following);
    strcat(userfile, username);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    t=0;
    fscanf(fptr,"%s",alluser[t]);
    while(!feof(fptr)){
        fscanf(fptr,"%s",alluser[t]);
        t++;
    }
    fclose(fptr);
    v=0;
    for(i=0;i<t;i++){
        if(!strcmp(alluser[i],username)) {
            strcpy(alluser[i], "!^");
            v=1;
        }
    }
    if(v==0){
        return "{\"type\":\"Error username\"}";
    }
    fptr=fopen(userfile,"w");
    for(i=0;i<t-1;i++){
        if(strcmp(alluser[i],"!^"))
            fprintf(fptr,"\n%s",alluser[i]);
    }
    fclose(fptr);

    return "{\"type\":\"Successful\"}";
}

char* tweet_profile(char* string){
    char* feedback=(char*)malloc(100000*sizeof(char));
    memset(feedback, 0, 100000);

    cJSON *profile=cJSON_CreateObject();
    cJSON *comments;
    cJSON *message;
    cJSON *allTweets;
    cJSON *tweet;

    cJSON_AddStringToObject(profile,"type","profile");

    FILE *fptr;FILE *fptr2;
    char userfile[150],tweetfile[150];
    char tok[40]="\0",type[20]="\0",*id;
    sscanf(string,"%s %s",type,tok);
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);

    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }


    message = cJSON_AddObjectToObject(profile,"message");

    cJSON_AddStringToObject(message,"username",id_str);

    strcpy(userfile, filesourse_user_bio);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    char bio[200];
    fscanf(fptr,"%[^\n]s",bio);
    fclose(fptr);
    if(!strcmp(bio,"!&^bio"))
        cJSON_AddStringToObject(message,"bio","");
    else
        cJSON_AddStringToObject(message,"bio",bio);

    strcpy(userfile, filesourse_user_followers);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    int t=0;
    char check[50];
    fscanf(fptr,"%s",check);
    while(getc(fptr) != EOF){
        fscanf(fptr,"%s",check);
        t++;
    }
    fclose(fptr);
    cJSON_AddNumberToObject(message,"numberOfFollowers",t);
    strcpy(userfile, filesourse_user_following);
    strcat(userfile, id_str);

    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    t=0;
    fscanf(fptr,"%s",check);
    while(getc(fptr) != EOF){
        fscanf(fptr,"%s",check);
        t++;
    }
    fclose(fptr);

    cJSON_AddNumberToObject(message,"numberOfFollowings",t);

    cJSON_AddStringToObject(message,"followStatus","Yourself");

    allTweets = cJSON_AddArrayToObject(message,"allTweets");


    strcpy(userfile, filesourse_user_tweetid);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    char author[50],content[200];
    char comment_user[50]="",comment_content[200]="",numberOflikes[10]="";
    char tweet_id[10]="";int tweet_id_int;
    fscanf(fptr, "%s", tweet_id);
    while(getc(fptr) != EOF) {
        tweet=cJSON_CreateObject();
        fscanf(fptr, "%s", tweet_id);
        tweet_id_int = atoi(tweet_id);
        cJSON_AddNumberToObject(tweet, "id", tweet_id_int);

        strcpy(tweetfile, filesourse_tweet_content);
        strcat(tweetfile, tweet_id);
        strcat(tweetfile, ".txt");
        fptr2 = fopen(tweetfile, "r");

        fscanf(fptr2, "%s\n", author);
        fscanf(fptr2, "%[^\n]s", content);

        cJSON_AddStringToObject(tweet, "author", author);
        cJSON_AddStringToObject(tweet, "content", content);

        fclose(fptr2);

        comments = cJSON_AddObjectToObject(tweet,"comments");

        strcpy(tweetfile, filesourse_tweet_comment);
        strcat(tweetfile, tweet_id);
        strcat(tweetfile, ".txt");
        fptr2 = fopen(tweetfile, "r");
        fscanf(fptr2, "%s", comment_user);
        while (getc(fptr2) != EOF) {
            fscanf(fptr2, "%s\n", comment_user);
            fscanf(fptr2, "%[^\n]s", comment_content);

            cJSON_AddStringToObject(comments, comment_user, comment_content);
        }
        fclose(fptr2);
        strcpy(tweetfile, filesourse_tweet_like);
        strcat(tweetfile, tweet_id);
        strcat(tweetfile, ".txt");
        fptr2=fopen(tweetfile,"r");
        t=0;
        fscanf(fptr2, "%s", comment_user);
        while (getc(fptr2)!=EOF) {
            fscanf(fptr2, "%s", comment_user);
            t++;
        }

        fclose(fptr2);


        cJSON_AddNumberToObject(tweet,"likes",t);

        cJSON_AddItemToArray(allTweets,tweet);
    }
    fclose(fptr);

    feedback=cJSON_Print(profile);
    cJSON_free(profile);
    cJSON_free(message);
    cJSON_free(tweet);
    return feedback;
}
char* search(char* string){
    char* feedback=(char*)malloc(100000*sizeof(char));
    memset(feedback, 0, 100000);

    cJSON *profile=cJSON_CreateObject();
    cJSON *message;
    cJSON *allTweets;
    cJSON *tweet;
    cJSON *comments;

    cJSON_AddStringToObject(profile,"type","search");
    FILE *fptr;FILE *fptr2;
    char userfile[150]="\0",tweetfile[150]="\0";
    char tok1[40]="\0",tok[40]="\0",type[20]="\0",username[100]="\0",*id;
    sscanf(string,"%s %s %s",type,tok1,username);
    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;

    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }

    message = cJSON_AddObjectToObject(profile,"message");

    cJSON_AddStringToObject(message,"username",username);

    strcpy(userfile, filesourse_user_bio);
    strcat(userfile, username);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    char bio[200];
    fscanf(fptr,"%[^\n]s",bio);
    fclose(fptr);
    if(!strcmp(bio,"!&^bio"))
        cJSON_AddStringToObject(message,"bio","");
    else
        cJSON_AddStringToObject(message,"bio",bio);

    strcpy(userfile, filesourse_user_followers);
    strcat(userfile, username);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");
    int t=0;
    char check[50];
    fscanf(fptr,"%s",check);
    while(getc(fptr) != EOF){
        fscanf(fptr,"%s",check);
        t++;
    }
    fclose(fptr);
    cJSON_AddNumberToObject(message,"numberOfFollowers",t);

    strcpy(userfile, filesourse_user_following);
    strcat(userfile, username);
    strcat(userfile, ".txt");

    fptr=fopen(userfile,"r");

    t=0;
    fscanf(fptr,"%s",check);

    while(getc(fptr) != EOF){
        fscanf(fptr,"%s",check);
        t++;
    }

    fclose(fptr);

    cJSON_AddNumberToObject(message,"numberOfFollowings",t);

    strcpy(userfile, filesourse_user_following);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    fptr=fopen(userfile,"r");

    int FolloingSatate=0;

    fscanf(fptr,"%s",check);
    while(getc(fptr) != EOF){
        fscanf(fptr,"%s",check);
        if(!strcmp(check,username))
            FolloingSatate=1;
    }
    if(FolloingSatate==1)
        cJSON_AddStringToObject(message,"followStatus","Followed");
    else
        cJSON_AddStringToObject(message,"followStatus","NotFollowed");

    allTweets = cJSON_AddArrayToObject(message,"allTweets");

    strcpy(userfile, filesourse_user_tweetid);
    strcat(userfile, username);
    strcat(userfile, ".txt");
    fptr=fopen(userfile,"r");

    char author[50],content[200];
    char comment_user[50]="\0",comment_content[200]="\0",numberOflikes[10]="\0";
    char tweet_id[10]="\0";int tweet_id_int;
    fscanf(fptr, "%s", tweet_id);

    while(getc(fptr) != EOF) {
        tweet=cJSON_CreateObject();
        fscanf(fptr, "%s", tweet_id);
        tweet_id_int = atoi(tweet_id);
        cJSON_AddNumberToObject(tweet, "id", tweet_id_int);

        strcpy(tweetfile, filesourse_tweet_content);
        strcat(tweetfile, tweet_id);
        strcat(tweetfile, ".txt");
        fptr2 = fopen(tweetfile, "r");

        fscanf(fptr2, "%s\n", author);
        fscanf(fptr2, "%[^\n]s", content);

        cJSON_AddStringToObject(tweet, "author", author);
        cJSON_AddStringToObject(tweet, "content", content);

        fclose(fptr2);

        comments = cJSON_AddObjectToObject(tweet,"comments");

        strcpy(tweetfile, filesourse_tweet_comment);
        strcat(tweetfile, tweet_id);
        strcat(tweetfile, ".txt");
        fptr2 = fopen(tweetfile, "r");
        fscanf(fptr2, "%s", comment_user);
        while (getc(fptr2) != EOF) {
            fscanf(fptr2, "%s\n", comment_user);
            fscanf(fptr2, "%[^\n]s", comment_content);

            cJSON_AddStringToObject(comments, comment_user, comment_content);
        }
        fclose(fptr2);
        strcpy(tweetfile, filesourse_tweet_like);
        strcat(tweetfile, tweet_id);
        strcat(tweetfile, ".txt");
        fptr2=fopen(tweetfile,"r");
        t=0;
        fscanf(fptr2, "%s", comment_user);
        while (getc(fptr2)!=EOF) {
            fscanf(fptr2, "%s", comment_user);
            t++;
        }

        fclose(fptr2);

        cJSON_AddNumberToObject(tweet,"likes",t);

        cJSON_AddItemToArray(allTweets,tweet);
    }
    fclose(fptr);

    feedback=cJSON_Print(profile);
    cJSON_free(profile);
    cJSON_free(message);
    cJSON_free(tweet);
    return feedback;


}
char* logout(char* string){
    char tok[40]="\0",type[20],*id;
    sscanf(string,"%s %s",type,tok);

    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);
    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error\"}";
    }

    char userfile[200];
    strcpy(userfile,filesourse_token);
    strcat(userfile, tok);
    strcat(userfile, ".txt");
    remove(userfile);

    strcpy(userfile,filesourse_user_online);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");
    remove(userfile);

    return "{\"type\":\"Successful\"}";

}

char* delete(char* string){

    char tok1[40]="\0",tok[40]="\0",type[20]="\0",tweet_id[10]="\0",*id;
    sscanf(string,"%s %s %s",type,tok1,tweet_id);
    FILE *fptr;

    int i=0;
    while(tok1[i]!=','){
        tok[i]=tok1[i];
        i++;
    }
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);


    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }

    char tweetfile[150];
    strcpy(tweetfile, filesourse_tweet_content);
    strcat(tweetfile, tweet_id);
    strcat(tweetfile, ".txt");

    if(!fileexists(tweetfile)){
        return "{\"type\":\"Error exist\"}";
    }

    char userfile[200];
    strcpy(userfile,filesourse_user_tweetid);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    int vexist=0;

    char check[6];
    char alltweet[100000][6];
    int t=0;

    fptr=fopen(userfile,"r");
    fscanf(fptr, "%s", check);
    while(getc(fptr) != EOF){
        fscanf(fptr, "%s", check);
        if(!strcmp(check,tweet_id))
            vexist=1;
        else{
            strcpy(alltweet[t],check);
            t++;
        }
    }
    fclose(fptr);
    if(vexist==0)
        return "{\"type\":\"Error user\"}";

    strcpy(userfile,filesourse_user_tweetid);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    fptr=fopen(userfile,"w");
    fprintf(fptr,"tweetID");

    for(i=0;i<t;i++){
        fprintf(fptr,"\n%s",alltweet[i]);
    }
    fclose(fptr);

    strcpy(tweetfile, filesourse_tweet_content);
    strcat(tweetfile, tweet_id);
    strcat(tweetfile, ".txt");

    remove(tweetfile);

    strcpy(tweetfile, filesourse_tweet_like);
    strcat(tweetfile, tweet_id);
    strcat(tweetfile, ".txt");

    remove(tweetfile);

    strcpy(tweetfile, filesourse_tweet_comment);
    strcat(tweetfile, tweet_id);
    strcat(tweetfile, ".txt");

    remove(tweetfile);

    return "{\"type\":\"Successful\"}";
}

char* refresh(char* string){
    int i=0;
    char* feedback=(char*)malloc(SIZE*sizeof(char));
    memset(feedback, 0, 100000);

    cJSON *refresh=cJSON_CreateObject();
    cJSON *comments;
    cJSON *allTweets;
    cJSON *tweet;

    cJSON_AddStringToObject(refresh,"type","List");

    FILE *fptr;FILE *fptr2;

    char userfile[150],tweetfile[150];
    char tok[40]="\0",type[20]="\0",*id;
    sscanf(string,"%s %s",type,tok);
    char* toke=tok;
    id=search_token(toke);
    char id_str[50];
    strcpy(id_str,id);

    if(!strcmp(id_str,"Error")){
        return "{\"type\":\"Error token\"}";
    }

    char allfolloing[10000][40];

    strcpy(userfile, filesourse_user_following);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    fptr=fopen(userfile,"r");
    i=0;
    fscanf(fptr,"%s",allfolloing[i]);
    while(getc(fptr) != EOF){
        fscanf(fptr,"%s",allfolloing[i]);
        i++;
    }
    fclose(fptr);

    int tfolloing=i;

    char alltweetArray[100000][6];
    i=0;
    int alltweetArray_int[100000];
    for(int j=0;j<tfolloing;j++) {
        strcpy(userfile, filesourse_user_tweetid);
        strcat(userfile, allfolloing[j]);
        strcat(userfile, ".txt");

        fptr=fopen(userfile,"r");

        fscanf(fptr, "%s", alltweetArray[i]);
        while (getc(fptr) != EOF) {
            fscanf(fptr, "%s", alltweetArray[i]);
            alltweetArray_int[i]=atoi(alltweetArray[i]);
            i++;
        }
        fclose(fptr);
    }

    fclose(fptr);
    int t=i;
    int a;
    for (int x = 0; x < t; ++x){
        for (int q = x + 1; q < t; ++q){

            if (alltweetArray_int[x] < alltweetArray_int[q]){

                a =  alltweetArray_int[x];
                alltweetArray_int[x] = alltweetArray_int[q];
                alltweetArray_int[q] = a;

            }
        }
    }

    for(int x=0;x<t;x++){
        sprintf(alltweetArray[x],"%d",alltweetArray_int[x]);
    }


    strcpy(userfile, filesourse_user_viewed);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    char check[6];
    for(i=0;i<t;i++){
        fptr=fopen(userfile,"r");
        fscanf(fptr,"%s",check);
        while(getc(fptr) != EOF){
            fscanf(fptr,"%s",check);
            if(!strcmp(check,alltweetArray[i])) {
                strcpy(alltweetArray[i], "del");

            }
        }
        fclose(fptr);
    }

    allTweets = cJSON_AddArrayToObject(refresh,"message");
    int tweet_id_int,like_number;
    char author[50],content[200];
    char comment_user[50]="",comment_content[200]="",numberOflikes[7]="";

    for(i=0;i<t;i++){
        if(strcmp(alltweetArray[i],"del")){
            strcpy(tweetfile, filesourse_tweet_content);
            strcat(tweetfile, alltweetArray[i]);
            strcat(tweetfile, ".txt");
            fptr=fopen(tweetfile,"r");

            tweet=cJSON_CreateObject();
            tweet_id_int=atoi(alltweetArray[i]);

            cJSON_AddNumberToObject(tweet, "id", tweet_id_int);

            fscanf(fptr,"%s\n",author);
            fscanf(fptr,"%[^\n]s",content);

            cJSON_AddStringToObject(tweet, "author", author);
            cJSON_AddStringToObject(tweet, "content", content);

            comments = cJSON_AddObjectToObject(tweet,"comments");

            strcpy(tweetfile, filesourse_tweet_comment);
            strcat(tweetfile, alltweetArray[i]);
            strcat(tweetfile, ".txt");
            fptr2 = fopen(tweetfile, "r");
            fscanf(fptr2, "%s", comment_user);

            while (getc(fptr2) != EOF) {
                fscanf(fptr2, "%s\n", comment_user);
                fscanf(fptr2, "%[^\n]s", comment_content);

                cJSON_AddStringToObject(comments, comment_user, comment_content);
            }
            fclose(fptr2);

            strcpy(tweetfile, filesourse_tweet_like);
            strcat(tweetfile, alltweetArray[i]);
            strcat(tweetfile, ".txt");

            fptr2=fopen(tweetfile,"r");

            like_number=0;
            fscanf(fptr2, "%s", comment_user);
            while (getc(fptr2)!=EOF) {
                fscanf(fptr2, "%s", comment_user);
                like_number++;
            }
            fclose(fptr2);


            cJSON_AddNumberToObject(tweet,"likes",like_number);

            cJSON_AddItemToArray(allTweets,tweet);
        }
    }
    fclose(fptr);

    feedback=cJSON_Print(refresh);

    strcpy(userfile, filesourse_user_viewed);
    strcat(userfile, id_str);
    strcat(userfile, ".txt");

    fptr = fopen(userfile, "a");

    for(i=0;i<t;i++) {
        if (strcmp(alltweetArray[i], "del")) {
            fputs("\n",fptr);
            fputs(alltweetArray[i],fptr);
        }
    }
    fclose(fptr);
    return feedback;
}
