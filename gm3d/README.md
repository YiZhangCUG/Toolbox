# 3D forward modeling of gravity and magnetic data under the Cartesian coordinates.

## Program Propose

3D model construction and forward calculation of gravity and magnetic data using the Cartesian coordinates. density or magnetic models are built using elements of rectangular blocks. The program can either build 3D models or forward calculating gravity and magnetic data from a input model file. Some typical source types are supported by the program for fast model construction. File format of the 3D model used in this program is the 2.0 .msh file of the [Gmsh](https://gmsh.info) software.

## Installation

You need to compile the executable file `gm3d` using the `makefile`. Please change the variable `CC` in the `makefile` to the compiler you use.

If you don't have `gcc` installed.
+ For Mac OS users, install `gcc` using [Homebrew](https://brew.sh) by

```bash
brew install gcc
```

+ For Linux users, install `gcc` by

```bash
apt-get install gcc
```

After the `gcc` is installed. Use `make test` the compile the program. Then move or symlink the executable file `gm3d` to any directory included in your $PATH, for instance, `/usr/local/bin`.

## Usage

```bash
Usage: gm3d [-i<input-msh-file>] [-o<output-msh-file>] [-f<output-data-file>] [-d<xs>/<dx>/<xe>/<ys>/<dy>/<ye>/<zs>/<dz>/<ze>] [-m<model-parameters>] [-e<element-data-name>] [-p<observation-file>|<xs>/<dx>/<xe>/<ys>/<dy>/<ye>/<elevation>] [-tVz|Vzx|Vzy|Vzz|DT|DTx|DTy|DTz|Hax|Hay|Za] [-v<I0>/<D0>/<I>/<D>] [-n<noise-mean>/<noise-dev>] [-r] [-h]
```

## Options

+ __-i__: Filename of the input Gmsh(.msh) model file for forward calculation.
+ __-o__: Filename of the output Gmsh(.msh) model file built with given parameters.
+ __-f__: Filename of the output observation file of gravity or magnetic data.
+ __-d__: 3D dimensions of the model space. the suffix 's' means the starting coordinate and 'e' represents the ending coordinate in axial directions. 'dx', 'dy' and 'dz' are step lengths. The default value is 10/20/990/10/20/990/10/20/490. The axial orientation adopted by the program is a right-hand Cartesian system with the z-axis point vertical downward.
+ __-m__: Model file that contains different types of model parameter. See instructions for formats of different model types.
+ __-e__: Element data name of the input/output Gmsh(.msh) file. Note that the name must be around by "".
+ __-p__: Observation locations. You can either initialize the observation points from parameters or a file. Each line of the file contain coordinates y(easting), x(northing) and z(elevation) of an observation point.
+ __-t__: Forward component Vz, Vzx, Vzy or Vzz for gravitational data and DeltaT, DeltaTx, DeltaTy, DeltaTz, Hax, Hay and Za for magnetic data.
+ __-v__: Inclination and declination of the geomagnetic field and magnetization.
+ __-n__: Add noise to the forward calculated data
+ __-r__: Remove model elements with no data in the output Gmsh(.msh) file.
+ __-h__: Display help information.

## Input File Formats

### model parameters

There are four model types currently supported by the program, which are __regular block__, __tilted block__, __sphere__ and __interface__. The program use key words to specify a model's type and associated geometric and physical properties. Therefore, an entrance of model parameters has following parts:

<model_type> <value_type> <physical_property> <geometric_property>

Formats of these key words for different models will be discussed below. Subsequently, the model file are made up by individual entrances of model parameters. Note that any line starts with "#" will be skipped.

#### regular block

The <model_type> for a regular block is simply "regular_block". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a regular block has the format of <xmin>/<xmax>/<ymin>/<ymax>/<zmin>/<zmax> which limits the dimensions of the block.   

regular_block replace|add|erase float-number <xmin>/<xmax>/<ymin>/<ymax>/<zmin>/<zmax>   

#### tilted block   

The <model_type> for a tilted block is simply "tilted_block". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a tilted block has the format of <xmin_z>/<xmax_z>/<ymin_z>/<ymax_z>/<zmin>/<xmin_Z>/<xmax_Z>/<ymin_Z>/<ymax_Z>/<zmax> which limits the dimensions of the block. The suffix z and Z indicate parameters of the shallow and deep facets of the tilted block.   

tilted_block replace|add|erase float-number <xmin_z>/<xmax_z>/<ymin_z>/<ymax_z>/<zmin>/<xmin_Z>/<xmax_Z>/<ymin_Z>/<ymax_Z>/<zmax>   

#### sphere 

The <model_type> for a sphere or ellipsoid is simply "sphere". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a sphere has the format of <x_c>/<y_c>/<z_c>/<x_radius>/<y_radius>/<z_radius> which limits the dimensions of the sphere. The suffix c represents the center coordinates of the model and radius specify radii of the sphere or ellipsoid in corresponding axises.   

sphere replace|add|erase float-number <x_c>/<y_c>/<z_c>/<x_radius>/<y_radius>/<z_radius>   

#### interface

The <model_type> for a interface is simply "interface". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a interface is a filename of a regular grid that are used to specify the undulation of the interface. A line consist of "# range=<xmin>/<dx>/<xmax>/<ymin>/<dy>/<ymax>" must be given in the file head to provide necessary information about the grid data.   

regular_block replace|add|erase float-number grid-filename   

### text file format 

#### Input observation locations

Input locations of the observation (see command -p) specify the coordinates of observation point. Each line of the input text file contains the y (easting), x (northing) and z (elevation) positions of a point. Note that any line starts with '#' will be skipped.   

#### Output observations

Output files of the observation contain positions and forward calculated data and their uncertainties. Each line of the input text file contains the y (easting), x (northing) and z (elevation) positions of a point and forward calculated data (mGal or nT) and associated uncertainties (mGal and nT).   
