/*Assignment problem 4
  To create a program to implement svc(Simple Version Control) of text files

 The concept used here is the creation and maintenance of following files:-
 1)	<filename>.txt - to store the time & date details of creation of various
			 filename versions.

 2)     <filename>(index).txt - to create version having index,
				base file has index 0.

 3)	<filename>_vf.txt - to maintain a list of latest version to be created
*/

#include<iostream.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<fstream.h>
							      //variables in use
fstream fp;
fstream cnt,dt;
char filename[25],fname[25],rname[25],ver[25],version[5];

void copy()
{
	fstream fp1,fp2;
	char ch;

	fp1.open(filename,ios::in);           //to copy contents of previous file version to new file version
	fp2.open(fname,ios::out);
	fp1.get(ch);
	while(!fp1.eof())
	{
		fp2.put(ch);
		fp1.get(ch);
	}
	fp1.close();
	fp2.close();
}



void append()
{                                                    //function to append a new line to file
	ofstream fp1;
	char buffer[10];
	fp1.open(fname,ios::app);

	cout<<"\nEnter line of text to append::";
	cin>>buffer;
	fp1<<"\n";
	fp1<<buffer;
	fp1.close();
}

void disp()                                        //to display file contents if exists
{
       int line=1;
	fp.open(filename,ios::in);

			if(fp.fail())
			{
				cout<<"\n***File Does Not Exist***\n";
			}
			else
			{
				char x[20];
				//fp.getline(x,20);
				cout<<"\nLine\tContent";
				cout<<"\n--------------------------";
				while(!fp.eof())
				{
				   fp.getline(x,20);
				   cout<<"\n"<<line<<"\t"<<x;

				   line++;
				}
			}
			cout<<"\n---------End Of File---------\n";
			fp.close();
}


void del()                           //function to delete any line from file
{
	int lno;
	char buffer[20];
	fstream fp1,fp2;

	disp();
	cout<<"\nSelect line number to delete:";
	cin>>lno;
	fp1.open(filename,ios::in);
	fp2.open(fname,ios::out);

	for(int i=0;i<lno-1;i++)
	{
		fp1.getline(buffer,20);
		if(i==0)
		fp2<<buffer;
		else
		fp2<<"\n"<<buffer;
	}
	fp1.getline(buffer,20);

	while(!fp1.eof())
	{
	      fp1.getline(buffer,20);
	      if(lno==1)
	      fp2<<buffer;
	      else
		fp2<<"\n"<<buffer;
	}
	fp1.close();
	fp2.close();
}

void operations()                      //to select operation to perform on existing file version
{
	int ch1;

	cout<<"\n\nEnter operation to perform on file:-"
		<<"\n1>Append line to the File"
		<<"\n2>Delete line from File";
	cin>>ch1;

	switch(ch1)
	{
		case 1: copy();
			append();
			break;

		case 2:del();
			break;
	}

}


void chk()
{
					 /*to display versions of a given filename
					    with details of date and time of creation*/
	char x;
	cout<<"\nThe available versions are:\n\n";
	dt.open(rname,ios::in);
	dt.get(x);
	while(!dt.eof())
	{
	    cout<<x;
	    dt.get(x);
	}
	cout<<"\n";
	dt.close();
}

int input()
{
   char buff[25];
   fstream test;
   cout<<"\nEnter filename:";          //to input filename from user
   cin>>filename;
   strcpy(buff,filename);
   strcat(buff,"(0).txt");
   test.open(buff,ios::in);            //check if file exists
   if(test.fail())
   {
	cout<<"\nNo such File exists";
	return(1);
   }
   else
   {
	strcpy(rname,filename);
	strcat(rname,".txt");
   chk();
   cout<<"Enter file version number:";     //accept version number from user
   cin>>version;
   }
   return(0);
}

int trial()
{                                           /*checking if version requested by user
					      exists in the file system*/
     char  buff[25],testcnt[3];
     fstream test;

     strcpy(buff,filename);
     strcat(buff,"_vf.txt");
     test.open(buff,ios::in);
     test.getline(testcnt,3);
     if(atoi(version)>atoi(testcnt))
     {
	cout<<"File version non-existent";
	return(1);
     }
   return(0);
}

void update(int vers)
{                                /*to store details about date and time of file creation*/
    dt<<"version"<<vers<<"\t";
    time_t now=time(0);          //to get current time
    char *dt1=ctime(&now);       //to convert time to string
    dt<<dt1;
}

void main()
{
	int ch;
	char newver[3];
	char buffer[10];

	do
	{
	cout<<"\n\nEnter the operation:-\n1>Create a new file\n2>Open existing file\n3>view file contents\n0>Exit\n-->";
	cin>>ch;
	switch(ch)
	{
		case 1: //operations associated with creation of new file
			cout<<"\nEnter filename : ";
			cin>>filename;
			strcpy(ver,filename);
			strcpy(rname,filename);
			strcat(ver,"_vf.txt");
			strcat(filename,"(0).txt");   //adding index 0 to base file
			strcat(rname,".txt");

			cnt.open(ver,ios::out);  //to store latest version of filename formed
			cnt<<"0";
			cnt.close();
			cout<<"Enter a line of text:";  //entering the first line of text
			cin>>buffer;
			fp.open(filename,ios::out);     //storing into filename
			fp<<buffer;
			dt.open(rname,ios::out);
			update(0);                      //to store the date and time details
			dt.close();
			fp.close();
			break;

		case 2: //operations on an existing file version
			int yes;
			yes=input();    //accept filename & check if file exists

			if(yes==0)
			{
				yes=trial();    //checking if requested version exists
				if(yes==0)
				{
					strcpy(ver,filename);
					strcat(ver,"_vf.txt");
					strcat(filename,"(");
					strcpy(fname,filename);
					strcat(filename,version);

					cnt.open(ver,ios::in);  //opening <filename>_vf.txt to get last version number of file formed
					char wd;
					cnt.getline(newver,3);
					cnt.close();

					strcat(fname,itoa(atoi(newver)+1,newver,10));  //to attach index of next version to be created
					strcat(filename,").txt");
					strcat(fname,").txt");
					cnt.open(ver,ios::out);           //to store the new version index in <filename>_vf.txt
					cnt<<newver;
					cnt.close();
					cout<<"\nEdited File version::"<<filename;
					cout<<"\nResultant File version::"<<fname;
					operations();                             //to select operations to perform
					dt.open(rname,ios::app);
					update(atoi(newver));                     //to store date and time of new version created
					dt.close();
				}
			}
			break;

		case 3:
			yes=input();    //accepting filename and checking if it exists
			if(yes==0)
			{
			strcat(filename,"(");         //to put filename in correct format
			strcat(filename,version);     //adding version index requested
			strcat(filename,").txt");
			disp();                       //display file contents
			}
			break;
	    }

	}while(ch!=0);
	getch();
	clrscr();
}