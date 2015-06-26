#include<stdio.h>
#include<stdlib.h>
#include<memory>
#include<curses.h>

int **matrix2d(int m)
{
	int **a,i;
	a=(int**)calloc(m,sizeof(*a));
	for(i=0;i<m;i++)
	a[i]=(int*)calloc(m,sizeof(int));
	return a;
}
int main()
{
	int i,j,**c,**g,*flag,*flag1,m,assign_row=0,*tot_zero,total=0,col_no;
	int row_no,min,max,slzero=0,*az,*k,t;
	//	clrscr();
	printf("enter the r&c in [] matrix\n");
	scanf("%d",&m);
	az=(int*)calloc((2*m),sizeof(int));
	flag=(int *)calloc(m,sizeof(int));
	flag1=(int*)calloc(m,sizeof(int));
	c=matrix2d(m);
	g=matrix2d(m);
	printf("enter the values of matrix\n");
	for(i=0;i<m;i++)
	{
	for(j=0;j<m;j++)
	{
	scanf("%d",&c[i][j]);
	g[i][j]=c[i][j];
	flag[j]=0;
	}
	flag1[i]=0;
	}
	for(i=0;i<m;i++)
	{
	min=c[i][0];
	for(j=0;j<m;j++)
	{
	if(min>=c[i][j])
	min=c[i][j];
	}
	for(j=0;j<m;j++)
	c[i][j]-=min;
	}
	printf("\n after row operation");
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
		printf("%d\t",c[i][j]);
		printf("\n");
	}
	for(i=0;i<m;i++)
	{
		min=c[0][i];
		for(j=0;j<m;j++)
		{
		if(min>=c[j][i])
		min=c[j][i];
		}
		for(j=0;j<m;j++)
		c[j][i]-=min;
	}
		printf("\n after column operation");
	for(i=0;i<m;i++)
	{
		for(j=0;j<m;j++)
		printf("%d\t",c[i][j]);
		printf("\n");
	}
	tot_zero=(int *)calloc(2*m,sizeof(int));
	k=(int *)calloc((2*m),sizeof(int));
	x:;
	assign_row=0;
	total=0;
	for(i=0;i<m;i++)
	{
		flag1[i]=0;
		flag[i]=0;
	}
	for(i=0;i<m;i++)
	{
		tot_zero[i]=0;
		az[i]=0;
		for(j=0;j<m;j++)
		{
			if(c[i][j]==0)
			tot_zero[i]++;
			if(c[i][j]==0&&flag[j]==0)
			az[i]++;
			if(flag[j]==0&&c[i][j]==0&&az[i]==1)
			col_no=j;
		}
		k[i]=i;
		total+=tot_zero[i];
		if(az[i]==1)
		{
			printf("\nrow %d is assigned %d column",i,col_no);
			printf("\t%d",g[i][col_no]);
		       assign_row++;
		       flag[col_no]=1;
		       flag1[i]=1;
		}
       }
       for(i=0;i<m;i++)
       {
		tot_zero[i+m]=0;
		az[i+m]=0;
		for(j=0;j<m;j++)
		{
			if(c[j][i]==0)
			tot_zero[i+m]++;
			if(c[j][i]==0&&flag1[j]==0&&flag[i]==0)
			az[i+m]++;
			if(flag1[j]==0&&c[j][i]==0&&flag[i]==0&&az[i+m]==1)
			row_no=j;
		}
		k[i+m]=i+m;
			if(az[i+m]==1)
			{
			printf("\n\n\nrow %d is assigned %d column",row_no,i);
			printf("\t%d",g[row_no][i]);
			assign_row++;
			flag[i]=1;
			flag1[row_no]=1;
			}
       }
       if(assign_row==m)
       {
		printf("all assignments done\n");
		getch();
		return 0;
       }
       for(i=0;i<(2*m);i++)
       {
		for(j=i+1;j<(2*m);j++)
		{
			if(tot_zero[j]>=tot_zero[i])
		{
			max=tot_zero[i];
			t=k[i];
			tot_zero[i]=tot_zero[j];
			k[i]=k[j];
			tot_zero[j]=max;
			k[j]=t;
		}
		}
       }
	     printf("\nthe count of zeroes are\n");
       for(i=0;i<(2*m);i++)
       printf(" \n  %d   %d  ",k[i],tot_zero[i]);
       printf("\n");
       printf("the value of total is%d \n",total);
       i=0;
       while(total>slzero)
       {
		if(k[i]>=m)
		{
			flag[k[i]-m]=2;
			slzero+=tot_zero[i];
		}
		else
		{
			flag1[k[i]]=2;
			slzero+=tot_zero[i];
		}
		i++;

	}
	printf("\nthe value of slzero %d  ",slzero);
	for(i=0;i<m;i++)
	printf("  %d ",flag1[i]);
	for(i=0;i<m;i++)
	printf(" %d",  flag[i]);
       for(i=0;i<(m*m);i++)
       if(flag[i%m]!=2&&flag1[i/m]!=2)
       {
       min=c[i/m][i%m];
       break;
       }
       for(i=0;i<(m*m);i++)
	if((flag[i%m]!=2)&&(flag1[i/m]!=2)&&(min>=c[i/m][i%m]))
	min=c[i/m][i%m];
	printf("the min element is %d\n",min);
     for(i=0;i<(m*m);i++)
     {
	if((flag[i%m]!=2)&&(flag1[i/m]!=2))
	c[i/m][i%m]-=min;
	else
	if((flag[i%m]==2)&&(flag1[i/m]==2))
	c[i/m][i%m]+=min;
     }
     for(i=0;i<m;i++)
     {
     for(j=0;j<m;j++)
     printf("\t%d",c[i][j]);
     printf("\n");
     }
     getch();
     goto x;
}
