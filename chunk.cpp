#include<string.h>
#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>
#define _BSD_SOURCE
using namespace std;
#define bcopy(s1, s2, n) memmove((s2), (s1), (n))
#define bzero(s, n) memset((s), 0, (n))


void error(const char *msg)
{
    perror(msg);
    exit(1);
}


struct details
{
    string username;
    string password;
    bool status;

};

struct group
{
    int groupid;
    string owner;
    vector<string> members;
};

struct entry{
    string id;
    struct details *point;
    struct entry* next;
};

struct entry *info;
struct group *listg;
struct fileinfo{
    string filename;
    string ipadd;
    string prtadd;
    string filesize;

};

struct group_info{
    string group_id;
    string owner;
    vector<string> joinreq;
    vector<string> members;
    vector<fileinfo*> sharedfiles;
};


unordered_map<string,struct group_info*> groupdata;

bool findinmap(string id)
{
    if(groupdata.find(id)==groupdata.end())
     {
        return true;
     }
     else
     {
        return false;
     }
}

string listing(string msg)
{
    string t="Groups:";
    for(auto i=groupdata.begin();i!=groupdata.end();i++)
    {
        t=t+i->first+":";
    }
    return t;
}

string shared(string msg)
{
int i=0;
    int n=msg.length();
     while(i<n&&msg[i]!=32)
        ++i;
    ++i;
    string gid="";
    
      while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     //check whether the particular group exists or not
     if(!findinmap(gid))
     {
       string t="";
       int k=groupdata[gid]->sharedfiles.size();
       for(int j=0;j<k;j++)
       {//check whether the particular user is online or not!
        t=t+groupdata[gid]->sharedfiles[j]->filename+";"+groupdata[gid]->sharedfiles[j]->ipadd+";"+groupdata[gid]->sharedfiles[j]->prtadd+";"+groupdata[gid]->sharedfiles[j]->filesize+":";
        }
        return t;
     }
     else
     {
        return gid ;
     }
}
string addgroup(string msg)
{
//extracting the data to be used
    int i=0;
    int n=msg.length();
     while(i<n&&msg[i]!=32)
        ++i;
    ++i;
    string gid="";
    string uss="";
    
      while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
     if(findinmap(gid))
     {
        group_info *temp=new group_info();
        temp->group_id=gid;
        temp->owner=uss;
        groupdata[gid] = temp;
        return "Group Created";
     }
     else
     {
        return "Group Already Exists";
     }

}

string joingroup(string msg)
{
int i=0;
    int n=msg.length();
     while(i<n&&msg[i]!=32)
        ++i;
    ++i;
    string gid="";
    string uss="";
    
      while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
      if(findinmap(gid))
     {
        
        return "Group does not exist!";
     }
     else
     {
        groupdata[gid]->joinreq.push_back(uss);
        return "Group join request registered!";
     }
}

string leaveg(string msg)
{
    int i=0;
    int n=msg.length();
     while(i<n&&msg[i]!=32)
        ++i;
    ++i;
    string gid="";
    string uss="";
    
      while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
      if(findinmap(gid))
     {
        
        return "Group does not exist!";
     }
     else
     {
        vector<string>::iterator position = find(groupdata[gid]->joinreq.begin(), groupdata[gid]->joinreq.end(),uss);
if (position != groupdata[gid]->joinreq.end()) // == myVector.end() means the element was not found
    groupdata[gid]->joinreq.erase(position);
        return "Request Deleted!";
     }

}

string list_req(string msg)
{
    int i=0;
    int n=msg.length();
     while(i<n&&msg[i]!=32)
        ++i;
    ++i;
    string gid="";
    string uss="";
    
      while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
      if(findinmap(gid))
     {
        
        return "Group does not exist!";
     }
     else
     {
        string t="";
        int len=groupdata[gid]->joinreq.size();
        for(int i=0;i<len;i++)
            t=t+groupdata[gid]->joinreq[i]+":";
        return t;
     }
}
string uploader(string msg)
{
     int i=0;
   int n=msg.length();
    while(i<n&&msg[i]!=32)
        ++i;
     string filen="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        filen+=msg[i];
        ++i;
     }
     string gid="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     string ipad="";
     ++i;
        while(i<n&&msg[i]!=32)
     {
        ipad+=msg[i];
        ++i;
     }
    string portadd="";
    ++i;
    while(i<n&&msg[i]!=32)
    {
        portadd+=msg[i];
        ++i;
    }
    string sizeoffile="";
    ++i;
    while(i<n&&msg[i]!=32)
    {
        sizeoffile+=msg[i];
        ++i;
    }
    //checking whether the particular group for which the request is sent exists or not
     
     if(!findinmap(gid))
     {
            struct fileinfo* temp =new fileinfo();
        temp->filename=filen;
        temp->ipadd=ipad;
        temp->prtadd=portadd;
        temp->filesize=sizeoffile;
        groupdata[gid]->sharedfiles.push_back(temp);
        return "Upload Successful!";
        //return "a"+ggid+"a";
     }
     else
     {
        return "a"+gid+"a";
     }


}



string acceptreq(string msg)
{
    int i=0;
    int n=msg.length();
     while(i<n&&msg[i]!=32)
        ++i;
    
    string gid="";
    string uss="";
    ++i;
      while(i<n&&msg[i]!=32)
     {
        gid+=msg[i];
        ++i;
     }
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
      if(groupdata.find(gid)==groupdata.end())
     {
        
        return "Group does not exist!";
     }
     else
     {
     int len=groupdata[gid]->joinreq.size();
        for(int i=0;i<len;i++)
        groupdata[gid]->members.push_back(groupdata[gid]->joinreq[i]);
        groupdata[gid]->joinreq.clear();    
        return "Accepted";
     }
}

string create(string msg)
{
    int i=0;
    int n=msg.length();
    while(i<n&&msg[i]!=32)
        ++i;
    string idd="";

     string uss="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
     idd=uss;
     string pass="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        pass+=msg[i];
        ++i;
     }
      int flag=0;
         i=0;
         entry *head=info;

        while(head!=NULL)
        {
            if(head->id==idd)
                {  flag=1;
                break;
            }
            if(head->next==NULL)
                break;
            head=head->next;
        }

        if(flag==0)
        {
            //user does not exist create it
           
            details *temp= new details();
            temp->username=uss;
            temp->password=pass;
            temp->status=true;
            entry *tem=new entry();
            tem->id=idd;
            tem->point=temp;
            tem->next=NULL;
            if(head==NULL)
            {
                info=tem;
            }
            else if(head->next==NULL)
            {
                info->next=tem;
            }
            else
            {
                head->next=tem;
                }
                return "User Created Successfully!";
        }
        else
        {
            return "User already exists!";
        }

}

string loggin(string msg)
{  
    int i=0;
   int n=msg.length();
    while(i<n&&msg[i]!=32)
        ++i;
    string idd="";

     string uss="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
     idd=uss;
     string pass="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        pass+=msg[i];
        ++i;
     }
     int flag=0;
         i=0;
         entry *head=info;

        while(head!=NULL)
        {
            if(head->id==idd)
                {  flag=1;
                break;
            }
            if(head->next==NULL)
                break;
            head=head->next;
        }
        if(flag==0)
        {
            return "User does not exist! Create user first and then try!";
        }
        else
        {
             details *temp=head->point;
            if(temp->username==uss&&temp->password==pass)
            {
                temp->status=true;
                return "Logged In";
            }
            else
            {
                return "Wrong password or username!";
            }
        }//else

}//loggin

string loggoff(string msg)
{

    int i=0;
   int n=msg.length();
    while(i<n&&msg[i]!=32)
        ++i;
    string idd="";

     string uss="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        uss+=msg[i];
        ++i;
     }
     idd=uss;
     string pass="";
     ++i;
     while(i<n&&msg[i]!=32)
     {
        pass+=msg[i];
        ++i;
     }
     int flag=0;
         i=0;
         entry *head=info;

        while(head!=NULL)
        {
            if(head->id==idd)
                {  flag=1;
                break;
            }
            if(head->next==NULL)
                break;
            head=head->next;
        }
        if(flag==0)
        {
            return "User does not exist! ";
        }
        else
        {
             details *temp=head->point;
            if(temp->username==uss)
            {
               if( temp->status)
               { temp->status=false;
                return "Logged Out";
            }
            else
            {
                return "User not logged in! First Login!";
            }
            }
            else
            {
                return "Wrong username!";
            }
        }//else

}



string handlerequest(char *input)
{

  string msg(input);
  string comm="";
  int i=0;
  int n=msg.length();
  while(i<n&&msg[i]!=32)
  {
    comm+=msg[i];
    ++i;
  }
  if(comm=="create_user")
  {
    return create(msg);
  }
  if(comm=="login")
  {
    return loggin(msg);
  }
  if(comm=="create_group")
  {
    return addgroup(msg);
  }

  if(comm=="join_group")
  {
    return joingroup(msg);
  }
  if(comm=="leave_group")
  {
    return leaveg(msg);
  }
  if(comm=="list_requests")
  {
    return list_req(msg);
  }
  if(comm=="accept_requests")
  {
    return acceptreq(msg);
  }
  if(comm=="logout")
  {
    return loggoff(msg);
  }
  if(comm=="upload_file")
  {
    return uploader(msg);
  }
  if(comm=="list_files")
  {
    return shared(msg);
  }
  else
    return "No request matched";

}

void* func_accept(void * newsockfd)
{
    //function for accepting multiple requests
    char buffer[256];
    int *a=(int *)(newsockfd);
   // int newsockfd;
     bzero(buffer,256);
    int h=recv(*a,buffer,255,0);
    if (h < 0) error("ERROR reading from socket");
     //the message is in buffer id is client id for now we assume that id is sent by the client
     string response="";

    response=handlerequest(buffer);
    int n=response.length();
    if(n==0)
      {  response="anja";
          n=4;
      }
       
       const void * aa = response.c_str();
          
               int  count=send(*a,aa,n,0);

}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];//to store the msg
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     // create a socket
     // socket(int domain, int type, int protocol)
     
     cout<<"Server started"<<endl;
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
// clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);

     /* setup the host_addr structure for use in bind call */
     serv_addr.sin_family = AF_INET;  

     // automatically be filled with current host's IP address
     serv_addr.sin_addr.s_addr = INADDR_ANY;  

     // convert short integer value for port must be converted into network byte order
     serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");


     int status=listen(sockfd,5);
     int enter=0;
        clilen = sizeof(cli_addr);
         int i=0;
     while(true)
     {
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
      int count;
      
      
      
   
    //count stores no of bytes transmitted -1 if error
    
     pthread_t connection[10];
     if(pthread_create(&connection[i],NULL,func_accept,(void *)&newsockfd)!=0)
        printf("Failed to create thread\n");
    if(i>3)
    {
        i=0;
        while(i<3)
        {
            pthread_join(connection[i++],NULL);
        }
        i=0;
    }
    /* bzero(buffer,256);
    n=recv(newsockfd,buffer,255,0);*/
    //n = read(newsockfd,buffer,255);

      //count=send(newsockfd,"Recieved",8,0);
   
   /*
     if (n < 0) error("ERROR reading from socket");
     //the message is in buffer id is client id for now we assume that id is sent by the client
     string response="";

    response=handlerequest(buffer);
    int n=response.length();
    if(n==0)
      {  response="anja";
          n=4;
      }
       
       const void * a = response.c_str();
          
                count=send(newsockfd,a,n,0);*/
            
        
     //}
    // close(sockfd);
 }
 close(newsockfd);
 close(sockfd);
    
    return 0;
}