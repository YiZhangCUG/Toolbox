#!/bin/bash
# 一些输出帮助文档的函数
# 显示命令简介 两个参数 命令名称 命令简介
#function dispTitle(){
#	winWidth=`tput cols`
#
#	cutLength=`expr ${winWidth} - 19`
#	segment=${2:0:${cutLength}}
#	printf "%s\n%-10s%s\n" "${1}" " " "${segment}"
#
#	times=1
#	while (( ${#segment} == ${cutLength} ))
#	do
#		st=`expr ${cutLength} \* ${times}`
#		segment=${2:${st}:${cutLength}}
#		printf "%-10s%s\n" " " "${segment}"
#		times=`expr ${times} + 1`
#	done
#	printf "\n"
#}

function dispTitle(){
	winWidth=`tput cols`
	message=${2}

	printf "%s\n%-9s" "${1}"

	head_length=19
	print_length=${head_length}
	for segment in ${message[@]}; do
		print_length=`expr ${print_length} + ${#segment} + 1`
		if [[ ${print_length} -le ${winWidth} ]]; then
			printf " %s" "${segment}"
		else
			print_length=`expr ${head_length} + ${#segment} + 1`
			printf "\n%-10s%s" " " "${segment}"
		fi
	done
	printf "\n\n"
}

# 显示作者信息 一个参数 作者信息
#function dispAuthorInfo(){
#	winWidth=`tput cols`
#
#	cutLength=`expr ${winWidth} - 19`
#	segment=${1:0:${cutLength}}
#
#	printf "%-10s%s\n" "Author:" "${segment}"
#
#	times=1
#	while (( ${#segment} == ${cutLength} ))
#	do
#		st=`expr ${cutLength} \* ${times}`
#		segment=${1:${st}:${cutLength}}
#		printf "%-10s%s\n" " " "${segment}"
#		times=`expr ${times} + 1`
#	done
#	printf "\n"
#}

function dispAuthorInfo(){
	winWidth=`tput cols`
	message=${1}

	printf "%-9s" "Author:"

	head_length=19
	print_length=${head_length}
	for segment in ${message[@]}; do
		print_length=`expr ${print_length} + ${#segment} + 1`
		if [[ ${print_length} -le ${winWidth} ]]; then
			printf " %s" "${segment}"
		else
			print_length=`expr ${head_length} + ${#segment} + 1`
			printf "\n%-10s%s" " " "${segment}"
		fi
	done
	printf "\n\n"
}

# 显示用法 一个参数 做法信息
#function dispUsage(){
#	winWidth=`tput cols`
#
#	cutLength=`expr ${winWidth} - 19`
#	segment=${1:0:${cutLength}}
#
#	printf "%-10s%s\n" "Usage:" "${segment}"
#
#	times=1
#	while (( ${#segment} == ${cutLength} ))
#	do
#		st=`expr ${cutLength} \* ${times}`
#		segment=${1:${st}:${cutLength}}
#		printf "%-10s%s\n" " " "${segment}"
#		times=`expr ${times} + 1`
#	done
#	printf "\nOptions:\n"
#}

function dispUsage(){
	winWidth=`tput cols`
	message=${1}

	printf "%-9s" "Usage:"

	head_length=19
	print_length=${head_length}
	for segment in ${message[@]}; do
		print_length=`expr ${print_length} + ${#segment} + 1`
		if [[ ${print_length} -le ${winWidth} ]]; then
			printf " %s" "${segment}"
		else
			print_length=`expr ${head_length} + ${#segment} + 1`
			printf "\n%-10s%s" " " "${segment}"
		fi
	done
	printf "\n\n"
}

# 短命令说明 两个参数 命令符号 命令说明
#function dispOptionShort(){
#	winWidth=`tput cols`
#
#	cutLength=`expr ${winWidth} - 19`
#	segment=${2:0:${cutLength}}
#	printf "%-4s%-6s%s\n" " " "${1}" "${segment}"
#
#	times=1
#	while (( ${#segment} == ${cutLength} ))
#	do
#		st=`expr ${cutLength} \* ${times}`
#		segment=${2:${st}:${cutLength}}
#		printf "%-10s%s\n" " " "${segment}"
#		times=`expr ${times} + 1`
#	done
#}

function dispOptionShort(){
	winWidth=`tput cols`
	message=${2}

	printf "%-4s%-5s" " " "${1}"

	head_length=19
	print_length=${head_length}
	for segment in ${message[@]}; do
		print_length=`expr ${print_length} + ${#segment} + 1`
		if [[ ${print_length} -le ${winWidth} ]]; then
			printf " %s" "${segment}"
		else
			print_length=`expr ${head_length} + ${#segment} + 1`
			printf "\n%-10s%s" " " "${segment}"
		fi
	done
	printf "\n"
}

# 长命令说明 三个参数 短命令符号 长命令符号 命令说明
#function dispOptionLong(){
#	winWidth=`tput cols`
#
#	cutLength=`expr ${winWidth} - 32`
#	segment=${3:0:${cutLength}}
#	printf "%-4s%-6s%-12s%s\n" " " "${1}" "${2}" "${segment}"
#
#	times=1
#	while (( ${#segment} == ${cutLength} ))
#	do
#		st=`expr ${cutLength} \* ${times}`
#		segment=${3:${st}:${cutLength}}
#		printf "%-22s%s\n" " " "${segment}"
#		times=`expr ${times} + 1`
#	done
#}

function dispOptionLong(){
	winWidth=`tput cols`
	message=${3}

	printf "%-4s%-6s%-11s" " " "${1}" "${2}"

	head_length=32
	print_length=${head_length}
	for segment in ${message[@]}; do
		print_length=`expr ${print_length} + ${#segment} + 1`
		if [[ ${print_length} -le ${winWidth} ]]; then
			printf " %s" "${segment}"
		else
			print_length=`expr ${head_length} + ${#segment} + 1`
			printf "\n%-22s%s" " " "${segment}"
		fi
	done
	printf "\n"
}