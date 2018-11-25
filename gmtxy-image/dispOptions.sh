#!/bin/bash
# 一些输出帮助文档的函数
# 显示命令简介 两个参数 命令名称 命令简介
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