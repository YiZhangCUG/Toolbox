#include "stt_class.h"
#include "progress_bar.h"

int SttGenerator::Routine(char input_options[][1024]){
	// set values of tree depths, terminate the program if failed
	if (set_tree_depth(input_options[0])) return -1;
	// set values of pole_radius_ and equator_radius_, terminate the program if failed
	if (set_pole_equator_radius(input_options[1])) return -1;
	// set orientation of the base icosahedron, terminate the program if failed
	if (set_icosahedron_orient(input_options[2])) return -1;
	// get extra-control information for control points, lines, polygons and circles. Terminate the program if failed
	// get outline and hole polygons
	if (GetControlPoint(input_options[7])) return -1;
	if (GetControlCircle(input_options[10])) return -1;
	if (GetControlLine(input_options[8],array_control_line_)) return -1;
	if (GetControlLine(input_options[9],array_control_polygon_)) return -1;
	if (GetControlLine(input_options[11],array_outline_polygon_)) return -1;
	if (GetControlLine(input_options[12],array_hole_polygon_)) return -1;

	// initial spaces for tree root
	for (int i = 0; i < 20; i++){
		forest_[i] = new QuadTree;
		forest_[i]->root = new QuadTreeNode;
	}
	// initial the base icosahedron
	// the radius of the base icosahedron is set to DefaultR
	InitialIcosahedron(DefaultR,icosahedron_orient_);
	// add vertices of the base icosahedron to array_stt_vert_, map_id_vertex_ and map_str_vertex_
	for (int i = 0; i < 12; i++){
		array_stt_vert_.push_back(base_icosahedron_.vert[i]);
		map_id_vertex_[base_icosahedron_.vert[i].id] = base_icosahedron_.vert[i];
		map_str_vertex_[GetStringIndex(base_icosahedron_.vert[i])] = base_icosahedron_.vert[i];
	}

	ProgressBar *bar = new ProgressBar(20,"Initialize STT");
	for (int i = 0; i < 20; i++){
		bar->Progressed(i);
		// initialize the tree index starts from 50 to avoid possible repetition of vertex's index
		CreateTree(i+50,base_icosahedron_.tri[i].ids[0],base_icosahedron_.tri[i].ids[1],base_icosahedron_.tri[i].ids[2],forest_[i]);
	}

	// close surface after construction
	CloseSurface(&forest_[0]);

	// if outline polygon exists
	if (!array_outline_polygon_.empty()){
		ProgressBar *bar3 = new ProgressBar(20,"Cut outline");
		for (int i = 0; i < 20; i++){
			bar3->Progressed(i);
			CutOutline(&(forest_[i]->root));
		}
	}

	// if hole polygon exists
	if (!array_hole_polygon_.empty())
	{
		ProgressBar *bar4 = new ProgressBar(20,"Cut holes");
		for (int i = 0; i < 20; i++){
			bar4->Progressed(i);
			CutHole(&(forest_[i]->root));
		}
	}

	// return leafs and prepare for outputs
	if (!array_out_tri_pointer_.empty()) array_out_tri_pointer_.clear();
	for (int i = 0; i < 20; i++)
		ReturnLeaf(&(forest_[i]->root));

	if (!OutputMshFile(input_options[3],pole_radius_,equator_radius_))
		clog << "file saved: " << input_options[3] << endl;
	if (!OutputVertexLocation(input_options[4],pole_radius_,equator_radius_))
		clog << "file saved: " << input_options[4] << endl;
	if (!OutputTriangleCenterLocation(input_options[5],pole_radius_,equator_radius_))
		clog << "file saved: " << input_options[5] << endl;
	if (strcmp(input_options[6],"NULL")){
		SortNeighbor(array_out_tri_pointer_);
		if (!OutputNeighbor(input_options[6]))
			clog << "file saved: " << input_options[6] << endl;
	}

	for (int i = 0; i < 20; i++){
		DeleteTree(&(forest_[i]->root));
		if (forest_[i] != NULL) delete forest_[i];
	}
	return 0;
}