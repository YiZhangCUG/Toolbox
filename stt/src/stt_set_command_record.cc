#include "stt_class.h"

int SttGenerator::set_command_record(int argv_num,char** argvs)
{
	command_record_ = argvs[0];
	for (int i = 1; i < argv_num; i++)
	{
		command_record_ += " ";
		command_record_ += argvs[i];
	}
	return 0;
}