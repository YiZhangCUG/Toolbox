#include "stt_class.h"

int SttGenerator::OutputNeighbor(char* filename)
{
	time_t now = time(0);
	char* dt = ctime(&now);
	ofstream outfile;
	Int2IntMap map_out_triangle_id_;

	if (!strcmp(filename,"NULL")) return 0;
	if (OpenOutfile(outfile,filename)) return -1;

	for (int i = 0; i < array_out_tri_pointer_.size(); i++){
		map_out_triangle_id_[array_out_tri_pointer_[i]->id] = i;
	}

	outfile << "# This file is created by stt-generator.ex on " << dt;
	outfile << "# Commands: " << command_record_ << endl;
	outfile << "# Triangle number: "<< array_out_tri_pointer_.size() << endl;
	outfile << "# triangle_id neighbor_id1 neighbor_id2 neighbor_id3" << endl;
	for (int i = 0; i < array_out_tri_pointer_.size(); i++){
		outfile << i;
		for (int j = 0; j < 3; j++){
			if (array_out_tri_pointer_[i]->neighbor[j] != NULL){
				outfile << " " << map_out_triangle_id_[array_out_tri_pointer_[i]->neighbor[j]->id];
			}
		}
		outfile << endl;
	}
	outfile.close();
	return 0;
}