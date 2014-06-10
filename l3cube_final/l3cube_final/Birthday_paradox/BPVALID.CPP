
/*---------------------------------------------------------------------------
According to the Birthday Paradox, in a set of N randomly choosen people, some
pair of them will have the same Birthday and 50% probability is reached with
23 people.
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
In this program we are reading the different birthdays from different
groups of 23 people and storing them in Binary Search Tree, if a person has a
birthday common to any previous birthday then just increament the corresponding
count value.

If the number of groups having at least one pair are greater than or equal to
total number of scanned groups then birthday paradox is valid.

---------------------------------------------------------------------------*/

#include<iostream.h>
#include<stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<fstream.h>
#include<string.h>
#include<ctype.h>

int no_of_database = 0;   		//Total no of groups scanned
int no_of_hits = 0;       		//Total no of groups having atleast one pair

class tree;

class node
{
	char name[2][30];               //stores the names of people with same birthday
	int day;                        //corresponding number of the day of the year
					//ex: day for 31st December = 365
	int count;                      //holds the no of persons having same birthday
	node *left,*right;
	friend class tree;

	public:
		node()
		{
			count=0;
			left=right=NULL;
		}
};

class tree
{
	node *root;
	fstream fp;
	char ch;
	public:
		node* getnode();                        //read the details of the next person
		int create();                           //entry in the tree
		void selection(char filename[]);
		int date_to_day();                   	//to convert the date to the corresponding number of the day of the year
};
node* tree::getnode()
{
    node *p;
    int i=0;
    p=new node();
    fp.get(ch);  				 //reading data from the group

    while(ch!='\t')
    {
	p->name[p->count][i]=ch;                	//storing the name of the person
	i++;
	fp.get(ch);
    }
    p->name[p->count][i]='\0';
    i=0;
    p->day=date_to_day();
    p->left=p->right=NULL;
    return (p);
}
int tree::create()
{
    node *p,*q;

    int ch,flag=0;
    p=getnode();
    if(root==NULL)
	root=p;
    else
    {
	q=root;
	while(1)
	{

	    if(q->day > p->day)
	    {
		if(q->left==NULL)
		{
		    q->left=p;
		    break;
		}
		else
		{
		    q=q->left;
		}
	    }
	    else if(q->day < p->day)
	    {
		if(q->right==NULL)
		{
		    q->right=p;
		    break;
		}
		else
		{
		    q=q->right;
		}
	    }
	    else if(q->day == p->day)				//if the day is equal then only increament the count value
	    {
		q->count=q->count+1;
		strcpy(q->name[q->count],p->name[0]);   	//store the name of new person
		no_of_hits++;					//counting number of groups having at least one pair
		flag = 1;                                       //terminate scannig the groups on the first occurence a pair
		break;
	    }
	}
	return(flag);
    }
 return(flag);
}


int tree::date_to_day()
{
	 int day,date,i=0;
	 day=0;
	 char tdate[20];            			//to read the date of the month from the group
	 char month[10];
	 fp.get(ch);
	 while(ch!=' ')
	 {
		if(!isalpha(ch))
			tdate[i++]=ch;
		fp.get(ch);
	 }
	 tdate[i]='\0';
	 date=atoi(tdate);
	 i=0;
	 fp.get(ch);
	 while(ch!=' ')
	 {
		month[i++]=ch;
		fp.get(ch);
	 }
	 month[i]='\0';

	 /*--------------------------------------------------------------
	 for converting the date to corresponding day of the year we are
	 simply counting the number of days till the birthday
	 -------------------------------------------------------------*/

		if(!strcmp(month,"January"))
			day=date;
		else if(!strcmp(month,"February"))
			day=31+date;
		else if(!strcmp(month,"March"))
			day=59+date;
		else if(!strcmp(month,"April"))
			day=90+date;
		else if(!strcmp(month,"May"))
			day=120+date;
		else if(!strcmp(month,"June"))
			day=151+date;
		else if(!strcmp(month,"July"))
			day=181+date;
		else if(!strcmp(month,"August"))
			day=212+date;
		else if(!strcmp(month,"September"))
			day=243+date;
		else if(!strcmp(month,"October"))
			day=273+date;
		else if(!strcmp(month,"November"))
			day=304+date;
		else if(!strcmp(month,"December"))
			day=335+date;

	 while(ch!='\n' && !fp.eof())
	 {
		fp.get(ch);
	 }
	 i=0;
	 return day;
}
void tree::selection(char filename[])    		//selecting people for the experiment
{
	fp.open(filename,ios::in);
	no_of_database++;			        //counting total number of groups taking into consideration

	int flag = 0;
	root=NULL;
	for(int i=0;i<23 && flag == 0;i++)
		flag = create();

	if(flag == 1)
	cout<<"\nA match found in "<<filename<<endl;
	else
	cout<<"\nNo match found in "<<filename<<endl;
	getch();
	fp.close();
	flushall();
}
int main()
{
    tree t;
    fstream fp;
    char temp[20];
    clrscr();
    fp.open("filename.txt",ios::in);			//Database containing the Groups to be scanned
    while(!fp.eof())
    {
	fp.getline(temp,20,'\n');
	t.selection(temp);
    }
    cout<<"\nNumber of Groups scanned:\t"<<no_of_database;
    cout<<"\nNumber of Groups with at least one pair:\t"<<no_of_hits;
    if(no_of_hits>=(no_of_database/2))
    {
		cout<<"\nSince, no_of_hits>=(no_of_database/2)";
		cout<<"\nHence, Birthday Paradox is Valid";
    }
	fp.close();
    getch();
    return 0;
}
