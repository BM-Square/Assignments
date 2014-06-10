/*--------------------------------------Assignment No. 1-------------------------------------------------------------------------
Problem Statement: Making sense of HTTP log file
----------------------------------------------------------------------------------------------------------------*/
#include<iostream.h>
#include<conio.h>
#include<fstream.h>
#include<string.h>
class sense;

class log
{
	char h[30];						//IP Address
	char l[30];						//Hypen
	char u[30];						//frank
	char t[30];						//Time
	char r[200];						//Request Line
	char call[6];					//Function in request Line
	char address[200];				//Address in request line
	char protocol[6];				//Protocol use - HTTP
	char version[10];				//Protocol Version
	char s[30];						//Status Code
	char b[30];						//Last entry, object returned 
	char refer[512];
	char agent[200];
	char combined[50];
	friend class sense;
};
class sense							//class for tokenization of log format
{
	char data[512*9];				
	log l;
	int i;
	int check_line(char *);
	void tokenization(char *);
public:
	void read_file();
};
int sense::check_line(char *line)	//checking end of http log
{
	int i=0;
	while(line[i]!='\0')
		i++;
	return i;
}
void sense::tokenization(char *token)	//Tokenization of data with " "
{
	int j=0;
	while(data[i]!=' ')
		token[j++]=data[i++];
	token[j]='\0';
	cout<<token<<endl;
	i++;

}
void sense::read_file()					//Reading http log file in data and making sense
{
	fstream fp,fout;
	char line[80];
	int j,k=0,count1=0;
	data[0]='\0';
	fp.open("weblog.txt",ios::in);
	while(!fp.eof())
	{
		fp.getline(line,80);
		strcat(data,line);
		if(check_line(line)<79)
		{
			i=0;
			//         IP Address
			cout<<"IP = ";
			tokenization(l.h);
			//			Hypen
			cout<<"Hypen = ";
			tokenization(l.l);
			//			Frank
			cout<<"Frank = ";
			tokenization(l.u);
			if(strcmp(l.u,"-")==0)
			{
				cout<<"Document is not Password protected"<<endl;
				cout<<"----------------------------------------------------------------------------\n\n";
			}
			else if(strcmp(l.u,"401")==0)
			{
				cout<<"Document is not Authenticated"<<endl;
				cout<<"----------------------------------------------------------------------------\n\n";
			}
			//			Time
			j=0;
			while(data[i]!=' ')
			{
				l.t[j]=data[i];
				i++;j++;
				if(data[i-1]!=']'&& data[i]==' ')
				{
					l.t[j]=data[i];
					i++;j++;
				}
			}
			l.t[j]='\0';
			cout<<"Time = "<<l.t<<endl;
			i++;
			//			Request Line
			j=0;
			while(data[i]!=' ')
			{
				l.r[j]=data[i];
				i++;j++;
				if(data[i-1]!='\"'&& data[i]==' ')
				{
					l.r[j]=data[i];
					i++;j++;
				}
			}
			l.r[j]='\0';
			i++;
			j=1;
			k=0;
			while(l.r[j]!=' ')
			{
				l.call[k]=l.r[j];
				k++;j++;
			}
			j++;
			l.call[k]='\0';
			k=0;
			while(l.r[j]!=' ')
			{
				l.address[k]=l.r[j];
				k++;j++;
			}
			j++;
			l.address[k]='\0';
			k=0;
			while(l.r[j]!='/')
			{
				l.protocol[k]=l.r[j];
				k++;j++;
			}
			j++;
			l.protocol[k]='\0';
			k=0;
			while(l.r[j]!='\"')
			{
				l.version[k]=l.r[j];
				k++;j++;
			}
			j++;
			l.version[k]='\0';
			cout<<"Request Line = "<<l.r<<endl;
			cout<<"Method used by Client = "<<l.call<<endl;
			cout<<"Resource Requested by Client (Query-String) = "<<l.address<<endl;
			cout<<"Protocol used by client = "<<l.protocol<<endl;
			cout<<"Protocol Version = "<<l.version<<endl;
			//			Status Code
			cout<<"Status Code = ";
			tokenization(l.s);
			if(l.s[0]=='2')
			{
				cout<< "Resulted in a successful response from Server"<<endl;
				cout<<"----------------------------------------------------------------------------\n\n";
			}
			else if(l.s[0]=='3')
			{
				cout<< "Resulted in a redirection response from Server"<<endl;
				cout<<"----------------------------------------------------------------------------\n\n";
			}
			else if(l.s[0]=='4')
			{
				cout<< "An error is caused by the client"<<endl;
				cout<<"----------------------------------------------------------------------------\n\n";
			}
			else if(l.s[0]=='5')
			{
				cout<< "There is an error in the server"<<endl;
				cout<<"----------------------------------------------------------------------------\n\n";
			}
			//			Last Entry
			cout<<"Size of the object returned to the client(not including the response headers) = ";
			tokenization(l.b);
			cout<<"----------------------------------------------------------------------------\n";
			if(strcmp(l.b,"-")==0)
			{
				cout<<"No content was returned to the client\n"<<endl;
				cout<<"----------------------------------------------------------------------------\n";
			}
				//			Refer
			j=0;
			while(data[i]!=' ')
			{
				l.refer[j]=data[i];
				i++;j++;
				if(data[i-1]!='\"'&& data[i]==' ')
				{
					l.refer[j]=data[i];
					i++;j++;
				}
			}
			l.refer[j]='\0';
			i++;
			cout<<"Reference = "<<l.refer<<endl;
				//			User Agent
			j=0;
			l.agent[j]=data[i];
			i++;j++;
			while(data[i]!='\"')
			{
				l.agent[j]=data[i];
				i++;j++;
			}
			l.agent[j]='\0';
			i++;
			cout<<"User Agent = "<<l.agent<<endl;
			data[0]='\0';
			cout<<"\n=========================================================================";
			cout<<"\n=========================================================================";
			cout<<"\n\n";
			count1++;
			getch();
		}
	}
}
void main()
{
	sense s;
	s.read_file();
}