---
layout: post
title: postToBlog
subtitle: "Post blogs to github.io from terminal."
author: Yi Zhang
date: 2018-11-12 16:31:25 -0800
tags: [Git,Jekyll]
---

* content
{:toc}




## Installation

**postToBolg** is a shell script that posts a markdown file to a [Github.pages](https://pages.github.com) hosted personal blog from terminal. The blog must be powered by the [Jekyll](https://jekyllrb.com). To use it, simply copy the shell scripts *dispOption.sh* and *postToBlog.sh* into any directory that is included in your $PATH, then reopen the terminal. You should be good to go by then.

## Setup

Open postToBlog.sh and change the following as indicated. To see your defaults, use -h option.

1. change the value of `_repository_address` to the directory of your blog's repository, which usually named like <*YourGithubCount*>.github.io.
```bash
_repository_address="directory to your github.io"
```

2. change values of `_posts_address` and `_posts_sub_address` to the relative directory of your posts' directory with respect to their upper directories. Normally, the value of `_posts_address` is *_posts/* and doesn't need to be changed. Meanwhile, the `_posts_sub_address` is added to classify your files to a default folder.
```bash
_posts_address="_posts/"
_posts_sub_address="2018/" #classify your files by year for instance
```

3. specify a default input filename by setting `_mfile`. This will save you some time I think.

4. setup head information that is needed by the Jekyll for configuring your post. The value of `_layout` must be defined in the Jekyll's styles.
```bash
_layout='post' #using the layout named post
_categories='Command Line Tools' #example category
_tags='shell,git,jekyll' #example tags
```

## Usage

```bash
postToBlog [-i<markdown-file>] [-l<layout>] [-T<title>] [-c<categories>] [-t<tags>] [-s<sub-directory>] [-S] [-h]
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
+ __-h__: Show help inforamtion.

## Example

To post the file your are reading now I used
```bash
postToBlog -i README.md -c "Command Line Tools" -t "shell,git,jekyll"
```
