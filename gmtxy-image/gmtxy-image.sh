#!/bin/bash
# 包含GMT自带脚本文件 其中包含了一些有用的功能 比如获取网格文件的范围
. gmt_shell_functions.sh
# 包含dispOptions
. dispOptions.sh
#GMT显示平面数据脚本，输入文件为网格文件，没有包含网格化语句因为网格化过程中的情况多样化，建议在其他脚本中个别添加再调用此脚本
# 初始化参数
data='null'
gridData='null'
unit='m'
color='rainbow'
range='null'
overwriteRange='null'
overwrite=0
labels=("x (m)" "y (m)")
plotgrad=0
axistick=("a" "a") #横纵坐标轴间隔 a表示自动
bartick="a" #色标轴标轴间隔 a表示自动
# 从命令行获取参数
while getopts "hi:r:u:c:a:G:t:v:g" arg
do
	case $arg in
		h)
			dispTitle "${0##*/}" "Simple grid file mapping using GMT script under x-y plane. The template accepts a grid (.nc .grid) file as input and outputs a .png and a .eps file. \
For futher explanations, please look for GMT's manuscripts."
			dispAuthorInfo "Yi Zhang (zhangyi.cugwuhan@gmail.com)" 
			dispUsage "${0##*/} -i<grid-data> [-r<xmin>/<xmax>/<ymin>/<ymax>] [-u<unit>] [-c<cpt-file>] [-a<x-label>,<y-label>] [-g]"
			dispOptionShort "-i" "Input grid file."
			dispOptionShort "-r" "Data range. The template will ditect the input data range automaticly, the use of this option will overwrite the range."
			dispOptionShort "-u" "Data unit. The default is meter."
			dispOptionShort "-c" "Color cpt. The default is rainbow."
			dispOptionShort "-t" "Intervals of axis's labels. the script will set the intervals automatically if -t option is not set."
			dispOptionShort "-v" "Intervals of color bar's labels. the script will set the intervals automatically if -v option is not set."
			dispOptionShort "-a" "Axis labels separated by semicolons. The deafults are x (m) and y (m)."
			dispOptionShort "-g" "Use grdgradient. The default is false."
			dispOptionShort "-G" "Provide a different input grid data for the use of grdgradient. This should be used with the '-g' option at the same time."
			exit 0;;
		i)
			data=$OPTARG;;
		u)
			unit=$OPTARG;;
		c)
			color=$OPTARG;;
		t)
			axistick=(${OPTARG//,/ });;
		v)
			bartick=$OPTARG;;
		r)
			overwrite=1
			overwriteRange=$OPTARG;;
		a)
			OLD_IFS="${IFS}"
			IFS=";"
			labels=(${OPTARG})
			IFS="${OLD_IFS}";;
		g)
			plotgrad=1;;
		G)
			gridData=$OPTARG;;
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
	#根据横纵坐标的范围计算图片的长和高 我们默认图片宽度为1.5i
	range_coor=(${range//// })
	pic_height=`echo "scale=4; 2.0 * (${range_coor[3]}-${range_coor[2]})/(${range_coor[1]}-${range_coor[0]})"|bc`
	# 设置绘图参数
	gmt gmtset \
	FONT_ANNOT_PRIMARY=7.5p,Times-Roman,black \
	MAP_FRAME_PEN=thinnest,black \
	MAP_GRID_PEN_PRIMARY=thinnest,black \
	MAP_TICK_PEN_PRIMARY=thinnest,black \
	MAP_TICK_LENGTH_PRIMARY=1p/0.5p \
	MAP_TITLE_OFFSET=7.5p \
	MAP_GRID_CROSS_SIZE_PRIMARY=2p \
	FONT_LABEL=7.5p,Times-Roman,black \
	MAP_FRAME_AXES=WeSnZ \
	MAP_LABEL_OFFSET=2.5p \
	MAP_ANNOT_OFFSET_PRIMARY=2.5p

	gmt grd2cpt ${data} -C${color} -R${range} -Z -D > $cptfile
	if [[ $plotgrad == 1 ]]; then
		gradfile=${data%.*}Grad.nc
		if [[ $gridData == 'null' ]]; then
			gmt grdgradient ${data} -G${gradfile} -Nt -A0/45
		else
			gmt grdgradient ${gridData} -G${gradfile} -Nt -A0/45
		fi
		gmt grdimage ${data} -R${range} -C${cptfile} -I${gradfile} -Bx${axistick[0]}g+l"${labels[0]}" -By${axistick[1]}g+l"${labels[1]}" -JX2.0i/${pic_height}i -K -P > $psfile
	else
		gmt grdimage ${data} -R${range} -C${cptfile} -Bx${axistick[0]}g+l"${labels[0]}" -By${axistick[1]}g+l"${labels[1]}" -JX2.0i/${pic_height}i -K -P > $psfile
	fi

	#-C${cptfile}+Uk 使用km（色标单位除1000
	#如果unit等于km则在cptfile后面添加+Uk
	if [[ ${unit} == 'km+Uk' ]]; then
		gmt psscale -Dx0.24i/-0.32i+w1.5i/0.05i+h -C${cptfile}+Uk -Bx${bartick} -By+l${unit} -O >> $psfile
	else
		gmt psscale -Dx0.24i/-0.32i+w1.5i/0.05i+h -C${cptfile} -Bx${bartick} -By+l${unit} -O >> $psfile
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