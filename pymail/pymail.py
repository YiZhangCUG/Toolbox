#!/usr/local/bin/python3
#change the upper directory to your Python3 location
# -*- coding=utf-8 -*-

import smtplib, sys, getopt
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
from os.path import basename

def disp_help():
    proname = (sys.argv[0]).strip().split('/')
    print ('This a pyhton template for using smtplib to send emails. \
For more information please go to the offical site of smtplib "https://docs.python.org/3.5/library/smtplib.html"\n\
Sender\'s address and password must be set manually within the script.\n\
Author: Yi Zhang (zhangyi.cugwuhan@gmail.com)\n')
    print ('Usage: '+proname[-1]+' [-a<to_address>,<to_address>,...|-A<address-file>] [-s<subject>] [-t<text>|-T<text-file>] [-f<attached_file>,<attached_file>,...] [-h] [-d]\n\n\
-a --to-address\tA list of receiving addresses separated by commas. The default must be set manually within the script.\n\
-A --ads-file\tRead receiving addresses from a file.\n\
-s --subject\tSubject of the email. The default must be set manually within the script.\n\
-t --text\tText message of the email. The default must be set manually within the script.\n\
-T --text-file\tRead text message from a file.\n\
-f --att-file\tFile attachments separated by commas.\n\
-h --help\tShow this information\n\
-d --defaults\tShow default settings')

'''
用户名 密码(应用专用密码)
发件邮箱(一般等于用户名) 收件邮箱(列表)
主题 文字 文件(地址列表) 默认收件地址
'''
def send_mail(usrname: str, passwd: str, 
    send_from: str, send_to: list,
    subject: str, text: str, files= None):
    #初始化邮件头信息
    msg = MIMEMultipart()
    msg['From'] = send_from
    msg['To'] = ', '.join(send_to)
    msg['Subject'] = subject
    #添加文件信息
    msg.attach(MIMEText(text))
    #添加附件文件
    for f in files or []:
        with open(f, "rb") as fil: 
            ext = f.split('.')[-1:]
            attachedfile = MIMEApplication(fil.read(), _subtype = ext)
            attachedfile.add_header(
                'content-disposition', 'attachment', filename=basename(f) )
        msg.attach(attachedfile)
    #发送文件
    try:
        #邮箱服务器SMTP配置 服务器地址 端口
        smtp = smtplib.SMTP(host="smtp.gmail.com", port= 587) 
        smtp.starttls()
        smtp.login(usrname,passwd)
        smtp.sendmail(send_from, send_to, msg.as_string())
        smtp.close()
    except Exception:
        print("fail to sent mail.")

def main(argv):
    userName = 'username of your mail box'
    passWord = 'application specific password of your mail box'
    toAddress = ['default receiving e-mail address']
    adsFile = 'null'
    subJect = 'default subject'
    textMsg = 'default message.'
    textFile = 'null'
    attachFile = None

    try:
        opts, args = getopt.getopt(argv,"hda:A:s:t:T:f:",["help","defaults","to-address=","ads-file=","subject=","text=","text-file=","att-file="])
    except getopt.GetoptError:
        disp_help()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            disp_help()
            sys.exit()
        elif opt in ("-d", "--defaults"):
            print("username: %s\npassword: %s\nto-address: %s\nsubject: %s\ntext: %s"
            % (userName,passWord,toAddress,subJect,textMsg))
            sys.exit()
        elif opt in ("-a", "--to-address"):
            toAddress = list(map(str,arg.strip().split(',')))
        elif opt in ("-A", "--ads-file"):
            adsFile = arg
        elif opt in ("-s", "--subject"):
            subJect = arg
        elif opt in ("-t", "--text"):
            textMsg = arg
        elif opt in ("-T", "--text-file"):
            textFile = arg
        elif opt in ("-f", "--att-file"):
            attachFile = list(map(str,arg.strip().split(',')))

    if adsFile != 'null':
        fa = open(adsFile,'r')
        toAddress = list(map(str,fa.read().strip().split(',')))
        fa.close()
        pass

    if textFile != 'null':
        fp = open(textFile,'r')
        textMsg = fp.read()
        fp.close()
        pass

    send_mail(userName,passWord,userName,toAddress,subJect,textMsg,attachFile)


if __name__ == "__main__":
    main(sys.argv[1:])