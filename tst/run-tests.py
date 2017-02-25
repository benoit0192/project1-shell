#!/usr/bin/env python2.7
# -*- coding: utf8 -*-

import os

COLOR_RESET='\033[39m'
COLOR_FAIL ='\033[31m'
COLOR_OK   ='\033[32m'
OK_STR  =COLOR_OK   + 'OK'   + COLOR_RESET
FAIL_STR=COLOR_FAIL + 'FAIL' + COLOR_RESET

def err2str(err):
    return OK_STR if err==0 else FAIL_STR

def test(name):
    os.system('./shell ../tst/scripts/{}.sh > /tmp/tst.stdout 2> /tmp/tst.stderr'.format(name))
    cmp_stdout = os.system('diff ../tst/expected/{}.stdout /tmp/tst.stdout'.format(name))
    cmp_stderr = os.system('diff ../tst/expected/{}.stderr /tmp/tst.stderr'.format(name))
    print 'Test {}.sh\t stdout: {}\t stderr: {}'.format(name, err2str(cmp_stdout), err2str(cmp_stderr))
    return 1 if (cmp_stdout==0 and cmp_stderr==0) else 0

def main():
    tests = ['empty', 'example1', 'example2', 'example3', 'example4', 'example5', 'example6', 'example7']
    nb_ok = 0
    for name in tests:
        nb_ok += test(name)
    if nb_ok == len(tests):
        print (COLOR_OK+'All tests succeeded'+COLOR_RESET)
    else:
        print (COLOR_FAIL+'Some tests failed'+COLOR_RESET+' ({}/{} succeeded)'.format(nb_ok, len(tests)))

if __name__ == '__main__':
    main()
