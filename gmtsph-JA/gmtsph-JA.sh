#!/bin/bash
# 包含GMT自带脚本文件 其中包含了一些有用的功能 比如获取网格文件的范围
. gmt_shell_functions.sh
# 包含dispOption脚本
. dispOptions
#GMT显示平面数据脚本，输入文件为网格文件，没有包含网格化语句因为网格化过程中的情况多样化，建议在其他脚本中个别添加再调用此脚本
# 初始化参数
data='null'
gridData='null'
unit='m'
color='rainbow -Z'
range='null'
overwriteRange='null'
overwrite=0
labels=("longitude (degree)" "latitude (degree)")
plotgrad=0
nocpt=0
plotsun=0
clon=("0/0/115" "180/0/115")
polyfile='null'
bartick="a" #色标轴标轴间隔 a表示自动
# 从命令行获取参数
while getopts "hi:r:u:c:a:v:G:l:p:gsn" arg
do
	case $arg in
		h)
			dispTitle "${0##*/}" "simple global wide data mapping using GMT script. The template accepts a grid (.nc .grid) file as input and outputs a .png and a .eps file. \
For futher explanations, please look for GMT's manuscripts."
			dispAuthorInfo "Yi Zhang (zhangyi.cugwuhan@gmail.com)"
			dispUsage "${0##*/} -i<grid-data> [-r<xmin>/<xmax>/<ymin>/<ymax>] [-l<clon>/<clat>/<horizon>] [-u<unit>] [-c<cpt-file>] [-a<x-label>,<y-label>] [-v<c-tick>] [-g] [-s] [-G<grad-data>] [-p<polygon-file>]"
			dispOptionShort "-i" "input grid file"
			dispOptionShort "-r" "data range. The template will ditect the input data range automaticly, the use of this option will overwrite the range."
			dispOptionShort "-l" "central longitude and latitude values of the plot, the default value is 0/0/115,180/0/115. "
			dispOptionShort "-u" "data unit. The default is meter. use 'km+Uk' to append '+Uk' option to the psscale command"
			dispOptionShort "-c" "color cpt. The default is 'rainbow -Z' in which '-Z' tells the grd2cpt to make a continuous cpt-file, remove it to disable the function."
			dispOptionShort "-n" "disable the use of grd2cpt."
			dispOptionShort "-s" "plot the day-night terminator. The default is false."
			dispOptionShort "-a" "axis labels. The deafults are x (m) and y (m)."
			dispOptionShort "-v" "intervals of color bar's labels. the script will set the intervals automatically if -v option is not set."
			dispOptionShort "-g" "use grdgradient. The default is false."
			dispOptionShort "-G" "provide a different input grid data for the use of grdgradient. This should be used with the '-g' option at the same time"
			dispOptionShort "-p" "plot polygons via a file."
			exit 0;;
		i)
			data=$OPTARG;;
		u)
			unit=$OPTARG;;
		c)
			color=$OPTARG;;
		n)
			nocpt=1;;
		s)
			plotsun=1;;
		r)
			overwrite=1
			overwriteRange=$OPTARG;;
		l)
			clon=(${OPTARG//,/ });;
		a)
			labels=(${OPTARG//,/ });;
		v)
			bartick=$OPTARG;;
		g)
			plotgrad=1;;
		G)
			gridData=$OPTARG;;
		p)
			polyfile=$OPTARG;;
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
	# 设置绘图参数
	gmt gmtset \
	FONT_ANNOT_PRIMARY=8p,Times-Roman,black \
	MAP_FRAME_PEN=thinnest,black \
	MAP_TICK_LENGTH_PRIMARY=1.5p/1p \
	MAP_TICK_PEN_PRIMARY=thinnest,black \
	MAP_TITLE_OFFSET=6.5p \
	MAP_GRID_PEN_PRIMARY=thinnest,black,-- \
	MAP_GRID_PEN_SECONDARY=thinnest,black,-- \
	FONT_LABEL=8p,Times-Roman,black \
	MAP_FRAME_AXES=WESnZ \
	MAP_LABEL_OFFSET=2.5p

	# 设置色标文件
	if [[ $nocpt == 1 ]]; then
		cptfile=${color}
	else
		gmt grd2cpt ${data} -R${range} -C${color} -D -M > $cptfile
	fi

	if [[ $plotgrad == 1 ]]; then
		gradfile=${data%.*}Grad.nc
		if [[ $gridData == 'null' ]]; then
			gmt grdgradient ${data} -G${gradfile} -Nt -A0/45
		else
			gmt grdgradient ${gridData} -G${gradfile} -Nt -A0/45
		fi
		gradPlotCommand=-I${gradfile}
	else
		gradPlotCommand=""
	fi

	gmt grdimage ${data} -R${range} -C${cptfile} $gradPlotCommand -Bxa30g30+l"${labels[0]}" -Bpya30g30+l"${labels[1]}" -JA${clon[0]}/1.5i -K -P > $psfile
	if [[ $plotsun == 1 ]]; then
		gmt pssolar -R${range} -JA${clon[0]}/1.5i -Td+d2016-02-09T16:00:00 -Gblack@75 -K -O >> $psfile
	fi
	if [[ $polyfile != 'null' ]]; then
		gmt psxy $polyfile -JA${clon[0]}/1.5i -R${range} -L -K -O >> $psfile
	fi
	gmt grdimage ${data} -R${range} -C${cptfile} $gradPlotCommand -Bxa30g30+l"${labels[0]}" -Bpya30g30+l"${labels[1]}" -JA${clon[1]}/1.5i -X1.7i -K -O >> $psfile
	if [[ $plotsun == 1 ]]; then
		gmt pssolar -R${range} -JA${clon[1]}/1.5i -Td+d2016-02-09T16:00:00 -Gblack@75 -K -O >> $psfile
	fi
	if [[ $polyfile != 'null' ]]; then
		gmt psxy $polyfile -JA${clon[1]}/1.5i -R${range} -L -K -O >> $psfile
	fi

	#-C${cptfile}+Uk 使用km（色标单位除1000
	#如果unit等于km则在cptfile后面添加+Uk
	if [[ ${unit} == 'km+Uk' ]]; then
		gmt psscale -I -Dx0.85i/-0.2i+w1.5i/0.05i+h -C${cptfile}+Uk -Bx${bartick} -By+l"km" -X-1.7i -O >> $psfile
	else
		gmt psscale -I -Dx0.85i/-0.2i+w1.5i/0.05i+h -C${cptfile} -Bx${bartick} -By+l${unit} -X-1.7i -O >> $psfile
	fi
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