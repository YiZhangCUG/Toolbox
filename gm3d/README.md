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

After the `gcc` is installed. Use `make` the compile the program. Then move or symlink the executable file `gm3d` to any directory included in your $PATH, for instance, `/usr/local/bin`.

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



### text file format 



## Examples


