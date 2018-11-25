---
layout: post
title: post2blog
subtitle: "Updating from postToBlog to post2blog."
author: Yi Zhang
date: 2018-11-14 18:01:57 -0800
tags: [Git,Jekyll]
---

* content
{:toc}




## Installation

**post2bolg** is a shell script that posts a markdown file to a [Github.pages](https://pages.github.com) hosted personal blog from terminal. The blog must be powered by the [Jekyll](https://jekyllrb.com). To use it, simply copy the shell script *post2blog.sh* into any directory you want and symlink the file to a directory that is included in your $PATH, for instance */usr/local/bin*, then reopen the terminal. You should be good to go by then.

**post2blog** is a updated version of **postToBlog**. The main diffidence is that **post2blog** could handle linked files, such as images and documents, automatically. Please note that the script will only copy local files to your github.io. Hyper links that start with *http* or *https* will be left alone.

## Setup

Open *post2blog.sh* and change the following as indicated. To see your defaults, use -h option.

1. change the value of `_my_repository_addresses` to the directories of your blog's repositories, which usually named like <*YourGithubCount*>.github.io. The script will use the first one by default.
```bash
_my_repository_addresses=("Directory to your github.io" "Directory to your github.io")
```

2. change values of `_posts_address` and `_posts_sub_address` to the relative directory of your posts' directory with respect to their upper directories. Normally, the value of `_posts_address` is *_posts/* and doesn't need to be changed. Meanwhile, the `_posts_sub_address` is added to classify your files to a default folder.
```bash
_posts_address="_posts/"
_posts_sub_address=`date +"%Y-%m/"` #classify your files by yyyy-mm for instance
```

3. change the value of `_assets_address` to the relative directory of your assets' directory with respect to the directory of your github.io's repository. Typically, the folder is named as following.
```bash
_assets_address="assets/" #directory of your assets
```
As for the classification of images and documents. The script will create folders named by *yyyy-mm* under your assets' folder. That is to say, your files will be sorted by month. Moreover, all files will be renamed by using the hyper keys. For example, the output filename for a hyper link with the key value `[This is a test name]` is `This-is-a-test-name`.

4. specify a default input filename by setting `_mfile`. This will save you some time I think.

5. setup head information that is needed by the Jekyll for configuring your post. The value of `_layout` must be defined in the Jekyll's styles.
```bash
_layout='post' #using the layout named post
_categories='Command Line Tools' #example category
_tags='shell,git,jekyll' #example tags
```

## Usage

```bash
postToBlog [-i<markdown-file>] [-l<layout>] [-T<title>] [-c<categories>] [-t<tags>] [-s<sub-directory>] [-S] [-V] [-r] [-h]
```

## Options

With proper settings, no argument is needed to run the script most of the times. Use following options if it's not one of those.

+ __-i__: Input markdown file.
+ __-l__: Post's layout.
+ __-T__: The blog's title. The h1 level head in the input markdown file will be used as the title by default. And manually input is needed if none was found. Using this option will overwrite the title.
+ __-c__: Post's category. Commas is needed if there were more than one.
+ __-t__: Post's tags. Commas is needed if there were more than one.
+ __-s__: Sub-directory for classify your files.
+ __-S__: Save a copy of the output file. The format of the output file's name is as defined as by the Jekyll, which consists of the post's date and the blog's title connected by '-'. For example, 2018-11-12-postToBlog-post-your-blog-to-Github-io-from-terminal.md. Note that special symbols in the blog's title such as ! and : will deleted.
+ __-V__: Run the script in silent mode.
+ __-r__: Choose the repository listed in the `_my_repository_addresses`.
+ __-h__: Show help inforamtion.

## Example

To post the file your are reading now I used
```bash
post2blog -i README.md -c "Command Line Tools" -t "Shell,Git,Jekyll"
```
