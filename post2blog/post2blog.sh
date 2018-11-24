#!/bin/bash
#define functions for display help information
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

#change initial parameters depending on your own file system
_my_repository_addresses=("Directory to your github.io" "Directory to your github.io")
_repository_address=${_my_repository_addresses[0]}
_posts_address="_posts/" #directory of your posts
_assets_address="assets/" #directory of your assets
_posts_sub_address=`date +"%Y-%m/"` #sub-directory of your posts
_mfile='README.md' #default input filename
_layout='post' #default post layout
_categories='Null' #default category
_tags='Null' #default tag
#
#end of self configuration. revise the left script with caution
#
_nowtime=`date +"%Y-%m-%d %H:%M:%S %z"` #get your current date, time and timezone
_outfile=`date +"%Y-%m-%d-"` #initial output filename
_title='Null'
_outfile_end='Null'
_savefile=0
_link_string=""
_no_questions=0
_repository_id=0
#get arguments from terminal
while getopts "hSVi:l:T:c:t:s:r:" arg
do
	case $arg in
		h)
			dispTitle "${0##*/}" "Post a markdown file to a personal blog hosted on the github.pages. The site is powered by the Jekyll (https://jekyllrb.com)."
			dispAuthorInfo "Yi Zhang (zhangyi.cugwuhan@gmail.com)"
			dispUsage "${0##*/} [-i<markdown-file>] [-l<layout>] [-T<title>] [-c<categories>] [-t<tags>] [-s<sub-directory>] [-S] [-h]"
			dispOptionShort "-i" "Input markdown file. The default is '${_mfile}'."
			dispOptionShort "-l" "Page layout defined in the Jekyll's styles. The default is '${_layout}'."
			dispOptionShort "-T" "Blog title. The default will use the h1 level title in the input markdown file. The use of this option will overwrite the default. The output filename will use the current date plus the blog title. Note that following symbols in the title will be replaced by the '-' symbol: '.',!','@','#','$','%','^','&','*','_',':'."
			dispOptionShort "-c" "Blog categories that are separated by commas. The default is/are '${_categories}'."
			dispOptionShort "-t" "Blog tags that are separated by commas. The default is/are '${_tags}'."
			dispOptionShort "-s" "Sub-directory of the posts. The main directory of the posts is placed under '${_posts_address}' folder of your github.io directory. The default is '${_posts_sub_address}'."
			dispOptionShort "-S" "Save output markdown file. The default will remove the output file after committing the blog to Github."
			dispOptionShort "-V" "Don't ask me questions."
			dispOptionShort "-r" "choose repository your want to use, The default will use the first one."
			dispOptionShort "-h" "Show help information."
			exit 0;;
		i)
			_mfile=$OPTARG;;
		l)
			_layout=$OPTARG;;
		T)
			_title=$OPTARG;;
		c)
			_categories=$OPTARG;;
		t)
			_tags=$OPTARG;;
		s)
			_posts_sub_address=$OPTARG;;
		S)
			_savefile=1;;
		V)
			_no_questions=1;;
		r)
			_repository_id=$OPTARG;;
		?)
			printf "\e[1m\e[31mError ==>\e[0m Unknow argument.\n"
			printf "Use -h option to see help information.\n"
			exit 1;;
	esac
done

if [[ ${_repository_id} != 0 ]]; then
	_repository_address=${_my_repository_addresses[${_repository_id}]}
fi

#check if _title is set or not. If so, skip getting _title and _outfile_end from the input file
if [[ ${_title} != 'Null' ]]; then
	#read input file and put it into a temporary file
	if [[ -f ${_mfile} ]]; then
		_tmpfile=`mktemp tmp.XXXX`
		while read _oneline
		do
			#first get all hyper links, including websites, images and files. save them to a single string separated by whitespace
			_link_string=${_link_string}`echo "${_oneline}" | grep -Eo "!?\[[^]]*\]\([^)]*\))"`" "
			echo "${_oneline}" >> ${_tmpfile}
		done < ${_mfile}
	else
		printf "\e[1m\e[31mError ==>\e[0m File not found: ${_mfile}\n"
		exit 1
	fi
	# remove special symbols from the title
	_outfile_end=${_title//[.:_!@#\$\%\^\&\*]/''}
	_outfile_end=${_outfile_end// /-}
else
	#read input file and put it into a temporary file
	if [[ -f ${_mfile} ]]; then
		_tmpfile=`mktemp tmp.XXXX`
		while read _oneline
		do
			if [[ `echo ${_oneline} | grep "^# "` ]]; then
				_outfile_end=${_oneline:2}
			else
				#first get all hyper links, including websites, images and files. save them to a single string separated by whitespace
				_link_string=${_link_string}`echo "${_oneline}" | grep -Eo "!?\[[^]]*\]\([^)]*\)"`" "
				echo "${_oneline}" >> ${_tmpfile}
			fi
		done < ${_mfile}
	else
		printf "\e[1m\e[31mError ==>\e[0m File not found: ${_mfile}\n"
		exit 1
	fi

	#check if _outfile_end is found or not
	if [[ ${_outfile_end} != 'Null' ]]; then
		_title=${_outfile_end}
		# remove special symbols from the h1 title
		_outfile_end=${_outfile_end//[.:_!@#\$\%\^\&\*]/''}
		_outfile_end=${_outfile_end// /-}
	else
		printf "\e[1m\e[31mError ==>\e[0m No h1 level title found in: ${_mfile}.\n"
		printf "Please set the blog title and output file name manually.\n"
		printf "\e[1m\e[32m==>\e[0m "
		read _outfile_end
		_title=${_outfile_end}
		# remove special symbols from the input title
		_outfile_end=${_outfile_end//[.:_!@#\$\%\^\&\*]/''}
		_outfile_end=${_outfile_end// /-}
	fi
fi

#combine output filename
_outfile=${_outfile}${_outfile_end}.md

#echo ${_link_string}
#get link names from _link_string and save a string
_link_names_string=`echo ${_link_string} | grep -Eo "\[[^]]*\]"`
#change whitespace in every link name into -
_link_names_string=${_link_names_string//" "/"-"}
#delete symbols \[ and \] in link names
_link_names_string=${_link_names_string//[\[|\]]/''}
_link_names_string=${_link_names_string//'.'/'-'}
#convert _link_names_string to an array
_link_names=(${_link_names_string})
#for i in ${_link_names[@]}; do
#	echo ${i}
#done

#get link addresses from _link_string and save a string
_link_addresses_string=`echo ${_link_string} | grep -Eo "\([^)]*\)"`
#echo ${_link_addresses_string}
#change whitespace in every link address into -
_link_addresses_string=${_link_addresses_string//" "/"-"}
#delete symbols \( and \) in link names
_link_addresses_string=${_link_addresses_string//[\(|\)]/''}
#convert _link_addresses_string to an array
_link_addresses=(${_link_addresses_string})
#for i in ${_link_addresses[@]}; do
#	echo ${i}
#done

#exclude http[s] links to get _copy_names and _copy_addresses
#we use year-month to sort files by month
unset _copy_names
unset _copy_addresses
for (( i = 0; i < ${#_link_names[@]}; i++ )); do
	if [[ ! `echo ${_link_addresses[i]} | grep -E "https?://"` ]]; then
		_copy_names+=(`date +"%Y-%m/"`"${_link_names[i]}.${_link_addresses[i]##*.}")
		_copy_addresses+=("${_link_addresses[i]}")
	fi
done

#read temporary file and change file link address, then input it into another temporary file
_tmpfile2=`mktemp tmp.XXXX`
while read _oneline
do
	#check every line for saved file links and replace theme with _copy_names
	for (( i = 0; i < ${#_copy_addresses[@]}; i++ )); do
		_oneline=${_oneline//${_copy_addresses[i]}/"/${_assets_address}${_copy_names[i]}"}
	done
	echo "${_oneline}" >> ${_tmpfile2}
done < ${_tmpfile}
rm ${_tmpfile}

#combine head information and temporary file into the output, then remove the temporary one
echo "---" > ${_outfile}
echo "layout: ${_layout}" >> ${_outfile}
echo "title: \"${_title}\"" >> ${_outfile}
echo "date: ${_nowtime}" >> ${_outfile}
echo "categories: [${_categories}]" >> ${_outfile}
echo "tags: [${_tags}]" >> ${_outfile}
echo "---" >> ${_outfile}
echo "" >> ${_outfile}
echo "* content" >> ${_outfile}
echo "{:toc}" >> ${_outfile}
echo "" >> ${_outfile}
echo "" >> ${_outfile}
cat ${_tmpfile2} >> ${_outfile}
rm ${_tmpfile2}

if [[ ${_no_questions} == 0 ]]; then
	printf "Output file generated. Please check it. Is every thing ok? (y/n) "
	read _check_result
else
	_check_result='y'
fi

if [[ ${_check_result} == 'y' ]]; then
	#copy files to github.io directory. Check directory exists or not
	#"${_repository_address}/${_assets_address}`date +"%Y-%m"`"
	if [[ ! -d "${_repository_address}/${_assets_address}`date +"%Y-%m"`" ]]; then
		mkdir ${_repository_address}/${_assets_address}`date +"%Y-%m"`
	fi

	if [[ ! -d "${_repository_address}/${_assets_address}${_posts_sub_address}" ]]; then
		mkdir ${_repository_address}/${_assets_address}${_posts_sub_address}
	fi

	for (( i = 0; i < ${#_copy_addresses[@]}; i++ )); do
		cp ${_copy_addresses[i]} ${_repository_address}/${_assets_address}${_copy_names[i]}
	done

	#pull your git repository
	git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} pull
	#copy output markdown file to posts directory
	cp ${_outfile} ${_repository_address}/${_posts_address}${_posts_sub_address}
	#add file to git
	git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} add ${_posts_address}${_posts_sub_address}${_outfile}
	for (( i = 0; i < ${#_copy_names[@]}; i++ )); do
		git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} add ${_assets_address}${_copy_names[i]}
	done
	#commit changes
	git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} commit -m "post blog ${_outfile}"
	#push to remote
	git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} push
fi

#check if the output file should be saved
if [[ ${_savefile} == 0 ]]; then
	rm ${_outfile}
fi
