#include<stdio.h>
#include<string.h>
#include<iostream>
#include<map>
#include<algorithm>
#include<list>
#include<vector>
using namespace std;


map<string,int> mv;
map<string,int>::iterator it1;

string char_to_binary(unsigned __int8 &x)
{
	string temp = "";
	while(x)
	{
		if(x&1)
			temp += '1';
		else
			temp += '0';
		x/=2;
	}
	x = 8 - temp.length();
	for(int i = 0 ; i < x ; i++)
	temp += '0';
	reverse(temp.begin(),temp.end());
	return temp;
}

void decompress(const string &filename, int &shift, string &comp, int &padding)
{
	string myfile = filename + "_output.txt";
	FILE *outputfile = fopen(myfile.c_str() , "w");
	list<int> li;
	list<int>::iterator it2;
	int len = comp.length();
	unsigned __int8 x;
	string s;
	string temp;
	for(int i = 0 ; i < len ; i++)
	{
		x = comp[i];
		s += char_to_binary(x);
	}
	len = s.length();
	len -= padding;
	int j = 0;
	while(j < len)
	{
		temp = "";
		temp += s[j];
		it1 = mv.find(temp);
		while(it1 == mv.end())
		{
		temp += s[++j];
		it1 = mv.find(temp);
	    }
	    li.push_back(it1->second);
	    j++;
	}
	for(it2 = li.begin() ; it2 != li.end() ; it2++)
	fprintf(outputfile,"%c",(char)(*it2 + shift));
}



void getfile(string &filename)
{
	int shift;
	int padding;
	int count_of_keys;
	string key;
	char s[1000000];
	int val;  
	char ch;
	int len;
	int final_length;
	int aux_size;
	string myfile = filename + "_compressed.txt";
	FILE *inputfile = fopen(myfile.c_str() , "r");
	
	if(inputfile == NULL)
	{
		printf("File not found!");
		return ;
	}
	fscanf(inputfile,"%d",&count_of_keys);
	for(int i = 0 ; i < count_of_keys ; i++)
	{
		fscanf(inputfile,"%s", s);

		fscanf(inputfile,"%d",&val);
		len = strlen(s);
		key = "";
		for(int i = 0 ; i < len ; i++)
		key += s[i];
		mv.insert(make_pair(key,val));
	}

	fscanf(inputfile,"%d",&shift);
	fscanf(inputfile,"%d",&padding);
	fscanf(inputfile,"%d",&final_length);
	fscanf(inputfile,"%d",&aux_size);

	vector<int> aux;
	if(aux_size)
	aux.resize(aux_size);
	
	for(int i = 0 ; i < aux_size ; i++)
	fscanf(inputfile,"%d",&aux[i]);

	string comp = "";
	int counter = 0;
	ch = fgetc(inputfile);
	unsigned char x;
	int aux_counter = 0;
	int tot_len = final_length + aux_size;
	while(counter < tot_len)
	{
		if(aux_counter < aux_size && counter == aux[ aux_counter ])
		{
			comp += (char)26;
			aux_counter++;
		}
		else
		{
		fscanf(inputfile,"%c",&x);
		comp += x;
		}
		counter++;
	}
	fclose(inputfile);
	decompress(filename,shift,comp,padding);
	printf("The file has been decompressed successfully!");
}

int main()
{
	string filename;
	getfile(filename);
	return 0;
}
