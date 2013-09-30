from os import path
from os import listdir
from math import log


class Seg:
    def __init__(self, dic_dir='./dic'):
        self.dic = {}
        self.dic_len = -1 
        self.init_dic(dic_dir)
        
    def init_dic(self, dic_dir):
        if not path.isdir(dic_dir):
            print '%s is not a directory'%dic_dir

        filelist = listdir(dic_dir)
        filelist = map(lambda x : path.join(dic_dir,x), filelist)
        filelist = filter(lambda x : x.rfind('.dic') > 0 and True or False, filelist)
        
        self.dic['__sum__'] = 0
        for f in filelist:
            for ln in open(f):
                try:
                    word, freq = ln.strip().decode('gb18030').split()[0:2]
                    self.dic_len = self.dic_len if self.dic_len > len(word) \
                                   else len(word)
                    self.dic[word] = float(freq)+1.0
                    self.dic['__sum__'] += float(freq)+1.0
                except:
                    print 'Dic format wrong. %s: %s.'%(f,ln)
                    exit()

    def seg(self, s, coding='gb18030'):
        lg2 = lambda n : float('-inf') if not n else log(n, 2)
        inpt = s.decode(coding)
        length = len(inpt)
        ls_weight = [0 for x in xrange(length+1)]
        ls_index = [0 for x in xrange(length)]
        sum = float(self.dic['__sum__'])

        for i in xrange(length-1, -1, -1):
            max_weight = float('-inf')
            max_index = 0
            for j in xrange(1,self.dic_len):
                if i+j > length:
                    break

                query = ''.join(inpt[i:i+j])
                weight = self.dic[query] if query in self.dic else 0 if len(query) > 1 else 1
                weight /= sum

                if max_weight < ls_weight[i+j]+lg2(weight):
                    max_weight = ls_weight[i+j]+lg2(weight)
                    max_index = i+j

            ls_weight[i] = max_weight
            ls_index[i] = max_index

        cur = 0
        while cur < length:
            yield inpt[cur:ls_index[cur]].encode(coding)
            cur = ls_index[cur]

if __name__ == '__main__':
    seg = Seg()
    import sys
    if len(sys.argv) != 3:
        print 'Usage: bin input coding'
        exit()

    inpt = sys.argv[1]
    coding = sys.argv[2]
    print '|'.join(seg.seg(inpt, coding))
