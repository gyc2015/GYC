import os,sys
import re


def MatchDefineLine(a):
    p = re.compile('(.*):')
    m = p.match(a)
    return m

def MatchUseLine(a):
    p = re.compile('(.*) +(.*)')
    m = p.match(a)
    return m

#hehe = MatchDefineLine('./fs/btrfs/ctree.ct:282:push_leaf_left:')
hehe = MatchUseLine('./fs/btrfs/ctree.ct:282:push_leaf_left:')
#hehe = MatchUseLine('./fs/btrfs/ctree.ct-283-|   free_extent_buffer')
#hehe = MatchDefineLine('./fs/btrfs/ctree.ct-283-|   free_extent_buffer')
if hehe:
    print 'hehehe'

#os.system("cat lsc.sif | awk -F' ' '{ print $1 }' > 1.txt")
#word_file = file('1.txt')
#for line in word_file:
#    word = line[0:len(line)-1]
#    cmd = "find ./fs -name '*.ct' -exec grep -wnH '" + word + ":' -A 1 {} \;"
#    os.system(cmd + " >> 2.txt")

find_file = file('2.txt')
filename = ''
for line in find_file:
    defline = MatchDefineLine(line)
    useline = MatchUseLine(line)
    if defline:
        filename = str(defline.group(0))
    if useline:
        print filename
