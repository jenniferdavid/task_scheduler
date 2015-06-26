#include<stdio.h>
#include<stdlib.h>
#include<curses.h>

int main()

{ 
  int c[20][20],min,l,m,sr[20],sc[20],flag[20][20],i,j,k,rf[20],cf[20],n;
  int nrz[20],ncz[20],cn,a,noz,nrz1[20],ncz1[20];
  printf("\nEnter the no of assignments:");
  scanf("%d",&n);
  printf("Enter the cost matrix:");
  for(i=0;i<n;i++)
  { for(j=0;j<n;j++)
     scanf("%d",&c[i][j]);
  }
  printf("Cost matrix:\n");
  for(i=0;i<n;i++)
  { for(j=0;j<n;j++)
     printf(" %d",c[i][j]);
    printf("\n");
  }
  for(i=0;i<n;i++)
  { min=c[i][0];
    for(j=0;j<n;j++)
    { if(min>c[i][j])
       min=c[i][j];
    }
    for(j=0;j<n;j++)
     c[i][j]=c[i][j]-min;
  }
  for(i=0;i<n;i++)
  { min=c[0][i];
    for(j=0;j<n;j++)
    { if(min>c[j][i])
       min=c[j][i];
    }
    for(j=0;j<n;j++)
     c[j][i]=c[j][i]-min;
  }
  printf("Cost matrix after row & column operation:\n");
  for(i=0;i<n;i++)
  { for(j=0;j<n;j++)
     printf(" %d",c[i][j]);
    printf("\n");
  }
  x:;
  a=0;noz=0,min=1000;
  for(i=0;i<n;i++)
  { for(j=0;j<n;j++)
     flag[i][j]=0;
  }
  for(i=0;i<n;i++)
  { cn=0;
    for(j=0;j<n;j++)
    { if(c[i][j]==0)
      { cn++;
	flag[i][j]=1;
      }
    }
    nrz[i]=cn;
    noz=noz+cn;
  }
  for(i=0;i<n;i++)
  { cn=0;
    for(j=0;j<n;j++)
    { if(c[j][i]==0)
      { cn++;
	flag[j][i]=1;
      }
    }
    ncz[i]=cn;
  }
  for(i=0;i<n;i++)
  { nrz1[i]=nrz[i];
    ncz1[i]=ncz[i];
  }
  k=0;
  while(nrz[k]!=0||ncz[k]!=0)
  {for(i=0;i<n;i++)
   { cn=0;
    for(j=0;j<n;j++)
    { if(flag[i][j]==1)
       cn++;
      nrz[i]=cn;
    }
    if(nrz[i]==1)
    { for(j=0;j<n;j++)
      { if(flag[i][j]==1)
	{ flag[i][j]=2;
	  for(k=0;k<n;k++)
	  { if(flag[k][j]==1)
	     flag[k][j]=0;
	  }
	}
      }
    }
  }
  for(i=0;i<n;i++)
  { cn=0;
    for(j=0;j<n;j++)
    { if(flag[j][i]==1)
       cn++;
      ncz[i]=cn;
    }
    if(ncz[i]==1)
    { for(j=0;j<n;j++)
      { if(flag[j][i]==1)
	{ flag[j][i]=2;
	  for(k=0;k<n;k++)
	  { if(flag[j][k]==1)
	     flag[j][k]=0;
	  }
	}
      }
    }
  }
  k++;
 }
  for(i=0;i<n;i++)
  { for(j=0;j<n;j++)
    { if(flag[i][j]==2)
	a++;
    }
  }
  getch();
  if(a==n)
  { printf("\nAssignments done!!\n");
    for(i=0;i<n;i++)
    { for(j=0;j<n;j++)
      { if(flag[i][j]==2)
	  printf(" %d->%d ",i+1,j+1);
      }
      printf("\n");
    }
    getch();
    exit(0);    
  }
  else
  { for(i=0;i<n;i++)
    { rf[i]=0,sr[i]=0;
      cf[i]=0,sc[i]=0;
    }
    for(k=n;(k>0&&noz!=0);k--)
    { for(i=0;i<n;i++)
      { m=0;
	for(j=0;j<n;j++)
	{ if((flag[i][j]==4)&&(c[i][j]==0))
	   m++;
	}
        sr[i]=m;
      }
      for(i=0;i<n;i++)
      { if(nrz1[i]==k&&nrz1[i]!=sr[i])
	{  rf[i]=1;
	   for(j=0;j<n;j++)
	   { if(c[i][j]==0)
	       flag[i][j]=4;
	   }
	   noz=noz-k;
	}
      }
      for(i=0;i<n;i++)
      { l=0;
	for(j=0;j<n;j++)
	{ if((flag[j][i]==4)&&(c[j][i]==0))
	   l++;
	}
        sc[i]=l;
      }
      for(i=0;i<n;i++)
      { if(ncz1[i]==k&&ncz1[i]!=sc[i])
	{  cf[i]=1;
	   for(j=0;j<n;j++)
	   { if(c[j][i]==0)
	       flag[j][i]=4;
	   }
	   noz=noz-k;
	}
      }
      for(i=0;i<n;i++)
      { for(j=0;j<n;j++)
	{ if(flag[i][j]!=3)
	  { if(rf[i]==1&&cf[j]==1)
	    {  flag[i][j]=3;
	       if(c[i][j]==0)
		 noz=noz+1;
	    }
	  }
	}
      }
    }
    for(i=0;i<n;i++)
    { for(j=0;j<n;j++)
      { if(rf[i]!=1&&cf[j]!=1)
	{ if(min>c[i][j])
	    min=c[i][j];
	}
      }
    }
    for(i=0;i<n;i++)
    { for(j=0;j<n;j++)
      { if(rf[i]!=1&&cf[j]!=1)
	  c[i][j]=c[i][j]-min;
      }
    }
    for(i=0;i<n;i++)
    { for(j=0;j<n;j++)
      { if(flag[i][j]==3)
	  c[i][j]=c[i][j]+min;
      }
    }
  }
  printf("\n\nmatrix:");
  for(i=0;i<n;i++)
  { for(j=0;j<n;j++)
     printf(" %d",c[i][j]);
    printf("\n");
  }
  goto x;
}
