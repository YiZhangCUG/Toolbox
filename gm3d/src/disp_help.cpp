#include "disp_help.h"

void DispHelp::addHeadInfo(string s1,string s2,string s3,string s4)
{
	ex_name = s1; version = s2; descript = s3; author = s4;
	return;
}

void DispHelp::addUsage(string usg)
{
	usages.push_back(usg);
	return;
}

void DispHelp::addOption(string msg,string sflag,string lflag = "")
{
	option tmp_option;
	tmp_option.message = msg; tmp_option.flag_s = sflag; tmp_option.flag_l = lflag;
	options.push_back(tmp_option);
	return;
}

void DispHelp::addOptionSec(string msg,int index = -1)
{
	if (index < 0)
	{
		options.back().sec_message.push_back(msg);
	}
	else options[index].sec_message.push_back(msg);
	return;
}

void DispHelp::addExample(string ex)
{
	examples.push_back(ex);
	return;
}

void DispHelp::changeLayerOut(int left,int right)
{
	front_space = left; back_space = right;
	return;
}

void DispHelp::show()
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