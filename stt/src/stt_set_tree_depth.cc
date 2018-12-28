#include "stt_class.h"

int SttGenerator::set_tree_depth(char* input_parameter){
	// if input_parameter is NULL or empty, set tree_depth_ and max_depth_ to zero
	if (!strcmp(input_parameter,"NULL") || !strcmp(input_parameter,"")){
		tree_depth_ = max_depth_ = 0;
	}
	// try to read the input parameter as tree_depth_/max_depth_ along with some boundary checks
	else if (2!=sscanf(input_parameter,"%d/%d",&tree_depth_,&max_depth_) 
		|| tree_depth_ > max_depth_ || max_depth_ < 0){
		cerr << BOLDRED << "Error ==> " << RESET << "fail to initialize the minimal and maximal quad-tree depths: " << input_parameter << endl;
		return -1;
	}
	return 0;
}