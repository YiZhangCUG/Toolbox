#!/bin/bash
# 包含GMT自带脚本文件 其中包含了一些有用的功能 比如获取网格文件的范围
. gmt_shell_functions.sh
#include dispOptions
. dispOptions
#GMT显示平面数据脚本，输入文件为网格文件，没有包含网格化语句因为网格化过程中的情况多样化，建议在其他脚本中个别添加再调用此脚本
# 初始化参数
data='null'
gridData='null'
unit='m'
color='rainbow'
range='null'
overwriteRange='null'
overwrite=0
labels=("longitude (degree)" "latitude (degree)")
plot_para=(0 0 0 0)
plotgrad=0
nocpt=0
inverseCPT=0
coastline=0
axistick=("a" "a") #横纵坐标轴间隔 a表示自动
bartick="a" #色标轴标轴间隔 a表示自动
# 不同图幅的绘图参数
pageChoice='small' # 默认尺寸为small
exSmallpage="1i,0.08i/-0.2i,0.85i/0.03i"
smallPage="1.5i,0.125i/-0.3i,1.2i/0.06i" # 图大小,色标位置,色标尺寸
middlePage="2i,0.2i/-0.35i,1.5i/0.07i"
largePage="2.5i,0.35i/-0.4i,1.75i/0.08i"
# 从命令行获取参数
while getopts "hi:r:u:c:a:t:v:l:G:gnpb" arg
do
	case $arg in
		h)
			dispTitle "${0##*/}" "Simple regional data mapping using GMT script. The template accepts a grid (.nc .grid) file as input and outputs a .png and a .eps file. For further explanations, please look for GMT's manuscripts." 
			dispAuthorInfo "Yi Zhang (zhangyi.cugwuhan@gmail.com)"
			dispUsage "${0##*/} -i<grid-data> [-r<xmin>/<xmax>/<ymin>/<ymax>] [-u<unit>] [-c<cpt-file>] [-a<x-label>,<y-label>] [-t<x-tick>,<y-tick>] [-v<c-tick>] [-l<size>] [-g] [-G<grad-data>] [-b] [-n] [-p]"
			dispOptionShort "-i" "Input grid file."
			dispOptionShort "-r" "Data range. The template will detect the input data range automatically, the use of this option will overwrite the range."
			dispOptionShort "-u" "Data unit. The default is meter. use 'km+Uk' to append '+Uk' option to the psscale command."
			dispOptionShort "-c" "Color cpt. The default is rainbow. The script will use grd2cpt to recalculate the color scale file, use -n option to disable the use of grd2cpt."
			dispOptionShort "-a" "Axis labels. The deafults are x (m) and y (m)."
			dispOptionShort "-t" "Intervals of axis's labels. the script will set the intervals automatically if -t option is not set."
			dispOptionShort "-v" "Intervals of color bar's labels. the script will set the intervals automatically if -v option is not set."
			dispOptionShort "-l" "Image layout and size. three available options are exsmall, small(1.5 inch wide), middle and large."
			dispOptionShort "-b" "Draw coast line."
			dispOptionShort "-g" "Use grdgradient. The default is false."
			dispOptionShort "-G" "Provide a different input grid data for the use of grdgradient. This should be used with the '-g' option at the same time."
			dispOptionShort "-n" "Disable the use of grd2cpt."
			dispOptionShort "-p" "Inverse the color pattern specified by the -c option, no use if -n option is implemented."
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
			labels=(${OPTARG//,/ });;
		t)
			axistick=(${OPTARG//,/ });;
		v)
			bartick=$OPTARG;;
		l)
			pageChoice=$OPTARG;;
		b)
			coastline=1;;
		g)
			plotgrad=1;;
		G)
			gridData=$OPTARG;;
		n)
			nocpt=1;;
		p)
			inverseCPT=1;;
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
	cptfile=${data%.*}.cpt
	psfile=${data%.*}.ps
	jpgfile=${data%.*}.png
	# 获取网格范围
	if [[ $overwrite == 1 ]]; then
		range=${overwriteRange}
		# 别忘了这里要加上括号 才能初始化数组
		range_sp=(${range//// })
		# 注意bash并不支持浮点运算 所以需要使用bc命令 bc命令是一个计算数学表达式工具 参数是一个数学表达式字符串
		plot_para[0]=$(echo "${range_sp[0]} * 0.5 + ${range_sp[1]} * 0.5"|bc)
		plot_para[1]=$(echo "${range_sp[2]} * 0.5 + ${range_sp[3]} * 0.5"|bc)
		plot_para[2]=$(echo "${plot_para[1]} * 0.5 + ${range_sp[2]} * 0.5"|bc)
		plot_para[3]=$(echo "${plot_para[1]} * 0.5 + ${range_sp[3]} * 0.5"|bc)
	else
		range=$(gmt_get_gridregion ${data})
		range_sp=(${range//// })
		plot_para[0]=$(echo "${range_sp[0]} * 0.5 + ${range_sp[1]} * 0.5"|bc)
		plot_para[1]=$(echo "${range_sp[2]} * 0.5 + ${range_sp[3]} * 0.5"|bc)
		plot_para[2]=$(echo "${plot_para[1]} * 0.5 + ${range_sp[2]} * 0.5"|bc)
		plot_para[3]=$(echo "${plot_para[1]} * 0.5 + ${range_sp[3]} * 0.5"|bc)
	fi
	# 设置绘图参数
	gmt gmtset \
	FONT_ANNOT_PRIMARY=7p,Times-Roman,black \
	MAP_FRAME_PEN=thinnest,black \
	MAP_FRAME_WIDTH=1p \
	MAP_TICK_LENGTH_PRIMARY=1.5p/1p \
	MAP_TICK_PEN_PRIMARY=thinnest,black \
	MAP_TITLE_OFFSET=6.5p \
	MAP_GRID_PEN_PRIMARY=thinnest,grey,-- \
	FONT_LABEL=8p,Times-Roman,black \
	MAP_FRAME_AXES=WESnZ \
	COLOR_NAN=white \
	MAP_LABEL_OFFSET=1.5p\
	MAP_ANNOT_OFFSET_PRIMARY=2.5p
	# 设置色标文件
	if [[ $nocpt == 1 ]]; then
		cptfile=${color}
	else
		if [[ $inverseCPT == 1 ]]; then
			gmt grd2cpt ${data} -R${range} -C${color} -Z -D -M -I > $cptfile
		else
			gmt grd2cpt ${data} -R${range} -C${color} -Z -D -M > $cptfile
		fi
	fi
	# 设置绘图尺寸 如果识别参数失败 则还是按small处理
	if [[ $pageChoice == 'small' ]]; then
		pagePara=(${smallPage//,/ })
	elif [[ $pageChoice == 'exsmall' ]]; then
		pagePara=(${exSmallpage//,/ })
	elif [[ $pageChoice == 'middle' ]]; then
		pagePara=(${middlePage//,/ })
	elif [[ $pageChoice == 'large' ]]; then
		pagePara=(${largePage//,/ })
	else
		pagePara=(${smallPage//,/ })
	fi

	if [[ $plotgrad == 1 ]]; then
		gradfile=${data%.*}Grad.nc
		if [[ $gridData == 'null' ]]; then
			gmt grdgradient ${data} -G${gradfile} -Nt -A0/45
		else
			gmt grdgradient ${gridData} -G${gradfile} -Nt -A0/45
		fi
		gmt grdimage ${data} -R${range} -C${cptfile} -I${gradfile} -Bx${axistick[0]}g+l"${labels[0]}" -By${axistick[1]}g+l"${labels[1]}" -JL${plot_para[0]}/${plot_para[1]}/${plot_para[2]}/${plot_para[3]}/${pagePara[0]} -K -P > $psfile
	else
		gmt grdimage ${data} -R${range} -C${cptfile} -Bx${axistick[0]}g+l"${labels[0]}" -By${axistick[1]}g+l"${labels[1]}" -JL${plot_para[0]}/${plot_para[1]}/${plot_para[2]}/${plot_para[3]}/${pagePara[0]} -K -P > $psfile
	fi

	if [[ $coastline == 1 ]]; then
		gmt pscoast -R${range} -JL -W0.25p -Dc -A5000 -K -O -P >> $psfile
	fi

	#-C${cptfile}+Uk 使用km（色标单位除1000
	#如果unit等于km则在cptfile后面添加+Uk
	if [[ ${unit} == 'km+Uk' ]]; then
		gmt psscale -Dx${pagePara[1]}+w${pagePara[2]}+h -C${cptfile}+Uk -Bx${bartick} -By+l${unit} -O >> $psfile
	else
		gmt psscale -Dx${pagePara[1]}+w${pagePara[2]}+h -C${cptfile} -Bx${bartick} -By+l${unit} -O >> $psfile
	fi
	gmt psconvert $psfile -A -TEG -E300
	# 删除临时文件 使用linux终端rm命令
	rm $psfile gmt.history gmt.conf
	if [[ $plotgrad == 1 ]]; then
		rm $gradfile
	fi
	if [[ $nocpt == 0 ]]; then
		rm $cptfile
	fi
	# 在终端显示图像 此命令需要imgcat.sh脚本和iTerm终端
	#imgcat $jpgfile
	# 打开图片文件 此命令使用MacOS终端open命令
	open $jpgfile
fi