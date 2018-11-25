#!/bin/bash
# 包含GMT自带脚本文件 其中包含了一些有用的功能 比如获取网格文件的范围
. gmt_shell_functions.sh
. dispOptions.sh
#GMT显示平面数据脚本，输入文件为网格文件，没有包含网格化语句因为网格化过程中的情况多样化，建议在其他脚本中个别添加再调用此脚本
# 初始化参数
data='null'
unit='m'
color='rainbow'
range='null'
overwriteRange='null'
overwrite=0
labels=("Centeral angle (degree)" "Depth (m)")
overwriteOff=0
offangle=0
# 从命令行获取参数
while getopts "hi:r:u:c:a:f:" arg
do
	case $arg in
		h)
			dispTitle "${0##*/}" "Simple grid file mapping using GMT script under polor coordinate. The template accepts a grid (.nc .grid) file as input and outputs a .png and a .eps file. \
For futher explanations, please look for GMT's manuscripts." 
			dispAuthorInfo "Yi Zhang (zhangyi.cugwuhan@gmail.com)"
			dispUsage "${0##*/} -i<grid-data> [-r<xmin>/<xmax>/<ymin>/<ymax>] [-u<unit>] [-c<cpt-file>] [-a<x-label>;<y-label>] [-f<off-set>]"
			dispOptionShort "-i" "Input grid file"
			dispOptionShort "-r" "Data range. The template will ditect the input data range automaticly, the use of this option will overwrite the range."
			dispOptionShort "-u" "Data unit. The default is meter."
			dispOptionShort "-c" "Color cpt. The default is rainbow."
			dispOptionShort "-a" "Axis labels. The deafults are central angle (degree) and depth (m)."
			dispOptionShort "-f" "Offset angle of the profile. the default will use mean value of the x-range."
			exit 0;;
		i)
			data=$OPTARG;;
		u)
			unit=$OPTARG;;
		c)
			color=$OPTARG;;
		r)
			overwrite=1
			overwriteRange=$OPTARG;;
		a)
			OLD_IFS="${IFS}"
			IFS=";"
			labels=(${OPTARG})
			IFS="${OLD_IFS}";;
		f)
			overwriteOff=1
			offangle=$OPTARG;;
		?)
			printf "error: unknow argument\nuse -h option to see help information\n"
			exit 1;;
	esac
done
# 进行必要的参数检查
if [[ $data == "null" ]]; then
	printf "error: no input file name\nuse -h option to see help information\n"
	exit 1
else
	# 初始化临时文件名
	cptfile=user.cpt
	psfile=${data%.*}.ps
	jpgfile=${data%.*}.png
	# 获取网格范围
	if [[ $overwrite == 1 ]]; then
		range=${overwriteRange}
	else
		range=$(gmt_get_gridregion ${data})
	fi
	# 设置偏移范围
	if [[ $overwriteOff == 0 ]]; then
		range_sp=(${range//// })
		offangle=$(echo "${range_sp[0]} * 0.5 + ${range_sp[1]} * 0.5"|bc)
	fi
	# 设置绘图参数
	gmt gmtset \
	COLOR_NAN=grey \
	FONT_ANNOT_PRIMARY=7p,Times-Roman,black \
	MAP_FRAME_PEN=thin,black \
	MAP_TICK_LENGTH_PRIMARY=2p/1p \
	MAP_TITLE_OFFSET=7p \
	MAP_GRID_CROSS_SIZE_PRIMARY=2p \
	FONT_LABEL=7p,Times-Roman,black \
	MAP_FRAME_AXES=wEsNZ \
	COLOR_NAN=white \
	MAP_LABEL_OFFSET=4p

	gmt grd2cpt ${data} -C${color} -Z -D -M > $cptfile
	gmt grdimage ${data} -R${range} -C${cptfile} -Bxag+l"${labels[0]}" -Byag+l"${labels[1]}" -JPa3i/${offangle}z -K -Y1i -P > $psfile
	gmt psscale -Dx0.4i/-0.25i+w2.2i/0.1i+h -C${cptfile} -Bxa -By+l${unit} -O >> $psfile
	gmt psconvert $psfile -A -TEG -E300
	# 删除临时文件 使用linux终端rm命令
	rm $cptfile $psfile gmt.history gmt.conf
	if [[ $plotgrad == 1 ]]; then
		rm $gradfile
	fi
	# 在终端显示图像 此命令需要imgcat.sh脚本和iTerm终端
	#imgcat $jpgfile
	# 打开图片文件 此命令使用MacOS终端open命令
	open $jpgfile
fi