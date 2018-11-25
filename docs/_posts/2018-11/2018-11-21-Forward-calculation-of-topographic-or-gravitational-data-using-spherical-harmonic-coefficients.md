---
layout: post
title: sh2xyz
subtitle: "Forward calculation of topographic or gravitational data using spherical harmonic coefficients."
date: 2018-11-21 18:51:30 -0800
tags: [Forward modeling, Spherical harmonic]
---

* content
{:toc}




## Installation

You need to compile the executable file `sh2xyz` using the `makefile`. Please change the variable `CC` in the `makefile` to the compiler you use. Parallel computing is enable using the `-fopenmp` flag, delete it if you don't want the parallel computing.

```makefile
CC = g++-8 -fopenmp
```

If you don't have `gcc` installed.
+ For Mac OS users, install `gcc` using [Homebrew](https://brew.sh) by

```bash
brew install gcc
```

+ For Linux users, install `gcc` by

```bash
apt-get install gcc
```

After the `gcc` is installed. Use `make` the compile the program. Then move or symlink the executable file `sh2xyz` to any directory included in your $PATH, for instance, `/usr/local/bin`.

## Usage

```bash
sph2xyz -t<table> -r<lonmin>/<lonmax>/<latmin>/<latmax>[/<altitude>] -i<dlon>/<dlat> -d<type> -f<ln>/<lm>/<hn>/<hm> [-p<loc-file>] [-s<refr>/<refR>] [-g<GM>/<R>] [-n<type>] [-c<col1>,<col2>,<col3>,<col4>] [-z<alti-file>[+d<col1>,<col2>,<col3>]] [-h] > out-file
```

## Options

+ __-t__: Input filename of the coefficients.
+ __-r__: Initialize the range of observation points. Altitude of the points is optional (will be 0 if no set).
+ __-i__: Intervals of the observation points.
+ __-d__: Types of forward calculation.
+ __-f__: Starting and ending order/degree that will be used for calculation.
+ __-p__: Interpolate the forward calculated data to spherical locations read from the file.
+ __-s__: short and long semi-radius of the reference coordinate system.
+ __-g__: Model parameters of gravity models. This option must be set if we are calculating gravitational data.
+ __-n__: Normalization type of the Associated Legendre Function.
+ __-c__: Select input columns of the coefficients.
+ __-z__: Initialize the observation's altitudes from the a input file.
+ __-h__: Show help information.

## Examples

To forward calculating an `example.SHcoeffs` which maximal degree/order are both 360.

```bash
sh2xyz -t example.SHcoeffs -r -180/180/-90/90/0 -i 0.5/0.5 -f 0/0/360/360 -s 10000/10000 > example.txt
```

The calculated data is shown as below:
![sh2xyz example](/Toolbox/assets/2018-11/sh2xyz-example.png)
