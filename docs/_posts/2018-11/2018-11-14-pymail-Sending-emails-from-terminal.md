---
layout: post
title: pymail
subtitle: "Sending emails from terminal."
author: Yi Zhang
date: 2018-11-14 20:27:59 -0800
tags: [Python]
---

* content
{:toc}




## Installation

**pymail** is a [python3](https://www.python.org) script that sends emails within terminal. The main library I used here is [smtplib](https://docs.python.org/3/library/smtplib.html). In my daily work, sometimes it takes days running a program before getting the result, so that I use this script to send data to my laptop from my work station after the process is done. This has saved me a lot fo time. To use the script, simply copy the script into any directory you want and symlink the file to a directory that is included in your $PATH, for instance */usr/local/bin*, then reopen the terminal.

## Setup

Firstly, you may need to change the first line to the directory where your python3 is. Then please change the following as show.

```python
userName = 'username of your mail box'
passWord = 'application specific password of your mail box'
toAddress = ['default receiving e-mail address']
subJect = 'default subject'
textMsg = 'default message.'
```

## Usage

```bash
pymail [-a<to_address>,<to_address>,...|-A<address-file>] [-s<subject>] [-t<text>|-T<text-file>] [-f<attached_file>,<attached_file>,...] [-h] [-d]
```

## Options

+ __-a__ __--to-address__: Receiving addresses separated by commas.
+ __-A__ __--ads-file__:  Read the addresses from a file. The addresses should also be separated by commas in the file.
+ __-s__ __--subject__: Subject of the sending email.
+ __-t__ __--text__: Body of the message.
+ __-T__ __--text-file__: Read the message from a file.
+ __-f__ __--att-file__: Attached file names separated by commas.
+ __-h__ __--help__: Show help information.
+ __-d__ __--defaults__: Show default settings.

