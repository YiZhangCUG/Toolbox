# Model construction and forward calculation of gravity and magnetic data using the Cartesian coordinates

## Program Propose



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
Usage: gm3d [-i<input-msh-name>] [-o<output-msh-name>] [-f<output-grav-file>] [-d<xs>/<dx>/<xe>/<ys>/<dy>/<ye>/<zs>/<dz>/<ze>] [-m<model-file>] [-e<element-data-name>] [-p<observation-file>|<xs>/<dx>/<xe>/<ys>/<dy>/<ye>/<elevation>] [-tg|gx|gy|gz] [-n<noise-mean>/<noise-dev>] [-r] [-h]
```

## Options

+ __-i__: Filename of the input Gmsh(.msh) file. You need to specify which element data is used for forward modeling using the -e option.
+ __-o__: Filename of the output Gmsh(.msh) file. 
+ __-f__: Filename of the output observation file.
+ __-d__: 3D dimensions of the model space. 's' means the starting coordinate and 'e' represents the ending coordinate. 'dx' is the step length. The default value is 10/20/990/10/20/990/10/20/490.
+ __-m__: Model file that contains different types of model parameter.
+ __-e__: Element data name of the input/output Gmsh(.msh) file.
+ __-p__: Observation locations
+ __-t__: Forward component Vz, Vzx, Vzy or Vzz.
+ __-n__: Add noise to the forward calculated data.
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

The <model_type> for a tilted block is simply "tilted_block". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a tilted block has the format of <xmin_z>/<xmax_z>/<ymin_z>/<ymax_z>/<zmin>/<xmin_Z>/<xmax_Z>/<ymin_Z>/<ymax_Z>/<zmax> which limits the dimensions of the block. The suffix \_z and \_Z indicate parameters of the shallow and deep facets of the tilted block.

tilted_block replace|add|erase float-number <xmin_z>/<xmax_z>/<ymin_z>/<ymax_z>/<zmin>/<xmin_Z>/<xmax_Z>/<ymin_Z>/<ymax_Z>/<zmax>

#### sphere

The <model_type> for a sphere or ellipsoid is simply "sphere". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a sphere has the format of <x_c>/<y_c>/<z_c>/<x_radius>/<y_radius>/<z_radius> which limits the dimensions of the sphere. The suffix \_c represents the center coordinates of the model and \_radius specify radii of the sphere or ellipsoid in corresponding axises.

sphere replace|add|erase float-number <x_c>/<y_c>/<z_c>/<x_radius>/<y_radius>/<z_radius>

#### interface

The <model_type> for a interface is simply "interface". You can choose either "replace", "add" or "erase" for the block's value type. The "replace" type will replace the physical values of model cells within the block's dimensions. Correspondingly, the "add" and "erase" type will add or erase values of the model cells. The <physical_property> is a float value that represents the density or magnetism within the block's dimension. The <geometric_property> of a interface is a filename of a regular grid that are used to specify the undulation of the interface. A line consist of "# range=<xmin>/<dx>/<xmax>/<ymin>/<dy>/<ymax>" must be given in the file head to provide necessary information about the grid data.

regular_block replace|add|erase float-number grid-filename

### text file format 



## Examples


