import os.path

class Hz2Py:
    def __init__(self, word_file='dat/word.dat', poly_file='dat/poly.dat'):
        self.d_word = {}
        self.d_poly = {}
        self.s_word = word_file
        self.s_poly = poly_file
        self.init_dat()

    def init_dat(self):
        if not os.path.exists(self.s_word):
            raise IOError('Not found word file: %s'%self.s_word)

        if not os.path.exists(self.s_poly):
            raise IOError('Not found poly file: %s'%self.s_poly)

        try:
            for ln in open(self.s_word):
                ln = ln.strip().decode('gb18030')
                hz,py = ln.split()[0:2]
                self.d_word[hz] = py 
        except UnicodeDecodeError, ude:
            print 'word_file coding is wrong, it needs gbk/gb18030/gb2312'%self.s_word
            exit()
        except IndexError, ie:
            print 'word_file format is wrong, it needs \"hz\tpy\"'%self.s_word
            exit()

        try:
            for ln in open(self.s_poly):
                ln = ln.strip().decode('gb18030')
                hz,py = ln.split()[0:2]
                self.d_poly[hz] = py
        except UnicodeDecodeError, ude:
            print 'poly_file coding is wrong, it needs gbk/gb18030/gb2312'%self.s_poly
            exit()
        except IndexError, ie:
            print 'poly_file format is wrong, it needs \"hz\tpy\"'%self.s_poly
            exit()

    def anno(self, inpt, coding='gb18030'):
        try:
            inpt = inpt.decode(coding)
        except UnicodeDecodeError, ude:
            print 'input coding %s is wrong...'%coding
            exit()
        except LookupError, ude:
            print 'unknown coding %s...'%coding
            exit()

        index = 0
        while index <= len(inpt):
            double_word = ''
            if not index+1 >= len(inpt):
                double_word = '%s%s'%(inpt[index],inpt[index+1])

            if double_word in self.d_poly:
                yield self.d_poly[double_word].encode(coding)
                index += 1 
            elif inpt[index] in self.d_word:
                yield self.d_word[inpt[index]].encode(coding)
            else:
                yield inpt[index].encode(coding)
            
            index += 1
            if index == len(inpt):
                break

if __name__ == '__main__':
    hz2py_obj = Hz2Py()

    import sys
    if len(sys.argv) != 3:
        print 'Usage: bin input coding'
        exit()

    inpt = sys.argv[1]
    coding = sys.argv[2]
    print '|'.join(hz2py_obj.anno(inpt, coding))
