---
layout: post
title: gmtsph-regional
subtitle: "Regional 2D grid data plotting using shell script and GMT5 under the spherical coordinates."
author: Yi Zhang
date: 2018-11-12 23:04:02 -0800
tags: [GMT]
---

* content
{:toc}





## Installation

gmtsph-regional.sh is a shell script that plots a regional grid data under the spherical coordinates using [GMT5](https://gmt.soest.hawaii.edu). The propose of this script is to enable a quick look of a .nc or .grid file, and hopefully ease our every work a little bit. For specified needs, a proper script is still needed. To use it, simply copy the shell scripts *dispOption.sh* and gmtsph-regional.sh into any directory that is included in your $PATH, then reopen the terminal.

## Setup

Please go to the end of *gmtsph-regional.sh*, where you will see lines as
```bash
imgcat $jpgfile #open the output file in terminal, this requires the iTerm.app and imgcat.sh
open $jpgfile #'open' is a command in-build within MacOS
```
Please use either of them as you see fit. Or for Ubuntu users, you may want to use the command `see`. I don't know the corresponding command for other Linux distributions. But you see the gist here.

## Usage

```bash
gmtsph-regional -i<grid-data> [-r<xmin>/<xmax>/<ymin>/<ymax>] [-u<unit>] [-c<cpt-file>] [-a<x-label>,<y-label>] [-t<x-tick>,<y-tick>] [-v<c-tick>] [-l<size>] [-g] [-G<grad-data>] [-b] [-n] [-p]
```

## Options

The only argument you need to input is name of the grid file to get a glance of the file. To get a better look, the following options need to be set.

+ __-i__: Name of the input grid file. Perfectly, you should use a .nc file. However, [Surfer](https://www.goldensoftware.com/products/surfer)'s griding format is also supported since the GMT5. Note that the range of the file should not across the equator. Otherwise, a internal error of the GMT5's projection '-JL' will be triggered. With that been said, you see it's easy to modify the script to plot using other projection by revising the '-J' options of the 'grdimage' command.
+ __-r__: Data range of the input grid file. The script will use the whole data range by default. The use of this option will force a customized range for plotting.
+ __-u__: Unit of the input data. A special case is that if you want to annotate the color bar with 'km' while the unit of the input data is in meter. Then please set the argument of this option as 'km+Uk' to turn on the feature.
+ __-c__: input cpt file used for generating a data specified cpt file. The GMT command 'grd2cpt' is used by default. To use the input cpt file directly, you need to disable the use of 'grd2cpt' by using the __-n__ option.
+ __-a__: Axis's labels separated by commas.
+ __-t__: Set intervals of the axis's labels manually.
+ __-v__: Set interval of the color bar's labels manually.
+ __-l__: Select the page layout from predefined types 'exsmall', 'small', 'middle' and 'large'. Details values of the types could be found within the script.
+ __-b__: Draw coast line.
+ __-g__: Draw an extra layer of shaded topography to get a 3D texture of the output image. The default will be the input grid data.
+ __-G__: Use another grid file to draw the shaded topography. Please note that the range of the file should be equal or larger than the data grid.
+ __-n__: Disable the use of 'grd2cpt'.
+ __-p__: Inverse the color pattern specified by the __-c__ option, invalid if __-n__ option is implemented.

## Example

The following is used to draw the `example.nc`. Names of the output files are taken from the input grid file. Moreover, the script will output a png (no background) and a eps file.

```bash
gmtsph-regional -i exmaple.nc
```

The output should be as below.

![example image for gmtsph-regional](/Toolbox/assets/2018-11/2018-11-13-example-image-for-gmtsph-regional.png)
