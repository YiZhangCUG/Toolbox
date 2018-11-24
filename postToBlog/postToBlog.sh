#!/bin/bash
#include dispOption
. dispOptions
#change initial parameters depending on your own file system
_repository_address="directory to your github.io"
_posts_address="_posts/" #directory of your posts
_posts_sub_address="2018/" #sub-directory of your posts
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
#get arguments from terminal
while getopts "hSi:l:T:c:t:s:" arg
do
	case $arg in
		h)
			dispTitle "${0##*/}" "Post a markdown file to a personal blog hosted on the Github.io. The site is powered by Jekyll (https://jekyllrb.com)."
			dispAuthorInfo "Yi Zhang (zhangyi.cugwuhan@gmail.com)"
			dispUsage "${0##*/} [-i<markdown-file>] [-l<layout>] [-T<title>] [-c<categories>] [-t<tags>] [-s<sub-directory>] [-S] [-h]"
			dispOptionShort "-i" "Input markdown file. The default is '${_mfile}'."
			dispOptionShort "-l" "Page layout defined in Jekyll's styles. The default is '${_layout}'."
			dispOptionShort "-T" "Blog title. The default will use the h1 level title in the input markdown file. The use of this option will overwrite the default. The output filename will use the current date plus the blog title. Note that following symbols in the title will be replaced by the '-' symbol: '.',!','@','#','$','%','^','&','*','_',':'."
			dispOptionShort "-c" "Blog categories that are separated by commas. The default is/are '${_categories}'."
			dispOptionShort "-t" "Blog tags that are separated by commas. The default is/are '${_tags}'."
			dispOptionShort "-s" "Sub-directory of the posts. The main directory of the posts is placed under '${_posts_address}' folder of your Github.io directory. The default is '${_posts_sub_address}'."
			dispOptionShort "-S" "Save output markdown file. The default will remove the output file after committing the blog to Github."
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
		?)
			printf "\e[1m\e[31mError ==>\e[0m Unknow argument.\n"
			printf "Use -h option to see help information.\n"
			exit 1;;
	esac
done

#check if _title is set or not. If so, skip getting _title and _outfile_end from the input file
if [[ ${_title} != 'Null' ]]; then
	#read input file and put it into a temporary file
	if [[ -f ${_mfile} ]]; then
		_tmpfile=`mktemp tmp.XXXX`
		while read oneline
		do
			echo ${oneline} >> ${_tmpfile}
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
		while read oneline
		do
			if [[ `echo ${oneline} | grep "^# "` ]]; then
				_outfile_end=${oneline:2}
			else
				echo ${oneline} >> ${_tmpfile}
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
cat ${_tmpfile} >> ${_outfile}
rm ${_tmpfile}

#pull your git repository
git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} pull
#copy output markdown file to posts directory
cp ${_outfile} ${_repository_address}/${_posts_address}${_posts_sub_address}
#add file to git
git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} add ${_posts_address}${_posts_sub_address}${_outfile}
#commit changes
git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} commit -m "added file ${_outfile}"
#push to remote
git --git-dir=${_repository_address}/.git --work-tree=${_repository_address} push

#check if the output file should be saved
if [[ ${_savefile} == 0 ]]; then
	rm ${_outfile}
fi