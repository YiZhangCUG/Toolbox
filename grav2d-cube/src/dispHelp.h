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

class dispHelp
{
public:
	dispHelp(){
		front_space = 0;
		back_space = 10;
		ex_name = "Execuable";
		version = "0.0.1";
		descript = "Brief information about this command.";
		author = "Author's information.";
	}
	~dispHelp(){}
	void addHeadInfo(string,string,string,string);
	void addUsage(string);
	void addOption(string,string,string);
	void addOptionSec(string,int);
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

void dispHelp::addHeadInfo(string s1,string s2,string s3,string s4)
{
	ex_name = s1; version = s2; descript = s3; author = s4;
	return;
}

void dispHelp::addUsage(string usg)
{
	usages.push_back(usg);
	return;
}

void dispHelp::addOption(string msg,string sflag,string lflag = "")
{
	option tmp_option;
	tmp_option.message = msg; tmp_option.flag_s = sflag; tmp_option.flag_l = lflag;
	options.push_back(tmp_option);
	return;
}

void dispHelp::addOptionSec(string msg,int index = -1)
{
	if (index < 0)
	{
		options.back().sec_message.push_back(msg);
	}
	else options[index].sec_message.push_back(msg);
	return;
}

void dispHelp::addExample(string ex)
{
	examples.push_back(ex);
	return;
}

void dispHelp::changeLayerOut(int left,int right)
{
	front_space = left; back_space = right;
	return;
}

void dispHelp::show()
{
	int line_length;
	string segment,full_message;
	stringstream ss_message;
	//获取终端窗口的行列数
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	//显示头信息
	full_message = ex_name + " " + version + " - " + descript;
	ss_message.clear(); ss_message.str(full_message);

	line_length = front_space + back_space;
	while(ss_message >> segment)
	{
		if ((line_length+segment.length()+1) <= w.ws_col)
		{
			if (line_length == (front_space + back_space))
			{
				for (int i = 0; i < front_space; i++) clog << " ";
				clog << segment << " ";
				line_length += (segment.length()+1);
			}
			else
			{
				clog << segment << " ";
				line_length += (segment.length()+1);
			}
		}
		else
		{
			clog << endl;
			for (int i = 0; i < front_space+4; i++) clog << " ";
			clog << segment << " ";
			line_length = (segment.length()+9+front_space+back_space);
		}
	}
	clog << endl;

	ss_message.clear(); ss_message.str(author);
	line_length = front_space + back_space;;
	while(ss_message >> segment)
	{
		if ((line_length+segment.length()+1) <= w.ws_col)
		{
			if (line_length == (front_space + back_space))
			{
				for (int i = 0; i < front_space; i++) clog << " ";
				clog << "Author: " << segment << " ";
				line_length += (segment.length()+9);
			}
			else
			{
				clog << segment << " ";
				line_length += (segment.length()+1);
			}
		}
		else
		{
			clog << endl;
			for (int i = 0; i < front_space+4; i++) clog << " ";
			clog << segment << " ";
			line_length = (segment.length()+9+front_space+back_space);
		}
	}
	clog << endl;

	if (!usages.empty())
	{
		for (int i = 0; i < front_space; i++) clog << " ";
		clog << "Usage:" << endl;
		for (int i = 0; i < usages.size(); i++)
		{
			ss_message.clear(); ss_message.str(usages[i]);

			line_length = front_space + back_space + 4;
			while(ss_message >> segment)
			{
				if ((line_length+segment.length()+1) <= w.ws_col)
				{
					if (line_length == (front_space+back_space+4))
					{
						for (int i = 0; i < front_space+4; i++) clog << " ";
						clog << segment << " ";
						line_length += (segment.length()+1);
					}
					else
					{
						clog << segment << " ";
						line_length += (segment.length()+1);
					}
				}
				else
				{
					clog << endl;
					for (int i = 0; i < front_space+9; i++) clog << " ";
					clog << segment << " ";
					line_length = (segment.length()+10+front_space+back_space);
				}
			}
			clog << endl;
		}
	}

	if (!options.empty())
	{
		for (int i = 0; i < front_space; i++) clog << " ";
		clog << "Options:" << endl;
		for (int i = 0; i < options.size(); i++)
		{
			if (options[i].flag_l == "")
			{
				full_message = options[i].flag_s+" : "+options[i].message;
				ss_message.clear(); ss_message.str(full_message);

				line_length = front_space + back_space + 4;
				while(ss_message >> segment)
				{
					if ((line_length+segment.length()+1) <= w.ws_col)
					{
						if (line_length == (front_space+back_space+4))
						{
							for (int i = 0; i < front_space+4; i++) clog << " ";
							clog << segment << " ";
							line_length += (segment.length()+1);
						}
						else
						{
							clog << segment << " ";
							line_length += (segment.length()+1);
						}
					}
					else
					{
						clog << endl;
						for (int i = 0; i < front_space+9; i++) clog << " ";
						clog << segment << " ";
						line_length = (segment.length()+10+front_space+back_space);
					}
				}
				clog << endl;

				if (!options[i].sec_message.empty())
				{
					for (int j = 0; j < options[i].sec_message.size(); j++)
					{
						ss_message.clear(); ss_message.str(options[i].sec_message[j]);

						line_length = front_space + back_space + 9;
						while(ss_message >> segment)
						{
							if ((line_length+segment.length()+1) <= w.ws_col)
							{
								if (line_length == (front_space+back_space+9))
								{
									for (int i = 0; i < front_space+9; i++) clog << " ";
									clog << segment << " ";
									line_length += (segment.length()+1);
								}
								else
								{
									clog << segment << " ";
									line_length += (segment.length()+1);
								}
							}
							else
							{
								clog << endl;
								for (int i = 0; i < front_space+13; i++) clog << " ";
								clog << segment << " ";
								line_length = (segment.length()+14+front_space+back_space);
							}
						}
						clog << endl;
					}
				}
			}
			else
			{
				full_message = options[i].flag_s+" | "+options[i].flag_l+" : "+options[i].message;
				ss_message.clear(); ss_message.str(full_message);

				line_length = front_space + back_space + 4;
				while(ss_message >> segment)
				{
					if ((line_length+segment.length()+1) <= w.ws_col)
					{
						if (line_length == (front_space+back_space+4))
						{
							for (int i = 0; i < front_space+4; i++) clog << " ";
							clog << segment << " ";
							line_length += (segment.length()+1);
						}
						else
						{
							clog << segment << " ";
							line_length += (segment.length()+1);
						}
					}
					else
					{
						clog << endl;
						for (int i = 0; i < front_space+9; i++) clog << " ";
						clog << segment << " ";
						line_length = (segment.length()+10+front_space+back_space);
					}
				}
				clog << endl;

				if (!options[i].sec_message.empty())
				{
					for (int j = 0; j < options[i].sec_message.size(); j++)
					{
						ss_message.clear(); ss_message.str(options[i].sec_message[j]);

						line_length = front_space + back_space + 9;
						while(ss_message >> segment)
						{
							if ((line_length+segment.length()+1) <= w.ws_col)
							{
								if (line_length == (front_space+back_space+9))
								{
									for (int i = 0; i < front_space+9; i++) clog << " ";
									clog << segment << " ";
									line_length += (segment.length()+1);
								}
								else
								{
									clog << segment << " ";
									line_length += (segment.length()+1);
								}
							}
							else
							{
								clog << endl;
								for (int i = 0; i < front_space+13; i++) clog << " ";
								clog << segment << " ";
								line_length = (segment.length()+14+front_space+back_space);
							}
						}
						clog << endl;
					}
				}
			}
		}
	}

	if (!examples.empty())
	{
		for (int i = 0; i < front_space; i++) clog << " ";
		clog << "Examples:" << endl;
		for (int i = 0; i < examples.size(); i++)
		{
			ss_message.clear(); ss_message.str(examples[i]);

			line_length = front_space + back_space + 4;
			while(ss_message >> segment)
			{
				if ((line_length+segment.length()+1) <= w.ws_col)
				{
					if (line_length == (front_space+back_space+4))
					{
						for (int i = 0; i < front_space+4; i++) clog << " ";
						clog << segment << " ";
						line_length += (segment.length()+1);
					}
					else
					{
						clog << segment << " ";
						line_length += (segment.length()+1);
					}
				}
				else
				{
					clog << endl;
					for (int i = 0; i < front_space+9; i++) clog << " ";
					clog << segment << " ";
					line_length = (segment.length()+10+front_space+back_space);
				}
			}
			clog << endl;
		}
	}
	return;
}
#endif