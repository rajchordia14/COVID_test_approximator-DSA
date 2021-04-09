#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 10000
int town[N][N];
int trans[N][N];
//int p, perS, infPR,choice;

int doExperiment();

int randomYes()
{
	return 1+rand()%1000;
}

int gcd(int a,int b)
{
	if (a == 0) 
       return b; 
    if (b == 0) 
       return a; 
  
    if (a == b) 
        return a; 
  
    if (a > b) 
        return gcd(a-b, b); 
    return gcd(a, b-a); 
}

int countPrime(int a)
{
	int res=0;
	if(a % 2 ==0)
	{
		res++;
		while(a%2==0)
			a=a/2;
	}
	for (int i=3;i<=pow(a,1/2);i+=2)
	{
		if(a%i==0)
		{
			res++;
			while(a%i==0)
				a=a/i;
		}
	}
	if (a>2)
		res++;
	return res;
}

int countPrimeFactors(int a,int b)
{
	int g;
	g= gcd(a,b);
	return countPrime(g);
}

int biasedYes(int x, int y) 
{
	/* Where x and y and indices of persons in
	Adjacency matrix. */
	int common = countPrimeFactors(x+1001, y+1001);
	/* Define function countPrimeFactors() that computes the
	Count of prime factors common in two arguments.
	More prime factor they have in common, more
	often the pair meets!*/
	// Return biased interaction
	while (common--)
		if (randomYes() == 1)
			return 1;
	return 0;
}

void createTransitiveClosure(int p,int i,int j)
{
	int k;
	trans[i][j]=1;
	for(k=0;k<p;k++)
		if(!trans[i][k] && town[j][k]==1)
			createTransitiveClosure(p,i,k);
}

int main()
{
	clock_t start = clock();
	int repeats;
	int results[10];
	int p, perS, infPR,choice,sum = 0;
	//int repeats,mean,choice,p,perS,infPR;
	float  SD = 0.0, mean;
	printf("enter population ( out of 2000, 5000, 10000 ) : ");
	scanf("%d",&p);

	printf("enter percentage of service providers ( out of 3, 8 ) : ");
	scanf("%d",&perS);

	printf("enter the number of infected persons( out of 5, 30, 100 ) : ");
	scanf("%d",&infPR);

	printf("\nVarious Social Distancing Scenarios ( Choose one ) :\n");
	printf("1. Without any social distancing (T100)\n");
	printf("2. With social distancing reducing contacts to 50% (T50)\n");
	printf("3. With social distancing reducing contacts to 33% (T33)\n");
	printf("4. With social distancing reducing contacts to 50% with assigned service provider (TC)\n\n");
	printf("Enter choice : ");
	scanf("%d",&choice);
	//return doExperiment(p,perS,infPR,choice);



	for (repeats = 0; repeats < 10; repeats++)
		results[repeats] = doExperiment(p,perS,infPR,choice);
	


	//for(int j = 0; j < 0; j++)
		//printf("TESTED: %d\n",results[j]);
	 
	for(int i=0;i<10;i++)
		sum += results[i];

    mean = sum / 10;
    for (int i = 0; i < 10; i++)
		SD += pow(results[i] - mean, 2);

	printf("Average persons to be TESTED: %lf",mean);
    printf("\nStandard Deviation: %lf\n", sqrt(SD / 10));

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Processing time: %f seconds\n", cpu_time_used);

}

int doExperiment(int p, int perS, int infPR, int choice)
{	
	srand(time(0));

	int i, j, absS, intrPR, kount_sr=0, kount_ipr=0, kount_PSR=0, kount_PR=0, count=0, kount_test=0;
	//printf("%d",p);
	//printf("%d",perS);
	//printf("%d",infPR);
	//printf("%d",choice);
	for(i=0;i<p;i++)
		for(j=0;j<p;j++)
			town[i][j]=0;

	int sr[p];
	absS=perS*p/100;
	while(kount_sr<absS)
	{
		for(i=0;i<p;i++)
		{
			if(kount_sr<absS && sr[i]!=1)
			{
				sr[i]=randomYes()==20;
				if(sr[i]==1)
					kount_sr++;
			}
		}
	}
	//printf("kount_sr:%d\n",kount_sr);

	int ipr[p];
	while(kount_ipr<infPR)
	{
		for(i=0;i<p;i++)
		{
			if(kount_ipr<infPR && ipr[i]!=1)
			{
				ipr[i]=randomYes()==20;
				if(ipr[i]==1)
					kount_ipr++;
			}
		}
	}
	//printf("kount_ipr:%d\n",kount_ipr);

	if(choice==1 || choice==2 || choice==3)
	{
		if(choice==1)
			intrPR=20;
		if(choice==2)
			intrPR=10;
		if(choice==3)
			intrPR=6;

		while(kount_PR < ((p-absS)*intrPR))
		{
			for(i=0;i<p;i++)
			{			
				if(sr[i]==0)
				{
					for(j=i+1;j<p;j++)
					{
						if(kount_PR < ((p-absS)*intrPR) && sr[j]==0 && town[i][j]!=1)
						{
							town[i][j]=biasedYes(i,j);
							if(town[i][j]==1)
							{
								kount_PR+=2;
								town[j][i]=town[i][j];
							}
						}
					}
				}
			}
		}

		int intrSR3=(5*p/100),intrSR8=(3*p/100);
		//printf("%d\n",intrSR3);
		//printf("%d\n",intrSR8);

		if(perS==3)
		{
			while(kount_PSR < (absS*intrSR3)*2)
			{
				for(i=0;i<p;i++)
				{			
					if(sr[i]==1)
					{
						for(j=i+1;j<p;j++)
						{
							if(kount_PSR < (absS*intrSR3)*2 && town[i][j]!=1)
							{
								town[i][j]=randomYes()==20;
								if(town[i][j]==1)
								{
									kount_PSR+=2;
									town[j][i]=town[i][j];
								}
							}
						}
					}
				}
			}
		}
		else if(perS==8)
		{
			while(kount_PSR < (absS*intrSR8)*2)
			{
				for(i=0;i<p;i++)
				{			
					if(sr[i]==1)
					{
						for(j=i+1;j<p;j++)
						{
							if(kount_PSR < (absS*intrSR8)*2 && town[i][j]!=1)
							{
								town[i][j]=randomYes()==20;
								if(town[i][j]==1)
								{
									kount_PSR+=2;
									town[j][i]=town[i][j];
								}
							}
						}
					}
				}
			}
		}
	}

	if(choice==4)
	{
		intrPR=10;

		while(kount_PR < ((p-absS)*intrPR))
		{
			for(i=0;i<p;i++)
			{			
				if(sr[i]==0)
				{
					for(j=i+1;j<p;j++)
					{
						if(kount_PR < ((p-absS)*intrPR) && sr[j]==0 && town[i][j]!=1)
						{
							town[i][j]=biasedYes(i,j);
							if(town[i][j]==1)
							{
								kount_PR+=2;
								town[j][i]=town[i][j];
							}
						}
					}
				}
			}
		}

		int intrSR3=(5*p/100),intrSR8=(3*p/100);
		printf("%d\n",intrSR3);
		//printf("%d\n",intrSR8);

		if(perS==3)
		{
			while(kount_PSR < (absS*intrSR3)*2)
			{
				for(i=0;i<p;i++)
				{			
					if(sr[i]==1)
					{
						for(j=i+1;j<p;j++)
						{
							if(kount_PSR < (absS*intrSR3)*2 && town[i][j]!=1 && i%2==j%2)
							{
								town[i][j]=randomYes()==20;
								if(town[i][j]==1)
								{
									kount_PSR+=2;
									town[j][i]=town[i][j];
								}
							}
						}
					}
				}
			}
		}
		else if(perS==8)
		{
			while(kount_PSR < (absS*intrSR8)*2)
			{
				for(i=0;i<p;i++)
				{			
					if(sr[i]==1)
					{
						for(j=i+1;j<p;j++)
						{
							if(kount_PSR < (absS*intrSR8)*2 && town[i][j]!=1 && i%2==j%2)
							{
								town[i][j]=randomYes()==20;
								if(town[i][j]==1)
								{
									kount_PSR+=2;
									town[j][i]=town[i][j];
								}
							}
						}
					}
				}
			}
		}	
	}

	//printf("kount_PR:%d\n",kount_PR);
	//printf("kount_PSR:%d\n",kount_PSR);
	for(i=0;i<p;i++)
		for(j=0;j<p;j++)
			if(town[i][j]==1)
				count++;
	//printf("total ones:%d\n",count);

	for(i=0;i<p;i++)
	{
		if (ipr[i]==1)
			createTransitiveClosure(p,i,i);
	}

	int test[p];
	for(i=0;i<p;i++)
	{
		test[i]=0;
	}

	for(i=0;i<p;i++)
	{
		if (ipr[i]==1)
		{
			for(j=0;j<p;j++)
			{
				if(test[j]!=1 && trans[i][j]==1)
				{
					test[j]=1;
				}
			}
		}
	}

	for(i=0;i<p;i++)
	{
		kount_test+=test[i];
	}
	
	return (kount_test-infPR);
	
}
