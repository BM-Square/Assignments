/* --------------------------Assignment No. 5-------------------------------------------
Problem Statement: Write a program to list duplicate files from hardrive
-------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
In this program, we've listed duplicate files from harddrive(Floppy Drive)
on FAT file system.The option for deletion of all duplicate files is provided.
For the program to run, it requires WIN XP along with virtual floppy drive.
-------------------------------------------------------------------------*/

#include<stdio.h>
#include<conio.h>
#include<string.h>
#define DRIVE "B:\\"                              //Should be changed upon drive A: or B:

static int j;                                     //Counter For No of Files
int no_files=0;                                   //Shows Total No of File
int comm_no=0;                                    //Shows No of Common File
char dir_name[50];                                //Saves Current Directory
/*--------------------------------------------File name-----------------------------------------------------------------*/
struct file_name                                  //Stores Name,path,Sector of File
{
	char nm[12],path[50];
	unsigned int size,address,line;
}name[50],common[25];
/*----------------------------------------------Queue-------------------------------------------------------------------*/
#define MAX 10
	 unsigned int data[10];
	 char dir[30][10];
	 int front,rear;
void init()
{
	front=rear=-1;
}
int empty()
{
	if(rear==front)
	return 1;
	return 0;
}
void enqueue(unsigned int clust,char *name)
{
	front=(front+1)%10;
	data[front]=clust;
	strcpy(dir[front],name);
}
unsigned int dequeue()
{
	rear=(rear+1)%10;
	strcpy(dir_name,dir[rear]);
	return data[rear];
}
/*----------------------------------------------------------End of Queue------------------------------------------------------------*/
/*---------------------------------------------------------Date And Time------------------------------------------------------------*/
void dispdate(char *rde)
{
	int offset1,date1,day,mm,yyyy;
	offset1=0x18;
	date1=*(int*)(rde+offset1);
	asm{
		mov ax,date1        //Calculating Day
		and ax,0x001f       //anding for 1st 5 bits
		mov day,ax

		mov ax,date1        //Calculating Month
		and ax,0x01e0       //anding for 4 bits
		mov cl,0x05
		shr ax,cl
		mov mm,ax

		mov ax,date1        //Calculating Year
		and ax,0xfe00       //anding for last 7 bits
		mov cl,0x09
		shr ax,cl
		mov yyyy,ax
	}
	printf("\t%d/%d/%d",day,mm,yyyy+1980);
}
void disptime(char *rde)
{
	int offset1,time1,hh1,mm1,ss1;
	offset1=0x16;
	time1=*(int*)(rde+offset1);
	asm{
		mov ax,time1        //Calculating Hours
		and ax,0x001f       //anding for 1st 5 bits
		mov ss1,ax

		mov ax,time1        //Calculating Minute
		and ax,0x07e0       //anding for 6 bits
		mov cl,0x05
		shr ax,cl
		mov mm1,ax

		mov ax,time1        //Calculating Second
		and ax,0xf800       //anding for last 5 bits
		mov cl,0x0b         //Shifting to lower nibble
		shr ax,cl
		mov hh1,ax
	}
	printf("\t%d:%d:%d",hh1,mm1,ss1*2);
}
/*-----------------------------------------------------Display List of Files ----------------------------------------------------*/
void display(struct file_name filname[],int counter)                //Display Data in Structure
{
	int i;
	printf("\nName of File/Folder\tPath\n");
	printf("---------------------------------------------------------\n");
	for(i=0;i<counter;i++)
	{
		printf("\n%s\t\t\t%s",filname[i].nm,filname[i].path);
	}
}
void search1();                               //Searching For Common Files
void del_file()                               //Deleting Common File
{
	int k=0;
	char fname[30];
	for(k=0;k<comm_no;k++)
	{
		strcpy(fname,DRIVE);
		strcat(fname,common[k].path);
		strcat(fname,common[k].nm);
		asm{
			mov ah,0x41							//AH=41h -Delete Command
			lea dx,fname						//Name of File to Delete
			int 0x21							//Dos interrup 21H
			jnc succ
		}
		printf("\n %s File is not deleted",fname);
		asm jmp uns
		succ:
		printf("\n %s File is deleted",fname);
		uns:
	}
}
void search1()                                //Searching Common File Name
{
	int i,l,k=0;
	char fname[30];
	for(i=0;i<no_files;i++)
	{
		for(l=i+1;l<no_files;l++)
		{
			if(strcmp(name[i].nm,name[l].nm)==0 && name[i].size==name[l].size)
			{
				common[k]=name[i];
				k++;
			}
		}
	}
	comm_no=k;
}

void read_dir(unsigned int);                  //Read Sector of Dir
void disp_dir(unsigned char * rde,unsigned int sector_address)      //DIsplay Dir
{
	char fname[8],ext[3],temp_path[50];
	int i,i1,l=0;
	for(l=0;l<224;l++)							//512*14=(224)*32: 512-size of sector,14-No of Sector,32-Size of one Directory
	{
	if(*rde==0x00)								//Shows last entry of Directory
	break;
	else if(*rde==0xe5)							//Shows vacant entry
	{
		asm jmp next1
	}
	else if(isalnum(*rde))						//start of file name should be Alphanumeric
	{
		printf("\n");
		name[j].address=sector_address;
		name[j].line=l;
		strcpy(name[j].path,dir_name);
		i1=i=0;
		for(i=0;i<8;i++)						//Name of file
		{
			printf("%c",*(rde+i));
			if(*(rde+i)!=' ')
			name[j].nm[i1++]=*(rde+i);
		}
		printf(".");

		if (*(rde+0x0b)&0x10)					//for DIR='\' and File= '.'
			name[j].nm[i1++]='\\';
		else
			name[j].nm[i1++]='.';

		for(i=8;i<11;i++)						//Extension
		{
			printf("%c",*(rde+i));
			if(*(rde+i)!=' ')
			name[j].nm[i1++]=*(rde+i);
		}
		name[j].nm[i1]='\0';
		dispdate(rde);							//Display Date of last update to file
		disptime(rde);							//Display Time of last update to file
		if (*(rde+0x0b)&0x10)					//checking for Directory
		{
			printf("  <DIR>");
			name[j].size=-1;
			strcpy(temp_path,name[j].path);
			strcat(temp_path,name[j].nm);
			enqueue(*(int*)(rde+26),temp_path);	//Enqueuing Dir and its path
			j++;
		}
		else
		{
		name[j].size=*(int*)(rde+0x1c);
		printf("  %d",*(int*)(rde+0x1c));
		j++;
		}
	}
	next1:
	rde=rde+32;									//Size of one Entry is 32
	}
	while(!empty())
	{
		read_dir(dequeue());					//Dequeuing Dir
	}
}
void read_dir(unsigned int clust)               //Read Sector of Dir
{
	char fat[512*9],*sub;
	int rem,que,i;
	unsigned char buff[514];
	unsigned int sector;
	asm{
			mov al,1                             //Drive No For A:0 or B:1
			mov cx,9                             //No of Sector to read
			mov dx,1                             //Sector No
			lea bx,fat                           //Buffer
			int 0x25							//interrupt 25h for reading sector
			jnc sucess1
		}
		printf("\n Error1");
		getch();
		exit(0);
		sucess1:
		while(1)
		{
			printf("\n Cluster = %d",clust);
			sector=clust+31;						//In FAT sector containing data starts form +30 to cluster no
			printf("\n Sector = %d",sector);
			asm{
			 mov al,1                                 //Drive No For A:0 or B:1
			 mov cx,1                                 //NO of Sector
			 mov dx,sector                            //Sector No
			 lea bx,buff                              //Buffer
			 int 0x25
			}
			sub=buff;
			disp_dir(sub,sector);
			clust=clust*3;
			que=clust/2;
			rem=clust%2;
			asm mov si,que
			asm mov ax,fat[si]
			asm mov clust,ax
			if(rem==0)
			{
				clust=clust & 0x0fff;
				if(clust==0x0fff)						//clust==0x0fff End of linked sector
				{
					break;
				}
			}
			else
			{
				clust=clust >> 4;
				if(clust==0x0fff)
				{
					break;
				}
			}
		}
}
void main()
{
	char* rde;
	unsigned char root[512*14];
	int i,ch;
	clrscr();
	do
	{
		j=0;
		dir_name[0]='\0';
		asm{
					mov ax,1                           //Drive no for A: 0 or B: 1
					mov cx,14                          //NO of sector to be read
					mov dx,19                          //Sector No.
					lea bx,root                        //Storage
					int 0x25
					jnc success
				}
				getch();
				exit(-1);
				success:
				printf("\n\n\tRoot directory found!");
				rde=root;
				disp_dir(rde,19);
				no_files=j;
				search1();
		printf("\n Enter Choice -\n1.View all File\n2.View all Duplicates file\n3.Delete Files\n4.Exit\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				display(name,no_files);
			break;
			case 2:
				display(common,comm_no);
			break;
			case 3:
				del_file();
			break;
			case 4:
			break;
			default:
				printf("\n Wrong Choice");
		}
		getch();
		clrscr();
	}while(ch!=4);
}
