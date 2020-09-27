#include <iostream>
#include <fstream>
#include <string>
using namespace std;
ofstream fo("map.txt"),fo2("anomap.txt");
int i,j,x,c,y; string a[1000][1000],b[1000][1000];
int main()
{
	printf("w   h  corner\n");
	scanf("%d%d%d",&x,&y,&c);
	fo << x << " " << y << endl;
	for(i = 0;i < y;++i)
    for(j = 0;j < x;++j)
    {
        a[i][j] = "00";
        b[i][j] = "00";
    }
	if(c == 1)
	{
		for(i = 1;i < x - 1;++i)
		{
	    	a[0][i] = "02";
	    	a[y - 1][i] = "02";
	        b[0][i] = "05";
	    	b[y - 1][i] = "05";
	    	
		}
		for(i = 1;i < y - 1;++i)
		{
	    	a[i][0] = "02";
	    	a[i][x - 1] = "02";
	    	b[i][0] = "05";
	    	b[i][x - 1] = "05";
		}
	}
	for(i = 0;i < y;++i)
	{
    	for(j = 0;j < x;++j)
    	{
    		fo << a[i][j] << " ";
    		fo2 << b[i][j] << " ";
    	}
    	fo << endl;
    	fo2 << endl;
	}
	fo.close();
	fo2.close();
	printf("Done!");
}