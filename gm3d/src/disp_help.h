#ifndef _DISPHELP_H
#define _DISPHELP_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iomanip>
#include <sys/ioctl.h>
#include "vector"

using namespace std;

typedef vector<string> strArray;

struct option
{
	string flag_s,flag_l;
	string message;
	strArray sec_message;
	option()
	{
		flag_s = flag_l = message = "";
	}
};
typedef vector<option> opArray;

class DispHelp
{
public:
	DispHelp(){
		front_space = 0;
		back_space = 10;
		ex_name = "Execuable";
		version = "0.0.1";
		descript = "Brief information about this command.";
		author = "Author's information.";
	}
	~DispHelp(){}
	void addHeadInfo(string,string,string,string);
	void addUsage(string);
	void addOption(string,string,string lflag = "");
	void addOptionSec(string,int index = -1);
	void addExample(string);
	void changeLayerOut(int,int);
	void show();
private:
	string ex_name,version,descript,author;
	int front_space,back_space;
	opArray options;
	strArray examples;
	strArray usages;
};
#endif