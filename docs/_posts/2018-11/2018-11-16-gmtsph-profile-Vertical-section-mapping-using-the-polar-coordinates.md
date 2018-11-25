---
layout: post
title: gmtsph-profile
subtitle: "Vertical section mapping using the polar coordinates."
author: Yi Zhang
date: 2018-11-16 20:17:51 -0800
tags: [GMT]
---

* content
{:toc}




## Installation

**gmtsph-profile** is a shell script that plots a 2D grid data using the polar coordinates using [GMT5](https://gmt.soest.hawaii.edu). To use it, simply copy the shell scripts *dispOption.sh* and *gmtsph-profile.sh* into any directory you want, and symlink them to a directory that is included in your $PATH, then reopen the terminal.

## Setup

Please go to the end of *gmtsph-profile.sh*, where you will see lines as
```bash
imgcat $jpgfile #open the output file in terminal, this requires the iTerm.app and imgcat.sh
open $jpgfile #'open' is a command in-build within MacOS
```
Please use either of them as you see fit. Or for Ubuntu users, you may want to use the command `see`. I don't know the corresponding command for other Linux distributions. But you see the gist here.

## Usage

```bash
gmtsph-profile -i<grid-data> [-r<start-deg>/<end-deg>/<r>/<R>] [-u<unit>] [-c<cpt-file>] [-a<x-label>;<y-label>] [-f<offset>] [-h]
```

## Options

The only argument you need to input is name of the grid file to get a glance of the file. To get a better look, the following options need to be set.

+ __-i__: Name of the input grid file. Perfectly, you should use a .nc file. However, [Surfer](https://www.goldensoftware.com/products/surfer)'s griding format is also supported since the GMT5.
+ __-r__: Data range of the input grid file. The script will use the whole data range by default. The use of this option will force a customized range for plotting.
+ __-u__: Unit of the input data.
+ __-c__: input cpt file used for generating a data specified cpt file. The GMT command 'grd2cpt' is used by default.
+ __-a__: Axis's labels separated by semicolons.
+ __-f__: Offset angle of the profile. the default will use mean value of the x-range. This will put the averaged x value in the center of the plotting.
+ __-h__: Show help information.

## Example

The following is used to draw the `example.nc`. Names of the output files are taken from the input grid file. Moreover, the script will output a png (no background) and a eps file.

```bash
gmtsph-profile -i exmaple.nc
```

The output should be as below.

![example image for gmtsph-profile](/Toolbox/assets/2018-11/example-image-for-gmtsph-profile.png)
