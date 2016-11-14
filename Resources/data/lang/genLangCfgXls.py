# coding=utf-8

import os
import sys
import xlrd
import codecs
import collections

##################### 生成lang.xml文件 ##########################
class XmlWriter:
    def __init__ (self):
        self.token = ''
        self.text = ''
    '''
    生成的Xml文件格式
    <?xml version="1.0" encoding="UTF-8" ?>
    <Lang>
        <LanguageName value="简体中文"/>
        ...
    </Lang>
    '''
    def genXmlHead (self):
        self.text = '<?xml version="1.0" encoding="UTF-8" ?>\n'
        self.text += '<Lang>\n'

    def addslashes(self, s):
        d = {'"':'&quot;', "<":"&lt;", ">":"&gt;", "&":"&amp;"}
        return ''.join(d.get(c, c) for c in s)

    def genXmlLine (self, name, value):
        # 内容值里面可能会带双引号需要去除
        value = self.addslashes(value)
        self.text += '    <string' + ' key="' + name + '"' + ' value="' + value + '"/>\n'

    def genXmlTail (self):
        self.text += "</Lang>\n"

    def reset (self, token):
        self.token = token
        self.text = ''

    def writeXml (self):
        fileName = self.token + '.xml'
        file = codecs.open(fileName, "w", "utf-8")
        if sys.version_info < (3, 0):
            file.write(self.text)  # python 2
        else:
            file.write(self.text)  # python 3
        file.close()

def genLangXml (fileName, nameIdx, idx):
    data = xlrd.open_workbook(fileName)
    table = data.sheets()[0]
    rowNum = table.nrows
    colNum = table.ncols
	
    # 准备XML文件
    xmlWriter = XmlWriter()

    if colNum <= idx:
        return False
	
    lang = ''
    for lineIdx in range(rowNum):
        name = table.cell(lineIdx,nameIdx).value      #line[nameIdx]
        value = table.cell(lineIdx,idx).value    #line[idx]
        if not name.strip():
            # 跳过没有对应token字体的文本
            continue
		

        if not value.strip():
            # 文本缺失
            print("For " + lang + "in line " + str(lineIdx+1) + "miss")
            continue
        
        if lineIdx == 0:
            # 首行
            lang = value
            xmlWriter.reset(value)
            xmlWriter.genXmlHead()
        xmlWriter.genXmlLine(name, value)

    # 文件结束，写文件
    xmlWriter.genXmlTail()
    xmlWriter.writeXml()

    return True

def saveLangXml (fileName, tokenIdx, startIdx):
    idx = startIdx
    while (1):
        bret = genLangXml(fileName, tokenIdx, idx)
        if (bret == False):
            break
        idx += 1

##################### 生成LangList.xml文件 ##########################
'''
生成多语言列表的Xml文件格式
<?xml version="1.0" encoding="UTF-8" ?>
<LangList>
    <Lang token="Chinese" name="简体中文"/>
    ...
</LangList>
'''
def saveLangListXml (fileName, startIdx):
    # 解析前两行
    data = xlrd.open_workbook(fileName)
    table = data.sheets()[0]
    colNum = table.ncols

    # 生成xml内容
    txt = '<?xml version="1.0" encoding="UTF-8" ?>\n'
    txt += '<LangList>\n'
    idx = startIdx
    while (idx < colNum):
        token = table.cell(0,idx).value  #lineList[0][idx]
        name = table.cell(1,idx).value   #lineList[1][idx]
        country = table.cell(2,idx).value
        if not token.strip() or not name.strip(): break
        idx += 1
        newLine = '    <Lang token="' + token + '" name="' + name + '" country="' + country + '"/>\n'
        txt += newLine
    txt += '</LangList>\n'

    # 写文件	
    file = codecs.open('LangList.xml', "w", "utf-8")
    if sys.version_info < (3, 0):
        file.write(txt)  # python 2
    else:
        file.write(txt)  # python 3
    file.close()

##################### 生成FntList.xml文件 ##########################
'''
生成字体风格对应配置文件
<?xml version="1.0" encoding="UTF-8" ?>
<FNTList>
    <GameTips fnt="ttf_1"/>
    ...
</FNTList>
'''


##################### 生成多语言字体工程目录 ##########################
'''
工程目录结构如下：
FNT
 | - Chinese
        | - ttf_1
              | - text.txt
        | - ttf_2
              | - text.txt
        | - ...
        | - ttf_N
              | - text.txt
 | - English
        | - ttf_1
              | - text.txt
        | - ttf_2
              | - text.txt
        | - ...
        | - ttf_N
              | - text.txt
最终text.txt中存放该语言该风格去除重复字符后的文本
'''
class FntWriter:
    def __init__ (self, lang, fnt):
        self.lang = lang
        self.fnt = fnt
        self.text = ''

    # 廋身，去除重复字符
    def keepFit (self):
        self.text = "".join(collections.OrderedDict.fromkeys(self.text))

    def newText (self, addText):
        self.text += addText
        self.keepFit()

    def beforeWriter (self):
        # 准备存放目录，例如Chinese\ttf_1
        subPath = os.path.join(self.lang, self.fnt)
        if not os.path.isdir(subPath):
            os.makedirs(subPath)

    def writeText (self):
        self.beforeWriter()
        fileName = self.lang + '/' + self.fnt + '/' + 'text.txt'
        file = open(fileName, "w")
        file.write(self.text.encode("utf-8"))

def getFntWriter (lang, writerList, fnt):
    for writer in writerList:
        if cmp(writer.fnt, fnt) == 0 : return writer
    newWriter = FntWriter(lang, fnt)
    writerList.append(newWriter)
    return newWriter

def saveText (lang, writerList, fnt, text):
    writer = getFntWriter(lang, writerList, fnt)
    writer.newText(text)

def makeFntProjForLang (fileName, fntIdx, idx):
    writerList = []

    data = xlrd.open_workbook(fileName, encoding_override="utf-8")
    table = data.sheets()[0]
    rowNum = table.nrows
    colNum = table.ncols

    if colNum <= idx:
        return False

    lang = ''
    for lineIdx in range(rowNum):
        fnt = table.cell(lineIdx,fntIdx).value      #line[fntIdx]
        value = table.cell(lineIdx,idx).value    #line[idx]
        
        if lineIdx == 0:
            # 首行
            lang = value
            if not os.path.isdir(lang):
                print("mkdir " + lang)
                os.makedirs(lang)
            continue

        if not fnt.strip():
            # 跳过没有对应fnt字体的文本
            continue

        if not value.strip():
            # 文本缺失
            print("For " + lang + " in line " + str(lineIdx+1) + "miss")
            continue
        
        #print(fnt + "   " + value)
        saveText(lang, writerList, fnt, value)

    # 所有文本已经归类，把他们保存起来
    for writer in writerList:
        writer.writeText()

    return True

def makeFntProj (fileName, fntIdx, startIdx):
    idx = startIdx
    while (1):
        bret = makeFntProjForLang(fileName, fntIdx, idx)
        if (bret == False):
            break
        idx += 1

# 
def endWith(*endstring):
    ends = endstring
    def run(s):
            f = map(s.endswith,ends)
            if True in f: return s
    return run
 
def findExcelFile ():
    list_file = os.listdir('./')
    a = endWith('.xlsx','.xls')
    f_file = filter(a,list_file)
    return f_file[0]

if __name__ == '__main__':
    # 自动查找excel文档- 
    xlsFile = 'D:\installpakage\sdk\cocos2d-x-3.7.1\cocos2d-x-3.7.1\projects\Solitaire\Resources\data\lang\lang.xlsx'
    # 各列位置（从0开始）
    tokenIdx = 0  # Token所在的列
    startIdx = 1  # 正式语言列
    fntIdx = 3    # 字体列ttf_xx

    # 生成对应文档
    saveLangXml(xlsFile, tokenIdx, startIdx)
    #saveLangListXml(xlsFile, startIdx)
    #makeFntProj(xlsFile, fntIdx, startIdx)

