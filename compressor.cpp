#include<stdio.h>
#include<math.h>
#include<iostream>
#include<vector>
#include<map>
#include<queue>
using namespace std;

vector<string> v(600);
map<string,int> mv;
map<string,int>::iterator it;
vector<int> aux;

struct MinHeapNode										//structure of a huffman node			
{
    int data;               
    unsigned freq;             
    MinHeapNode *left, *right;

    MinHeapNode(int data, unsigned freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

struct compare
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);
    }
};

void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;

    if (root->data != -1)
        cout << root->data << ": " << str << "\n";

    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

void genCodes(struct MinHeapNode* root, string str)			//Generates huffman codes for each character
{
    if (!root)
        return;

    if (root->data != -1)
    {
        v[root->data]=str;
        mv.insert(make_pair(str,root->data));
        return;
    }

    genCodes(root->left, str + "0");
    genCodes(root->right, str + "1");
}

void HuffmanCodes(int data[], int freq[], int size)
{
    struct MinHeapNode *left, *right, *top;

    
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
    for (int i = 0; i < size; ++i)
    minHeap.push(new MinHeapNode(data[i], freq[i]));

 
    while (minHeap.size() != 1)
    {
 
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        top = new MinHeapNode(-1, left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }


 // printCodes(minHeap.top(), "");
    genCodes(minHeap.top(),"");
}

unsigned __int8 retrieve(int x, string &compcode)		//Conversion from 8-bit binary to ascii character  
{
	unsigned __int8 num = 0;
	for(int i = 7 ; i >= 0 ; i--)
	{
		if(compcode[x++] == '1')
		num += pow(2,i);
	}
	return num;
}

void compressedOutput(string &filename, string &final, int &count_of_keys, int &shift, int &padding, int &final_length)
{
	string myfile = filename + "_compressed.txt";
	FILE *outputfile = fopen(myfile.c_str(),"w");
	
	if(outputfile == NULL)
	{
		printf("File not found!");
		return ;
	}
	
	fprintf(outputfile,"%d ",count_of_keys);
	
	for(it = mv.begin(); it != mv.end(); it++)
	fprintf(outputfile,"%s %d ",(it->first).c_str(),it->second);
	
	fprintf(outputfile,"%d %d ",shift,padding);
	
	fprintf(outputfile,"%d ",final_length);
	
	fprintf(outputfile,"%d ",aux.size());
	
	for(vector<int>::iterator itt = aux.begin() ; itt != aux.end() ; itt++)
	fprintf(outputfile,"%d ",*itt);
	
	for(int i = 0 ; i < final_length ; i++)
	fprintf(outputfile,"%c",final[i]);
	
	fclose(outputfile);
}

void result(string &filename, int len)	
{
	string myfile = filename + "_compressed.txt";
	FILE *outputfile = fopen(myfile.c_str(),"r");
	if(outputfile == NULL)
	{
		printf("File not found!");
		return ;
	}
	int size;
	fseek(outputfile,0,2);
	size = ftell(outputfile);
	cout<<"Compressed file size(in bytes): "<<size<<endl;
    cout<<"Compression Ratio: "<<(float)size/len;	
}

void getfile(string &filename, string &extension)
{
	int shift;
	int padding;
	int count_of_keys;
	char ch;
	int len,tot,index,complen;
	map<char,int> m;
	map<char,int>::iterator itt;
	string myfile = filename + extension;
	FILE *inputfile = fopen(myfile.c_str() , "r");
	
	if(inputfile == NULL)
	{
		printf("File not found!");
		return ;
	}
	
	string initial = "";
	index = 0;
	
	while( (ch = fgetc(inputfile)) != EOF )
	initial += ch; 
    fclose(inputfile);
    len = initial.length();
  	printf("The file has been compressed successfully!\n\n");
	printf("Uncompressed file size(in bytes): %d\n",len);
	
	for(int i = 0 ; i < len ; i++)
	{
	itt = m.find(initial[i]);
	if(itt == m.end())
	{
		m.insert(make_pair(initial[i],1));
	}
	else
	itt->second++;
    }
    tot=m.size();
    
    int arr[tot];
    int freq[tot];
    shift = (*m.begin()).first;
    if(shift>0)
    shift=0;
    for(itt = m.begin() ; itt != m.end() ; itt++)
    {
    	arr[index] = (itt->first)-shift;
    	freq[index++] = itt->second;
	}
  
    HuffmanCodes(arr, freq, index);
    
    string compcode = "";
    for(int i = 0 ; i < len ; i++)
    compcode += v[initial[i]-shift];
    complen = compcode.length();
    padding = complen%8;
    if(padding != 0)
    {
    padding = (8-padding);
    complen += padding;
    for(int i = 0 ; i < padding ; i++)
    compcode += "0";
    }

	string final = "";
	int final_length = 0; 
	unsigned __int8 ch2;
	for(int i = 0 ; i < complen ; i += 8)
    {
	ch2 = retrieve(i,compcode);
	if(ch2 != 26)
	{
	final += (char)ch2;	
	final_length++;
	}
    else
    {
   	aux.push_back(i/8);
	}
	}
	count_of_keys = mv.size();

    compressedOutput(filename,final,count_of_keys,shift,padding,final_length);
    
    result(filename,initial.length());
}

int main()
{
	string filename; 
	string extension;  
	getfile(filename,extension);
	return 0;
}
