
#include <stdio.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/inet.h>
using namespace std;
//struct sockaddr_in{
 //   short sin_family;//code for address family
   // u_short sin_port;//port number
    //struct in_addr sin_addr;//ip address of host
    //char sin_zero[8];
//};
#define bcopy(s1, s2, n) memmove((s2), (s1), (n))
#define bzero(s, n) memset((s), 0, (n))


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

string username="";
bool status;//to tell whether someone is logged in or not
int contiflag;//used for the continue statement in the while loop this tells us about the status of the user
int downloadmode=0;//used to set the download mode will be 1 when we will be downloading any file
string filen;
string path;
string filepath;//to store the path of the fike to be downloaded!
int portc;
int sockf;
//to store destination path of the downloaded file:path

int threader=0;



string processmsg()
{
    string msg;
    cin>>msg;
    if(msg=="create_user")
    {
        string a;
        cin>>a;
        msg=msg+" "+a;
        cin>>a;
        msg=msg+" "+a;
        contiflag=0;
        return msg;

    }
    else if(msg=="login")
    {
       string a;
        cin>>a;
        msg=msg+" "+a;
        username=a;//storing the username
        cin>>a;
        msg=msg+" "+a;
        contiflag=0;
        return msg; 
    }
    else  if(msg=="create_group")
        {
           
            string a;
            cin>>a;
            msg=msg+" "+a;
            if(status==false)
            {   cout<<status<<endl;
                cout<<"first login then try..";
            }
            else
                msg=msg+" "+username;
              if(status==false)
            {
                contiflag=1;
                return msg;
            }
            return msg;
        }

    else if(msg=="join_group")
    {
         
          string a;
            cin>>a;
            msg=msg+" "+a;
            if(status==false)
            {
                cout<<"first login then try..";
            }
            else
                msg=msg+" "+username;
                  if(status==false)
            {
                contiflag=1;
                
            } 
            return msg;
    }
    else if(msg=="leave_group")
    {
        
         string a;
            cin>>a;
            msg=msg+" "+a;
            if(status==false)
            {
                cout<<"first login then try..";
            }
            else
                msg=msg+" "+username; 
                              if(status==false)
            {
                contiflag=1;
                
            } 
            return msg;
    }
    else if(msg=="list_requests")
    {
        
         string a;
            cin>>a;
            msg=msg+" "+a;
                              if(status==false)
            {
                contiflag=1;
                
            } 
            return msg;
            
    }
    else if(msg=="accept_requests")
    {
        
          string a;
        cin>>a;
        msg=msg+" "+a;
        cin>>a;
        msg=msg+" "+a;
        return msg;
                          if(status==false)
            {
                contiflag=1;
                
            } 
            return msg;
    }
    else if(msg=="list_groups")
    {                  if(status==false)
            {
                contiflag=1;
                
            } 
            
        return msg;
    }
    else if(msg=="list_files")
    {
         
        string a;
        cin>>a;
        msg=msg+" "+a;
                          if(status==false)
            {
                contiflag=1;
                
            } 
            
        return msg;
    }
    else if(msg=="upload_file")
    {
         if(status==false)
            {
                contiflag=1;
                return msg;
            }
        string a;
        cin>>a;
        msg=msg+" "+a;//filepath
        filepath=a;
        cin>>a;
        msg=msg+" "+a;//groupid
        msg=msg+" 127.0.0.1";//ip address
        msg=msg+" "+to_string(portc);//port no
        ifstream in_file(filepath, std::ios::binary);
        in_file.seekg(0, std::ios::end);
        int file_size = in_file.tellg();
        msg=msg+" "+to_string(file_size);
        //adding port no.
        //adding port no.
                          if(status==false)
            {
                contiflag=1;
                
            } 
            return msg;

    }
    else if(msg=="logout")
    {
         
        msg=msg+" "+username;
                          if(status==false)
            {
                contiflag=1;
                
            } 
            
           
        return msg;
    }
    else if(msg=="download_file")
    {
       downloadmode=1;
       string gid;
       cin>>gid;
       cin>>filen;
       cin>>path;
       string m="list_files ";
       m+=gid;
       return m;

    }
    return msg;
}


void downloadfile(char* buff)
{
  string det(buff);
  int k=det.length();
  //searching for the required file in the given list of files
  string fn="";
  int i=0;
  while(i<k&&det[i]!=';')
  {
    fn=fn+det[i];
    i++;
  }
  while(fn!=filen)
  {
    i++;
    while(i<k&&det[i]!=':')
    {
      ++i;
    }
    ++i;
    fn="";
    while(i<k&&det[i]!=';')
  {
    fn=fn+det[i];
    i++;
  }
  }
  if(i==k)
    cout<<"File Not found in that group!"<<endl;
  else{
    //file is found
    ++i;
    //extract ip
    string ippp="";
    while(i<k&&det[i]!=';')
    {
      ippp+=det[i];
      ++i;
    }
    ++i;
    //extracting port
    string po="";
    while(i<k&&det[i]!=';')
    {
      po+=det[i];
      ++i;
    }
    cout<<"IP"<<ippp<<"Port"<<po<<endl;
    struct sockaddr_in cli_addr;
    char buffer[512];
    int n;
    int cli_sockfd=socket(AF_INET,SOCK_STREAM,0);
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = inet_addr(ippp.c_str());  // taking just one client for now
    cli_addr.sin_port = htons(stoi(po));   
      int sta = connect(cli_sockfd,(struct sockaddr *) &cli_addr,sizeof(cli_addr));

    if (sta < 0)
        perror("Error in connecting");
    else
        printf("Connected...\n");
    string jio="sendfile";
    //converting string msg to char*
     const void * aaja = jio.c_str();
          

    send(cli_sockfd,aaja, 512, 0);

            FILE *fp = fopen (path.c_str()  , "wb" );
            int fd = fileno(fp);

    while ((n = recv(cli_sockfd , buffer , 512, 0) ) > 0){
                write(fd, buffer, n);
                memset ( buffer , '\0', n);
                
    }
    threader=1;
   close(cli_sockfd);
  }
}


using namespace std;

void * filegiver(void * threadarg){

    int newsockfd;
    struct sockaddr_in cli_addr,ser_addr;
    socklen_t clientlen = sizeof(cli_addr);

    int portno;
    int BUFFSIZE=2048;

    sockf = socket(AF_INET, SOCK_STREAM, 0);
       
    portno = portc;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser_addr.sin_port = htons(portno);
     
    bind(sockf, (struct sockaddr *)&ser_addr, sizeof(ser_addr));

    listen(sockf,5);

    while(1){


        newsockfd = accept(sockf, (struct sockaddr *) &cli_addr, &clientlen);
        char info[BUFFSIZE];
        recv(newsockfd, info, BUFFSIZE, 0);   // download dialoge with path and
        cout<<info<<endl;
        int n;
        
        string str(info);
        if(str == "sendfile"){
            //cout<<"path"<<endl;
            FILE *fp = fopen(filepath.c_str(), "rb");
            int fd = fileno(fp);
            if(fd < 0){
                perror("file not opened");
                exit(3);
            }

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            rewind(fp);
            char Buffer[BUFFSIZE];

            while (( n = fread( Buffer , sizeof(char) , BUFFSIZE , fp )) > 0  && size > 0 ){
                printf("%s \n", Buffer);
                send(newsockfd , Buffer, n, 0 );
                memset(Buffer , '\0', BUFFSIZE);
                size = size - n ;
            }

            fclose(fp);
        }
    }

    //close(newsockfd);

}



int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    status=false;
    contiflag=0;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    int enter=1;
    cout<<"Please createuser or login:"<<endl;
    int countermy = 0;
    portc=atoi(argv[2]);
    pthread_t serverlistnerThread;
    countermy++;
    pthread_create(&serverlistnerThread, NULL, filegiver, NULL);
    while(true)
    {
    portno = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
   // bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,  (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    int stat=connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(enter==0)
    {
      n = read(sockfd, buffer, 255);
    
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);
    enter=1;
}

    if (stat < 0) 
        error("ERROR connecting");
     bzero(buffer,256);
    printf("Please enter the message: ");
   
    string msg=processmsg();

    if(contiflag!=0)
    {
        cout<<"Please Login to continue!!";
        continue;
    }
    //converting string to const void*

     const void * aa = msg.c_str();
          
              
   send(sockfd,aa,msg.length(),0);
   bzero(buffer,256);
   
   //if(threader==0)
   n=read(sockfd,buffer,255);
   //else
   {
    threader=0;
   }
   if(buffer[0]=='L')
   {
    status=true;
   }
   if(downloadmode==0)
   {
   cout<<buffer<<endl;
   //cout<<status<<endl;
}
else
{
    downloadmode=0;
    downloadfile(buffer);
}
 

    close(sockfd);
}
    
  

    pthread_join(serverlistnerThread, NULL);
    countermy--;
    if(countermy == 0)    
      pthread_create(&serverlistnerThread, NULL, filegiver, NULL);
    close(sockf);






    return 0;
}